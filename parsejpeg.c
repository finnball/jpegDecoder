#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "parsejpeg.h"
#include "definitions.h"
#include "tools.h"


// FORMATS:
//http://vip.sugovica.hu/Sardi/kepnezo/JPEG%20File%20Layout%20and%20Format.htm
/*
ZZ = [0,  1,  5,  6,  14, 15, 27, 28,
      2,  4,  7,  13, 16, 26, 29, 42,
      3,  8,  12, 17, 25, 30, 41, 43,
      9,  11, 18, 24, 31, 40, 44, 53,
      10, 19, 23, 32, 39, 45, 52, 54,
      20, 22, 33, 38, 46, 51, 55, 60,
      21, 34, 37, 47, 50, 56, 59, 61,
      35, 36, 48, 49, 57, 58, 62, 63]
 */


jpeg_t *jpegParse(const uint8_t *fileContents, const uint32_t fileSize)
{
  uint16_t *id = malloc(sizeof(uint16_t));

  if (!id)
    return NULL;
  *id = 0;
  
  uint32_t *fileSizePtr = malloc(sizeof(uint32_t));

  if (!fileSizePtr)
    return NULL;
  *fileSizePtr = fileSize;

  jpeg_t *jpeg = initJpeg();
  
  parseHeader(&fileContents, fileSizePtr, id);
  
  if (*id != JPEG_HEADER_SOI)
    {
      fprintf(stderr, "SOI not found at start \n");
      return NULL;
    }

  //parseHeader(&fileContents, fileSizePtr, id);
  //parseData(&fileContents, fileSizePtr, &(jpeg->jfif), id);
  
  free(id);
  free(fileSizePtr);
  return jpeg;
}

uintptr_t parseHeader(const uint8_t **filePtr, uint32_t *fileSize, uint16_t *id)
{
  
  if (!filePtr || !fileSize || !id)
    return 0;

  if (*fileSize < 2)
    {
      fprintf(stderr, "ERROR: fileSize < 2, cannot read\n");
      return 0;
    }
  
  const uint16_t *header = (const uint16_t *)(*filePtr);
  
  switch (*header)
    {
    case JPEG_HEADER_SOI:
      printf("INFO: SOI\n");
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JFIF_HEADER:
      printf("INFO: JFIF\n");
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_COM:
      printf("INFO: COM\n");
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_SOS:
      printf("INFO: SOS\n");
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_SOF0:
      printf("INFO: SOF0\n");
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_SOF2:
      printf("INFO: SOF2\n");
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_DHT:
      printf("INFO: DHT\n");
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_DQT:
      printf("INFO: DQT\n");
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_DRI:
      printf("INFO: DRI\n");
      *filePtr += 2;
      *fileSize -= 2;
      break;
            
    case JPEG_HEADER_EOI:
      printf("INFO: EOI\n");
      *filePtr += 2;
      *fileSize -= 2;
      break;
          
    default:
      fprintf(stderr, "ERROR: Header not detected \n");
      break;
      return 0;
    }

  
  if (id)
    *id = *header;
  
  return *fileSize;
}

uintptr_t parseData(const uint8_t **filePtr, uint32_t *fileSize, uint8_t **buffer, uint16_t *id)
{
  
  if (!filePtr || !fileSize || !id)
    return 0;

  if (*fileSize < 2)
    {
      fprintf(stderr, "ERROR: fileSize < 2, cannot read\n");
      return 0;
    }
  
  switch(*id)
    {
    case JPEG_HEADER_SOI:
      printf("WARNING: No data in SOI to parse... \n");
      break;
      
    case JFIF_HEADER:
      parseBuffer(filePtr, fileSize, buffer);
      break;
      
    case JPEG_HEADER_COM:
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_SOS:
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_SOF0:
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_SOF2:
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_DHT:
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_DQT:
      *filePtr += 2;
      *fileSize -= 2;
      break;
      
    case JPEG_HEADER_DRI:
      *filePtr += 2;
      *fileSize -= 2;
      break;
            
    case JPEG_HEADER_EOI:
      *filePtr += 2;
      *fileSize -= 2;
      break;
          
    default:
      fprintf(stderr, "ERROR: Header not detected \n");
      break;
      return 0;
    }

  return *fileSize;
}


uintptr_t parseBuffer(const uint8_t **filePtr, uint32_t *fileSize, uint8_t **buffer)
{

  if (!filePtr || !fileSize)
    return 0;
  
  if (*fileSize < 2)
    {
      fprintf(stderr, "ERROR: fileSize < 2, cannot read\n");
      return 0;
    }

  const uint16_t dataLength = (endian_fix16( (uint16_t *)(*filePtr) )) - 2;
  *filePtr += 2;
  *fileSize-= 2;

  if (*fileSize < dataLength)
    {
      fprintf(stderr, "ERROR: fileSize < %d, cannot read\n", dataLength);
      return 0;
    }

  uint8_t *bufferNew = realloc( *buffer, (dataLength) * sizeof(uint8_t) );

  if (!bufferNew)
    return 0;

  for (uintptr_t i = 0; i < dataLength; ++i)
    {
      bufferNew[i] = *(*filePtr++);
    }

  *buffer = bufferNew;

  *fileSize -= dataLength;
  
  return *fileSize;
}

