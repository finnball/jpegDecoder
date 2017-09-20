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
  uintptr_t filePtr = 0;
  uint16_t *id = malloc(sizeof(uint16_t));

  jpeg_t *jpeg = initJpeg();
  
  filePtr = parseHeader(fileContents, filePtr, fileSize, id);
  
  if(*id != JPEG_HEADER_SOI)
    {
      fprintf(stderr, "SOI not found at start \n");
      return NULL;
    }

  filePtr = parseHeader(fileContents, filePtr, fileSize, id);

  if (*id == JFIF_HEADER)
    {
      filePtr = parseData(fileContents, filePtr, fileSize, &(jpeg->jfif) );
      filePtr = parseHeader(fileContents, filePtr, fileSize, id);
    }
  
    if (*id == JPEG_HEADER_COM)
    {
      filePtr = parseData(fileContents, filePtr, fileSize, &(jpeg->com) );
      filePtr = parseHeader(fileContents, filePtr, fileSize, id);
    }
      
  if (*id == JPEG_HEADER_DQT)
    {
      filePtr = parseData(fileContents, filePtr, fileSize, &(jpeg->dqt) );
      filePtr = parseHeader(fileContents, filePtr, fileSize, id);
    }
  
  if (*id == JPEG_HEADER_SOF0)
    {
      filePtr = parseData(fileContents, filePtr, fileSize, &(jpeg->sof0) );
      filePtr = parseHeader(fileContents, filePtr, fileSize, id);
    }

  if (*id == JPEG_HEADER_DHT)
    {
      filePtr = parseData(fileContents, filePtr, fileSize, &(jpeg->dht) );
      filePtr = parseHeader(fileContents, filePtr, fileSize, id);
    }
  
  
  free(id);
  return jpeg;
}


uintptr_t parseHeader(const uint8_t *fileContents, uintptr_t filePtr,
		       const uint32_t fileSize, uint16_t *id)
{
  
  if (!fileContents || fileSize == 0 || (filePtr > fileSize) )
    return 0;

  const uint16_t *header = (const uint16_t *)&fileContents[filePtr];
  
  switch(*header)
    {
    case JPEG_HEADER_SOI:
      printf("INFO: SOI\n");
      filePtr += 2;
      break;
      
    case JFIF_HEADER:
      printf("INFO: JFIF\n");
      filePtr += 2;
      break;
      
    case JPEG_HEADER_COM:
      printf("INFO: COM\n");
      filePtr += 2;
      break;
      
    case JPEG_HEADER_SOS:
      printf("INFO: SOS\n");
      filePtr += 2;
      break;
      
    case JPEG_HEADER_SOF0:
      printf("INFO: SOF0\n");
      filePtr += 2;
      break;
      
    case JPEG_HEADER_SOF2:
      printf("INFO: SOF2\n");
      filePtr += 2;
      break;
      
    case JPEG_HEADER_DHT:
      printf("INFO: DHT\n");
      filePtr += 2;
      break;
      
    case JPEG_HEADER_DQT:
      printf("INFO: DQT\n");
      filePtr += 2;
      break;
      
    case JPEG_HEADER_DRI:
      printf("INFO: DRI\n");
      filePtr += 2;
      break;
            
    case JPEG_HEADER_EOI:
      printf("INFO: EOI\n");
      break;
            
    default:
      fprintf(stderr, "ERROR: Header not detected \n");
      break;
      return 0;
    }

  if (filePtr > fileSize)
    {
      fprintf(stderr, "File pointer larger than file size\n");
      return 0;
    }
  
  if (id)
    *id = *header;
    
  
  return filePtr;
}

uintptr_t parseData(const uint8_t *fileContents, uintptr_t filePtr,
		    const uint32_t fileSize, uint8_t **buffer)
{
  if ( (!fileContents) || (filePtr > fileSize) )
    return 0;
    
  const uint16_t dataLength = (endian_fix16( (uint16_t *)&fileContents[filePtr] )) - 2;
  filePtr += 2;

  
  //printf("%d\n",dataLength);
  uint8_t *bufferNew = realloc( *buffer, (dataLength) * sizeof(uint8_t) );

  if (!bufferNew)
    return 0;

  for (uintptr_t i = 0; i < dataLength; ++i)
    {
      bufferNew[i] = fileContents[filePtr++];

      //     printf(">>%lu %02x\n",i, fileContents[filePtr]);
    }

  *buffer = bufferNew;
  
  return filePtr;
}

