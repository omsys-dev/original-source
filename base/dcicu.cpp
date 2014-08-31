// dcicu.cpp contains code for dcl ICU conversions, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.


#include "dcappl.h"
#include "dcicu.h"
#include <windows.h>


unl DCicu::CodePage = 1252;
char *DCicu::Locale = "usa";
char *DCicu::XMLEncoding = "windows-1252";

ituopen DCicu::fpucnv_open = NULL;
ituclose DCicu::fpucnv_close = NULL;
ituerrname DCicu::fpu_errorName = NULL;
itugetname DCicu::fpucnv_getName = NULL;
itucnvuc DCicu::fpucnv_fromUChars = NULL;

UConverter *DCicu::ConvCPFromU = NULL;
UErrorCode DCicu::ConvError = U_ZERO_ERROR;
DCnlist DCicu::ConvOpenErrors;
long DCicu::ConvErrorCount = 0;
DCnlist DCicu::ConvUsed;
long DCicu::ConvUsedCount = 0;

bool DCicu::ICUloaded = false;
bool DCicu::ICUConvLoaded = false;
bool DCicu::ICUBreakLoaded = false;
bool DCicu::ICUloadfailed = false;
char *DCicu::ICUdll = "icuuc40.dll";
void *DCicu::ICUlib = NULL;



void DCicu::SetCodePageConverter(char *cp)
{
	if (ICUloadfailed)
		return;

	if (!ICUloaded) {
		SetLastError(0);
		if ((ICUlib = LoadLibrary(ICUdll)) == NULL) {
			char buf[256];
			long err = GetLastError();
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0,
			              buf, 256, NULL);
			LogEvent(logwarn, 1, "Cannot find ICU library ", ICUdll, " ", buf);
			ICUloadfailed = true;
			return;
		}
		ICUloaded = true;
	}
	if (!ICUConvLoaded) {
		// ucnv_open, ucnv_close, u_errorName, ucnv_getName, ucnv_fromUChars
		fpucnv_open = (ituopen) ::GetProcAddress((HINSTANCE) ICUlib, "ucnv_open_4_0");
		fpucnv_close = (ituclose) ::GetProcAddress((HINSTANCE) ICUlib, "ucnv_close_4_0");
		fpucnv_getName = (itugetname) ::GetProcAddress((HINSTANCE) ICUlib, "ucnv_getName_4_0");
		fpucnv_fromUChars = (itucnvuc) ::GetProcAddress((HINSTANCE) ICUlib, "ucnv_fromUChars_4_0");
		if (!fpu_errorName)
			fpu_errorName = (ituerrname) ::GetProcAddress((HINSTANCE) ICUlib, "u_errorName_4_0");
		if (!fpucnv_open
		 || !fpucnv_close
		 || !fpu_errorName
		 || !fpucnv_getName
		 || !fpucnv_fromUChars) {
			LogEvent(logwarn, 1, "Cannot load ICU library code page entry points");
			ICUloadfailed = true;
			return;
		}
		ICUConvLoaded = true;
	}

	ConvCPFromU = fpucnv_open(cp, &ConvError);

	if (U_FAILURE(ConvError)) {
		if (ConvOpenErrors(cp))
			return;
		ConvOpenErrors.add(cp, ++ConvErrorCount);
		LogEvent(logwarn, 1, "Error opening ICU converter for ",
		 cp, ": ", fpu_errorName(ConvError));
		ConvCPFromU = NULL;
		return;
	}
	if (ConvUsed(cp))
		return;
	ConvUsed.add(cp, ++ConvUsedCount);
	LogEvent(loginfo, 2, "Using ICU converter ",
	 fpucnv_getName(ConvCPFromU, &ConvError), " for ", cp);
}


void DCicu::CloseCodePageConverter(void)
{
	if (!ConvCPFromU)
		return;
	fpucnv_close(ConvCPFromU);
	ConvCPFromU = NULL;
}


unl DCicu::GetCPCharFromUnicode(unl wch)
{
	if (ConvCPFromU == NULL)
		return 0;

	char target[4];
	char *tptr = &target[0];
	UChar source[2];
	unl rval = 0;
	int32_t len = 0;

	source[0] = (UChar) (wch & 0xFFFF);
	source[1] = 0;
	target[0] = target[1] = target[2] = target[3] = 0;
	
#if 0
	char *tlim = &target[3];
	UChar *slim = &source[1];
	ucnv_fromUnicode(ConvCPFromU, &tptr, tlim, (const UChar **) &source, slim,
		                 NULL, TRUE, &ConvError);
	if (tptr >= &target[3])
		rval |= (((unc) target[2]) & 0xFF) << 16;
	if (tptr >= &target[2])
		rval |= (((unc) target[1]) & 0xFF) << 8;
	if (tptr >= &target[1])
		rval |= (((unc) target[0]) & 0xFF);
#endif

	len = fpucnv_fromUChars(ConvCPFromU, target, 3, (const UChar *) source, 1,
		                  &ConvError);

	if (U_FAILURE(ConvError)) {
		char chbuf[10];
		sprintf(chbuf, "U+%0.4X", wch);
		LogEvent(logwarn, 1, "Error getting code page character: ",
		 fpu_errorName(ConvError));
		LogEvent(logwarn, 1, "Char is ", chbuf, ", ", CharSetType(wch));
		return 0;
	}

	if (len > 2)
		rval |= (((unc) target[2]) & 0xFF) << 16;
	if (len > 1)
		rval |= (((unc) target[1]) & 0xFF) << 8;
	if (len > 0)
		rval |= (((unc) target[0]) & 0xFF);

	return rval;
}


char *DCicu::CharSetType(unl uch)
{
	if ((uch >= 0x30A0) && (uch <= 0x30FF))
		return "katakana";
	if ((uch >= 0x3041) && (uch <= 0x309F))
		return "hiragana";
	if ((uch >= 0x4E00) && (uch <= 0x9FFF))
		return "unified Han";

	return "unknown";
}


void DCicu::SetCodePage(char *lang)
{
	if (!lang
	 || ICUloadfailed)
		return;

	unl langid = strtoul(lang, NULL, 0);
	// 0x409 English, 0x408 Greek, 0x419 Russian,
	// 0x41F Turkish, 0x405 Czech (use for CE)

	switch (langid) {

		case 0x409:  // English: United States
			CodePage = 1252;
			XMLEncoding = "windows-1252";
			Locale = "usa";
			break;
		case 0x436:  // Afrikaans: South Africa
		case 0x42D:  // Basque: Spain
		case 0x403:  // Catalan: Spain
		case 0x406:  // Danish: Denmark
		case 0x813:  // Dutch: Belgium
		case 0x413:  // Dutch: Netherlands
		case 0xC09:  // English: Australia
		case 0x2809: // English: Belize
		case 0x1009: // English: Canada
		case 0x2409: // English: Caribbean
		case 0x1809: // English: Ireland
		case 0x2009: // English: Jamaica
		case 0x1409: // English: New Zealand
		case 0x3409: // English: Philippines
		case 0x1C09: // English: South Africa
		case 0x2C09: // English: Trinidad
		case 0x809:  // English: United Kingdom
		case 0x3009: // English: Zimbabwe
		case 0x438:  // Faeroese: Faeroe Islands
		case 0x40B:  // Finnish: Finland
		case 0x80C:  // French: Belgium
		case 0xC0C:  // French: Canada
		case 0x40C:  // French: France
		case 0x140C: // French: Luxembourg
		case 0x180C: // French: Monaco
		case 0x100C: // French: Switzerland
		case 0xC07:  // German: Austria
		case 0x407:  // German: Germany
		case 0x1407: // German: Liechtenstein
		case 0x1007: // German: Luxembourg
		case 0x807:  // German: Switzerland
		case 0x40F:  // Icelandic: Iceland
		case 0x421:  // Indonesian: Indonesia
		case 0x410:  // Italian: Italy 
		case 0x810:  // Italian: Switzerland
		case 0x83E:  // Malay: Brunei Darussalam
		case 0x43E:  // Malay: Malaysia
		case 0x414:  // Norwegian: Norway (Bokmål)
		case 0x814:  // Norwegian: Norway (Nynorsk)
		case 0x416:  // Portuguese: Brazil
		case 0x816:  // Portuguese: Portugal
		case 0x2C0A: // Spanish: Argentina
		case 0x400A: // Spanish: Bolivia
		case 0x340A: // Spanish: Chile
		case 0x240A: // Spanish: Colombia
		case 0x140A: // Spanish: Costa Rica
		case 0x1C0A: // Spanish: Dominican Republic
		case 0x300A: // Spanish: Ecuador
		case 0x440A: // Spanish: El Salvador
		case 0x100A: // Spanish: Guatemala
		case 0x480A: // Spanish: Honduras
		case 0x80A:  // Spanish: Mexico
		case 0x4C0A: // Spanish: Nicaragua
		case 0x180A: // Spanish: Panama
		case 0x3C0A: // Spanish: Paraguay
		case 0x280A: // Spanish: Peru
		case 0x500A: // Spanish: Puerto Rico
		case 0xC0A:  // Spanish: Spain (Modern Sort)
		case 0x40A:  // Spanish: Spain (International Sort)
		case 0x380A: // Spanish: Uruguay
		case 0x200A: // Spanish: Venezuela
		case 0x441:  // Swahili: Kenya
		case 0x81D:  // Swedish: Finland
		case 0x41D:  // Swedish: Sweden
			CodePage = 1252;
			XMLEncoding = "windows-1252";
			Locale = "????";
			break;

		case 0x419:  // Russian: Russia
			CodePage = 1251;
			XMLEncoding = "windows-1251";
			Locale = "rus";
			break;
		case 0x82C:  // Azeri: Azerbaijan (Cyrillic)
		case 0x423:  // Belarusian: Belarus
		case 0x402:  // Bulgarian: Bulgaria
		case 0x42F:  // Macedonian (FYROM)
		case 0x440:  // Kyrgyz: Kyrgyzstan
		case 0x450:  // Mongolian: Mongolia
		case 0xC1A:  // Serbian: Serbia (Cyrillic)
		case 0x444:  // Tatar: Tatarstan
		case 0x422:  // Ukrainian: Ukraine
		case 0x843:  // Uzbek: Uzbekistan (Cyrillic)
			CodePage = 1251;
			XMLEncoding = "windows-1251";
			Locale = "????";
			break;

		case 0x408:  // Greek: Greece
			CodePage = 1253;
			XMLEncoding = "windows-1253";
			Locale = "greek";
			break;

		case 0x41F:  // Turkish: Turkey
			CodePage = 1254;
			XMLEncoding = "windows-1254";
			Locale = "trk";
			break;

		case 0x405:  // Czech: Czech Republic
			CodePage = 1250;
			XMLEncoding = "windows-1250";
			Locale = "czech";
			break;
		case 0x41C:  // Albanian: Albania
		case 0x42C:  // Azeri: Azerbaijan (Latin)
		case 0x41A:  // Croatian: Croatia
		case 0x40E:  // Hungarian: Hungary
		case 0x415:  // Polish: Poland
		case 0x418:  // Romanian: Romania
		case 0x81A:  // Serbian: Serbia (Latin)
		case 0x41B:  // Slovak: Slovakia
		case 0x424:  // Slovenian: Slovenia
		case 0x443:  // Uzbek: Uzbekistan (Latin)
			CodePage = 1250;
			XMLEncoding = "windows-1250";
			Locale = "????";
			break;

		case 0x827:  // Classic Lithuanian: Lithuania
		case 0x425:  // Estonian: Estonia
		case 0x426:  // Latvian: Latvia
		case 0x427:  // Lithuanian: Lithuania
			CodePage = 1257;
			XMLEncoding = "windows-1257";
			Locale = "????";
			break;

		case 0x411:  // Japanese: Japan 
			CodePage = 932;
			SetCodePageConverter("Shift_JIS");
			XMLEncoding = "Shift_JIS";
			Locale = "jpn";
			break;

		case 0x404:  // Chinese: Taiwan (Traditional)
		case 0xC04:  // Chinese: Hong Kong SAR, PRC (Traditional)
		case 0x1404: // Chinese: Macao SAR (Traditional)
			CodePage = 950;
			SetCodePageConverter("Big5");
			XMLEncoding = "Big5";
			Locale = "cht";
			break;

		case 0x804:  // Chinese: PRC (Simplified)
		case 0x1004: // Chinese: Singapore (Simplified)
			CodePage = 936;
			SetCodePageConverter("windows-936");
			XMLEncoding = "GBK";
			Locale = "chs";
			break;

		case 0x412:  // Korean (Extended Wansung): Korea 
			CodePage = 949;
			SetCodePageConverter("windows-949");
			XMLEncoding = "KSC_5601";
			Locale = "kor";
			break;

		case 0x41E:  // Thai: Thailand
			CodePage = 874;
			SetCodePageConverter("windows-874");
			XMLEncoding = "windows-874";
			Locale = "????";
			break;

		case 0x42A:  // Vietnamese: Vietnam
			CodePage = 1258;
			SetCodePageConverter("windows-1258");
			XMLEncoding = "windows-1258";
			Locale = "????";
			break;


		// RTL: not supported
		case 0x40D:  // Hebrew: Israel
			CodePage = 1255;
			SetCodePageConverter("windows-1255");
			XMLEncoding = "windows-1255";
			Locale = "????";
			break;

		case 0x1401: // Arabic: Algeria
		case 0x3C01: // Arabic: Bahrain
		case 0xC01:  // Arabic: Egypt
		case 0x801:  // Arabic: Iraq
		case 0x2C01: // Arabic: Jordan
		case 0x3401: // Arabic: Kuwait
		case 0x3001: // Arabic: Lebanon
		case 0x1001: // Arabic: Libya
		case 0x1801: // Arabic: Morocco
		case 0x2001: // Arabic: Oman
		case 0x4001: // Arabic: Qatar
		case 0x401:  // Arabic: Saudi Arabia
		case 0x2801: // Arabic: Syria
		case 0x1C01: // Arabic: Tunisia
		case 0x3801: // Arabic: U.A.E.
		case 0x2401: // Arabic: Yemen
		case 0x429:  // Farsi: Iran
		case 0x820:  // Urdu: India
		case 0x420:  // Urdu: Pakistan
			CodePage = 1256;
			SetCodePageConverter("windows-1256");
			XMLEncoding = "windows-1256";
			Locale = "????";
			break;


		// Unicode only for:
		case 0x42B:  // Armenian: Armenia
		case 0x437:  // Georgian: Georgia
		case 0x447:  // Gujarati: India
		case 0x439:  // Hindi: India
		case 0x44B:  // Kannada: India
		case 0x457:  // Konkani: India
		case 0x44E:  // Marathi: India
		case 0x446:  // Punjabi: India
		case 0x44F:  // Sanskrit: India
		case 0x44A:  // Telgu: India
			CodePage = 0;
			XMLEncoding = "UTF-8";
			Locale = "????";
			break;


		default:
			CodePage = 1252;
			XMLEncoding = "windows-1252";
			Locale = "usa";
			break;
	}
}


void DCicu::CloseCodePage()
{
	CloseCodePageConverter();
}



unc DCicu::CyrLatinSet[64] = {
 0, 0, 1, 0, 1, 1, 1, 1,   // 0x80
 0, 1, 0, 1, 0, 0, 0, 0,   // 0x88
 0, 1, 1, 1, 1, 1, 1, 1,   // 0x90
 0, 1, 0, 1, 0, 0, 0, 0,   // 0x98
 1, 0, 0, 0, 1, 0, 1, 1,   // 0xA0
 0, 1, 0, 1, 1, 1, 1, 0,   // 0xA8
 1, 1, 0, 0, 0, 1, 1, 1,   // 0xB0
 0, 0, 0, 1, 0, 0, 0, 0    // 0xB8
};

uns DCicu::CyrLowSet[16] = {
 0x0, 0xAB, 0x80, 0x81, 0xAA, 0xBD, 0xB2, 0xAF,
 0xA3, 0x8A, 0x8C, 0x8E, 0x8D, 0x0, 0xA1, 0x8F
};

uns DCicu::CyrHighSet[16] = {
 0x0, 0xB8, 0x90, 0x83, 0xBA, 0xBE, 0xB3, 0xBF,
 0xBC, 0x9A, 0x9C, 0x9E, 0x9D, 0x0, 0xA2, 0x9F
};

unc DCicu::GreekLatinSet[64] = {
 1, 1, 1, 1, 1, 1, 1, 1,   // 0x80
 0, 1, 0, 1, 0, 1, 1, 1,   // 0x88
 1, 1, 1, 1, 1, 1, 1, 1,   // 0x90
 0, 1, 0, 1, 0, 1, 1, 0,   // 0x98
 1, 0, 0, 1, 1, 1, 1, 1,   // 0xA0
 1, 1, 0, 1, 1, 1, 1, 0,   // 0xA8
 1, 1, 1, 1, 0, 1, 1, 1,   // 0xB0
 0, 0, 0, 1, 0, 1, 0, 0    // 0xB8
};

unc DCicu::EasternEuroLatinSet[128] = {
 1, 1, 1, 0, 1, 1, 1, 1,   // 0x80
 0, 1, 1, 1, 0, 0, 0, 0,   // 0x88
 1, 1, 1, 1, 1, 1, 1, 1,   // 0x90
 0, 1, 1, 1, 0, 0, 0, 0,   // 0x98
 1, 0, 0, 0, 1, 0, 1, 1,   // 0xA0
 1, 1, 0, 1, 1, 1, 1, 0,   // 0xA8
 1, 1, 0, 0, 1, 1, 1, 1,   // 0xB0
 1, 0, 0, 1, 0, 0, 0, 0,   // 0xB8
 0, 1, 1, 0, 1, 0, 0, 1,   // 0xC0
 0, 1, 0, 1, 0, 1, 1, 0,   // 0xC8
 0, 0, 0, 1, 1, 0, 1, 1,   // 0xD0
 0, 0, 1, 0, 1, 1, 0, 1,   // 0xD8
 0, 1, 1, 0, 1, 0, 0, 1,   // 0xE0
 0, 1, 0, 1, 0, 1, 1, 0,   // 0xE8
 0, 0, 0, 1, 1, 0, 1, 1,   // 0xF0
 0, 0, 1, 0, 1, 1, 0, 0    // 0xF8
};

uns DCicu::EasternEuroSet[128] = {
 0x0, 0x0, 0xC3, 0xE3, 0xA5, 0xB9, 0xC6, 0xE6, // 0x100
 0x0, 0x0, 0x0, 0x0, 0xC8, 0xE8, 0xCF, 0xEF,
 0xD0, 0xF0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // 0x110
 0xCA, 0xEA, 0xCC, 0xEC, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // 0x120
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // 0x130
 0x0, 0xC5, 0xE5, 0x0, 0x0, 0xBC, 0xBE, 0x0,
 0x0, 0xA3, 0xB3, 0xD1, 0xF1, 0x0, 0x0, 0xD2, // 0x140
 0xF2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0xD5, 0xF5, 0x0, 0x0, 0xC0, 0xE0, 0x0, 0x0, // 0x150
 0xD8, 0xF8, 0x8C, 0x9C, 0x0, 0x0, 0xAA, 0xBA,
 0x8A, 0x9A, 0xDE, 0xFE, 0x8D, 0x9D, 0x0, 0x0, // 0x160
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xD9, 0xF9,
 0xDB, 0xFB, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // 0x170
 0x0, 0x8F, 0x9F, 0xAF, 0xBF, 0x8E, 0x9E, 0x0
};

unc DCicu::BalticLatinSet[128] = {
 0, 0, 0, 0, 0, 0, 0, 0,   // 0x80
 0, 0, 0, 0, 0, 0, 0, 0,   // 0x88
 0, 0, 0, 0, 0, 0, 0, 0,   // 0x90
 0, 0, 0, 0, 0, 0, 0, 0,   // 0x98
 1, 0, 1, 1, 1, 0, 1, 1,   // 0xA0
 0, 1, 0, 1, 1, 1, 1, 0,   // 0xA8
 1, 1, 1, 1, 1, 1, 1, 1,   // 0xB0
 0, 1, 0, 1, 1, 1, 1, 0,   // 0xB8
 0, 0, 0, 0, 1, 1, 0, 0,   // 0xC0
 0, 1, 0, 0, 0, 0, 0, 0,   // 0xC8
 0, 0, 0, 1, 0, 1, 1, 1,   // 0xD0
 0, 0, 0, 0, 1, 0, 0, 1,   // 0xD8
 0, 0, 0, 0, 1, 1, 0, 0,   // 0xE0
 0, 1, 0, 0, 0, 0, 0, 0,   // 0xE8
 0, 0, 0, 1, 0, 1, 1, 1,   // 0xF0
 0, 0, 0, 0, 1, 0, 0, 0    // 0xF8
};

uns DCicu::BalticSet[128] = {
 0xC2, 0xE2, 0x0, 0x0, 0xC0, 0xE0, 0xC3, 0xE3, // 0x100
 0x0, 0x0, 0x0, 0x0, 0xC8, 0xE8, 0x0, 0x0,
 0x0, 0x0, 0xC7, 0xE7, 0x0, 0x0, 0xCB, 0xEB, // 0x110
 0xC6, 0xE6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0xCC, 0xEC, 0x0, 0x0, 0x0, 0x0, // 0x120
 0x0, 0x0, 0xCE, 0xEE, 0x0, 0x0, 0xC1, 0xE1,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xCD, 0xED, // 0x130
 0x0, 0x0, 0x0, 0xCF, 0xEF, 0x0, 0x0, 0x0,
 0x0, 0xD9, 0xF9, 0xD1, 0xF1, 0xD2, 0xF2, 0x0, // 0x140
 0x0, 0x0, 0x0, 0x0, 0xD4, 0xF4, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xAA, 0xBA, // 0x150
 0x0, 0x0, 0xDA, 0xFA, 0x0, 0x0, 0x0, 0x0,
 0xD0, 0xF0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // 0x160
 0x0, 0x0, 0xDB, 0xFB, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0xD8, 0xF8, 0x0, 0x0, 0x0, 0x0, // 0x170
 0x0, 0xCA, 0xEA, 0xDD, 0xFD, 0xDE, 0xFE, 0x0
};


unl DCicu::GetUnicodeCpg(unl wch, unl cpg)
{
	unl lcpg = 0;
	unl xch = 0;

	if (ICUloadfailed)
		return '?';

	if (cpg != CodePage) {
		switch (cpg) {
			case 932:  // Japanese
				CloseCodePage();
				CodePage = 932;
				SetCodePageConverter("Shift_JIS");
				XMLEncoding = "Shift_JIS";
				Locale = "jpn";
				break;
			case 950:  // Chinese (Traditional)
				CloseCodePage();
				CodePage = 950;
				SetCodePageConverter("Big5");
				XMLEncoding = "Big5";
				Locale = "cht";
				break;
			case 936:  // Chinese (Simplified)
				CloseCodePage();
				CodePage = 936;
				SetCodePageConverter("windows-936");
				XMLEncoding = "GBK";
				Locale = "chs";
				break;
			case 949:  // Korean
				CloseCodePage();
				CodePage = 949;
				SetCodePageConverter("windows-949");
				XMLEncoding = "KSC_5601";
				break;

			case 874:  // Thai
				CloseCodePage();
				CodePage = 874;
				SetCodePageConverter("windows-874");
				XMLEncoding = "windows-874";
				break;
			case 1258:  // Vietnamese
				CloseCodePage();
				CodePage = 1258;
				SetCodePageConverter("windows-1258");
				XMLEncoding = "windows-1258";
				break;

			case 1255:  // Hebrew
				CloseCodePage();
				CodePage = 1255;
				SetCodePageConverter("windows-1255");
				XMLEncoding = "windows-1255";
				break;
			case 1256:  // Arabic
				CloseCodePage();
				CodePage = 1256;
				SetCodePageConverter("windows-1256");
				XMLEncoding = "windows-1256";
				break;

			default:  // others we handle ourselves
				break;
		}
	}
	lcpg = CodePage;
	CodePage = cpg;
	xch = GetUnicodeEquiv(wch);
	CodePage = lcpg;
	return xch;
}


unl DCicu::GetUnicodeEquiv(unl wch)
{
	// if two byte return (CJK), nch = ((firstbyte << 8) | second_byte);

	unl nch = 0;

	if ((CodePage >= 1250)
	 && (CodePage < 1258)) { // same for all non-Asian code pages
		if (wch < 0x2000) {
			switch (wch) {
				case 0xA0:
					nch = 0xA0;
					break;
				case 0xA4:
					nch = 0xA4;
					break;
				case 0xA6:
					nch = 0xA6;
					break;
				case 0xA7:
					nch = 0xA7;
					break;
				case 0xA9:
					nch = 0xA9;
					break;
				case 0xAB:
					nch = 0xAB;
					break;
				case 0xAC:
					nch = 0xAC;
					break;
				case 0xAD:
					nch = 0xAD;
					break;
				case 0xAE:
					nch = 0xAE;
					break;
				case 0xB0:
					nch = 0xB0;
					break;
				case 0xB1:
					nch = 0xB1;
					break;
				case 0xB5:
					nch = 0xB5;
					break;
				case 0xB6:
					nch = 0xB6;
					break;
				case 0xB7:
					nch = 0xB7;
					break;
				case 0xBB:
					nch = 0xBB;
					break;
			}
		}
		else {  // U+2000 and up
			if (wch < 0x200B)
				nch = 0xA0;
			else switch (wch - 0x2000) {
				case 0x1A:
					nch = 0x82;
					break;
				case 0x1E:
					nch = 0x84;
					break;
				case 0x26:
					nch = 0x85;
					break;
				case 0x20:
					nch = 0x86;
					break;
				case 0x21:
					nch = 0x87;
					break;
				case 0x30:
					nch = 0x89;
					break;
				case 0x39:
					nch = 0x8B;
					break;
				case 0x18:
					nch = 0x91;
					break;
				case 0x19:
					nch = 0x92;
					break;
				case 0x1C:
					nch = 0x93;
					break;
				case 0x1D:
					nch = 0x94;
					break;
				case 0x22:
					nch = 0x95;
					break;
				case 0x13:
					nch = 0x96;
					break;
				case 0x14:
					nch = 0x97;
					break;
				case 0x122:
					nch = 0x99;
					break;
				case 0x3A:
					nch = 0x9B;
					break;
				case 0xAC:
					nch = 0x80;
					break;
				default:
					break;
			}
		}
		if (nch)
			return nch;
	}

	switch (CodePage) {
		case 932:     // 0x411 Japanese Shift-JIS
			if (ICUloadfailed)
				return '?';
			if ((wch == 0xA0)
			 || ((wch >= 0x2000)
			  && (wch <= 0x200B)))
				wch = 0x3000;  // ideographic space
			nch = GetCPCharFromUnicode(wch);
			break;

		case 950:     // 0x404 Chinese (Traditional)
		case 936:     // 0x804 Chinese (Simplified)
		case 949:     // 0x412 Korean
		case 874:     // 0x41E Thai: Thailand
		case 1258:    // 0x42A Vietnamese: Vietnam
		case 1255:    // 0x40D Hebrew: Israel
		case 1256:    // Arabic, Urdu
			if (ICUloadfailed)
				return '?';
			nch = GetCPCharFromUnicode(wch);
			break;

		case 1252:    // ANSI, 1252
			if (wch <= 0xFF)
				nch = wch;
			else if (wch == 0x192)
				nch = 0x83;
			else if (wch == 0x2C6)
				nch = 0x88;
			else if (wch == 0x160)
				nch = 0x8A;
			else if (wch == 0x152)
				nch = 0x8C;
			else if (wch == 0x2DC)
				nch = 0x98;
			else if (wch == 0x161)
				nch = 0x9A;
			else if (wch == 0x153)
				nch = 0x9C;
			else if (wch == 0x178)
				nch = 0x9F;
			break;

		case 1251:  // Cyrillic, 1251
			if (wch <= 0xFF) {
				if ((wch < 0xC0)
				 && CyrLatinSet[wch - 0x80])
					nch = wch;
			}
			else if ((wch >= 0x410)
			 && (wch <= 0x44F))
				nch = wch - 0x350;
			else if ((wch >= 0x400)
			 && (wch <= 0x40F))
				nch = CyrLowSet[wch - 0x400];
			else if ((wch >= 0x450)
			 && (wch <= 0x45F))
				nch = CyrHighSet[wch - 0x450];
			else if (wch == 0x490)
				nch = 0xA5;
			else if (wch == 0x491)
				nch = 0xB4;
			else if (wch == 0x2116)
				nch = 0xB9;
			break;

		case 1250:  // Eastern European, 1250
			if (wch <= 0xFF) {
				if (EasternEuroLatinSet[wch - 0x80])
					nch = wch;
			}
			else if ((wch >= 0x100)
			 && (wch <= 0x17F))
				nch = EasternEuroSet[wch - 0x100];
			else if (wch == 0x2C7)
				nch = 0xA1;
			else if (wch == 0x2D8)
				nch = 0xA2;
			else if (wch == 0x2DB)
				nch = 0xB2;
			else if (wch == 0x2DD)
				nch = 0xBD;
			else if (wch == 0x2D9)
				nch = 0xFF;
			break;

		case 1257:  // Baltic, 1257
			if (wch <= 0xFF) {
				if (BalticLatinSet[wch - 0x80])
					nch = wch;
			}
			else if ((wch >= 0x100)
			 && (wch <= 0x17F))
				nch = BalticSet[wch - 0x100];
			else if (wch == 0xA8)
				nch = 0x8D;
			else if (wch == 0x2C7)
				nch = 0x8E;
			else if (wch == 0xB8)
				nch = 0x8F;
			else if (wch == 0xAF)
				nch = 0x9D;
			else if (wch == 0x2DB)
				nch = 0x9E;
			else if (wch == 0xC6)
				nch = 0xAF;
			else if (wch == 0xE6)
				nch = 0xBF;
			else if (wch == 0x2D9)
				nch = 0xFF;
			break;

		case 1253:  // Greek, 1253
			if (wch <= 0xFF) {
				if ((wch < 0xC0)
				 && GreekLatinSet[wch - 0x80])
					nch = wch;
			}
			else if ((wch >= 0x388)
			 && (wch <= 0x3CF))
				nch = wch - 0x2D0;
			else if (wch == 0x192)
				nch = 0x83;
			else if (wch == 0x3F4)
				nch = 0xA1;
			else if (wch == 0x386)
				nch = 0xA2;
			else if (wch == 0x2015)
				nch = 0xAF;
			else if (wch == 0x3F3)
				nch = 0xB4;
			break;

		case 1254:  // Turkish, 1254
			if (wch <= 0xFF) {
				if ((wch != 0xD0)
				 && (wch != 0xF0)
				 && (wch != 0xDD)
				 && (wch != 0xFD)
				 && (wch != 0xDE)
				 && (wch != 0xFE))
					nch = wch;
			}
			else if (wch == 0x11E)
				nch = 0xD0;
			else if (wch == 0x130)
				nch = 0xDD;
			else if (wch == 0x15E)
				nch = 0xDE;
			else if (wch == 0x11F)
				nch = 0xF0;
			else if (wch == 0x131)
				nch = 0xFD;
			else if (wch == 0x15F)
				nch = 0xFE;
			break;

		default:
			break;
	}
	return nch;
}


unl DCicu::GetUnicodeLower(unl wch)
{
	return u_tolower(wch);
}


unl DCicu::GetUnicodeUpper(unl wch)
{
	return u_toupper(wch);
}


// word breaking for OH FTS

UBreakIterator *DCicu::Boundary = NULL;
char *DCicu::Location = NULL;
UErrorCode DCicu::UErrCode = U_ZERO_ERROR;
UChar *DCicu::UString = NULL;

itustrlen DCicu::fpu_strlen = NULL;
itubrkopen DCicu::fpubrk_open = NULL;
itubrkclose DCicu::fpubrk_close = NULL;
itubrkfirst DCicu::fpubrk_first = NULL;
itubrknext DCicu::fpubrk_next = NULL;
itustrFromUTF8Lenient DCicu::fpu_strFromUTF8Lenient = NULL;
itustrToUTF8 DCicu::fpu_strToUTF8 = NULL;


void
DCicu::SetUpWordBreak(char *loc)
{
	// loc is language code, as in "en_US"

	Location = NewName(loc);
	UErrCode = U_ZERO_ERROR;
	UString = NULL;

	// load library and get function addresses

	if (ICUloadfailed)
		return;

	if (!ICUloaded) {
		SetLastError(0);
		if ((ICUlib = LoadLibrary(ICUdll)) == NULL) {
			char buf[256];
			long err = GetLastError();
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0,
			              buf, 256, NULL);
			LogEvent(logwarn, 1, "Cannot find ICU library ", ICUdll, " ", buf);
			ICUloadfailed = true;
			return;
		}
		ICUloaded = true;
	}
	if (!ICUBreakLoaded) {
		// u_strlen, ubrk_open, ubrk_close, ubrk_first, ubrk_next,
		// u_strFromUTF8Lenient, u_strToUTF8
		fpu_strlen = (itustrlen) ::GetProcAddress((HINSTANCE) ICUlib, "u_strlen_4_0");
		fpubrk_open = (itubrkopen) ::GetProcAddress((HINSTANCE) ICUlib, "ubrk_open_4_0");
		fpubrk_close = (itubrkclose) ::GetProcAddress((HINSTANCE) ICUlib, "ubrk_close_4_0");
		fpubrk_first = (itubrkfirst) ::GetProcAddress((HINSTANCE) ICUlib, "ubrk_first_4_0");
		fpubrk_next = (itubrknext) ::GetProcAddress((HINSTANCE) ICUlib, "ubrk_next_4_0");
		fpu_strFromUTF8Lenient = (itustrFromUTF8Lenient) ::GetProcAddress((HINSTANCE) ICUlib, "u_strFromUTF8Lenient_4_0");
		fpu_strToUTF8 = (itustrToUTF8) ::GetProcAddress((HINSTANCE) ICUlib, "u_strToUTF8_4_0");
		if (!fpu_errorName)
			fpu_errorName = (ituerrname) ::GetProcAddress((HINSTANCE) ICUlib, "u_errorName_4_0");

		if (!fpu_strlen
		 || !fpubrk_open
		 || !fpubrk_close
		 || !fpubrk_first
		 || !fpubrk_next
		 || !fpu_errorName
		 || !fpu_strFromUTF8Lenient
		 || !fpu_strToUTF8) {
			LogEvent(logwarn, 1, "Cannot load ICU library break entry points");
			ICUloadfailed = true;
			return;
		}
		ICUBreakLoaded = true;
	}
}


void
DCicu::SetWordBreakText(const char *txt, short len)
{
	// set up for processing txt
	static char *nt = NULL;
	UChar *s = UString;
	UErrCode = U_ZERO_ERROR;

	if (Boundary) {
		fpubrk_close(Boundary);
		Boundary = NULL;
	}

	if (s) {
		delete [] s;
		s = NULL;
	}
	//s = new UChar[len + 1];

	if (len
	 && (txt[len] != '\0')) {  // not terminated
		if (nt)
			DeleteName(nt);
		nt = NewName(txt, len);
		s = UTF8to16(nt, len);
	}
	else {
		len = strlen(txt);
		s = UTF8to16(txt, len);
	}
	UString = s;

	Boundary = fpubrk_open(UBRK_WORD, Location, s, fpu_strlen(s), &UErrCode);

	if (U_FAILURE(UErrCode)) {
		LogEvent(logwarn, 1, "Failed to set word break text: ",
			                    fpu_errorName(UErrCode));
		Boundary = NULL;
	}
}


UChar *
DCicu::UTF8to16(const char *txt, short len)
{
	// convert from UTF-8 to UTF-16 in newly allocated string
	int32_t dlen = 0; // receives dest length
	UChar *s = new UChar [len + 1];  // UTF-16 always shorter than 8
	UErrCode = U_ZERO_ERROR;

	s = fpu_strFromUTF8Lenient(s, len, &dlen, txt, len, &UErrCode);
	if (U_FAILURE(UErrCode)) {
		LogEvent(logwarn, 1, "Failed to convert UTF-8 to UTF-16: ",
			                    fpu_errorName(UErrCode));
	}
	if (dlen > len)
		LogEvent(logwarn, 1, "UTF-16 conversion overflow for ", txt);
	return s;
}


char *
DCicu::UTF16to8(const UChar *s, short len)
{
	// convert from UTF-16 to UTF-8 in newly allocated string
	int32_t dlen = 0; // receives dest length
	if (!len)  // determine it ourselves
		len = fpu_strlen(s);
	int32_t ulen = len;  // UTF-16 length, allow for longer UTF-8
	len *= 3;  // enough because 4-byte UTF-8 is surrogate pair in 16
	char *txt = NewName(len);
	UErrCode = U_ZERO_ERROR;

	fpu_strToUTF8(txt, len, &dlen, s, ulen, &UErrCode);
	if (U_FAILURE(UErrCode)) {
		LogEvent(logwarn, 1, "Failed to convert UTF-16 to UTF-8: ",
			                    fpu_errorName(UErrCode));
	}
	if (dlen > len)
		LogEvent(logwarn, 1, "UTF-8 conversion overflow for ", txt);
	return txt;
}


DClist *
DCicu::MakeWordList(const char *txt, short len)
{
	SetWordBreakText(txt, len);

	DClist *dl = new DClist();
	if (!UString || !Boundary)
		return dl;  // empty list

	UChar* str = UString;
	int32_t end = 0;
  int32_t start = fpubrk_first(Boundary);
  for (end = fpubrk_next(Boundary);
       end != UBRK_DONE;
       start = end, end = fpubrk_next(Boundary))
  {
			//if (((end - start) > 1)
			// || (*(str + start) != 0x20))
	      dl->append((long) UTF16to8(str + start, end - start));
  }
	return dl;
}



#if 0

// C++ method, requires always having ICU library

void
DCicu::SetUpWordBreak(char *loc)
{
	// create word break iterator for this locale
	// loc is language code, as in "en_US"
	UErrorCode status = U_ZERO_ERROR;

  Boundary = BreakIterator::createWordInstance(Locale::Locale(loc), status);

	if (U_FAILURE(status)) {
		LogEvent(logwarn, 1, "Failed to create word break iterator: ",
			                    u_errorName(status));
		Boundary = NULL;
	}
}


void
DCicu::SetWordBreakText(const char *txt, short len)
{
	// set up for processing txt
	static char *nt = NULL;
	static UnicodeString *s = NULL;

	if (s) {
		DeleteMem(s);
		s = NULL;
	}
	if (len
	 && (txt[len] != '\0')) {  // not terminated
		if (nt)
			DeleteName(nt);
		nt = NewName(txt, len);
		s = UTF8to16(nt);
	}
	else	
		s = UTF8to16(txt);
	if (s)
		Boundary->setText(*s);
}


UnicodeString *
DCicu::UTF8to16(const char *txt)
{
	// convert from UTF-8 to UTF-16 in newly allocated string
	UnicodeString *s = NULL;
	StringPiece *utf8 = new StringPiece(txt);
	s->fromUTF8(*utf8);
	return s;
}


char *
DCicu::UTF16to8(const UnicodeString *s)
{
	// convert from UTF-16 to UTF-8 in newly allocated string
	long len = s->length() * 4;
	char *txt = NewName((short) len);
	//StringClass *result = new StringClass(s);
	//s->toUTF8String(*result);

	s->toUTF8(0, s->length(), txt, len);
	return txt;
}


DClist *
DCicu::MakeWordList(void)
{
	DClist *dl = new DClist();
  int32_t start = Boundary->first();
  for (int32_t end = Boundary->next();
       end != BreakIterator::DONE;
       start = end, end = Boundary->next())
  {
      dl->append((long) SetTextRange(start, end));
  }
	return dl;
}

char *
DCicu::SetTextRange(int32_t start, int32_t end)
{
  CharacterIterator *strIter = Boundary->getText().clone();
  UnicodeString  s;
  strIter->getText(s);

  char *txt = UTF16to8(&UnicodeString(s, start, end - start));
  delete strIter;
	return txt;
}
#endif


// logging functions

logfunc DCicu::LogEventFunc = NULL;


void
DCicu::LogEvent(logtype lt, unc sev, const char *desc,
	const char *d2, const char *d3, const char *d4, const char *d5)
{
	if (!LogEventFunc)
		return;

	LogEventFunc(lt, sev, desc, d2, d3, d4, d5);
}


// end of dcicu.cpp

