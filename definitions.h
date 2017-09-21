#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>


#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
typedef enum
  {    
    JPEG_HEADER_SOI = 0xffd8,
    JPEG_HEADER_SOS = 0xffda,
    JPEG_HEADER_SOF0 = 0xffc0,
    JPEG_HEADER_SOF2 = 0xffc2,
    JPEG_HEADER_DHT = 0xffc4,
    JPEG_HEADER_DQT = 0xffdb,
    JPEG_HEADER_DRI = 0xffdd,
    JPEG_HEADER_COM = 0xfffe,
    JPEG_HEADER_EOI = 0xffd9,
  }jpeg_header_e;

static const uint16_t JFIF = 0xffe0;
static const uint64_t JFIF_ID = 0x4A46494600;

#define endian_fix16(x)

#elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)

typedef enum
  {
    JPEG_HEADER_SOI = 0xd8ff,
    JPEG_HEADER_SOS = 0xdaff,
    JPEG_HEADER_SOF0 = 0xc0ff,
    JPEG_HEADER_SOF2 = 0xc2ff,
    JPEG_HEADER_DHT = 0xc4ff,
    JPEG_HEADER_DQT = 0xdbff,
    JPEG_HEADER_DRI = 0xddff,
    JPEG_HEADER_COM = 0xfeff,
    JPEG_HEADER_EOI = 0xd9ff,
    JFIF_HEADER = 0xe0ff,
    
  }jpeg_header_e;

static const uint64_t JFIF_ID = 0x004649464A;

static inline uint16_t endian_fix16(uint16_t* x)
{
  return ((*x << 8) | (*x >> 8));
}

#else

#error "ERROR: Only big and little endian supported"

#endif

typedef struct
{
  uint16_t jfifLength, comLength, dqtLength, sof0Length, dhtLength, sosLength, sof2Length, scanLength;
  uint8_t *jfif, *com, *dqt, *sof0, *dht, *sos, *sof2, *scan;
} jpeg_t;

jpeg_t *initJpeg();
int freeJpeg(jpeg_t *jpeg);


#endif
