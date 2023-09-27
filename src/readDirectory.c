#include "readDirectory.h"

#define cluster(N) ((fp + MB->ClusterHeapOffset * bytesPerSector) + ((N - 2) * bytesPerSector * sectorsPerCluster))

void printDirectory(void *fp, Main_Boot *MB, GenericDirectoryStruct *GDS, int bytesPerSector, int sectorsPerCluster, int tabs)
{
    int i = 0;
    while (GDS[i].EntryType)
    {
        //printf("%8lx\tThe Entry Type is %x  \tFirstCluster %d\n", (void *)&GDS[i] - fp, GDS[i].EntryType, GDS[i].FirstCluster);

        if (GDS[i].InUse && GDS[i].EntryType == 0xc1)
        {
            // This is a temporary structure
            // Needs to be 'well defined' like the generic one, above
            struct x
            {
                uint8_t EntryType : 8;
                uint8_t GeneralSecondaryFlags : 8;
                uint8_t FileName[30];
            };
            struct x *clusterStruct = (struct x *)&GDS[i];

            if (clusterStruct->EntryType == 0xc1)
            {
                char *ch = (char*) clusterStruct->FileName;
                for(int i = 0; i<tabs; i++)
                {
                    printf("\t");
                }
                
                while (*ch)
                {
                    if(*ch > 0)
                    {
                        printf("%c", *ch);
                    }
                    // go to the next, and skip one bc of .
                    ch++;
                    ch++;
                }
                printf("\n");
                //goes back to 0xc0 in order to get the files in the directory
                i--;
            }

            //print the files in this directory
            if (GDS[i].EntryType == 0xc0)
            {
                int cluster = GDS[i].FirstCluster;
                
                if(2<= cluster && cluster <= (MB->ClusterCount)+1)
                {
                    GenericDirectoryStruct *sec = cluster(GDS[i].FirstCluster);
                    printDirectory(fp, MB, sec, bytesPerSector, sectorsPerCluster, tabs+1);
                }
            }
            
            //go to the next directory
            if(GDS[i].EntryType == 0xc0)
            {
                // eat the next 2 directory entries
                i++;
                i++;
            }
        }
        i++;
    }
}

void readDirectory()
{
    // open the file system image file
    int fd = open("test.image", O_RDWR);
    if (fd == -1)
    {
        perror("file could not be opened: ");
        exit(1);
    }

    off_t size = 0;
    struct stat statbuf;
    if (fstat(fd, &statbuf))
    {
        perror("stat of file could not be done:");
        exit(1);
    }
    size = statbuf.st_size;
    //printf("The file size is %ld\n", size);

    // mmap the entire file into memory
    // every data item we reference, will be relative to fp...
    void *fp = (void *)mmap(NULL,
                            size,
                            PROT_READ,
                            MAP_PRIVATE,
                            fd,
                            0); // note the offset

    if (fp == (void *)-1)
    {
        perror("mmap couldn't be opened:");
        exit(1);
    }

    // first, is the Main Boot record
    Main_Boot *MB = (Main_Boot *)fp;

    /* print out some things we care about
    printf("FileSystemName %s\n", MB->FileSystemName); // warning, not required to be terminated
    printf("\n");
    printf("BytesPerSectorShift %d\n", MB->BytesPerSectorShift);
    printf("SectorsPerClusterShift %d\n", MB->SectorsPerClusterShift);
    printf("NumberOfFats %d\n", MB->NumberOfFats);
    */
    int bytesPerSector = 2 << (MB->BytesPerSectorShift - 1);
    int sectorsPerCluster = 2 << (MB->SectorsPerClusterShift - 1);

    //printf("-----> the bytesPerSector are %d\n", bytesPerSector);
    //printf("-----> the sectorsPerCluster are %d\n", sectorsPerCluster);

    // next, the Backup Boot record
    //Main_Boot *BBR = (Main_Boot *)(fp + 12 * bytesPerSector);
    //printf("BackUpBootRecord %p\n", BBR);
    //printf("FileSystemName %s\n", BBR->FileSystemName); // warning, not required to be terminated

    //printf("The size of GenericDirectoryStruct %ld %lx\n", sizeof(GenericDirectoryStruct), sizeof(GenericDirectoryStruct));
    assert(sizeof(GenericDirectoryStruct) == 32);

    // directory
    GenericDirectoryStruct *GDS = cluster(MB->FirstClusterOfRootDirectory);
    //printf("FirstClusterofRootDirectory %d\n", MB->FirstClusterOfRootDirectory);

    printf("The directory listing\n");
    // The directory listings are complicated.  This code just extracts the file names.
    // it does not handle directories.

    int tabs = 1;

    printDirectory(fp, MB, GDS, bytesPerSector, sectorsPerCluster, tabs);

    // unmap the file
    if (munmap(fp, size))
    {
        perror("error from unmap:");
        exit(1);
    }

    // close the file
    if (close(fd))
    {
        perror("close:");
        exit(1);
    }
    fd = 0;
}
