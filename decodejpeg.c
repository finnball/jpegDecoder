#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "decodejpeg.h"
#include "definitions.h"
#include "tools.h"

int jpegDecode(const jpeg_t *jpeg)
{
  if (!jpeg)
    return 1;

  decodeHT(jpeg->dht[0], jpeg->dhtLength[0]);
  printf("\n\n");
  decodeHT(jpeg->dht[1], jpeg->dhtLength[1]);
  
  return 0;
}

int decodeHT(const uint8_t *data, const uint16_t length)
{
  ht_t ht;

  if (! data)
    return 1;
  
  const uint16_t dhtLength = length;
  const uint8_t sizeNSymbols = 16;
  
  ht.id = (*data) & 0x0f;
  ht.type = (*data >> 4);

  if (ht.type > 1)
    {
      fprintf(stderr, "ERROR: Unknown HT type: %0x02x\n", ht.type);
    }

  data++;

  printf("id>%02x\n", ht.id);
  printf("type>%02x\n", ht.type);
  printf("length>%02x\n", length);
  
  
  for (int i = 0; i < sizeNSymbols; ++i)
    {
      ht.nSymbols[i] = *data++;
      //printf("%02x\n", ht.nSymbols[i]);
    }

  if (dhtLength < (sizeNSymbols - 2))

  for(int i = 0; i < (dhtLength - sizeNSymbols - 2); ++i)
    {
      ht.symbols[i] = *data++;
      printf("%02x\n", *data);
    }

  //  printf("#%02x\n", ht.symbols[i]);

  return 0;
}
