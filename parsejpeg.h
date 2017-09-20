#ifndef PARSEJPEG_H
#define PARSEJPEG_H

#include <stdint.h>
#include "definitions.h"

jpeg_t *jpegParse( const uint8_t *fileContents, const uint32_t fileSize );

uintptr_t parseHeader(const uint8_t **filePtr, uint32_t *fileSize, uint16_t *id);
uintptr_t parseData(const uint8_t **filePtr, uint32_t *fileSize, uint8_t **buffer, uint16_t *id);
uintptr_t parseBuffer(const uint8_t **filePtr, uint32_t *fileSize, uint8_t **buffer);

#endif
