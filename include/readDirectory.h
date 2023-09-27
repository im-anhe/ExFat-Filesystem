#pragma once

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

//GDS struct has a total of 4 members
typedef struct
{
    //union has 2 members but can only have 1 member active at a time. Other resets.
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
} GenericDirectoryStruct;

void printDirectory(void *fp, Main_Boot *MB, GenericDirectoryStruct *GDS, int bytesPerSector, int sectorsPerCluster, int tabs);
void readDirectory();

#ifdef __cplusplus
    extern "C"
};
#endif