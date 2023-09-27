#include "process_cmd_args.h"
#include "routines.h"
#include "extfat.h"
#include "read_write.h"
#include "deleteFile.h"
#include "extractFile.h"

int process_args(int argc, char *argv[], CmdAndFlags *files, fileInfo *inputFILE, fileInfo *outputFILE)
{
        //initializing useful variables
        extern char *optarg;
        files->cFlag = 0;
        char commands[10]; 
        int count = 0;
        char fileInOpt[30];
        char fileOutOpt[30];
        char deleteName[30];
        char fileExtOpt[30];

        FILE *fileArray[3];

        //setting input and output file names to nothing/null
        strcpy(inputFILE->fileName,"");
        strcpy(outputFILE->fileName,"");

        if(argc<2){
                fprintf(stderr,"Please add command line arguments in order to proceed.\n");
                exit(1);
        }
        //dummy is just used to for making sure no invalid command is entered
        //like name suggests, it is a dummy only.
        int dummy=10;
        //while loop collects all the entered commands
        while((files->command = getopt(argc, argv, ":i:o:D:x:hcvd")) != -1)
        {
                commands[count] = files->command;

                if(commands[count] == ':')
                {
                        printf("Need arguments for the command!\n");
                        exit(1);
                }
                else if(commands[count] == 'i')
                {
                        strcpy(fileInOpt,optarg);
                }
                else if(commands[count] == 'o')
                {
                        strcpy(fileOutOpt,optarg);
                }
                else if(commands[count] == 'D')
                {
                        strcpy(deleteName,optarg);
                }
                else if(commands[count] == 'c')
                {
                        dummy++;
                }
                else if(commands[count] == 'v')
                {
                        dummy++;
                }
                else if(commands[count] == 'd')
                {
                        dummy++;
                }
                else if(commands[count] == 'h')
                {
                        dummy++;
                }
                else if(commands[count] == 'x')
                {
                        strcpy(fileExtOpt,optarg);
                }
                //invalid command example is -g
                else
                {
                        fprintf(stderr,"Invalid command entered. Please enter a valid one.\n");
                        exit(1);
                }
                count++;
        }
        //the following 2 for loops show how the program behaves when a command is found
        for(int loop=0;loop<count;loop++)
        {   
                if(commands[loop] == 'i')
                {
                        //not allow files that are not .image
                        if(strcmp(fileInOpt,"test.image"))
                        {
                                printf("-i must be followed by test.image file type\n");
                                exit(1);
                        }
                        strcpy(inputFILE->fileName, fileInOpt);
                        FILE *fp1 = fopen(inputFILE->fileName, "r");
                        if(fp1 == NULL) {
                                printf("Failed to open input file %s\n", inputFILE->fileName);
                                exit(1);
                        }
                        printf("Input file: %s\n", fileInOpt);
                        fileArray[0] = fp1;
                }
                if(commands[loop] == 'o')
                {
                        strcpy(outputFILE->fileName,fileOutOpt);
                        FILE *fp2 = fopen(outputFILE->fileName, "w");
                        if(fp2 == NULL) 
                        {
                                printf("Failed to open output file %s\n", outputFILE->fileName);
                                exit(1);
                        }
                        printf("Output file: %s\n", fileOutOpt);
                        fileArray[1] = fp2;
                }
                if(commands[loop] == 'h')
                {
                        //updated help text
                        printf("*****HELP TEXT*****\n");
                        printf("-i : for file input\n-o : for file output\n-h : for help text\n");
                        printf("-c : denotes copy\n-v : for verify (input file is required)\n");
                        printf("-d : to print out directory of files in the disk image\n");
                        printf("-D : to delete a file\n");
                        printf("-x : to extract file from the disk image\n");
                        exit(0); 
                }
                if(commands[loop] == 'd')
                {
                        //calls another file for -d
                        readDirectory();
                        exit(0);
                        break;
                }
                if(commands[loop] == 'D')
                {
                        //calls another file for -D
                        deleteFile(deleteName);
                        exit(0);
                }
                if(commands[loop] == 'x')
                {
                        strcpy(inputFILE->fileName, fileInOpt);
                        if(strcmp(fileInOpt,"test.image"))
                        {
                                printf("-i must be followed by test.image file type\n");
                                exit(1);
                        }
                        strcpy(outputFILE->fileName,fileOutOpt);
                        FILE *fp2 = fopen(outputFILE->fileName, "wb");       
                        if(fp2 == NULL || (strcmp(outputFILE->fileName,"@")==0))
                        {
                                printf("ERROR: Need to specify output file\n");
                                exit(1);
                        }
                        extractDirectory(fileExtOpt,fp2);
                        break;
                }
        }

        for(int loop=0;loop<count;loop++)
        {
                if(commands[loop] == 'c')
                {
                        files->cFlag = 1;
                        //added for handling segmentation fault if no inputfile specified
                        //if inputfile is empty/null, then we exit as fail
                        if((strcmp(inputFILE->fileName,""))==0)
                        {
                                fprintf(stderr,"Needed to specify input file at least before using -c.\n");
                                exit(1);
                        }
                        //if outputfile is empty, it is not fail. Inputfile is outputfile
                        if((strcmp(outputFILE->fileName,""))==0)
                        {
                                strcpy(outputFILE->fileName,inputFILE->fileName);
                                printf("Output file name is %s\n",inputFILE->fileName);
                        }
                }
                //verify command
                if((commands[loop] == 'v') && (fileArray[0] != NULL))
                {
                        off_t size = 0;
                        struct stat statbuf;
                        int fd = open("test.image", O_RDWR);
                        if(fd == -1)
                        {
                                perror("Unable to open the file: ");
                                exit(1);
                        }
                        if (fstat(fd, &statbuf))
                        {
                                perror("stat of the file:");
                                exit(1);
                        }
                        size = statbuf.st_size;
                        void *fp = (void *)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
                        printf("Checking if main and backup boot sectors are the same...\n");
                        Main_Boot *MB = (Main_Boot *)fp;
                        int bytesPerSector = 2 << (MB->BytesPerSectorShift - 1);
                        printf("shift is %d\n",MB->BytesPerSectorShift);
                        printf("bytes per = %d\n",bytesPerSector);
                        Main_Boot *BBR = (Main_Boot *)(fp + 12 * bytesPerSector);
                        uint32_t mainBootCheck = BootChecksum((uint8_t*) MB, (short) bytesPerSector);
                        uint32_t backupBootCheck = BootChecksum((uint8_t*)BBR, (short) bytesPerSector);

                        if(mainBootCheck == backupBootCheck)
                        {
                                printf("Both main and boot sectors match!\n");
                                printf("%x vs %x\n",mainBootCheck,backupBootCheck);
                        }
                        else
                        {
                                printf("Main and backup boot sectors do not match!\n");
                                printf("%x vs %x\n",mainBootCheck,backupBootCheck);
                        }
                        close(fd);
                        munmap(fp,size);
                }
                if(commands[loop] == 'D')
                {
                        //deleteFile[deleteName];
                        exit(0);
                        break;
                }
        }
        return 0;
}