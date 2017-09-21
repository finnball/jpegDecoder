#ifndef TOOLS_H
#define TOOLS_H

void dumpFilePtr(const uint8_t **filePtr, uint32_t n);

static inline void rwFilePtr(const uint8_t **filePtr, const uint32_t n)
{
  (*filePtr)-=n;
}

static inline void ffFilePtr(const uint8_t **filePtr, const uint32_t n)
{
  (*filePtr)+=n;
}


#endif
