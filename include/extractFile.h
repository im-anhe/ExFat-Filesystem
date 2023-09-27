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
#include "readDirectory.h"

void extractDirectory(char *fileName, FILE* fpp);
void extractFile(void *fp, Main_Boot *MB, GenericDirectoryStruct *GDS, int bytesPerSector, int sectorsPerCluster, char *fileName, FILE* fpp);

#ifdef __cplusplus
    extern "C"
};
#endif