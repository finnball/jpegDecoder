#include <stdio.h>
#include <stdint.h>

#include "tools.h"
#include "definitions.h"

void dumpFilePtr(const uint8_t **filePtr, uint32_t n)
{
  
  for (uint32_t i = 0; i < n; ++i)
    {
      printf("INFO: (0x%04x) 0x%02x\n",i, *((*filePtr)++));
    }

  rwFilePtr(filePtr, n);

}




