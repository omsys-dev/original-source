// dcicu.h is the header for dcl ICU conversions, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#ifndef DCICU_H
#define DCICU_H

#include <stdio.h>
#include <string>
#include "unicode/utypes.h"
#include "unicode/uchar.h"
#include "unicode/locid.h"
//#include "unicode/stringpiece.h"
#include "unicode/utf8.h"
#include "unicode/ustring.h"
#include "unicode/ucnv.h"
#include "unicode/unistr.h"
#include "unicode/brkiter.h"


typedef UConverter * (*ituopen)(const char *, UErrorCode *);
typedef void (*ituclose)(UConverter *);
typedef const char * (*ituerrname)(UErrorCode);
typedef const char * (*itugetname)(const UConverter *, UErrorCode *);
typedef int32_t (*itucnvuc)(UConverter *, char *, int32_t,
	const UChar *, int32_t, UErrorCode *);

typedef int32_t (*itustrlen)(const UChar *);
typedef UBreakIterator * (*itubrkopen)(UBreakIteratorType,
  const char *, const UChar *, int32_t, UErrorCode *);
typedef void (*itubrkclose)(UBreakIterator *);
typedef int32_t (*itubrkfirst)(UBreakIterator *);
typedef int32_t (*itubrknext)(UBreakIterator *);
typedef UChar * (*itustrFromUTF8Lenient)(UChar *, int32_t, int32_t *,
  const char *, int32_t, UErrorCode *);
typedef char * (*itustrToUTF8)(char *, int32_t, int32_t *,
  const UChar *, int32_t, UErrorCode *);

typedef void (*logfunc)(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5);


class DCicu {
 protected:
	static ituopen fpucnv_open;
	static ituclose fpucnv_close;
	static ituerrname fpu_errorName;
	static itugetname fpucnv_getName;
	static itucnvuc fpucnv_fromUChars;

	static UConverter *ConvCPFromU;
	static UErrorCode ConvError;
	static DCnlist ConvOpenErrors;
	static long ConvErrorCount;
	static DCnlist ConvUsed;
	static long ConvUsedCount;

	static bool ICUloaded;
	static bool ICUConvLoaded;
	static bool ICUBreakLoaded;
	static bool ICUloadfailed;
	static void *ICUlib;


	static unc CyrLatinSet[64];
	static uns CyrLowSet[16];
	static uns CyrHighSet[16];
	static unc GreekLatinSet[64];
	static unc EasternEuroLatinSet[128];
	static uns EasternEuroSet[128];
	static unc BalticLatinSet[128];
	static uns BalticSet[128];

	static void SetCodePageConverter(char *cp);
	static void CloseCodePageConverter(void);
	static unl GetCPCharFromUnicode(unl wch);

	static char *CharSetType(unl uch);

	static UBreakIterator *Boundary;
	static char *Location;
	static UErrorCode UErrCode;
	static UChar *UString;

	static itustrlen fpu_strlen;
	static itubrkopen fpubrk_open;
	static itubrkclose fpubrk_close;
	static itubrkfirst fpubrk_first;
	static itubrknext fpubrk_next;
	static itustrFromUTF8Lenient fpu_strFromUTF8Lenient;
	static itustrToUTF8 fpu_strToUTF8;

	static void SetWordBreakText(const char *txt, short len = 0);

	static void LogEvent(logtype lt, unc sev, const char *desc,
	              const char *d2 = NULL, const char *d3 = NULL,
	              const char *d4 = NULL, const char *d5 = NULL);

 public:
	static char *XMLEncoding;
	static char *Locale;
	static logfunc LogEventFunc;
	static unl CodePage;
	static char *ICUdll;

	static void SetCodePage(char *lang);
	static void CloseCodePage(void);
	static unl GetUnicodeCpg(unl wch, unl cpg);
	static unl GetUnicodeEquiv(unl wch);
	static unl GetUnicodeLower(unl wch);
	static unl GetUnicodeUpper(unl wch);

	static void SetUpWordBreak(char *loc);
	static DClist *MakeWordList(const char *txt, short len);
	static UChar *UTF8to16(const char *txt, short len);
	static char *UTF16to8(const UChar *s, short len);
};


#endif


