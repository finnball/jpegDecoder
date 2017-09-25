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

typedef struct node_t
{
  uint8_t value;
  struct node_t *l;
  struct node_t *r;
} node_t;

typedef struct
{
  uint16_t jfifLength, comLength, sof0Length, sosLength, sof2Length, scanLength;
  uint8_t *jfif, *com, *sof0, *sos, *sof2, *scan;
  uint16_t dhtN, dqtN;
  uint16_t *dhtLength, *dqtLength;
  uint8_t **dht, **dqt;
} jpeg_t;

typedef struct
{
  uint8_t nComp;
  uint8_t *idComp;
  uint8_t *idHTable;
}sos_t;

typedef struct
{
  uint8_t id; // id e.g Y or colour                                                                     
  uint8_t type; // 0:DC 1:AC                                                                            
  uint16_t nSymbols[16];
  uint8_t *symbols;
}ht_t;

typedef struct
{
  uint8_t precision;
  uint16_t height;
  uint16_t width;
  uint8_t nComp;
  uint8_t *idComp;
  uint8_t *factor;
  uint8_t *idQt;
}sof0_t;


jpeg_t *initJpeg();
int freeJpeg(jpeg_t *jpeg);
void treeInsert(node_t **node, uint8_t data);
node_t *treeSearch(node_t *node, uint8_t data);
void treeFree(node_t *node);
#endif
