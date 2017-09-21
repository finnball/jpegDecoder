//TODO get proper INFO print system working

#include <stdio.h>
#include <stdint.h>

#include "tools.h"
#include "definitions.h"


void dump(const uint8_t **filePtr, uint32_t n)
{
  
  for (uint32_t i = 0; i < n; ++i)
    {
      printf("INFO: (0x%04x) 0x%02x\n",i, *((*filePtr)++));
    }

  (*filePtr)-=n; // rewind

}


      /*
      else if ( ( (header >> 8) == 0xff) && (header & 0x00e0)) // APPn header: 0xffen
	{
	  const uint16_t header_length = fileContents[i+2] << 8 | fileContents[i+3];x
	  printf("INFO: (0x%04x)  APPn(%04db) 0x%04x\n", i, header_length, header);
	  i+=1;
	}
      
      else if ( ( (header >> 8) == 0xff) && (header & 0x00d0)) // RSTn header: 0xffdn
	{
	  printf("INFO: (0x%04x)  RSTn: 0x%04x \n", i, header);
	  i+=1;
	}

      else if ( ( (header >> 8) == 0xff) && (header & 0x00ff) ) // unknown header, excluding 0xff00
	{
	  printf("INFO: (0x%04x)  UH: 0x%04x \n", i, header);
	  i+=1;
	}
      */
//}
//}
