#include <malloc.h>
#include "definitions.h"


/*uint16_t endian_fix16(uint16_t* x)
{

}*/

jpeg_t *initJpeg() // add checks
{
  jpeg_t *jpeg = malloc(sizeof(jpeg_t));

  if (!jpeg)
    return NULL;

  jpeg->jfif = NULL;
  jpeg->com = NULL;
  jpeg->dqt = NULL;
  jpeg->sof0 = NULL;
  jpeg->dht = NULL;
  
  return (jpeg == NULL) ? NULL : jpeg; 
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

  
  free(jpeg);

  return 0;
}


