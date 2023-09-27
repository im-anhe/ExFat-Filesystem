/*
   This is an example of using mmap to read an extFAT image file.
   To make the sample file:
    % # create the file system image
    % dd if=/dev/zero of=test.image count=1 bs=1G
    % sudo losetup /dev/loop2 test.image
    % sudo /usr/sbin/mkexfatfs /dev/loop2
    % # put something in the file system image
    % mkdir /tmp/d
    % sudo mount /dev/loop2 /tmp/d
    % cp examples/mmap.c /tmp/d
    % # clean up
    % sudo umount /tmp/d
    % sudo losetup -d /dev/loop2
    % rm -rf /tmp/d
    % rm test.image
   Written by Bud Davis, jimmie.davis@uta.edu
   (c) 2023, All Rights Reserved
   Provided to students of CSE3310, UTA. Any use
   other than this course is prohibited.
*/

//how to execute: ./extfat -i test.image -o outputfile.image -c

#include "extfat.h"
#include "process_cmd_args.h"
#include "read_write.h"
#include "routines.h"


int main(int argc, char *argv[])
{
    //initialzing the two extfat data structure for input and output
    fileInfo* inputFILE = (fileInfo *) malloc(sizeof(fileInfo));
    fileInfo* outputFILE = (fileInfo *) malloc(sizeof(fileInfo));
    //checking if malloc was succesfull, if not print error message
    if(inputFILE == NULL || outputFILE == NULL)
    {
        printf("Malloc failed!\n");
        exit(0);
    }
    //make a struct to hold cmds data structure 
    CmdAndFlags* files= (CmdAndFlags *) malloc(sizeof (CmdAndFlags));
    //temporary values
    files->cFlag = 0;
    //Call to task 1's function, will return any files into the struct if needed

    int test_pass = process_args(argc, argv, files, inputFILE, outputFILE);
    if (files->cFlag == 1 && test_pass==0)
    {
        //Requirement 5 for Future uses
        if(strcmp(inputFILE->fileName, outputFILE->fileName) != 0)
        {
            //Task 2 code goes here
            int n=1;
            //select either accessing the file with mmap() or with fread()
            //if n==1, we understand that it will be mmap(). scanf and if/else can be added later
            //to accept using fread and such functions
            if(n==1)
            {
                long int m=-1;
                char* total_str=NULL;
                int* readfd=malloc(sizeof(int));
                printf("\n\b***If the file is very big, it can take some time or the window may even reload.***\n");
                total_str=myread(inputFILE->fileName,&m,readfd);
                mywrite(m,total_str,outputFILE->fileName,readfd);
                free(readfd);
            }
        }
    }
    //freeing the memory used for the struct input
    free(inputFILE);
    //freeing the memory used for the struct output
    free(outputFILE);
    //freeing the memory used for the struct filesandflags
    free(files);
    return 0;
}