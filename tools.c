//TODO get proper INFO print system working

#include <stdio.h>
#include <stdint.h>

#include "tools.h"
#include "definitions.h"

void dump(const uint8_t *fileContents, uint32_t start, uint32_t end)
{
  uint32_t ctr = 0;
  for(uint32_t i = start; i < end; ++i)
    {
      printf("INFO: %d(0x%04x)%s 0x%02x\n", ctr++, i, "    ", fileContents[i]);
    }
}

void dumpHeaders(const uint8_t *fileContents, uint32_t start, uint32_t end)
{
  for(uint16_t i = start; i < end; ++i)
    {
      
      const uint16_t *header = (const uint16_t*)&fileContents[i];

      if ( ( *header & 0x00ff ) == 0x00ff )
	{
	  printf("INFO: (0x%08x) ", i);
	  ++i;
      
	  switch (*header)
	    {
	    case JPEG_HEADER_SOI:
	      printf("SOI\n");
	      break;
	    case JPEG_HEADER_SOS:
	      printf("SOS\n");
	      break;
	    case JPEG_HEADER_SOF0:
	      printf("SOf0\n");
	      break;
	    case JPEG_HEADER_SOF2:
	      printf("SOF2\n");
	      break;
	    case JPEG_HEADER_DHT:
	      printf("DHT\n");
	      break;
	    case JPEG_HEADER_DQT:
	      printf("DQT\n");
	      break;
	    case JPEG_HEADER_DRI:
	      printf("DRI\n");
	      break;
	    case JPEG_HEADER_COM:
	      printf("COM\n");
	      break;
	    case JPEG_HEADER_EOI:
	      printf("EOI\n");
	      break;
	    case JFIF_HEADER:
	      printf("JFIF HEADER\n");
	      break;
	    default:
	      printf("Header: 0x%x \n",*header);	      
	      break;
	    }
	}
    }
  return;
}



      /*
      else if ( ( (header >> 8) == 0xff) && (header & 0x00e0)) // APPn header: 0xffen
	{
	  const uint16_t header_length = fileContents[i+2] << 8 | fileContents[i+3];
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
