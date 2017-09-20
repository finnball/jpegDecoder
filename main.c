#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

#include "definitions.h"
#include "readfile.h"
#include "parsejpeg.h"
#include "decodejpeg.h"


int main(int argc, char* argv[])
{
  (void)argc;
  (void)argv;
  
  printf("Starting...\n");
  
  
  uint8_t *fileContents = NULL;
  
  
  //char fileName[] = "imgs/speedy.jpg";
  //char fileName[] = "imgs/huff_simple0.jpg";
  char fileName[] = "imgs/tiny.jpg";
  
  //read file
  uint32_t fileSize = read(&fileContents, fileName);

  if ( (fileSize == 0) || !(fileContents) ) 
    {
      return -1; 
    }
  
  //parse file
  
  jpeg_t *jpeg = jpegParse(fileContents, fileSize);

  if (!jpeg)
    return -1;

  //decode jpeg
  //decode(jpeg);
  
  //write file
  //write(jpeg, type)

  freeJpeg(jpeg);
  
  free(fileContents);
    
  printf("Finished.\n");
  return 0;
}
