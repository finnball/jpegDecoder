#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "parsejpeg.h"
#include "definitions.h"
#include "tools.h"

jpeg_t *jpegParse(const uint8_t *fileContents, const uint32_t fileSize)
{
  uint16_t id ;
  uint32_t fileSizePtr;

  fileSizePtr = fileSize;
  
  jpeg_t *jpeg = initJpeg();  
  
  if (parse(&fileContents, &fileSizePtr, jpeg, &id))
    {
      freeJpeg(jpeg);
      return NULL;
    }
  
  if (id != JPEG_HEADER_SOI)
    {
      fprintf(stderr, "SOI not found at start \n");
      freeJpeg(jpeg);
      return NULL;
    }

  while ( (id != JPEG_HEADER_EOI) && fileSizePtr > 0)
    {
      if(parse(&fileContents, &fileSizePtr, jpeg, &id))
	{
	  freeJpeg(jpeg);
	  return NULL;
	}
    }
  
  return jpeg;
}

int parse(const uint8_t **filePtr, uint32_t *fileSizePtr,
	  jpeg_t *jpeg, uint16_t *id)
{
  if (parseHeader(filePtr, fileSizePtr, id))
    return 1;
  
  if (parseData(filePtr, fileSizePtr, jpeg, id))
    return 1;

  return 0;
}


int parseHeader(const uint8_t **filePtr, uint32_t *fileSize, uint16_t *id)
{
  if (!filePtr || !fileSize || !id)
    {
      return 1;
    }

  if (*fileSize < 2)
    {
      fprintf(stderr, "ERROR: fileSize < 2, cannot read\n");
      *id = JPEG_HEADER_EOI;
      return 1;
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
      *filePtr += 1;
      *fileSize -= 1;
      *id = JPEG_HEADER_EOI;
      return 1;      
    }
 
  if (id)
    *id = *header;
  
  return 0;
}

int parseData(const uint8_t **filePtr, uint32_t *fileSize, jpeg_t *jpeg, const uint16_t *id)
{  
  if (!filePtr || !fileSize || !id)
    return 1;

  switch (*id)
    {
    case JPEG_HEADER_SOI:
      
      break;
      
    case JFIF_HEADER:
      
      return (parseBuffer(filePtr, fileSize, &jpeg->jfif, &jpeg->jfifLength));
      
    case JPEG_HEADER_COM:
      
      return (parseBuffer(filePtr, fileSize, &jpeg->com, &jpeg->comLength));
      
    case JPEG_HEADER_SOS:
      
      if (parseBuffer(filePtr, fileSize, &jpeg->sos, &jpeg->sosLength))
	return 1;
      if (parseScan(filePtr, fileSize, &jpeg->scan, &jpeg->scanLength))
	return 1;
      return 0;
      
    case JPEG_HEADER_SOF0:
      
      return (parseBuffer(filePtr, fileSize, &jpeg->sof0, &jpeg->sof0Length));
      
    case JPEG_HEADER_SOF2:
      
      return (parseBuffer(filePtr, fileSize, &jpeg->sof2, &jpeg->sof2Length));
      
    case JPEG_HEADER_DQT:
      
      return (parseMultipleBuffer(filePtr, fileSize, &jpeg->dqt, &jpeg->dqtLength, &jpeg->dqtN));
      
    case JPEG_HEADER_DHT:

      return (parseMultipleBuffer(filePtr, fileSize, &jpeg->dht, &jpeg->dhtLength, &jpeg->dhtN));
      
    case JPEG_HEADER_DRI:
      
      printf("ERROR: DRI not supported\n");
      return 1;
            
    case JPEG_HEADER_EOI:
      
      break;
          
    default:
      
      fprintf(stderr, "ERROR: Header not detected \n");
      return 1;
    }
  return 0;
}

int parseBuffer(const uint8_t **filePtr, uint32_t *fileSize,
		      uint8_t **buffer, uint16_t *bufferLength)
{
  if (!filePtr || !fileSize || !bufferLength)
    return 1;
  
  if (*fileSize < 2)
    {
      fprintf(stderr, "ERROR: fileSize < 2, cannot read\n");
      return 1;
    }
  
  const uint16_t dataLength = (endian_fix16( (uint16_t *)(*filePtr) )) - 2;
  
  *filePtr += 2;
  *fileSize-= 2;
  
  if (*fileSize < dataLength)
    {
      fprintf(stderr, "ERROR: fileSize (%d) < dataLength (%d), cannot read\n", *fileSize, dataLength);
      return 1;
    }

  uint8_t *bufferNew = realloc( *buffer, (dataLength) * sizeof(uint8_t) );

  if (!bufferNew)
    return 1;

  for (uintptr_t i = 0; i < dataLength; ++i)
    {
      bufferNew[i] = *((*filePtr)++);
    }

  *buffer = bufferNew;

  *fileSize -= dataLength;

  *bufferLength = dataLength;
  
  return 0;
}
 
int parseScan(const uint8_t **filePtr, uint32_t *fileSize,
	     uint8_t **buffer, uint16_t *bufferLength)
{
  if (!filePtr || !fileSize || !bufferLength)
    return 1;
  
  if (*fileSize < 2)
    {
      fprintf(stderr, "ERROR: fileSize < 2, cannot read\n");
      return 1;
    }
  
  const uint16_t dataLength = *fileSize - 2;
  
  uint8_t *bufferNew = realloc( *buffer, (dataLength) * sizeof(uint8_t) );

  if (!bufferNew)
    return 1;

  for (uintptr_t i = 0; i < dataLength; ++i)
    {
      bufferNew[i] = *((*filePtr)++);
    }

  *buffer = bufferNew;

  *fileSize -= dataLength;

  *bufferLength = dataLength;
  
  return 0;
}

int parseMultipleBuffer (const uint8_t **filePtr, uint32_t *fileSize,
	      uint8_t ***buffer, uint16_t **bufferLength, uint16_t *bufferNumber)
{  
  if (!filePtr || !fileSize || !buffer || !bufferLength || !bufferNumber)
    return 1;
  
  if (*fileSize < 2)
    {
      fprintf(stderr, "ERROR: fileSize < 2, cannot read\n");
      return 1;
    }
  const uint16_t dataLengthNewConst = (endian_fix16( (uint16_t *)(*filePtr) )) - 2;
  
  const uint16_t dataN = (*bufferNumber) + 1;
  
  uint16_t *dataLengthNew = realloc(*bufferLength, dataN * sizeof(uint16_t));
  if (!dataLengthNew) 
    return 1;
  
  uint8_t **dataNew = realloc(*buffer,
			      (dataLengthNewConst * sizeof(uint8_t)) + sizeof(**buffer));
  if (!dataNew)
    return 1;
  
  dataNew[dataN - 1] = NULL;

  if(parseBuffer(filePtr, fileSize, &dataNew[dataN - 1], &dataLengthNew[dataN - 1]))
    return 1;

  *buffer = dataNew;
  *bufferLength = dataLengthNew;
  *bufferNumber = dataN;
  
  return 0;
}
