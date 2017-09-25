#ifndef DECODEJPEG_H
#define DECODEJPEG_H

#include <stdint.h>
#include "definitions.h"

int jpegDecode(const jpeg_t *jpeg);

int decode(const uint8_t *data);
sos_t decodeSOS(const uint8_t *data);
sof0_t decodeSof0(const uint8_t *data);
ht_t decodeHT(const uint8_t *data, const uint16_t length);

#endif
