#include <malloc.h>
#include "definitions.h"

jpeg_t *initJpeg()
{
  jpeg_t *jpeg = malloc(sizeof(jpeg_t));

  if (!jpeg)
    return NULL;
  
  jpeg->jfif = NULL;
  jpeg->com = NULL;
  jpeg->sof0 = NULL;
  jpeg->sof2 = NULL;
  jpeg->sos = NULL;
  jpeg->scan = NULL;

  jpeg->dhtN = 0;
  jpeg->dhtLength = NULL;
  jpeg->dht = NULL;

  jpeg->dqtN = 0;
  jpeg->dqtLength = NULL;
  jpeg->dqt = NULL;

  return jpeg; 
}

int freeJpeg(jpeg_t *jpeg)
{
  if(!jpeg)
    return 1;
  
  if (jpeg->jfif)
    free(jpeg->jfif);

  if (jpeg->com)
    free(jpeg->com);

  if (jpeg->dhtLength)
    free(jpeg->dhtLength);

  for (uint16_t i = 0; i < jpeg->dhtN; ++i)
    {
      if (jpeg->dht[i])
	free(jpeg->dht[i]);
    }
  
  if (jpeg->dht)
    free(jpeg->dht);

  if (jpeg->dqtLength)
    free(jpeg->dqtLength);
  
  for (uint16_t i = 0; i < jpeg->dqtN; ++i)
    {
      if (jpeg->dqt[i])
	free(jpeg->dqt[i]);
    }
  
  if (jpeg->dqt)
    free(jpeg->dqt);
  
  if (jpeg->sof0)
    free(jpeg->sof0);

  if (jpeg->sof2)
    free(jpeg->sof2);

  if (jpeg->sos)
    free(jpeg->sos);

  if (jpeg->scan)
    free(jpeg->scan);

  free(jpeg);

  return 0;
}


node_t *treeSearch(node_t *node, uint8_t data)
{
  if (!node)
    return NULL;
  
  if ( data == (node->value) )
    return node;
  
  else if (data < node->value )
    return treeSearch((node->l), data);

  else
    return treeSearch((node->r), data);

  return NULL;
}

void treeInsert(node_t **node, uint8_t data)
{
  if (!*node)
    {
      *node = malloc(sizeof(node_t));
      if (!*node)
	return;
      (*node)->l = NULL;
      (*node)->r = NULL;
      (*node)->value = data;
    }

  else if (data < (*node)->value)
    treeInsert(&(*node)->l, data);

  else if (data > (*node)->value)
    treeInsert(&(*node)->r, data);

  return;
}

void treeFree(node_t *node)
{
  if (!node)
    return;

  if (node->l)
    treeFree(node->l);

  if (node->r)
    treeFree(node->r);

  free(node);

  return;
}
