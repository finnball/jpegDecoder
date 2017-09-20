#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "decodejpeg.h"
#include "definitions.h"
#include "tools.h"

int jpegDecode(jpeg_t *jpeg)
{
  printf("%s\n", jpeg->com);
  return 0;
}
