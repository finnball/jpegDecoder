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

  ht_t ht_0 = decodeHT(jpeg->dht[0], jpeg->dhtLength[0]);
  ht_t ht_1 = decodeHT(jpeg->dht[1], jpeg->dhtLength[1]);
  sos_t sos = decodeSOS(jpeg->sos);
  sof0_t sof0 = decodeSof0(jpeg->sof0);

  decode(jpeg->scan);

  free(ht_0.symbols);
  free(ht_1.symbols);
  free(sos.idComp);
  free(sos.idHTable);
  free(sof0.idComp);
  free(sof0.factor);
  free(sof0.idQt);

  return 0;
}

int decode(const uint8_t *data)
{
  if(!data)
    return 1;
  
  for(int i = 0; i < 16; ++i)
    printf("0x%02x\n", *data++);

  return 0;
}

sof0_t decodeSof0(const uint8_t *data)
{
  sof0_t sof0;
  sof0.idComp = NULL;
  sof0.factor = NULL;
  sof0.idQt = NULL;

  if (!data)
    return sof0;

  const uint8_t precision = *data++;

  if (precision > 8)
    {
      fprintf(stderr, "ERROR: Precision %d not supported", precision);
      return sof0;
    }
  
  const uint8_t height = *data++;
  const uint8_t width = *data++;
  const uint8_t nComp = *data++;

  sof0.idComp = malloc(nComp * sizeof(uint8_t));
  if (!sof0.idComp)
    return sof0;
  
  sof0.factor = malloc(nComp * sizeof(uint8_t));
  if (!sof0.factor)
    return sof0;
  
  sof0.idQt = malloc(nComp * sizeof(uint8_t));
  if (!sof0.idQt)
    return sof0;


  for (uint8_t i = 0; i < nComp; ++i)
    {
      sof0.idComp[i] = *data++;
      sof0.factor[i] = *data++;
      sof0.idQt[i] = *data++;
    }
  
  sof0.precision = precision;
  sof0.height = height;
  sof0.width = width;
  sof0.nComp = nComp;

  return sof0;
}

sos_t decodeSOS(const uint8_t *data)
{
  sos_t sos;
  sos.idComp = NULL;
  sos.idHTable = NULL;

  if (!data)
    return sos;
  
  const uint8_t nComp = *data++;

  if (nComp < 1 || nComp > 4)
    return sos;

  sos.idComp = malloc(nComp * sizeof(uint8_t));
  if (!sos.idComp)
    return sos;

  sos.idHTable = malloc(nComp * sizeof(uint8_t));
  if (!sos.idHTable)
    return sos;

  for (uint8_t i = 0; i < nComp; ++i)
    {
      sos.idComp[i] = data[i];
      sos.idHTable[i] = data[i];
    }

  sos.nComp = nComp;
  return sos;
}

ht_t decodeHT(const uint8_t *data, const uint16_t length)
{
  ht_t ht;
  ht.symbols = NULL;

  if (! data)
    return ht;
  
  const uint16_t dhtLength = length;
  const uint8_t sizeNSymbols = 16;

  const uint8_t id = (*data) & 0x0f;
  const uint8_t type = (*data >> 4);
  
  if (type > 1)
    {
      fprintf(stderr, "ERROR: Unknown HT type: %0x02x\n", ht.type);
    }

  data++;
  
  for (uint8_t i = 0; i < sizeNSymbols; ++i)
    {
      ht.nSymbols[i] = *data++;
    }

  if (dhtLength < (sizeNSymbols - 1))
    return ht;
    
  const uint16_t sizeHTTable = dhtLength - sizeNSymbols - 1;

  ht.symbols = malloc(sizeHTTable * sizeof(uint8_t));
  
  if (!ht.symbols)
    return ht;

  for(uint16_t i = 0; i < sizeHTTable; ++i)
    {
      ht.symbols[i] = *data++;
    }

  printf("\n");

  printf("HTTable:\n");

  uint16_t ctr = 0;
  for (uint8_t i = 0; i < sizeNSymbols; ++i)
    {
      printf("%d    ", 1+i);
      for(int j = 0; j < ht.nSymbols[i]; ++j)
	{
	  printf("0x%02x ", ht.symbols[ctr]);
	  ctr++;
	}
      printf("\n");
    }

  ht.id = id;
  ht.type = type;

  return ht;
}
