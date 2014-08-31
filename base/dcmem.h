
// dcmem.h is the header for dcl memory classes in DOS, JHG, 9/20/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#ifndef DCMEM_H
#define DCMEM_H

#ifdef DOSMEM


// dcl mem handling


// allocate a heap from DOS, suballocate from it

class DCmem {
 protected:
  size_t msize;
  size_t mfree;
  uns mseg;
  void *mstart;
  void *mlast;
  void *mnext;
  DCmem *hnext;
  enum mem_enum { MCBSize = 16, MSizeSmall = 16384, MSizeLim = 49152 };

  static bool mem_find(void *v);

 public:
  DCmem(size_t sz = 0);
  virtual ~DCmem();

  static void *mem_new(size_t sz = 0);
  static void *mem_resize(void *v, size_t sz);
  static void mem_delete(void *v);

  void *operator new(size_t sz) { return calloc(sz, (size_t) 1); }
  void operator delete(void *v) { free(v); }
};


#endif
#endif

