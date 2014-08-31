
// dcbase.cc contains code for the base dcl classes, JHG
// Copyright (c) 1993-2001 by Omni Systems, Inc.  All rights reserved.


#include "dcglob.h"
#include "dcbase.h"
#include "dcnode.h"

#pragma warning( disable : 4244 4514 )

// basic DCctl member functions

void *
DCctl::ex(const void *e, size_t l)
{
  if (ext)
    delete [] ext;
  if (l == 0)
    l = size;
  ext = (void *) new char[l];
  if (ext && e) {
    memcpy(ext, e, l);
    size = l;
  }
  return ext;
}


void *
DCctl::ex(const char *e)
{
	return ex((void*) e, (e ? strlen(e) : 0) + 1);
}


DCctl& 
DCctl::operator= (DCctl& c)
{ 
	dtype = c.dtype; 
	major = c.major; 
	minor = c.minor; 
	data = c.data;
  if (ext) 
		delete [] ext; 
	size = c.size; 
	ext = NULL;
	if (size
	 && c.ext
	 && (dtype >= ebyte)
	 && (dtype <= ecode))
		memcpy(ext = new char[size], c.ext, size);
	return *this; 
}


DCctl& 
DCctl::operator= (DCblock& b)
{ 
	if (ext) 
		delete [] ext; 
	ext = b.p(); 
	size = b.l(); 
	b.clear();
	return *this; 
}


bool
DCctl::match(uns mj, uns mn, uns sdat, unl siz)
{
	if ((major != mj)
	 || (minor != mn)
	 || (sdat && (sdat != data))
	 || (siz && (siz != size)))
		return false;
	return true;
}


short
DCctl::cmp(DCctl& c)
{
	if (&c == NULL)
		return 1;
	if (dtype != c.dtype)
		return (dtype < c.dtype) ? -1 : 1;
	if (major != c.major)
		return (major < c.major) ? -1 : 1;
	if (minor != c.minor)
		return (minor < c.minor) ? -1 : 1;
	if (data != c.data)
		return (data < c.data) ? -1 : 1;
	if (size) {
		switch (dtype) {
			case ibyte:
			case ishort:
			case ilong:
			case imnem:
			case stamp:
				if (size != c.size)
					return (size < c.size) ? -1 : 1;
				break;
			case ebyte:
			case eshort:
			case elong:
			case edoub:
				if (size != c.size)
					return (size < c.size) ? -1 : 1;
				if (!ext || !c.ext)
					return 1;
				return memcmp(ext, c.ext, size);
				break;
			case ename:
			case etext:
			case ecode:
				if (size != c.size)
					return (size < c.size) ? -1 : 1;
				if (!ext || !c.ext)
					return 1;
				return _strnicmp((char *) ext, (char *) c.ext, size);
				break;
			default:  // difference doesn't matter for others
				break;
		}
	}
	return 0;
}

void
DCctl::nprint()
{
  // of limited use; no type or ext info. aprint() is preferred.
  printf("(%hu %hu %hu %lu).\n", (uns) major, (uns) minor, data, size);
}


// DCctl name-conversion functions

uns
DCctl::getMajor(char *c)
{
  DCnode *n;

  if (c == NULL)
    return 0;
  if (isdigit(*c))
    return (uns) atoi(c);
  if ((n = DCnode::FindMajor(c)) != NULL)
    return n->val();
  return 0;
}

char *
DCctl::getMajor(uns v)
{
  DCnode *n;
  static char buf[12];

  if (v == 0)
    v = major;
  if ((n = DCnode::FindMajor(v)) != NULL)
    return n->nam();
  sprintf(buf, "%hu", v);
  return buf;
}

uns
DCctl::getMinor(char *c, uns majval)
{
  DCnode *n;

  if (c == NULL)
    return 0;
  if (isdigit(*c))
    return (uns) atoi(c);
  if (majval == 0)
    majval = major;
  if ((n = DCnode::FindMinor(c, majval)) != NULL)
    return n->val();
  return 0;
}

char *
DCctl::getMinor(uns v, uns majval)
{
  DCnode *n;
  static char buf[12];

  if (v == 0)
    v = minor;
  if (majval == 0)
    majval = major;
  if ((n = DCnode::FindMinor(v, majval)) != NULL)
    return n->nam();
  sprintf(buf, "%hu", v);
  return buf;
}


uns
DCctl::getData(char *c, uns minval, uns majval)
{
  DCnode *n;

  if (c == NULL)
    return 0;
  if (isdigit(*c))
    return (uns) atol(c);
  if (majval == 0)
    majval = major;
  if (minval == 0)
    minval = minor;
  if ((n = DCnode::FindData(c, minval, majval)) != NULL)
    return n->val();
  return 0;
}

char *
DCctl::getData(uns v, uns minval, uns majval)
{
  DCnode *n;
  static char buf[12];

  if (v == 0)
    return "0";
  if (majval == 0)
    majval = major;
  if (minval == 0)
    minval = minor;
  if ((n = DCnode::FindData(v, minval, majval)) != NULL)
    return n->nam();
  sprintf(buf, "%hu", v);
  return buf;
}

unl
DCctl::getSize(char *c, uns datval, uns minval, uns majval)
{
  DCnode *n;

  if (c == NULL)
    return 0;
  if ((*c == '-') || isdigit(*c))
    return (unl) atol(c);
  if (majval == 0)
    majval = major;
  if (minval == 0)
    minval = minor;
  if (datval == 0)
    datval = data;
  if ((n = DCnode::FindSize(c, datval, minval, majval)) != NULL)
    return n->val();
  return 0;
}

char *
DCctl::getSize(unl v, uns datval, uns minval, uns majval)
{
  DCnode *n;
  static char buf[12];

  if (majval == 0)
    majval = major;
  if (minval == 0)
    minval = minor;
  if (datval == 0)
    datval = data;
  if ((n = DCnode::FindSize( (uns) v, datval, minval, majval)) != NULL)
    return n->nam(); // mnemonics can only be defined for uns or smaller 
  if (v == 0)
    return "";
  sprintf(buf, "%lu", v);
  return buf;
}

static char *Dtypes[] = {
  "Intern bytes", "Intern shorts", "Intern long", "Mnemonic",
  "Ext bytes", "Ext shorts", "Ext longs", "Ext doubles",
  "Ext name", "Ext text", "Ext code", "Timestamp",
  "Reserved 12", "Reserved 13", "End group", "Start group"
};


void
DCctl::append(char *pt)
{
	if (!pt)
		return;
	pt += strlen(pt);

	switch (dtype) {
		case imnem:
		case ibyte:
			*pt++ = (char) (size & 0xFF);
			break;

		case etext:
		case ename:
			memcpy(pt, ext, size);
			pt += size;
			break;

		default:
			break;
	}

	*pt = '\0';
}


void
DCctl::aprint()
{
  unl i, lim;
  unc *p;
  short *s;
  long *l;
  double *d;
  char *c;
  time_t tval;


  printf("%s: (%s %s %s %s) ", Dtypes[(int) dtype],
               getMajor(), getMinor(), getData(data), getSize(size));

  switch (dtype) {
    case ebyte:
      p = (unc *) ext;
      for (i = 0; i < size; i++)
        printf("0x%X ", *p++);
      break;
    case eshort:
      s = (short *) ext;
      lim = size / sizeof(short);
      for (i = 0; i < lim; i++)
        printf("%hd ", *s++);
      break;
    case elong:
      l = (long *) ext;
      lim = size / sizeof(long);
      for (i = 0; i < lim; i++)
        printf("%ld ", *l++);
      break;
    case edoub:
      d = (double *) ext;
      lim = size / sizeof(double);
      for (i = 0; i < lim; i++)
        printf("%g ", *d++);
      break;
    case ename:
    case etext:
      c = (char *) ext;
      printf("\"");
      for (i = 0; i < size; i++) {
        if (iscntrl(*c)) {
          switch (*c) {
            case 0:
              printf("\"");
              break;
            case '\n':
            case '\r':
              printf("%c", *c);
              break;
            case '\t':
              printf("\\t");
              break;
            default:
              printf("\\%c", 'A' + *c);
              break;
          }
          c++;
        }
        else if (*c == '"')
          printf("\\%c", *c++);
        else printf("%c", *c++);
      }
      break;
    case ecode:
      printf("block of %lu bytes of code", size);
      break;
    case stamp:
      tval = (time_t) size;
      printf(" #%s", ctime(&tval));
      break;
    default:
      break;
  }
  printf("\n");
}



// DCblock destructive assignment from ctl

DCblock&
DCblock::operator= (DCctl &c)
{
  if (ptr)
    DeleteMem(ptr);
  len = c.size;
  ptr = c.ext;
  c.ext = NULL;
  return *this;
}


// end of dcbase.cc

