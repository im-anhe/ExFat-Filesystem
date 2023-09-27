#include <stdio.h>
#include <stdlib.h>

//this c file just changes the test.image file such that MBR and BBR will be different.
void main(){
    FILE* fptr=fopen("test.image","rb+");
    if (fptr == NULL){
      perror("File could not be opened.\n");
      exit(1);
   }
   //seeking to 5th byte
   if(!fseek(fptr,5,SEEK_SET)){
      printf("Successfully seeked to the 5th byte.\n");
   }
   else{
      perror("Error seeking to the fifth byte.\n");
      exit(1);
   }
   //writing a single byte using fwrite
   if((fwrite("P",1,1,fptr))<1){
      fclose(fptr);
      perror("Error writing a single byte to the 5th byte.\n");
      exit(1);
   }
   else{
      printf("Successfully wrote a single byte to the 5th byte.\n");
      fclose(fptr);
   }
}