#ifndef DECODEJPEG_H
#define DECODEJPEG_H

#include <stdint.h>
#include "definitions.h"

int jpegDecode(const jpeg_t *jpeg);

int decodeHT(const uint8_t *data, const uint16_t length);

#endif
