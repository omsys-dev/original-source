// drmifsm.cpp is the code for dcl FrameMaker sample making, JHG, 5/13/01
// Copyright (c) 2001 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dcini.h"
#include "drmif.h"


#ifdef SAMPLE

char *MIFrd::SampList[] = {
 "twas brillig", "and the", "slithy toves",
 "did gyre", "and gimble", "in the wabe",
 "all mimsy", "were the", "borogoves",
 "and the", "mome raths", "outgrabe",
 "beware the", "jabberwock", "my son",
 "the jaws", "that bite", "the claws", "that catch",
 "beware the", "jubjub bird", "and shun",
 "the frumious", "bandersnatch",
 "he took", "his vorpal sword", "in hand",
 "long time", "the manxome foe", "he sought",
 "so rested", "he by the", "tumtum tree",
 "and stood", "awhile", "in thought",
 "and as in", "uffish thought", "he stood",
 "the jabberwock", "with eyes", "of flame",
 "came whiffling", "through the", "tulgey wood",
 "and burbled", "as it came",
 "one two", "one two", "and through", "and through",
 "the vorpal", "blade went", "snicker-snack",
 "he left", "it dead", "and with", "its head",
 "he went", "galumphing", "back",
 "and hast", "thou slain", "the jabberwock",
 "come to", "my arms", "my beamish boy",
 "o frabjous day", "callooh callay",
 "he chortled", "in his joy"
};

int *MIFrd::SampListLen;
uns MIFrd::SampListNum = 0;
uns MIFrd::SampListCount = 0;

bool MIFrd::SampDelayRand = true;
uns MIFrd::SampDelay = 0;
uns MIFrd::SampDelayLimit = 85;
uns MIFrd::SampDelayMin = 40;

bool MIFrd::SampStart = false;
bool MIFrd::SampEnd = false;
uns MIFrd::SampStartPos = 0;
uns MIFrd::SampWordMin = 4;


void
MIFrd::SetUpSample(void)
{
	char *cpt = NewName(DRmifVer);
	*cpt = 's';
	DRmifVer = cpt;

	if (IniFile->Section("Jabberwocky")) {
		if (IniFile->Find("SampDelayLimit"))
			SampDelayLimit = (uns) IniFile->IntRight();
		if (IniFile->Find("SampWordMin"))
			SampWordMin = (uns) IniFile->IntRight();
		if (IniFile->Find("SampDelayRand"))
			SampDelayRand = IniFile->BoolRight();
		if (IniFile->Find("SampDelayMin"))
			SampDelayMin = (uns) IniFile->IntRight();
	}

	if (SampDelayRand) {
		srand( (unsigned)time( NULL ) );
		SampDelayLimit = (uns) ((rand() % (SampDelayLimit - SampDelayMin)) + SampDelayMin);
	}

	SampListCount = sizeof SampList / sizeof SampList[0];
	SampListLen = new int[SampListCount];
	for (uns i = 0; i < SampListCount; i++)
		SampListLen[i] = strlen(SampList[i]);
}


void
MIFrd::SampConvert(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++) {
		if (__isascii(str[i])) {
			if (str[i] == '\\') {  // skip over hex char codes
				if (str[++i] == 'x')
					i += 3;
			}
			else if (SampStart) {
				if (str[i] == ' ') {
					SampStart = false;
					SampStartPos = i;
					SampEnd = true;
				}
			}
			else if (SampEnd) {
				if (strchr(" ,.?/!;:)}]=", str[i])) {
					if (i <= (SampStartPos + SampWordMin)) {
						SampEnd = false;
						SampStart = true;
					}
					else {
						assert(SampListLen);
						int len = SampListLen[SampListNum];
						bool uc = isupper(str[SampStartPos + 1]) ? true : false;
						memmove(&str[SampStartPos + 1 + len], &str[i], strlen(&str[i]) + 1);
						memcpy(&str[SampStartPos + 1], SampList[SampListNum], len);
						if (uc)
							str[SampStartPos + 1] = toupper(*SampList[SampListNum]);
						if (++SampListNum >= SampListCount)
							SampListNum = 0;
						SampEnd = false;
						SampDelay = 0;
					}
				}
			}
			else if (++SampDelay >= SampDelayLimit)
				SampStart = true;
		}
	}
	if (SampEnd) {
		SampEnd = false;
		SampStart = true;
	}
}

#endif


// end of drmifsm.cpp


