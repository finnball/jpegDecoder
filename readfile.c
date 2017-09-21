#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>

#include "readfile.h"

uint32_t read(uint8_t **buffer, const char *fileName)
{
  FILE *fp = fopen(fileName, "r");  // open file read mode

  if (!fp)
    {
      fprintf(stderr, "File (%s) does not exist\n", fileName);    
      return 0;
    }

  printf("INFO: File (%s) opened\n", fileName);    
  
  fseek(fp, 0L, SEEK_END); // Skip to the end to find size
  
  uint32_t fileSize = ftell(fp);
  rewind(fp);
  
  printf("INFO: Filesize %d / 0x%04x bytes \n", fileSize, fileSize);
  
  void *bufferNew = realloc(*buffer, fileSize * sizeof(uint8_t)); // copy file contents to buffer
  
  if (!bufferNew)
    return 0;

  *buffer = bufferNew;
  
  if (fread(*buffer, fileSize, 1, fp) != 1) return 0;

  fclose(fp);
  
  return fileSize;
  
}
