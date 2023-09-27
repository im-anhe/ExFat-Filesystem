#pragma once

#include "extfat.h"
#ifdef __cplusplus
extern "C"
{
#endif

//include any other headers thats will be used
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "read_write.h"
#include "extfat.h"
#include "readDirectory.h"
#include "deleteFile.h"
#include "extractFile.h"


//include the struct header file that will have the output and input filenames
typedef struct 
{
    int command;
    int cFlag;
}CmdAndFlags;

int process_args(int argc, char *argv[], CmdAndFlags* files, fileInfo *input, fileInfo *output);