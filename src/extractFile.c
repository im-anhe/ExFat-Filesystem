#include "extractFile.h"
#include <string.h>

#define cluster(N) ((fp + MB->ClusterHeapOffset * bytesPerSector) + ((N - 2) * bytesPerSector * sectorsPerCluster))

void extractDirectory(char *fileName, FILE *fpp)
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

    Main_Boot *MB = (Main_Boot *)fp;
    int bytesPerSector = 2 << (MB->BytesPerSectorShift - 1);
    int sectorsPerCluster = 2 << (MB->SectorsPerClusterShift - 1);
    assert(sizeof(GenericDirectoryStruct) == 32);
    GenericDirectoryStruct *GDS = cluster(MB->FirstClusterOfRootDirectory);

    //found correct spot, now attempt extraction
    extractFile(fp, MB, GDS, bytesPerSector, sectorsPerCluster, fileName,fpp);

    if (munmap(fp, size))
    {
        perror("error from unmap:");
        exit(0);
    }

    // close the file
    if (close(fd))
    {
        perror("close:");
    }
    fd = 0;
}

void extractFile(void *fp, Main_Boot *MB, GenericDirectoryStruct *GDS, int bytesPerSector, int sectorsPerCluster, char *fileName, FILE *fpp)
{
    int i = 0;
    int check = 0;
    while (GDS[i].EntryType)
    {
        if (GDS[i].InUse && GDS[i].EntryType == 0xc1)
        {
            struct x
            {
                uint8_t EntryType : 8;
                uint8_t GeneralSecondaryFlags : 8;
                uint8_t FileName[30];
            };
            struct x *clusterStruct = (struct x *)&GDS[i];

            //starting at 0xc1 file location to start extracting 
            if (clusterStruct->EntryType == 0xc1)
            {
                char *ch = (char*) clusterStruct->FileName;
                char * tmpWord = cluster(GDS[i-1].FirstCluster);
                char extractName[30] = "";
                //extracting file name in image for comparison
                while (*ch)
                {
                    if(*ch > 0)
                    {
                        strcat(extractName,  ch);
                    }
                    // go to the next, and skip one bc of .
                    ch++;
                    ch++;
                }

                //checking if filename looking for matches with image file
                //if it doesn't print error
                if(strcmp(fileName,extractName))
                {
                    check = 1;
                }
                //if it finds file that matches extracts to output file
                else
                {   
                    //execute extraction
                    check = 0;
                    printf("Extracting %s from image!\n",extractName);
                    while(*tmpWord != 0)
                    {
                        fprintf(fpp,"%c",*tmpWord);
                        tmpWord++;
                    }
                    exit(0);
                }
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
                    extractFile(fp, MB, sec, bytesPerSector, sectorsPerCluster, fileName, fpp);
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

    //checking if filename looking for matches with image file
    //if it doesn't print error
    if(check == 1)
    {
        printf("ERROR: File not found in image!\n");
    }
}