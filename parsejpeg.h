#ifndef PARSEJPEG_H
#define PARSEJPEG_H

#include <stdint.h>
#include "definitions.h"

jpeg_t *jpegParse( const uint8_t *fileContents, const uint32_t fileSize );
int parse(const uint8_t **filePtr, uint32_t *fileSize,
	  jpeg_t *jpeg, uint16_t *id);

int parseHeader(const uint8_t **filePtr, uint32_t *fileSize, uint16_t *id);
int parseData(const uint8_t **filePtr, uint32_t *fileSize,
		    jpeg_t *jpeg, const uint16_t *id);
int parseBuffer(const uint8_t **filePtr, uint32_t *fileSize,
		      uint8_t **buffer, uint16_t *bufferLength);
int parseScan(const uint8_t **filePtr, uint32_t *fileSize,
	     uint8_t **buffer, uint16_t *bufferLength);
int parseDHT (const uint8_t **filePtr, uint32_t *fileSize, jpeg_t *jpeg);

#endif
