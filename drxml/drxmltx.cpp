// drxmltx.cpp is the code for dcl XML text processing, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcelem.h"
#include "drxml.h"
#ifdef DITA
#include "expat.h"
#endif
#ifdef UDOC
#include "mxlparser.h"
#include "drxmlud.h"
#endif
#include "drxmlfl.h"



long XMLrd::ItemID = 1000;

char *XMLrd::PendingText = NULL;
long XMLrd::PendingLen = 0;
long XMLrd::InterpolatedP = 0;
char *XMLrd::InterpolatedPName = NULL;
bool XMLrd::CloseInterP = false;
long XMLrd::TextBlockElemLevel = 0;
char *XMLrd::TextBlockElemName = NULL;
DCgroup *XMLrd::TextGroup = NULL;

DCnlist XMLrd::FormatUsed;
DClist XMLrd::FormatInline;
DCnlist XMLrd::ForcedClose;
char *XMLrd::ContinuedFormatSuffix = "Cont";
bool XMLrd::AllowText = false;
DClist XMLrd::AllowTextLevels;
bool XMLrd::InMap = false;
bool XMLrd::IsInline = false;
bool XMLrd::IsPreform = false;
bool XMLrd::PreInline = false;
bool XMLrd::StartingPre = false;
long XMLrd::PreformDepth = 0;
char *XMLrd::CurrFormat = NULL;

bool XMLrd::UseElemNameForFormat = true;
bool XMLrd::UseOutclassForFormat = true;
bool XMLrd::KeepDraftComments = true;
bool XMLrd::IncludeElementTags = true;
bool XMLrd::ShowElementPath = true;
bool XMLrd::DisplayElementPath = false;

char *XMLrd::DefInlineFormat = "Char";
char *XMLrd::DefBlockFormat = "Para";
char *XMLrd::ElementPathFormat = "ElementPath";

bool XMLrd::InWhite = false;
bool XMLrd::InElemText = false;
DCilist XMLrd::InText;
DCilist XMLrd::InLine;
long XMLrd::LastDepth = 0;


void XMLrd::SetStringProps(long depth)
{
	if (depth > LastDepth) {
		InText.replace(InElemText, LastDepth);
		if (!IsInline) {
			InElemText = false;
			InWhite = false;
		}
		else
			InLine.replace(IsInline, depth);
	}
	else if (depth < LastDepth) {
		if (!InLine.find(LastDepth))
			InElemText = (bool) InText.find(depth);
		else
			InElemText = true;
		InWhite = false;
	}
	LastDepth = depth;
}


strln *XMLrd::ProcStr(const char *str, int len, long depth)
{
	//static bool KeepWhite = false;
	//static char *lastelemname = NULL;

	strln *sl = new strln;
	sl->str = (char *) str;
	sl->len = len;

	if (len == 0)
		return sl;

	int i = 0;
	int j = len - 1;

	SetStringProps(depth);

	char *s = (char *) str;
	int olen = len;

	if (!IsPreform
	 || InIndex) {
		for (i = 0; i < len; i++) {
			if ((s[i] < 0) || !isspace(s[i]))
				break;
		}
		for (j = len - 1; j >= 0; j--) {
			if (s[i] < 0)
				break;
			if ((s[j] == 0x0A)
			 && (s[j - 1] == 0x0A))
				break;
			if (!isspace(s[j]))
				break;
		}
	}

	if (i == len) { // all whitespace, ignore it
		if (!AllowText
		 || !InElemText
		 || InWhite) {
			sl->len = 0;
			return sl;
		}
		s = " ";
		len = 1;
		InWhite = true;
		j = olen;  // no trailing adjustment
	}
	else if (InWhite
	 && (i > 0)) {
		s += i;
		len -= i;
		InWhite = false;
	}
	else
		InWhite = false;

	if (j < (olen - 1)) {  // trailing whitespace
		InWhite = true;
		int adj = ((olen - 1) - j) - 1;
		if (len >= adj)
			len -= adj;
	}

	InElemText = true;
	sl->str = s;
	sl->len = len;

	return sl;
}


void
XMLrd::ProcChar(const char *str, int len, long depth)
{
	assert(depth < 128);
	//CurrDepth = depth;

#ifdef DITA
	if (RecordingElements)
		RecText(str, len, depth);
#endif

	if (SkipTopic
	 || SkipMapRef)
		return;

	if (ExcludeDepth
	 && (depth >= ExcludeDepth))
		return;

	if (MapDescDepth)
		return;

	char *txt = NULL;
	char *ntx = NULL;

	strln *sl = ProcStr(str, len, depth);
	str = sl->str;
	len = sl->len;
	delete sl;
	if (!len)
		return;

	if (CurrElemInfo)
		CurrElemInfo->isEmpty = false;

	// if first in elem, determine format
	// else concat with text already found
	if (PendingText) {
		txt = NewName(PendingText, PendingLen + len);
		ntx = txt + PendingLen;
		memcpy(txt + PendingLen, str, len);
		DeleteName(PendingText);
		PendingText = txt;
		PendingLen += len;
		//if (UMXL)
		//	ProcParaTextShorthand(ntx);
	}
	else {  // either starting, or resuming after inline
		//if (UMXL
		// && !ParaStarted)
		//	ProcParaStartShorthand((char **) &str);  // sets str to new text start
		if (!*str)
			return;
#ifdef UDOC
		// if UMXL and elem is not inline and doesn't have the text prop,
		// start a new p element before seting PendingText
		if (!TextBlockElemLevel
		 && !IsInline
		 && !XMLud::InMXMLIndexEntry
		 && (!CurrElemInfo
		  || (!XMLud::CheckUMXLElemName(CurrElemInfo->elemname, epText)
		   && !XMLud::CheckUMXLElemName(CurrElemInfo->elemname, epData))
		  || XMLud::CheckUMXLElemName(CurrElemInfo->elemname, epInline))) {
			InterpolatedP = depth;
			InterpolatedPName = "p";
			StartElement(InterpolatedPName, NULL, depth++);
			while (str
			 && *str
			 && (len > 0)
			 && ((*str == ' ')
			  || (*str == '\n')))
				str++, len--;
		}
#endif
		PendingText = NewName(str, PendingLen = len);
		if (!FormatUsed.find(CurrDepth)
		 && !(ParaStarted && InGlossTerm)
		 && !InAlt
		 && !InDesc
		 && !InRelLinks
		 && !PrePrologText
		 && !InObjectDesc) {
			if ((len > 1)
			 || (*str != ' '))
				SetElemFormat();
		}
	}

#ifdef UDOC
	char *dbl = NULL;
	char *npt = NULL;
	long dblen = 0;
	long dalen = 0;
	char *ename = NULL;

	// in UMXL, if in a text elem and double return, remove all returns,
	// flush preceding text, end the elem, and start a new one
	if (!XMLud::InMXMLIndexEntry
	 && (TextBlockElemLevel == depth - 1)
	 && !strcmp(CurrElemInfo->elemname, TextBlockElemName)) {
		while (PendingText
		 && ((dbl = strstr(PendingText,"\n\n")) != NULL)
		 && ((dbl > PendingText))) {
			dblen = dbl - PendingText;
			while (*dbl == '\n')
				dbl++;
			if ((dalen = strlen(dbl)) > 0)
				npt = NewName(dbl, dalen);
			PendingText[dblen] = '\0';
			PendingLen = dblen;
			ProcParaTextShorthand(PendingText);
			EndParaTextShorthand();
			ename = NewName(TextBlockElemName);
			EndElement(ename, depth - 1);  // flushes PendingText
			if (npt) {
				while (*npt == '\n')
					npt++;
				if ((dalen = strlen(npt)) == 0) {
					PendingText = NULL;
					PendingLen = 0;
				}
				else {
					StartElement(ename, NULL, depth - 1);
					DeleteName(ename);  // TextBlockElemName was reset from it
					PendingText = npt;
					PendingLen = dalen;
					SetElemFormat();
				}
			}
		}
	}
	if (PendingText)
		ProcParaTextShorthand(PendingText);
#endif


	if (ReadTopicTitle
	 && !InIndex) {
		// if title contains an elem with a conref,
		// must rewrite title ctl after conref processed ****
		if (!CurrTopicInfo->title)
			CurrTopicInfo->title = NewName(PendingText);
		else {
			int olen = strlen(CurrTopicInfo->title);
			txt = NewName(CurrTopicInfo->title, olen + len);
			memcpy(txt + olen, str, len);
			DeleteName(CurrTopicInfo->title);
			CurrTopicInfo->title = txt;
		}
	}
}


#ifdef UDOC
DCilist XMLrd::CurrShortPairs;  // pairs in effect in order added
DCnlist XMLrd::CurrShortEcode;  // tags in order added
DCilist XMLrd::CurrShortDepth;  // tag depth
DCnlist XMLrd::CurrShortEtext;  // endtext in order added
long XMLrd::CurrShortPairLevel = 0;


void
XMLrd::ProcParaTextShorthand(char *cpt)
{
	// called with ptr to new content in PendingText
	// symbols are * b, _ i, " q, ` tt, and ^ <>,
	//  adjust PendingText and PendingLen if any shorthand found
	if (!cpt
	 || !*cpt)
		return;

	char *txt = cpt;
	uns len = strlen(cpt) * 5;
	if (PendingText)
		len += cpt - PendingText;
	char *obuf = NewName(len);
	char *xbuf = obuf;
	bool changed = false;
	char *tag = NULL;
	char *sch = NULL;
	char *ech = NULL;
	long clev = 0;
	bool incode = false;

	long i = 0;
	long sval = 0;
	wikisym *sym = NULL;
	char sstr[5];
	long tlen = 0;
	bool bslash = false;
	long sym32 = 0;

	while (*txt) {
		if (*txt == '\\') {
			bslash = true;
			txt++;
			if (*txt == '\\') {
				txt++;
				*obuf++ = '\\';
				bslash = false;
				changed = true;
				continue;
			}
		}
		sstr[0] = *txt;
		tlen = 1;
		sym32 = (unc) *txt;
		if (*txt < 0) { // UTF-8
			tlen = IsUTF8((unc *) txt);
			if (!tlen)
				tlen = 1;
			else {
				unc *cppt = (unc *) txt;
				sym32 = UTF8(&cppt);
				for (i = 1; i < tlen; i++)
					sstr[i] = txt[i];
			}
		}
		sstr[tlen] = 0;

		if ((sval = XMLud::CurrWikiSyms->getitem(sstr)) != 0) {
			if (bslash) {
				changed = true;
				for (i = 0; i < tlen; i++)
					*obuf++ = *txt++;
				bslash = false;
				continue;
			}
			sym = (wikisym *) XMLud::WikiSymsAttrs.find(sval);
			if (sym->space) {
				bool sok = (txt == PendingText)
					|| (*(txt - 1) == ' ')
					|| (*(txt - 1) == '\t')
					|| (*(txt - 1) == '\n')
					|| ispunct(*(txt - 1));
				bool eok = (*(txt + 1) == '\0')
					|| (*(txt + 1) == ' ')
					|| (*(txt + 1) == '\t')
					|| (*(txt + 1) == '\n')
					|| ispunct(*(txt + 1));
				if (!sok && !eok) { // not preceded or followed by space
					for (long i = 0; i < tlen; i++)
						*obuf++ = *txt++;
					continue;
				}
			}
			if (!sym->code
			 && (incode
			  || IsPreform)) {
				for (i = 0; i < tlen; i++)
					*obuf++ = *txt++;
				continue;
			}
			if (sym->tag
			 || sym->sch
			 || sym->ech) {
				txt += tlen;  // skip symbol itself
				changed = true;
				clev = CurrShortPairs.getitem(sym32);
				if (!clev) { // new, add it
					CurrShortPairs.add(sym32, ++CurrShortPairLevel);
					if (sym->code == 2)
						incode = true;
					if (obuf > xbuf) {
						PendingText = NewName(xbuf);
						PendingLen = strlen(xbuf);
						DeleteName(xbuf);
						cpt = txt;
						len = strlen(cpt) * 5;
						obuf = NewName(len);
						xbuf = obuf;
						FlushText();
					}
					else {
						PendingText = NULL;
						PendingLen = 0;
					}
					if (sym->tag) {
						StartElement(sym->tag, NULL, CurrDepth + 1);
						CurrShortEcode.add(sym->tag, CurrShortPairLevel);
					}
					CurrShortDepth.add(CurrDepth, CurrShortPairLevel);
					if (sym->sch) {
						strcat(obuf, sym->sch);
						obuf += strlen(sym->sch);
						*obuf = '\0';
					}
					if (sym->ech)
						CurrShortEtext.add(sym->ech, CurrShortPairLevel);
				}
				else {  // ending
					if (sym->code == 2)
						incode = false;
					while (clev <= CurrShortPairLevel) {
						if ((ech = CurrShortEtext.find(CurrShortPairLevel)) != NULL) {
							strcat(obuf, ech);
							obuf += strlen(ech);
							*obuf = '\0';
							CurrShortEtext.remove(CurrShortPairLevel);
						}
						if (obuf > xbuf) {
							PendingText = NewName(xbuf);
							PendingLen = strlen(xbuf);
							DeleteName(xbuf);
							cpt = txt;
							len = strlen(cpt) * 5;
							obuf = NewName(len);
							xbuf = obuf;
							FlushText();
						}
						if ((tag = CurrShortEcode.find(CurrShortPairLevel)) != NULL) {
							if (CurrShortDepth.find(CurrShortPairLevel) == CurrDepth)
								EndElement(tag, CurrDepth);
							CurrShortEcode.remove(CurrShortPairLevel);
						}
						CurrShortDepth.remove(CurrShortPairLevel);
						CurrShortPairs.remove(CurrShortPairLevel--);
					}
				}
			}
			continue;
		}
		else {
			if (bslash) {
				*obuf++ = '\\';
				bslash = false;
			}
			for (i = 0; i < tlen; i++)
				*obuf++ = *txt++;
		}
	}

	if (changed) {
		PendingText = xbuf;
		PendingLen = strlen(xbuf);
	}


	// //maybe later: [TextShorthand] has symbol = tagname
	// //if not usual start/end use start space end, as for `=&lt; &gt;
	// // in that case, include tags like tt in <>s
}



void
XMLrd::EndParaTextShorthand(void)
{
	// if not closed before, all end at end of para
	// end any open pairs at end of para, in nested order
	if (!CurrShortPairLevel)
		return;

	long cspl = CurrShortPairLevel;
	CurrShortPairLevel = 0;
	if (cspl < 1)
		return;

	if ((PendingLen == 1)
	 && isspace(*PendingText)) {  // artifact at end of line
		PendingText = NULL;
		PendingLen = 0;
	}
	else
		FlushText();

	char *obuf = PendingText = NewName(cspl * 10);
	char *xbuf = obuf;
	char *tag = NULL;
	char *ech = NULL;

	while (cspl > 0) {
		if ((ech = CurrShortEtext.find(cspl)) != NULL) {
			strcat(obuf, ech);
			obuf += strlen(ech);
			*obuf = '\0';
			CurrShortEtext.remove(cspl);
		}
		if (obuf > xbuf) {
			PendingText = NewName(xbuf);
			PendingLen = strlen(xbuf);
			DeleteName(xbuf);
			obuf = NewName(cspl * 10);
			xbuf = obuf;
			FlushText();
		}
		if ((tag = CurrShortEcode.find(cspl)) != NULL) {
			if (CurrShortDepth.find(cspl) == CurrDepth)
				EndElement(tag, CurrDepth);
			CurrShortEcode.remove(cspl);
		}
		CurrShortDepth.remove(cspl);
		CurrShortPairs.remove(cspl--);
	}
	if (PendingText)
		DeleteName(PendingText);
	PendingText = NULL;
	PendingLen = 0;
}

#endif


void
XMLrd::RemoveTrailingSpace(void)
{
	while ((PendingLen > 0)
	 && ((PendingText[PendingLen - 1] == ' ')
		|| (PendingText[PendingLen - 1] == '\n')))
		PendingLen--;
	if (PendingLen == 0)
		PendingText = NULL;
	else
		PendingText[PendingLen] = '\0';
}


void
XMLrd::FlushText(void)
{
	char *eform = NULL;

	if (PendXref)
		StartXref(CurrDepth);

	if (!PendingText) {
		if (!FormatUsed.find(CurrDepth)
		 && (TextRefType == trnone)
		 && !InRelLinks
		 && !PrePrologText
		 && !CurrElemInfo->isEmpty
		 && !CurrElemInfo->checkType(epImage)
		 && !CurrElemInfo->checkType(epAlt))
			SetElemFormat(true);
		return;
	}

#ifdef UDOC
	if (XMLud::InMXMLIndexEntry) {
		if (IsPreform)
			XMLud::InMXMLIndexEntry = InIndex = false;
		else {
			XMLud::ProcUMXLIndexText(PendingText, PendingLen);
			PendingText = NULL;
			PendingLen = 0;
			return;
		}
	}
#endif

	if (InIndex) {
		RemoveTrailingSpace();
		if (!PendingText)
			return;
		ProcIndexItemText();
	}

	if (PrePrologText
	 && !InIndex
	 && !KeepPrologText) {
		PendingText = NULL;
		PendingLen = 0;
		return;
	}

	if (InObjectDesc) {
		ObjectDesc = PendingText;
		PendingText = NULL;
		PendingLen = 0;
		return;
	}

	if (InImageShape) {
		ProcImageAreaShape(PendingText);
		PendingText = NULL;
		PendingLen = 0;
		InImageShape = false;
		return;
	}
	if (InImageCoords) {
		ProcImageAreaCoords(PendingText);
		PendingText = NULL;
		PendingLen = 0;
		InImageCoords = false;
		return;
	}
	if (InImageXref) {
		CurrImageMapArea->alt = PendingText;
		PendingText = NULL;
		PendingLen = 0;
		InImageXref = false;
		return;
	}

	if (InAlt) {
		if (PendingLen) {
			if (GraphAlt) {
				char *cpt = NewName(GraphAlt, strlen(GraphAlt) + PendingLen);
				strcat(cpt, PendingText);
			}
			else
				GraphAlt = PendingText;
			PendingText = NULL;
			PendingLen = 0;
		}
		return;
	}

	if (InDesc) {
		if (PendingLen) {
			if (GraphDesc) {
				char *cpt = NewName(GraphDesc, strlen(GraphDesc) + PendingLen);
				strcat(cpt, PendingText);
				PendingText = NULL;
				PendingLen = 0;
				return;
			}
			else if (InFig
			 || InTable) {
				GraphDesc = PendingText;
				PendingText = NULL;
				PendingLen = 0;
				return;
			}
			else {
				TopicDesc = PendingText;
#ifdef UDOC
				if (!XMLud::UseDescInText) {
#endif	
					PendingText = NULL;
					PendingLen = 0;
					return;
#ifdef UDOC
				}
#endif	
			}
		}
	}

	if ((eform = ForcedClose.find(CurrDepth + 1)) != NULL) {
		ForcedClose.remove(CurrDepth + 1);
		StartPara();
		if (ContinuedFormatSuffix
		 && strncmp(eform + strlen(eform) - strlen(ContinuedFormatSuffix),
			           ContinuedFormatSuffix, strlen(ContinuedFormatSuffix))) {
			char *nform = NewName(eform, strlen(eform) + strlen(ContinuedFormatSuffix) + 1);
			strcat(nform, ContinuedFormatSuffix);
			SetFormat(nform);
		}
		else
			SetFormat(eform);
		SetFlags(CurrDepth);
	}

	if (StartPern) {
		char *pform = FormatUsed.find(PernDepth);
		if (pform) {
			StartPara();
			SetFormat(pform);
		}
		else
			SetElemFormat(true);
		StartPern = false;
		PernDepth = 0;
	}

	if (TopicAlias) {
		TextGroup->add(*TopicAlias);
		TopicAlias = NULL;
	}

	if (InXref
	 && !InImageMap)  // has text, so it is a hyperlink
		StartXrefContent();

	if (ColTabSet) {
		while (*PendingText == ' ') {
			PendingText++;
			PendingLen--;
		}
		ColTabSet = false;
	}

	if (PendAlignChar) {
		char *apos = strchr(PendingText, PendAlignChar);
		if (apos) {
			PendAlignCharPos = LinePos + (apos - PendingText);
			PendAlignChar = '\0';
		}
	}

	LinePos += PutTextCtrls(PendingText, PendingLen,
	             PreInline || InIndex,
							 IsPreform && (UMXL || !StartingPre));
	StartingPre = false;

	PendingText = NULL;
	PendingLen = 0;
}


long
XMLrd::PutTextCtrls(char *text, long len, bool brspc, bool brctl)
{
	if (!text
	 || !*text)
		return 0;

	long tcnt = 0;
	long i = 0;
	long nextout = 0;
	unc *chp = NULL;
	static char nstr[NumLenMax];

	// default brspc == false, brctl == true

	if (len == 0)
		len = strlen(text);

	for (; i < len; i++) {
		if (((text[i] == '\r')
		 || (text[i] == '\n'))) {
			if (brspc
			 || !brctl) {
				text[i] = ' ';
				tcnt++;
			}
			else {
				if (i > nextout) { // write previous string
					PutOneTextCtl(&text[nextout], (size_t) (i - nextout));
					tcnt += (i - nextout);
				}
				if (brctl) {
					DCctl *cp = new DCctl(imnem, 2, 11, 10, 1);
					TextGroup->add(*cp);
				}
				if ((text[i] == '\r')
				 && (text[i + 1] == '\n'))
					i++;
				nextout = i + 1;
			}
		}
		else if (text[i] < 0) {  // UTF-8 start
			tcnt++;
			if (i > nextout)  // write previous string
				PutOneTextCtl(&text[nextout], (size_t) (i - nextout));
			chp = (unc *) &text[i];
			if (::IsUTF8(chp)) {  // put Unicode char as single ctrl
				PutTextCharCtl(::UTF8(&chp));
				i = (((char *) chp) - text) - 1;
			}
			else {
				PutTextCharCtl(((unl) text[i]) & 0xFF); // put raw char
				sprintf(nstr, "0x%0.2X", (unc) (text[i] & 0xFF));
				LogEvent(logwarn, 1, "Non-UTF8 character ", nstr);
			}
			nextout = i + 1;
		}
		if (IsPreform) {
			StartingPre = false;
			brctl = true;
		}
		else if (text[i] == ' ') {
			if ((i > nextout)
			 && (text[i - 1] == ' ')) {
				PutOneTextCtl(&text[nextout], (size_t) (i - nextout));
				tcnt += (i - nextout);
				while (text[i]
				 && (text[i] == ' '))
					i++;
				nextout = i;
				i--;
			}
		}
	}

	if (i > nextout) {
		PutOneTextCtl(&text[nextout], (size_t) (i - nextout));
		tcnt += (i - nextout);
	}

	return tcnt;
}


void 
XMLrd::PutOneTextCtl(char *text, size_t len)
{
#ifdef SAMPLE
	static char buff[4096];
	char *txt = buff;
	bool deltxt = false;
#endif

	if (len == 1)
		PutTextCharCtl(((unl) *text) & 0xFF);
	else {
#ifdef SAMPLE
		if (len > 4096) {
			txt = NewName(len * 2);
			deltxt = true;
		}
		memcpy(txt, text, len);
		txt[len] = '\0';
		SampConvert(txt);
		len = strlen(text = txt);
#endif
		PutTextStrCtl(text, len);
#ifdef SAMPLE
		if (deltxt)
			DeleteName(txt);
#endif
	}
}



// format processing


void
XMLrd::SetElemFormat(bool flush)
{
	long depth = CurrDepth;
	DCelem *einfo = NULL;
	DCelem *epinfo = NULL;
	char *form = NULL;
	long fnum = 0;
	bool inl = false;
	DCctl *cp = NULL;
	long i = 0;

	// back up to last unset block format or unset inlines
	while (depth
	 && FormatInline.find(depth)
	 && !FormatUsed.find(depth))
		depth--;

	if (!depth)
		depth = 1;

	while (FormatUsed.find(depth))
		depth++;

	if (PernDepth)
		depth = PernDepth;

	// get prefix for wrapper element if any
	if (!FormatUsed.find(depth)
	 && !FormatUsed.find(depth - 1)
	 && ((einfo = (DCelem *) CurrEInfo.find(depth)) != NULL)
	 && einfo->isfirst
	 && ((epinfo = (DCelem *) CurrEInfo.find(depth - 1)) != NULL))
		SetParentPrefix(einfo, epinfo, depth - 1);

	// set all parents and current format as needed
	for ( ; depth <= CurrDepth; depth++) {
		einfo = (DCelem *) CurrEInfo.find(depth);
		if (!einfo)
			break;;
		if (!einfo->classatt)
			continue;
		if (einfo->checkType(epFtn)) {  // special cases
			StartFootnote((DCtopelem *) einfo, depth);
			FormatUsed.add(FootnoteFormat, depth);
		}
#ifdef UDOC
		else if (einfo->checkType(epRef | (epInline << 8))
#endif
#ifdef DITA
		else if (einfo->checkType(epRef, epTopic | (epPre << 8))
#endif
		) {
			if (PendXref
			 && !InXref)
				StartXref(depth);
		}
		else if (einfo->checkType((epMap << 16) | (epMeta << 8) | epTopic))
			;
		else if (einfo->checkType(epImage))
			;
		else if (einfo->checkType(epAlt))
			;
		else if (einfo->checkType(epTypo))
			;
#ifdef UDOC
		else if (einfo->checkType(epTab | (epInline << 8)))
			;
		else if (einfo->checkType(epVar | (epInline << 8)))
			;
		else if (einfo->checkType(epAbbr | (epInline << 8)))
			;
		else if (einfo->checkType(epTmark | (epInline << 8)))
			;
		else if (einfo->checkType(epVar | (epData << 8)))
			;
		else if (einfo->checkType(epVar | (epDef << 8)))
			;
		else if (einfo->checkType((epDef << 16) | (epRef << 8) | epElem))
			;
		else if (einfo->checkType(epGroup | (epNolev << 8)))
			;
#endif
		else if (einfo->checkType(epIndex))
			;
		else {                  // none of the above
			form = einfo->elform;
			if (InKeyDef) {
				inl = true;
				char *en = einfo->elemname;
				if (!strcmp(en, "keyword")
				 || !strcmp(en, "linktext"))
					continue;
			}
			else {
				inl = FormatInline.find(depth);
				if (inl) {
					if (SetFlags(depth)
					 && !strcmp(einfo->elemname, "ph")
					 && !einfo->outclass) {
						einfo->elskip = true;
						form = "no";
					}
				}
			}
			if (!einfo->elpath)
				einfo->elpath = GetElemPath(depth);
#if 0
			if (CurrNoteType
			 && UseNoteFormats)
				form = SetNoteFormat();
#endif
			if (!form)
				form = GetElemForm(einfo, einfo->outclass,
			                     einfo->elemname, inl);
			// write ctl for format if not "No"
			if (form
			 && _stricmp(form, "no")) {
				bool pstart = false;
				if (!inl
				 && (!flush
				  || (depth < CurrDepth))) {
					if (CurrTaskHeadType) {
						if ((einfo->parent->elemtype == fig)
						 && !FigTitleAboveImage)
							;  // leave for fig anchor to put out
						else if (CurrTaskHeadElem != einfo)
							SetStepsPara(einfo);
						else
							CurrTaskHeadUsed = true;
					}
					StartPara();
					pstart = true;
				}
				if (!inl
				 && (InDefinitionList
				  || InParameterList)) {
					char *nf = NewName(form, strlen(form) + 4);
					strcat(nf, "Tbl");
					DeleteName(form);
					form = nf;
				}
				if ((inl
				  || !flush
				  || pstart)
				 && !(einfo->formctl
				  || (PendXref && !UMXL)
				  || (InXref && !pstart))) {
					einfo->elform = form;
					einfo->formctl = SetFormat(CurrFormat = form, inl && !pstart);
					if (!inl)
						SetFlags(depth);
					else if (InKeyDef)
						KeyDefForm++;
				}
				if (ShowElementPath && !einfo->epctl && pstart) {
					einfo->epctl = cp = new DCctl(ename, 2, 121, 6);
					cp->ex(einfo->elpath);
					TextGroup->add(*cp);
					if (einfo->elfpath
					 && strcmp(einfo->elpath, einfo->elfpath)) {
						einfo->epfctl = cp = new DCctl(ename, 2, 121, 6);
						cp->ex(einfo->elfpath);
						TextGroup->add(*cp);
					}
				}
				if (BorderFmt) {
					SetSpecialFormat(sfmt_br, BorderFmt);
					BorderFmt = NULL;
				}
				if (ShadeFmt) {
					SetSpecialFormat(sfmt_sh, ShadeFmt);
					ShadeFmt = NULL;
				}
				if (pstart)
					WriteTextNumPrefix(einfo, form);
				if (BlockElemCount > BlockElemsStarted)
					SetBlockElemTags();  // must precede any real text
				if (pstart) {
					if (PendingIndexItems) {
						TextGroup->add(*((DCctl *) PendingIndexItems));
						PendingIndexItems = NULL;
					}
					if (DisplayElementPath
					 && einfo->elpath) {
						SetFormat(ElementPathFormat, true);
						PutTextStrCtl(einfo->elpath);
						SetFormat(NULL, true);
					}
					SetFilterClasses(false, inl);
#ifdef DITA
					if (ReadTopicTitle)  // provide place for metadata
						SetMetadata();
#endif
					WriteFormatRunin(einfo, form);
				}
				if (!strcmp(CurrElemName, "info"))
					PutTextCharCtl(0x20);
			}
			else if (BlockElemCount > BlockElemsStarted)
				SetBlockElemTags();
			if (inl) {
				SetFilterClasses(false, inl);
				if (InlineElemCount > InlineElemsStarted)
					SetInlineElemTags();
			}
			FormatUsed.add(form, depth);
		}
		ProcessVariableStart(einfo);
	}
}


void 
XMLrd::SetParentPrefix(DCelem *einfo, DCelem *epinfo, long depth)
{
	char *form = epinfo->elform;
	bool inl = FormatInline.find(depth);
	if (inl) {
		if (SetFlags(depth)
		 && !strcmp(epinfo->elemname, "ph")
		 && !einfo->outclass) {
			einfo->elskip = true;
			form = "no";
		}
	}
	if (!epinfo->elpath)
		epinfo->elpath = GetElemPath(depth);
	if (!form)
		form = GetElemForm(epinfo, epinfo->outclass,
			                 epinfo->elemname, inl);
	FormatUsed.add(epinfo->elform = form, depth);

	if ((depth < CurrDepth)
	 && (CurrElemDepth > 3)
	 && stricmp(epinfo->elform, "delete")
	 && stricmp(epinfo->elform, "no")
	 && strcmp(epinfo->elform, epinfo->elemname)
	 && !strcmp(einfo->elemname, "p")
	 && !einfo->outclass)
		FormatUsed.add(einfo->elform = form, depth + 1);
}


void 
XMLrd::SetMetadata(void)
{
	DCctl *cp = NULL;
	char *cpt = NULL;

	if (!CurrTopicInfo->metatext)
		CurrTopicInfo->metatext = new DCgroup;
	TextGroup->add(*((DCctl *) (CurrTopicInfo->metatext)));

	if (TopicAlias) {
		TextGroup->add(*TopicAlias);
		TopicAlias = NULL;
	}

	// get locked navtitle as title marker
	if (CurrTopicInfo->mref
	 && CurrTopicInfo->mref->navtitle
	 && CurrTopicInfo->mref->usenav) {
		cp = new DCctl(etext, 2, 110, 3);
		cpt = NewName("title:",
		              strlen(CurrTopicInfo->mref->navtitle) + 7);
		cp->ex(strcat(cpt, CurrTopicInfo->mref->navtitle));
		TextGroup->add(*cp);
	}

	// get topic print and search usage
	if (CurrTopicInfo->mref) {
		if (CurrTopicInfo->mref->usage) {
			cp = new DCctl(etext, 2, 110, 3);
			cpt = NewName("print:", 11);
			strcat(cpt, (CurrTopicInfo->mref->usage == outprint) ? "only" :
			      ((CurrTopicInfo->mref->usage == outweb) ? "no" : "yes"));
			cp->ex(cpt);
			TextGroup->add(*cp);
		}
		if (CurrTopicInfo->mref->search) {
			cp = new DCctl(etext, 2, 110, 3);
			cpt = NewName("search:", 11);
			strcat(cpt, CurrTopicInfo->mref->search ? "yes" : "no");
			cp->ex(cpt);
			TextGroup->add(*cp);
		}
	}

	// add any metadata from topicref topicmeta
	DCmapelem *tmeta = NULL;
	if (CurrTopicInfo->mref
	 && CurrTopicInfo->mref->children
	 && CurrTopicInfo->mref->children->next)
		tmeta = (DCmapelem *) (CurrTopicInfo->mref->children->next->id);
	if (tmeta
	 && tmeta->metatext) {
		DCctl *cp = NULL;
		DCitem *ip = tmeta->metatext->first;
		while (ip) {
			cp = ip->ctl;
			if (cp->match(2, 110, 1))  // index tokens
				TextGroup->add(*cp);
			ip = ip->next;			
		}
	}
	//TextGroup->add(*((DCctl *) tmeta->metatext));
}


char *
XMLrd::GetElemPath(long lev, long num)
{
	static char epath[ElemPathLen];
	*epath = '\0';
	char *str = NULL;
	DCelem *ei = NULL;

	for (long i = lev; i >= CurrRootDepth; i--) {
		str = CurrElems.find(i);
		if (str) {
			ei = (DCelem *) CurrEInfo.find(i);
			if (!ei
			 || ei->checkType(epNolev))
				continue;
#ifdef UDOC
			if (!stricmp(str, "div")
			 || (InTextref
			  && (i > CurrRootDepth)
			  && !stricmp(str, "doc")))
				continue;
#endif
			if (!strcmp(str, "topichead"))
				str = "topicref";
			else if (!strcmp(str, "codedocref"))
				str = "docref";
			strcat(epath, str);
			if ((ei != NULL)
			 && (ei->attrcnt > 0)
			 && IniFile
			 && IniFile->Section("ElementPathAttributes")
			 && IniFile->Find(str)
			 && IniFile->ListRight()) {
				char *attr = NULL;
				while ((attr = IniFile->ListItem()) != NULL) {
					long pos = ei->attrnames->getitem(attr);
					if (pos) {
						char *val = ei->attrvals->find(pos);
						if (val && *val) {
							strcat(epath, "@");
							strcat(epath, attr);
							strcat(epath, ":");
							strcat(epath, val);
						}
					}
					DeleteName(attr);
				}
			}
			if (((ei != NULL)
			  && ei->isfirst)
			 || (IsFirst
			  && (i == CurrElemInfo->elemdepth)))
			  //&& (i == lev
				strcat(epath, "^");
			else
				strcat(epath, "/");
		}
	}
	sprintf(epath + strlen(epath), "%d", num ? num : CurrDocLevel);
	char *ep = epath;
	return NewName(epath);
}


void
XMLrd::GetExtendedElemPath(DCelem *elem, long lev)
{
	static char epath[ElemPathLen];

	//if (CurrDocRef == InitDocRef)
	//	return;

	char *ept = NULL;
	DCmapelem *mpelem = elem->GetMapParent();
	long nest = mpelem ? (lev ? lev : CurrDocLevel) - mpelem->mlevel : 0;

	strcpy(epath, elem->elpath);

	if (!InGlossary
	&& (nest > 0)) {
		ept = epath + strlen(epath);
		// get depth number
		while ((ept > epath)
		 && (*ept != '/')
		 && (*ept != '^'))
			ept--;
		char *endnum = NewName(++ept);
		*ept = '\0';
		for (long i = 0; i < nest; i++)
#ifdef DITA
			strcat(epath, "topicref/");
#endif
#ifdef UDOC
			strcat(epath, "docref/");
#endif
		// restore depth number
		strcat(epath, endnum);
		DeleteName(endnum);
	}

	if (InGlossary) {
		ept = epath + strlen(epath);
		// remove depth number
		while ((ept > epath)
		 && (*ept != '/')
		 && (*ept != '^'))
			ept--;
		*++ept = '\0';
		strcpy(ept, DocRoot);
		strcat(ept, "/map/1");
	}
	else {
		while (mpelem
		 && mpelem->elpath) {
			ept = epath + strlen(epath);
			// remove depth number
			while ((ept > epath)
			 && (*ept != '/')
			 && (*ept != '^'))
				ept--;
			*++ept = '\0';
			// remove map layer to normalize path
			if ((ept > (epath + 5))
			 && ((*(ept - 5) == '/')
				|| (*(ept - 5) == '^'))
			 && !strncmp(ept - 4, "map", 3)) {
				// remove preceding topicref too
				ept -= 4;
				*ept = '\0';
				if ((ept > (epath + 8))
				 && ((ept == epath + 9)
					|| (*(ept - 10) == '/')
					|| (*(ept - 10) == '^'))
#ifdef DITA
				 && !strncmp(ept - 9, "topicref", 8)) {
#endif
#ifdef UDOC
				 && !strncmp(ept - 9, "mapref", 6)) {
#endif
					ept -= 9;
					*ept = '\0';
				}
			}
			strcpy(ept, mpelem->elpath);
			mpelem = mpelem->map->mref;
		}
	}
	elem->elfpath = NewName(epath);
}


char *
XMLrd::GetElemForm(DCelem *elem, const char *outclass,
	                 const char *ename, bool inl, long lev)
{
	char *oclass = NULL;
	char *nform = NULL;

	if (outclass
	 && strcmp(outclass, "-dita-use-conref-target")
	 && IniFile->Section(inl ? "InlineOutclassMaps" : "BlockOutclassMaps")
	 && IniFile->Find(outclass)
	 && ((oclass = IniFile->StrRight()) != NULL)
	 && (*oclass != '\0'))
		nform = oclass;

	else if (UseOutclassForFormat
	 && outclass
	 && strcmp(outclass, "-dita-use-conref-target"))
		nform = NewName(outclass);

	else {
		if (!elem->elfpath)
			GetExtendedElemPath(elem, lev);  // check extended path
		if (IniFile->Section(inl ? "InlineFormatMaps" : "BlockFormatMaps")
		 && (IniFile->FindElemPath(elem->elfpath)
			|| IniFile->FindElemPath(elem->elpath)))
			nform = IniFile->StrRight();
		else if (UseElemNameForFormat)
			nform = NewName(ename);
		else
			nform = NewName(inl ? DefInlineFormat : DefBlockFormat);
	}

	GetElementAttrPrefixes(elem, &nform);

	return nform;
}


DCnlist XMLrd::BlockFormatList;
DCnlist XMLrd::InlineFormatList;
long XMLrd::BlockFormCount = 0;
long XMLrd::InlineFormCount = 0;

DCclist XMLrd::BlockFormats;
DCclist XMLrd::InlineFormats;
long XMLrd::BlockFormUseCount = 0;
long XMLrd::InlineFormUseCount = 0;


long 
XMLrd::StoreFormat(const char *form, bool inl)
{
	if (inl) {
		InlineFormatList.addsorted(NewName(form), ++InlineFormCount);
		return InlineFormCount;
	}
	BlockFormatList.addsorted(NewName(form), ++BlockFormCount);
	return BlockFormCount;
}


DCctl *
XMLrd::SetFormat(const char *form, bool inl, bool setgroup)
{
	if (!TextGroup && setgroup)
		return NULL;
	long fnum = 0;  // used for restore, when form == NULL
	DCctl *cp = NULL;

	if (!InResTopic) {
		if (inl) {
			if (form
			 && ((fnum = InlineFormatList.getitem(form)) == 0))
				fnum = StoreFormat(form, inl);
			cp = new DCctl(imnem, 2, 82, 2, fnum);
			if (setgroup)
				TextGroup->add(*cp);
			InlineFormats.add(cp, ++InlineFormUseCount);
		}
		else {
			if (form
			 && ((fnum = BlockFormatList.getitem(form)) == 0))
				fnum = StoreFormat(form, inl);
			cp = new DCctl(imnem, 2, 82, 1, fnum);
			if (setgroup)
				TextGroup->add(*cp);
			BlockFormats.add(cp, ++BlockFormUseCount);
		}
	}

	SetUniqueID();

	SetStartTextGroup();

	return cp;
}


void
XMLrd::SetSpecialFormat(sfmt sftype, const char *fname)
{
	DCctl *cp = new DCctl(ename, 2, 85, 0);

	switch (sftype) {
		case sfmt_tb:  // table (non-FM) format
			cp->dat(1);
			break;

		case sfmt_tr:  // table row format
			cp->dat(2);
			break;

		case sfmt_tc:  // table cell format
			cp->dat(3);
			break;

		case sfmt_br:  // border format
			cp->dat(4);
			break;

		case sfmt_sh:  // shading format
			cp->dat(5);
			break;

		default:
			break;
	}

	cp->ex(fname);

	if (TextGroup)
		TextGroup->add(*cp);
}


//tabs

tset *XMLrd::Tabs[TabMax];
tset *XMLrd::CurrTab = NULL;
bool XMLrd::TabPending = false;
long XMLrd::TabCount = 0;
long XMLrd::PendTabLinePos = 0;
unc XMLrd::PendAlignChar = '\0';
long XMLrd::PendAlignCharPos = 0;
DCgroup *XMLrd::PendTabStart = NULL;
tunit XMLrd::TabUnits = tuNone;
bool XMLrd::ColTabSet = false;
bool XMLrd::PreTabPreform = false;
bool XMLrd::IsTabPreform = false;


void XMLrd::SetTabs(DCelem *elem)
{
	//long anum = elem->attrnames->getitem("units");
	//char *units = elem->attrvals->find(anum);
	//if (!stricmp(units, "cols")) {
		TabUnits = tuCol;
		PreTabPreform = IsPreform;
		IsTabPreform = true;
		CurrTab = NULL;
		TabPending = false;
		PendTabLinePos = 0;
		PendAlignCharPos = 0;
		PendTabStart = NULL;
	//}
	// add more units here

	long anum = 0;
	char *tname = NULL;
	char *tval = NULL;
	long tnum = 0;
	tset *tbset = NULL;
	DCnlist *tlist = new DCnlist();
	TabCount = elem->attrnames->getallwildval("t*", tlist);
	while ((tname = tlist->dequeue(&anum)) != NULL) {
		if (((tnum = atol(tname + 1)) == 0)
		 || (tnum > TabMax))
			continue;
		Tabs[tnum - 1] = (tbset = new tset);
		tval = elem->attrvals->find(anum);
		switch (*tval) {
			case 'l':
			case 'L':
				tbset->type = 1;
				tval++;
				break;

			case 'c':
			case 'C':
				tbset->type = 2;
				tval++;
				break;

			case 'r':
			case 'R':
				tbset->type = 3;
				tval++;
				break;

			default:
				if (isdigit(*tval))
					tbset->type = 1;
				else
					tbset->type = *tval++;
				break;
		}
		tbset->pos = atol(tval);
		while (isdigit(*tval))
			tval++;
		if (*tval) {
			if (!stricmp(tval, "dot"))
				tbset->leader = '.';
			else if (!stricmp(tval, "spc"))
				tbset->leader = ' ';
			else
				tbset->leader = *tval;
		}
	}
}


void XMLrd::ClearTabs(void)
{
	for (long i = 0; i < TabCount; i++)
		delete Tabs[i];
	TabCount = 0;
	TabUnits = tuNone;
	if (IsTabPreform) {
		IsPreform = PreTabPreform;
		IsTabPreform = false;
	}
	CurrTab = NULL;
	TabPending = false;
	PendTabLinePos = 0;
	PendAlignCharPos = 0;
	PendTabStart = NULL;
}


void XMLrd::UseTab(char *tnum)
{
	if (TabPending)
		SetPendingTab();

	long tpos = atol(tnum);
	if (!tpos)
		return;

	FlushText();

	if (TabUnits == tuNone) {  // pass indexed tab to output module
		DCctl *cp = new DCctl(ilong, 2, 41, 10, tpos);
		TextGroup->add(*cp);
		return;
	}

	if (tpos > TabCount)
		return;
	tset *tbset = Tabs[tpos - 1];
	long spcnt = 0;
	char *spcs = NULL;

	if (TabUnits == tuCol) {
		if (LinePos >= tbset->pos) {
			PutTextCharCtl(0x20);
			LinePos++;
		}
		else switch (tbset->type) {
			case 0:
				break;

			case 1:  // left
				spcnt = tbset->pos - LinePos;
				spcs = NewName(spcnt + 1);
				memset(spcs, ' ', spcnt);
				PutTextStrCtl(spcs, spcnt);
				DeleteName(spcs);
				LinePos = tbset->pos;
				break;

			case 2:  // center
				StartPendingTab(tbset);
				break;

			case 3:  // right
				StartPendingTab(tbset);
				break;

			default: // align char
				StartPendingTab(tbset);
				PendAlignChar = tbset->type;;
				break;
		}
		ColTabSet = true;
		// ensure in pre for col tabs
		IsPreform = IsTabPreform;

	}
}


void
XMLrd::StartPendingTab(tset *tbset)
{
	if (TabUnits == tuNone)
		return;

	TabPending = true;
	CurrTab = tbset;
	PendTabLinePos = LinePos;
	PendAlignChar = '\0';
	PendAlignCharPos = 0;
	PendTabStart = new DCgroup();
	TextGroup->add(*PendTabStart);
}


void
XMLrd::SetPendingTab(void)
{
	// at next tab or end of line
	// may be other tags in between
	// put spaces into start group as needed

	if (TabUnits == tuNone)
		return;

	DCgroup *ltext = TextGroup;
	long splen = LinePos - PendTabLinePos;
	if (!PendAlignCharPos)
		PendAlignCharPos = LinePos;

	long startpos = 0;
	if (CurrTab->type == 2)       // center
		startpos = CurrTab->pos - (splen / 2); 
	else if (CurrTab->type == 3)  // right
		startpos = CurrTab->pos - splen; 
	else                          // align char
		startpos = CurrTab->pos - (PendAlignCharPos - PendTabLinePos);

	long spcnt = startpos - PendTabLinePos;
	TextGroup = PendTabStart;
	if (startpos <= PendTabLinePos) {
		PutTextCharCtl(0x20);
		LinePos++;
	}
	else {
		char *spcs = NewName(spcnt + 1);
		memset(spcs, ' ', spcnt);
		PutTextStrCtl(spcs, spcnt);
		DeleteName(spcs);
		LinePos = startpos + splen;
	}
	TextGroup = ltext;

	CurrTab = NULL;
	TabPending = false;
	PendTabLinePos = 0;
	PendAlignCharPos = 0;
	PendTabStart = NULL;
}



// page processing

uns XMLrd::PageNum = 0;
DCpage *XMLrd::CurrPageInfo = NULL;
DCvlist XMLrd::Pages;
DCvlist XMLrd::MapPages;

long XMLrd::PageSeqNum = 0;
long XMLrd::LastPageSeq = 0;
DCilist XMLrd::PageSequence;   // orig pages indexed by final pagenums
DClist XMLrd::RemovedPages;    // orig pages no longer used in sequence
DClist XMLrd::ConrefPages;     // conref'd pages not used in sequence

uns XMLrd::FirstTopicPage = 1;
DCgroup *XMLrd::PageGroup = NULL;
DCgroup *XMLrd::PageTextGroup = NULL;
DCclist XMLrd::PageList;
bool XMLrd::PageStarted = false;
DCclist XMLrd::PageTextCtls;  // start of page text, use to insert figs

uns XMLrd::FrameNum = 0;
DCilist XMLrd::FramePageList;  // pagenum indexed by framenum
uns XMLrd::TextNum = 1;  // same for whole stream, diff for maps and topics


void
XMLrd::StartPage(void)
{
	DCctl *cp = NULL;
	DCgroup *gp = NULL;

	if (PageStarted)
		EndPage();

	//PendXref = PendLink = InXref = false;
	//LinkHref = XrefHref = NULL;

	//if (FootnoteReset == 1)
	//	CurrFootnoteSeq = 0;
	//if (TblFootReset == 1)
	//	CurrTblFootSeq = 0;
	ResetStreams(rtTopic);
	IsInline = false;

	CurrPageInfo = new DCpage(++PageNum);
	Pages.add(CurrPageInfo, PageNum);
 
	if (InMap
	 && !ProcessingConrefs) {
		FirstTopicPage++;
		if (UseContents)
			PageSequence.add(PageNum, ++PageSeqNum);
		else
			RemovedPages.add(PageNum);
	}
	else if (InResTopic)
		RemovedPages.add(PageNum);
	else if (!ProcessingConrefs)
		PageSequence.add(PageNum, ++PageSeqNum);
	else
		ConrefPages.add(PageNum);

	LastPageSeq = PageSeqNum;


	// set all ctls for *first* page that is also *last* page

	PageGroup = new DCgroup(DCctl(group, 3, 10, 1));  // pagenum
	CurrPageInfo->pgroup = PageGroup;
	PageList.add(PageGroup, PageNum);

	cp = new DCctl(ilong, 2, 112, 2, ++ItemID);  // loc
	PageGroup->add(*cp);

	cp = new DCctl(imnem, 3, 11, 2, InMap ? 4 : 3);  // usage map/topic
	PageGroup->add(*cp);

	cp = new DCctl(imnem, 3, 11, 3, 0);  // not master
	PageGroup->add(*cp);


	// layout frame for page
	CurrPageInfo->firstframe = ++FrameNum;
	CurrPageInfo->textnum = TextNum;

	gp = new DCgroup(DCctl(group, 3, 20, 1));  // text framenum
	CurrPageInfo->layout = gp;
	PageGroup->add(*gp);

	FramePageList.add(PageNum, FrameNum);

	cp = new DCctl(ilong, 2, 112, 2, ++ItemID);  // loc
	gp->add(*cp);

	cp = new DCctl(imnem, 3, 40, 1, 0);  // textnum, next framenum
	CurrPageInfo->nextframe = cp;
	gp->add(*cp);
		
	cp = new DCctl(imnem, 3, 21, 2, InMap ? 6 : 1);  // frame usage
	gp->add(*cp);


	// text for page
	PageTextGroup = new DCgroup(DCctl(group, 2, 1, 1));    // text stream textnum
	PageTextCtls.add(PageTextGroup, PageNum);
	CurrPageInfo->pagetext = PageTextGroup;
	PageGroup->add(*PageTextGroup);

	cp = new DCctl(imnem, 2, 3, 0, 0); // text prev frame and pagenum
	CurrPageInfo->textprev = cp;
	PageTextGroup->add(*cp);

	cp = new DCctl(imnem, 2, 2, 1, 1); // text framenum, pagenum
	CurrPageInfo->textframe = cp;
	PageTextGroup->add(*cp);

	cp = new DCctl(ename, 2, 6, 1);    // flow tag
	cp->ex(InMap ? "Map" : "Topic");
	CurrPageInfo->flowtag = cp;
	PageTextGroup->add(*cp);

	TextGroup = PageTextGroup;  // for further text additions
	PageStarted = true;

	if ((PageNum == 1)
	 && InMap
	 && (!UsingBookmap
	  || KeepTOCWithTitlePage)
	 && UseContents
	 && TOCFile
	 && *TOCFile)
		SetMapName = true;

}


bool XMLrd::SetMapName = false;

void
XMLrd::SetChunkMarker(char *fname)
{
	DCctl *cp = new DCctl(etext, 2, 110, 3);
	cp->ex("chunk", 6);
	TextGroup->add(*cp);

	char *txt = NewName("chunkfile:", strlen(fname) + 11);
	strcat(txt, fname);
	cp = new DCctl(etext, 2, 110, 3);
	cp->ex(txt, strlen(txt));
	TextGroup->add(*cp);
}


void
XMLrd::EndPage(void)
{
	DCctl *cp = NULL;
	bool exclude = false;

	if (!PageGroup)
		return;

	// close any open elements in map/topic
	if (InMap) {
		TextGroup = PageTextGroup;
		EndPara();
	}
	else if (ParaStarted) {
		EndPara();
		TextGroup = PageTextGroup;
	}
	else
		TextGroup = PageTextGroup;

	if (CurrCondFlags)
		EndFlags();

	if (CurrPageInfo->topic
	 && CurrPageInfo->topic->mref) {
		switch (CurrPageInfo->topic->mref->usage) {
			case outprint:
				if (!PrintProject)
					exclude = true;
				break;

			case outweb:
				if (PrintProject)
					exclude = true;
				break;

			case outall:
			case outnone:
			default:
				break;
		}
	}

	if ((PageNum != 1)
	 || !InMap) {  // provide for related links
		DCgroup *gp = new DCgroup();
		CurrPageInfo->relend = gp;
		PageTextGroup->add(*gp);
	}

#if 0
	else if (!IndexInTOC   // at end of map, add token for index
	 && OutputHTML) {
		cp = new DCctl(imnem, 2, 117, 2, 0);
		PageTextGroup->add(*cp);
	}
#endif

	if (TOCWrapper
	 && (PageNum == 1)) {
		PageTextGroup = TextGroup = CurrPageInfo->pagetext;
		//PageGroup->add(*TOCWrapper);
	}

	cp = new DCctl(imnem, 2, 11, 2, 0); //make sure of para end
	PageTextGroup->add(*cp);

	cp = new DCctl(imnem, 2, 4, 0, 0); // next frame num, next page
	CurrPageInfo->textnext = cp;
	PageTextGroup->add(*cp);

	//cp = new DCctl(endgroup, 2, 1, 1);
	//CurrPageInfo->textend = cp;
	//PageGroup->add(*cp);

	PageGroup = PageTextGroup = TextGroup = NULL;
	PageStarted = false;

	if (exclude) {
		PageSequence.remove(PageSeqNum--);
		RemovedPages.add(PageNum);
		if (LastPageSeq > PageSeqNum)
			LastPageSeq = PageSeqNum;
	}
}


void 
XMLrd::SetFirstPage(void)
{
	// ensure initial values are set for start
	long page = PageSequence.find(1);
	DCpage *pg = (DCpage *) Pages.find(page);
	if (!pg)
		return;

	// set graphic frames on first page
	pg->firstframe = 1;
	if (pg->graphics)
		pg->lastframe = SetGraphicFrames(pg, 1);
	else
		pg->lastframe = 1;

	pg->textnum = 1;
}


void 
XMLrd::ConnectPage(long pgseq, long lpgseq)
{
	// last page frame and text info
	long lpage = PageSequence.find(lpgseq);
	DCpage *lpg = (DCpage *) Pages.find(lpage);
	uns lasttextframe = (uns) lpg->firstframe;
	unl lasttextpage = lpgseq;
	uns lasttextnum = lpg->textnum;

	// current page info
	long pagenum = PageSequence.find(pgseq);
	DCpage *cpg = (DCpage *) Pages.find(pagenum);
	uns textframe = lpg->lastframe + 1;
	unl textpage = pgseq;
	uns textnum = lpg->textnum;

	cpg->firstframe = textframe;
	if (cpg->graphics)
		cpg->lastframe = SetGraphicFrames(cpg, pgseq);
	else
		cpg->lastframe = textframe;

	cpg->pgroup->dat((uns) pgseq);

	cpg->layout->dat(textframe);
	cpg->nextframe->dat(textnum);
	lpg->nextframe->siz(textframe);  // last next is current

	cpg->textnum = textnum;
	cpg->pagetext->dat(textnum);

	cpg->textframe->dat(textframe);
	cpg->textframe->siz(pgseq);

	if (lasttextnum == textnum) {  // continuing same stream
		cpg->textprev->dat(lasttextframe);
		cpg->textprev->siz(lasttextpage);
		cpg->flowtag->maj((uns) 0);
		lpg->textnext->dat(textframe);
		lpg->textnext->siz(pgseq);
	}

	//cpg->textend->dat(textnum);
}




// para processing

bool XMLrd::ParaStarted = false;
long XMLrd::LinePos = 0;
bool XMLrd::StartPern = false;
long XMLrd::PernDepth = 0;
bool XMLrd::StartIgnoreGroup = false;
bool XMLrd::EndIgnoreGroup = false;
bool XMLrd::InIgnoreGroup = false;
DCgroup *XMLrd::PreIgnoreGroup = NULL;
DCgroup *XMLrd::IgnoreGroup = NULL;
DCgroup *XMLrd::LastParaStartGroup = NULL;

void
XMLrd::StartPara(void)
{
	DCctl *cp = NULL;

	if (!PageStarted)
		StartPage();
	if (ParaStarted)
		EndPara(LastParaStartGroup);
	if (PendingBreak) {
		TextGroup->add(*PendingBreak);
		PendingBreak = NULL;
	}
	if (StartIgnoreGroup) {
		if (!InIgnoreGroup) {
			PreIgnoreGroup = TextGroup;
			IgnoreGroup = new DCgroup(DCctl(group, 1, 30, 1));
			TextGroup->add(*IgnoreGroup);
			TextGroup = IgnoreGroup;
			InIgnoreGroup = true;
		}
		StartIgnoreGroup = false;
	}
	cp = new DCctl(imnem, 2, 11, 1);
	TextGroup->add(*cp);

	ParaStarted = true;
	LastParaStartGroup = TextGroup;
	LinePos = 0;
}


void
XMLrd::EndPara(DCgroup *txt)
{
	DCctl *cp = NULL;
	DCgroup *ltxt = TextGroup;
	if (txt)
		TextGroup = txt;
	if (TabPending)
		SetPendingTab();

	SetEndTextGroup();

	if (CurrAnumGroup) {
		TextGroup->add(*CurrAnumGroup);
		CurrAnumGroup = NULL;
	}

	cp = new DCctl(imnem, 2, 11, 2);
	TextGroup->add(*cp);
	if (InIgnoreGroup) {
		//cp = new DCctl(endgroup, 1, 30, 1);
		//TextGroup->add(*cp);
		TextGroup = PreIgnoreGroup;
		InIgnoreGroup = false;
		EndIgnoreGroup = false;
	}
	ParaStarted = false;
	TextGroup = ltxt;
	if (LastParaStartGroup == txt)
		LastParaStartGroup = NULL;
}



// utilities


void
XMLrd::SetUniqueID(void)
{
	if (!TextGroup)
		return;

	DCctl *cp = new DCctl(ilong, 2, 112, 2, ++ItemID);
	TextGroup->add(*cp);
}


void
XMLrd::SetStartTextGroup(void)
{
	// add elem->starttext group
	if (SetupComplete
	 && !InMap
	 && !ProcessingConrefs
	 && !InGenerateFile
	 && !ConrefsDone) {
		DCelem *elem = NULL;
		if ((elem = (DCelem *) ElemTextGroups.find((long)TextGroup)) != NULL) {
			if (!elem->starttext) {
				DCgroup *gp = new DCgroup();
				elem->starttext = gp;
				TextGroup->add(*gp);
			}
			else if (!InXref
			 && strcmp(elem->elemname, "section")
			 && strcmp(elem->elemname, "step"))
				LogEvent(logwarn, 2,"Elem starttext already set for ", elem->elemname);
		}
		else if (!InFootnote
		 && !InTable
		 && !InFig
		 && !WritingSubformatHead)
			LogEvent(logwarn, 2,"Can't find elem for starttext setup");
	}
}


void
XMLrd::SetEndTextGroup(void)
{
	// add elem->endtext group
	if (SetupComplete
	 && !InMap
	 && !ProcessingConrefs
	 && !InGenerateFile
	 && !ConrefsDone) {
		DCelem *elem = NULL;
		if ((elem = (DCelem *) ElemTextGroups.find((long)TextGroup)) != NULL) {
			if (!elem->endtext) {
				DCgroup *gp = new DCgroup();
				elem->endtext = gp;
				TextGroup->add(*gp);
			}
			else if (!InXref
			 && strcmp(elem->elemname, "section")
			 && strcmp(elem->elemname, "step"))
				LogEvent(logwarn, 2,"Elem endtext already set for ", elem->elemname);
		}
		else if (!InFootnote
		 && !InTable
		 && !InFig
		 && !WritingSubformatHead)
			LogEvent(logwarn, 2,"Can't find elem for endtext setup");
	}
}


void
XMLrd::SetBlockElemTags(void)
{
	long i = 0;
	DCctl *cp = NULL;
	DCelem *einfo = NULL;
	DCtopic *tinfo = NULL;
	char numstr[33] = "";

	for (i = BlockElemsStarted + 1; i <= BlockElemCount; i++) {
		if (IncludeElementTags) {
			cp = new DCctl(ename, 2, 121, 1);
			cp->ex(BlockElems.find(i));
			TextGroup->add(*cp);
		}

		if ((einfo = (DCelem *) BlockElemInfo.find(i)) != NULL) {
			SetAnchorIDs(einfo);
			if (IncludeElementTags)
				einfo->setAttrCtls(TextGroup);
#ifdef DITA
			if (AttrPassthrough)
				SetPassthroughAttrs(einfo, TextGroup);
#endif
		}
		else
			LogEvent(logwarn, 1, "Missing info for block elem ",
				                   itoa(i, numstr, 10));
	}
	BlockElemsStarted = BlockElemCount;
}


void
XMLrd::SetInlineElemTags(void)
{
	long i = 0;
	DCctl *cp = NULL;
	DCelem *einfo = NULL;
	char *name = NULL;
	char numstr[33] = "";

	for (i = InlineElemsStarted + 1; i <= InlineElemCount; i++) {
		if (IncludeElementTags) {
			if ((name = InlineElems.find(i)) == NULL) {
				InlineElemsStarted = i - 1;
				return;  // not there yet
			}
			cp = new DCctl(ename, 2, 121, 1);
			cp->ex(name, strlen(name));
			TextGroup->add(*cp);
		}

		if ((einfo = (DCelem *) InlineElemInfo.find(i)) != NULL) {
			SetAnchorIDs(einfo);
			if (IncludeElementTags)
				einfo->setAttrCtls(TextGroup);
#ifdef DITA
			if (AttrPassthrough)
				SetPassthroughAttrs(einfo, TextGroup);
#endif
		}
		else
			LogEvent(logwarn, 1, "Missing info for inline elem ",
				                   itoa(i, numstr, 10));
	}
	InlineElemsStarted = InlineElemCount;
}


void
XMLrd::SetMapElemTags(void)
{
	long i = 0;
	DCctl *cp = NULL;
	DCelem *einfo = NULL;
	char numstr[33] = "";

	for (i = MapElemsStarted + 1; i <= MapElemCount; i++) {
		if (TextGroup
		 && IncludeElementTags) {
			cp = new DCctl(ename, 2, 121, 1);
			cp->ex(MapElems.find(i));
			TextGroup->add(*cp);
		}

		if ((einfo = (DCelem *) MapElemInfo.find(i)) != NULL) {
			if (TextGroup) {
				SetAnchorIDs(einfo);
				if (IncludeElementTags)
					einfo->setAttrCtls(TextGroup);
#ifdef DITA
				if (AttrPassthrough)
					SetPassthroughAttrs(einfo, TextGroup);
#endif
			}
		}
		else
			LogEvent(logwarn, 1, "Missing info for map elem ",
				                   itoa(i, numstr, 10));
	}
	MapElemsStarted = MapElemCount;
	if (SetMapName) {
		SetChunkMarker(TOCFile);
		SetMapName = false;
	}
}


bool XMLrd::GenerateUIDs = true;
bool XMLrd::UseLinkID = true;

void 
XMLrd::SetAnchorIDs(DCelem *einfo)
{
	if (InTextref)
		return;

	DCctl *cp = NULL;
	char *id = einfo->id;
	if (!id)
		return;
	einfo->idgrp = new DCgroup;
	if (InFig
	 && InTitleElem
	 && FigTextIDGroup) {
		TextGroup->add(*FigTitleIDGroup);
		FigTextIDGroup->add(*((DCctl *) (einfo->idgrp)));
	}
	else
		TextGroup->add(*((DCctl *) (einfo->idgrp)));

	if (GenerateUIDs
	 && !einfo->elemuid) {
		char *ref = NewName("#", strlen(id) + 2);
		strcat(ref, id);
		char *nref = FixReference(ref);
		einfo->elemuid = MakeAnchorName(nref, einfo);
		DeleteName(ref);
		DeleteName(nref);
		id = einfo->elemuid;
	}

	if (TopicTitleStarting) {  // topic title element
		CurrTopicInfo->chunked = false;
		char *nm = NULL;
		char *txt = NULL;
		if (ChunkDoc) { // add Chunk marker for doc name
			CurrTopicInfo->chunked = true;
			cp = new DCctl(etext, 2, 110, 3);
			cp->ex("chunk", 6);
			einfo->idgrp->add(*cp);
			if (CurrDocRef
			 && CurrDocRef->copyto)
				nm = CurrDocRef->copyto;
			else
				nm = XMLFile->getBaseName();
			nm = MakeNameUnique(nm, einfo);
			einfo->elfile = nm;
			txt = NewName("chunkfile:", strlen(nm) + 11);
			strcat(txt, nm);
			cp = new DCctl(etext, 2, 110, 3);
			cp->ex(txt, strlen(txt));
			einfo->idgrp->add(*cp);
			ChunkDoc = false; // just once
			ChunkDocSet = true;
		}
		if (ChunkTopic    // add Chunk markers for each topic
		 || SplitThisTopic) {
			CurrTopicInfo->chunked = true;
			cp = new DCctl(etext, 2, 110, 3);
			cp->ex("chunk", 6);
			einfo->idgrp->add(*cp);
			if (CurrDocRef
			 && CurrDocRef->copyto)
				nm = CurrDocRef->copyto;
			else
				nm = CurrTopicInfo->id; // name based on TopicID
			if (!nm) {  // create an ID
				nm = "missingid";
			}
			nm = MakeNameUnique(nm, einfo);
			txt = NewName("chunkfile:", strlen(nm) + 11);
			strcat(txt, nm);
			cp = new DCctl(etext, 2, 110, 3);
			cp->ex(txt, strlen(txt));
			einfo->idgrp->add(*cp);
			SplitThisTopic = false;
		}
		TopicTitleStarting = false;
	}

	char *uid = einfo->elemuid ? einfo->elemuid : einfo->id;
	cp = new DCctl(etext, 2, 110, 4);  // for xrefs
	cp->ex(uid, strlen(uid));
	einfo->idgrp->add(*cp);
	einfo->xanchor = cp;

	if (UseLinkID) {
		cp = new DCctl(etext, 2, 112, 2);  // for links
		cp->ex(uid, strlen(uid));
		einfo->idgrp->add(*cp);
		einfo->anchor = cp;
	}

	if (InResTopic)
		return;

	if (InFigTitle) {
		LOFItemList->add((long) einfo);
		einfo->inLOF = true;
		InFigTitle = false;
	}
	if (InTblTitle) {
		LOTItemList->add((long) einfo);
		einfo->inLOT = true;
		InTblTitle = false;
	}

	einfo->elform = CurrFormat;
	AddGenListItem(einfo, CurrFormat);
}


bool XMLrd::DuplicateNameCheck = true;
char *XMLrd::UniqueNameSuffixFormat = "X%0.3d";
long XMLrd::UniqueNameSuffixLength = 4;
DCnlist XMLrd::InputFileNames;  // names to be used for unique IDs
DCnlist XMLrd::OutputFileNames; // names for output files in chunk marker


char *
XMLrd::MakeNameUnique(char *onm, DCelem *einfo, bool inp)
{
	char *nnm = NULL;
	DCnlist *lst = inp ? &InputFileNames : &OutputFileNames;

	//long doc = einfo ? ((DCtopelem *) einfo)->topic->mref->dcount : 1;
	long doc = einfo->GetDocCount();

	if (!DuplicateNameCheck)
		return onm;

	long dnum = lst->getitem(onm);
	if (!dnum) {
		lst->add(onm, doc);
		return onm;
	}
	if (dnum == doc)
		return onm;

	nnm = NewName(onm, strlen(onm) + UniqueNameSuffixLength + 1);
	char *cpt = nnm + strlen(nnm);
	for (int i = 1; i < 1000; i++) {
		sprintf(cpt, UniqueNameSuffixFormat, i);
		if ((dnum = lst->getitem(nnm)) == 0) {
			lst->add(nnm, doc);
			return nnm;
		}
		if (dnum == doc)
			return nnm;
	}
	LogEvent(logwarn, 1, "Unable to make unique name for file ", onm);
	return onm;
}


DCctl *
XMLrd::PutTextStrCtl(const char *str, size_t len)
{
	if (!str
	 || !TextGroup)
		return NULL;

	DCctl *cp = NULL;
	char *stp = (char *) str;
	if (len == 0)
		len = strlen(str);

	while (len > 256) {
		cp = new DCctl(etext, 2, 10, 1);
		cp->ex(stp, 256);
		TextGroup->add(*cp);
		if (TitleGroup
		 && !InIndex)
			TitleGroup->add(*cp);
		stp += 256;
		len -= 256;
	}
	if (len > 0) {
		cp = new DCctl(etext, 2, 10, 1);
		cp->ex(stp, len);
		TextGroup->add(*cp);
		if (TitleGroup
		 && !InIndex)
			TitleGroup->add(*cp);
	}
	return cp;
}


DCctl *
XMLrd::PutTextCharCtl(unl uch)
{
	DCctl *cp = new DCctl(imnem, 2, 10, 1, uch);
	TextGroup->add(*cp);
	if (TitleGroup
	 && !InIndex)
		TitleGroup->add(*cp);
	return cp;
}



// end of drxmltx.cpp


