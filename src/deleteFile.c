#include "deleteFile.h"
#include <string.h>
#include <math.h>

#define cluster(N) ((fp + MB->ClusterHeapOffset * bytesPerSector) + ((N - 2) * bytesPerSector * sectorsPerCluster))

void findFile(int fd, char file_D[], void *fp, Main_Boot *MB, DirectoryStruct *GDS, int bytesPerSector, int sectorsPerCluster)
{
    int i = 0;
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

            if (clusterStruct->EntryType == 0xc1)
            {
                char *ch = (char*) clusterStruct->FileName;
                char name[30] = "";
                while (*ch)
                {
                    if(*ch > 0)
                    {
                        strcat(name,  ch);
                    }
                    // go to the next, and skip one bc of .
                    ch++;
                    ch++;
                }

                if(strcmp(file_D, name) == 0)
                {
                    printf("\nfound it\n");
                    
                    uint8_t x = 0x00;
                    
                    //clearing FAT chains
                    uint32_t *FAT = (uint32_t *)((void *)fp + (MB->FatOffset * bytesPerSector));
                    // defined as always these values
                    assert(FAT[0] == 0xfffffff8);
                    assert(FAT[1] == 0xffffffff);

                    int have_done = 0;
                    int temp = 0;
                    //start at the cluster number  
                    int a = GDS[i-1].FirstCluster;  
                    //allocation bitmap
                    char * address = cluster(2);
                    int start = 2;
                    int end = 9;
                    
                    while(2<=FAT[a] && FAT[a] <= (size_t)MB->ClusterCount - 1 
                            && 2<= a && a <= (MB->ClusterCount)+1)
                    {
                        switch (FAT[a])
                        {
                            case 0x00000000:
                                break;
                            case 0xfffffff7:
                                break;
                            case 0xffffffff:
                                // end of chain
                                break;
                            default:
                                //need to delete when its a chain of not 1
                                temp = FAT[a]; //is 7
                                int power = 1;
                                //getting to the right byte for the bitmap
                                while(! (start <= a && a <= end))
                                {
                                    address+=1;
                                    start+=8;
                                    end+=8;
                                }
                                //6-1 or a-1 to get the cluster #
                                int shiftamt = a-start-1;
                                uint8_t y = *address;
                                //subtract 2^N depending on whatever cluster
                                y = y - (power << shiftamt);
                                memcpy(address, &y, 1);

                                memcpy(&FAT[a], &x, 1); //FAT[6] is 0
                                
                                a = temp; // a is 7
                                have_done = 1;
                                break;
                        }
                    }
                    if(have_done == 1 && FAT[a] == 0xFFFFFFFF)
                    {
                        //clear 0xffffffff
                        uint32_t y = 0x00000000;
                        memcpy(&FAT[a], &y, 4);
                    }
                    
                    
                    //deleting the filename
                    char *cha = (char*) clusterStruct->FileName;
                    while (*cha)
                    {
                        if(*cha > 0)
                        {
                            memcpy(cha, &x, 1);
                            printf("%c", *cha);
                        }
                        // go to the next, and skip one bc of .
                        cha++;
                        cha++;
                    }

                    //Flipping the inuse bits
                    //Changing the 0x85, 0xc0, 0xc1
                    x = GDS[i].EntryType^128;

                    memcpy(&GDS[i], &x, 1);
                    i--;
                    x = GDS[i].EntryType^128;
                    memcpy(&GDS[i], &x, 1);
                    i--;
                    x = GDS[i].EntryType^128;
                    memcpy(&GDS[i], &x, 1);
                    i+=2;

                    //Cleans up the file data
                    x = 0x00;
                    char * start_address = cluster(GDS[i-1].FirstCluster);
                    while(*start_address != '\0')
                    {
                        memcpy(start_address, &x, 1);
                        start_address++;
                    }
                    
                    
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
                    //int num = GDS[i].FirstCluster;
                    DirectoryStruct *sec = cluster(GDS[i].FirstCluster);
                    findFile(fd, file_D, fp, MB, sec, bytesPerSector, sectorsPerCluster);
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

void deleteFile(char file_D[])
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

    // mmap the entire file into memory
    // every data item we reference, will be relative to fp...
    void *fp = (void *)mmap(NULL,
                            size,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
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

    assert(sizeof(DirectoryStruct) == 32);

    // directory
    DirectoryStruct *GDS = cluster(MB->FirstClusterOfRootDirectory);

    printf("Deleting %s\n", file_D);
    //printf("\n%d", MB->FirstClusterOfRootDirectory);

    //delete here
    findFile(fd, file_D, fp, MB, GDS, bytesPerSector, sectorsPerCluster);

    // unmap the file
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