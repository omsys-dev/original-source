
// dcmem.cc provides memory allocation in DOS, JHG, 9/20/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#ifdef DOSMEM
#include "dcglob.h"
#include "dcmem.h"


// global start of chain of heaps


DCmem *FirstHeap = NULL;
DCmem *CurrHeap = NULL;



// heap constructor and destructor


DCmem::DCmem(size_t sz)
{
   mlast = NULL;
   hnext = NULL;

   // allow for DOS MCB at start of seg
   mfree = msize = (sz ? sz : (MSizeLim - MCBSize));

   mnext = mstart = calloc(msize, (size_t) 1);
   mseg = *((uns __far *) &mstart + 1);
}


DCmem::~DCmem()
{
  free(mstart);
}



// global allocation functions


void *
DCmem::mem_new(size_t MemLen)
{
  void *m;

  if (FirstHeap == NULL)
    CurrHeap = FirstHeap = new DCmem;

  if (MemLen > MSizeSmall)
    return (void *) calloc(MemLen, (size_t) 1);

  if (MemLen > CurrHeap->mfree) {
#if 0
    CurrHeap = FirstHeap;
    while (MemLen > CurrHeap->mfree) {
#endif
      if (CurrHeap->hnext == NULL)
        CurrHeap->hnext = new DCmem;
      if ((CurrHeap->hnext == NULL)
       || (CurrHeap->hnext->mstart == NULL))
        return (void *) calloc(MemLen, (size_t) 1);
      CurrHeap = CurrHeap->hnext;
#if 0
    }
#endif
  }

  CurrHeap->mlast = CurrHeap->mnext;
  CurrHeap->mnext = (void *) (((char *)(CurrHeap->mnext)) + MemLen);
  CurrHeap->mfree -= MemLen;
  return CurrHeap->mlast;
}


bool
DCmem::mem_find(void *m)
{
  DCmem *heap;
  uns ms = *((uns __far *) &m + 1);

  if (ms && (ms == CurrHeap->mseg))
    return true;

  for (heap = FirstHeap; heap != NULL; heap = heap->hnext) {
    if (ms == heap->mseg) {
      CurrHeap = heap;
      return true;
    }
  }
  return false;
}


void *
DCmem::mem_resize(void *m, size_t MemLen)
{
  void *n;
  size_t OldLen;

  if (m == NULL)
    return NULL;

#if 0
  if (!mem_find(m))
    return (void *) realloc(m, MemLen);
#endif

  if (m == CurrHeap->mlast) {
    CurrHeap->mfree += (OldLen = ((char *) CurrHeap->mnext - (char *) m));
    CurrHeap->mnext = m;
  }

  if (MemLen < CurrHeap->mfree) {
    CurrHeap->mfree -= MemLen;
    CurrHeap->mnext = (void *) (((char *)(CurrHeap->mnext)) + MemLen);
    return m;
  }

  if (n = mem_new(MemLen))
    memcpy(n, m, OldLen);
  return n;
}

void
DCmem::mem_delete(void *m)
{
  if (m == NULL)
    return;

#if 0
  if (!mem_find(m)) {
    free(m); // not ours, must be system's
    return;
  }
#endif

  if (m == CurrHeap->mlast) {
    CurrHeap->mfree += ((char *) CurrHeap->mnext - (char *) m);
    CurrHeap->mnext = m;
  }
}


#endif
// end of dcmem.cc


