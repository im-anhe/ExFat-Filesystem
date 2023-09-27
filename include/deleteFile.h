#pragma once

/* ensure this header can be used in a C++ program */
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdint.h>

#include "extfat.h"


typedef struct
{
    union
    {
        uint8_t EntryType;
        struct
        {
            uint8_t TypeCode : 5;
            uint8_t TypeImportance : 1;
            uint8_t TypeCategory : 1;
            uint8_t InUse : 1;
        };
    };
    uint8_t CustomDefined[19];
    uint32_t FirstCluster;
    uint64_t DataLength;
} DirectoryStruct;

void findFile(int fd, char file_D[], void *fp, Main_Boot *MB, DirectoryStruct *GDS, int bytesPerSector, int sectorsPerCluster);
void deleteFile(char fileInOpt[]);


#ifdef __cplusplus
    extern "C"
};
#endif