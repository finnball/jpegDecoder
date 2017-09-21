#include <malloc.h>
#include "definitions.h"

jpeg_t *initJpeg()
{
  jpeg_t *jpeg = malloc(sizeof(jpeg_t));

  if (!jpeg)
    return NULL;

  jpeg->jfif = NULL;
  jpeg->com = NULL;
  jpeg->dqt = NULL;
  jpeg->sof0 = NULL;
  jpeg->dht = NULL;
  jpeg->sof2 = NULL;
  jpeg->sos = NULL;
  
  return jpeg; 
}

int freeJpeg(jpeg_t *jpeg)
{
  if(!jpeg)
    return -1;
  
  if (jpeg->jfif)
    free(jpeg->jfif);

  if (jpeg->com)
    free(jpeg->com);

  if (jpeg->dqt)
    free(jpeg->dqt);

  if (jpeg->sof0)
    free(jpeg->sof0);

  if (jpeg->dht)
    free(jpeg->dht);

  if (jpeg->sof2)
    free(jpeg->sof2);

  if (jpeg->sos)
    free(jpeg->sos);
  
  free(jpeg);

  return 0;
}


