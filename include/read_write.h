#pragma once

//#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <string.h>

//the two necessary function declarations
char* myread(char* inputfile,long int* mp,int* readfd);
void mywrite(long int m,char* total_str, char* outputfile,int* readfd);


