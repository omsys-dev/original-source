// d2ird.c provides .ini-reading services for dtd2ini
// Copyright )c) 2007 Jeremy H. Griffith. All rights reserved.
// Licensed under the General Public License (GPL), version 2.

#include "dtd2ini.h"
#include <windows.h>
#include <windef.h>
#include <winnt.h>
#include <winbase.h>

// globals

#define MAX_PROFILE 2048
static unc IniBuf[MAX_PROFILE];
#define MAX_KEYLIST 4096
static unc SectBuf[MAX_KEYLIST];
static char *SectPart = NULL;

// functions

void SetIniName(const char *argv0, const char *argv3)
{
	const char *basename = NULL;
	char *dot = NULL;
	char path[1025];
	long len = 0;
	long plen = 0;
	unc nopath = 0;

	if (argv3) {
		basename = argv3;
		if ((strrchr(argv3, '\\') != NULL)
		 || (strrchr(argv3, '/') != NULL))
			nopath = 1;
	}
	else {
		basename = strrchr(argv0, '\\');
		if (!basename)
			basename = strrchr(argv0, '/');
		if (!basename)
			basename = "dtd2ini";
		else
			basename++;
	}

	dot = strrchr(basename, '.');
	len = dot ? (dot - basename) : strlen(basename);

	if (!nopath)
		plen = GetCurrentDirectory(1024, path);
	IniName = (char *) calloc(plen + len + 6, sizeof(char));
	if (plen > 0) {
		strcpy(IniName, path);
		strcat(IniName, "\\");
	}
	strncat(IniName, basename, len);
	strcat(IniName, ".ini");
}

void GetIniString(const char *section, const char *key, char **str)
{
	GetPrivateProfileString(section, key, "", IniBuf, MAX_PROFILE, IniName);

	if (*IniBuf != '\0') {
		*str = (char *) malloc(strlen(IniBuf) + 1);
		strcpy(*str, IniBuf);
	}
}

void GetIniInt(const char *section, const char *key, int *varp)
{
	GetPrivateProfileString(section, key, "", IniBuf, MAX_PROFILE, IniName);

	if (*IniBuf)
		*varp = atol(IniBuf);
}


void TrueFalseIni(const char *section, const char *key, unc *varp)
{
	unc ch = 0;
	char *buf = IniBuf;

	GetPrivateProfileString(section, key, "", IniBuf, MAX_PROFILE, IniName);

	while (*buf != 0) {
		if (isspace(*buf))
			buf++;
		else {
			ch = *buf;
			break;
		}
	}

	if (ch == 0)
		return;

	ch = toupper(ch);

	if ((ch == 'Y')
	 || (ch == 'J')
	 || (ch == 'O')
	 || (ch == '1')
	 || (ch == 'T'))
		*varp = 1;
	else if ((ch == 'N')
	 || (ch == '0')
	 || (ch == 'F'))
		*varp = 0;
}


void GetIniMatch(const char *section, const char *key, char **list, int *varp)
{
	unc ch = 0;
	unc ch2 = 0;
	int i = 0;
	char *buf = NULL;

	GetPrivateProfileString(section, key, "", IniBuf, MAX_PROFILE, IniName);
	buf = (char *) malloc(strlen(IniBuf) + 1);
	strcpy(buf, IniBuf);

	while (*buf != 0) {
		if (isspace(*buf))
			buf++;
		else
			break;
	}

	if (*buf == 0)
		return;

	for (i = 0; list[i] != NULL; i++) {
		if (!_stricmp(buf, list[i])) {
			*varp = i + 1;
			return;
		}			
	}		
}


void GetIniSect(const char *section)
{
	GetPrivateProfileString(section, NULL, "", SectBuf, MAX_KEYLIST, IniName);
	SectPart = SectBuf;
}


unc CheckSectPart(void)
{
	if (!SectPart)
		return 0;

	if (*SectPart == '\0') {
		SectPart = NULL;
		return 0;
	}

	if ((SectPart + strlen(SectPart)) > (SectBuf + MAX_KEYLIST)) {
		SectPart = NULL;
		return 0;
	}

	return 1;
}

char *GetIniSectKey(void)
{
	char *str = NULL;

	if (!CheckSectPart())
		return NULL;
	str = (char *) malloc(strlen(SectPart) + 1);
	strcpy(str, SectPart);
	SectPart += (strlen(SectPart) + 1);

	return str;
}


int GetIniSectCount(void)
{
	int count = 0;
	char *str = NULL;

	if (!SectPart)
		return 0;

	if ((*SectPart == '\0')
	 || ((SectPart + strlen(SectPart)) > (SectBuf + MAX_KEYLIST))) {
		SectPart = NULL;
		return 0;
	}

	for (str = SectPart;
	     (str < (SectBuf + MAX_KEYLIST)) && (*str != '\0');
	     str += (strlen(str) + 1))
		count++;

	return count;
}


char *GetFullIniSect(const char *name)
{
	FILE *ifp = NULL;
	static unc sbuf[1024];
	static char *linebuf = NULL;
	int len = strlen(name);
	int linelen = 0;
	char *temp;
	unc inSect = 0;

	if (linebuf) {
		free(linebuf);
		linebuf = NULL;
	}
	if ((ifp = fopen(IniName, "rt")) == NULL)
		return NULL;
	while (fgets(sbuf, 1024, ifp)) {
		if (!inSect
		 && (sbuf[0] == '[')
		 && !_strnicmp(name, sbuf + 1, len)
		 && (sbuf[len + 1] == ']')) {
			inSect = 1;
			linebuf = (char *) malloc(strlen(sbuf) + 1);
			*linebuf = '\0';
			strcat(linebuf, sbuf);
			linelen = strlen(linebuf);
		}
		else if (inSect) {
			if (sbuf[0] == '[')
				break;
			if (sbuf[0] == ';')
				continue;
			temp = (char *) malloc(linelen + strlen(sbuf) + 1);
			*temp = '\0';
			strcpy(temp, linebuf);
			strcat(temp, sbuf);
			free(linebuf);
			linebuf = temp;
			linelen = strlen(linebuf);			
		}
	}
	fclose(ifp);
	if (inSect)
		return linebuf;
	return NULL;
}

