
// dcglob.cc provides global application-level services, JHG, 4/9/97
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.


#ifdef VC4
#define bool unsigned char
#define false 0
#define true 1
#endif

#include "dcglob.h"

#ifdef DOSMEM
#include "dcmem.h"
#endif

#pragma warning( disable : 4244 4514 )

// global endian indexes, default is big-endian

unc dc_msb_index = 0;
unc dc_lsb_index = 1;   
dc_ftyp dc_mach;  // machine int storage type


// global debug controls on deallocation
bool NoMemDel = false;
bool NoNameDel = false;



// global allocation functions


void *
::operator new(size_t BlockLen)
{
  return NewMem(BlockLen);
}

void
::operator delete(void *c)
{
  DeleteMem(c);
}


// string allocation functions

 
char *
NewName(short NameLen)
{
  char *c;

  c = (char *) NewMem( (size_t) NameLen + 1);
  return c;
}

char *
NewName(const char *str, size_t len, bool mem)
{
  char *c;

  if (!str)
    return NULL;
  if (!len)
    len = strlen(str);
  
  c = (char *) NewMem(len + 1);

	if (len) {
		if (mem)
			memcpy(c, str, len);
		else
			strncpy(c, str, len);
	}
	else
		*c = '\0';

  return c;
}


void
IncName(char *c, bool inc)
{
	// increment from right, digits or letters
	char *p = c + strlen(c) - 1;
	while (p >= c) {
		if (isdigit(*p)) {
			if (inc) {
				if (*p == '9')
					*p = '0';
				else {
					*p = *p + 1;
					return;
				}
			}
			else {
				if (*p == '0')
					*p = '9';
				else {
					*p = *p - 1;
					return;
				}
			}
		}
		else if (isupper(*p)) {
			if (inc) {
				if (*p == 'Z')
					*p = 'A';
				else {
					*p = *p + 1;
					return;
				}
			}
			else {
				if (*p == 'A')
					*p = 'Z';
				else {
					*p = *p - 1;
					return;
				}
			}
		}
		else if (islower(*p)) {
			if (inc) {
				if (*p == 'z')
					*p = 'a';
				else {
					*p = *p + 1;
					return;
				}
			}
			else {
				if (*p == 'a')
					*p = 'z';
				else {
					*p = *p - 1;
					return;
				}
			}
		}
		else
			return;
		p--;
	}
}


void
DeleteName(char *c)
{
	if (NoNameDel)
		return;

  DeleteMem( (void *) c);
}



// all other allocation funcs wind up here

unl mem_total = 0;
unl mem_count = 0;
unl mem_freed = 0;


void *
NewMem(size_t MemLen)
{
  void *m;

#ifdef DOSMEM
  if ((m = (void *) DCmem::mem_new(MemLen)) == NULL)
#else
  if ((m = (void *) calloc(MemLen, (size_t) 1)) == NULL)
#endif
    MemErr("allocation", MemLen);
  else  {
    mem_total += MemLen;
    mem_count++;
  }
  return m;
}

void *
ResizeMem(void *m, size_t MemLen)
{
#if 0
  void *n = NewMem(MemLen);
	DeleteMem(m);
	return n;
#endif

  void *n;
#ifdef DOSMEM
  if ((n = (void *) DCmem::mem_resize(m, MemLen)) == NULL)
#else
  if ((n = (void *) realloc(m, MemLen)) == NULL)
#endif
		{
			if (MemLen > 0)
		    MemErr("reallocation", MemLen);
			else if (m != NULL)
				mem_freed++;
		}
  else  {
    mem_total += MemLen;
    mem_count++;
  }
  return n;
}

void
DeleteMem(void *m)
{
	if (NoMemDel)
		return;

#ifdef DOSMEM
  DCmem::mem_delete(m);
#else
  free(m);
#endif
  mem_freed++;
}

void
MemErr(const char *c, size_t l)
{
  fprintf(stderr, "\nFatal memory error: %s of %lu.\n",
                                         c,    (unl) l);
  fprintf(stderr, "Total requested %lu, count %lu, freed %lu.\n",
                                   mem_total, mem_count, mem_freed);
  exit(dc_err_mem);
}


// Unicode encoding functions

unc ::IsUTF8(unc *cpt)
{
	if (!cpt)
		return 0;

	if ((*cpt & 0xF8) == 0xF0) { // start of 4-byte sequence
		if (((*(cpt + 1) & 0xC0) == 0x80)
		 && ((*(cpt + 2) & 0xC0) == 0x80)
		 && ((*(cpt + 3) & 0xC0) == 0x80))
			return 4;
	}
	else if ((*cpt & 0xF0) == 0xE0) { // start of 3-byte sequence
		if (((*(cpt + 1) & 0xC0) == 0x80)
		 && ((*(cpt + 2) & 0xC0) == 0x80))
			return 3;
	}
	else if ((*cpt & 0xE0) == 0xC0) { // start of 2-byte sequence
		if ((*(cpt + 1) & 0xC0) == 0x80)
			return 2;
	}
	return 0;
}

unl ::UTF8(unc **cpt)
{
	unl uch = 0;

	if (!cpt || !*cpt)
		return 0;

	if ((**cpt & 0xF8) == 0xF0) { // start of 4-byte sequence
		uch = (*(*cpt)++ & 0x7) << 18;
		uch |= (*(*cpt)++ & 0x3F) << 12;
		uch |= (*(*cpt)++ & 0x3F) << 6;
		uch |= (*(*cpt)++ & 0x3F);
	}
	else if ((**cpt & 0xF0) == 0xE0) { // start of 3-byte sequence
		uch = (*(*cpt)++ & 0xF) << 12;
		uch |= (*(*cpt)++ & 0x3F) << 6;
		uch |= (*(*cpt)++ & 0x3F);
	}
	else if ((**cpt & 0xE0) == 0xC0) { // start of 2-byte sequence
		uch = (*(*cpt)++ & 0x1F) << 6;
		uch |= (*(*cpt)++ & 0x3F);
	}
	return uch;
}

unc *::UTF8(unl uch, unc *cpt)
{
	static unc ubuf[5] = { 0, 0, 0, 0, 0 };
	unc *ret = NULL;
	if (!cpt)
		ret = cpt = ubuf;

	if (uch < 0x80)
		*cpt++ = (unc) (uch & 0xFF);
	else if (uch < 0x0800) {
		*cpt++ = (unc) (0xC0 | ((uch >> 6) & 0x1F));
	  *cpt++ = (unc) (0x80 | (uch & 0x3F));
	}
	else if (uch < 0x10000) {
		*cpt++ = (unc) (0xE0 | ((uch >> 12) & 0xF));
		*cpt++ = (unc) (0x80 | ((uch >> 6) & 0x3F));
	  *cpt++ = (unc) (0x80 | (uch & 0x3F));
	}
	else {
		*cpt++ = (unc) (0xF0 | ((uch >> 18) & 0x7));
		*cpt++ = (unc) (0x80 | ((uch >> 12) & 0x3F));
		*cpt++ = (unc) (0x80 | ((uch >> 6) & 0x3F));
	  *cpt++ = (unc) (0x80 | (uch & 0x3F));
	}
	*cpt = '\0';
	return (ret ? ret : cpt);
}


// global file name utilities


void ::GetBaseName(char *fname, char *basepath, char *basename)
{
	if (!fname)
		return;
	char *cp = NULL;

	if (((cp = strrchr(fname, '\\')) != NULL)
	 || ((cp = strrchr(fname, '/')) != NULL) 
	 || ((cp = strrchr(fname, ':')) != NULL)) {
		cp++;  // step to char after the delimiter
		if (basepath) {
			strncpy(basepath, fname, cp - fname); // the path and drive
			basepath[cp - fname] = '\0';
		}
		if (basename)
			strcpy(basename, cp);	        // the filename and ext
	}
	else {
		if (basepath)
			*basepath = '\0';
		if (basename)
			strcpy(basename, fname);
	}
	if (basename
	 && ((cp = strrchr(basename, '.')) != NULL))
		*cp = '\0';  // trim off the extension, so we have just base name
}


// end of dcglob.cc


