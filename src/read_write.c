#include "read_write.h"


char* myread(char* inputfile,long int* mp,int* readfd){
   //file descriptor being initialized to help access inputfile
   *readfd = open(inputfile, O_RDONLY);
   if (*readfd == -1){
      perror("File could not be opened.\n");
      exit(1);
   }
   struct stat size;
   //already exisiting function that helps find the total size of input file memary
   //by initializing a struct here
   if((fstat(*readfd,&size))==-1){
      perror("Unable to find the size of the file.\n");
      exit(1);
   }
   //this char* address has access to all the info of the file now 
   //since mmap returns address of the file
   //also, "size.st_size" helped get the right memory size
   char* total_str = (char*)mmap(NULL,
                        size.st_size,
                        PROT_READ,
                        MAP_SHARED,
                        *readfd,
                        0); //offset
   if (total_str == (char*)-1){
      perror("error from mmap:");
      exit(1);
   }
   //mp is the right memory size
   *mp=size.st_size;
   return total_str;
}

//writes the content of char* obtained from mmap() into outputfile.image
void mywrite(long int m,char* total_str, char* outputfile, int* readfd){
   //getting file descriptor using fopen
   FILE* fd = fopen(outputfile, "w");
   if (fd == NULL){
      perror("File could not be opened..\n");
      exit(1);
   }
   //seeking to the end of file
   if(!fseek(fd,m-1,SEEK_SET)){
      printf("Successfully seeked to the end of file.\n");
   }
   else{
      perror("Error seeking to the end of file.\n");
      exit(1);
   }
   //writing a single byte using fwrite
   if((fwrite("\0",1,1,fd))<1){
      fclose(fd);
      perror("Error writing a single byte to the output file.\n");
      exit(1);
   }
   else{
      printf("Successfully wrote a single byte to the output file.\n\n");
      fclose(fd);
   }

   //we need to use fopen again in a+ mode so that out file size is set already and we can access it.
   FILE* nfd = fopen(outputfile, "a+");
   //converting file pointer to file descriptor
   int x=fileno(nfd);
   //using mmap
   void* fmem =mmap(NULL,
                  m,
                  PROT_WRITE,
                  MAP_SHARED,
                  x,
                  0); //offset
   if (fmem == (void*)-1){
      perror("error from mmap in mywrite():");
      exit(1);
   }
   printf("\n\n");
   //copying total_str into the address of fmem
   memcpy(fmem,total_str,m); 
   printf("Please check the output file.\n\n");
   //unmapping and closing
   munmap(fmem,m);
   munmap(total_str,m);
   fclose(nfd);
   close(*readfd);
}
