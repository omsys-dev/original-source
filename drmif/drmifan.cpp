 
// drmifan.cc is the code for dcl FrameMaker autonumber reading, JHG, 12/21/96
// Copyright (c) 1993-1996 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dcini.h"
#include "drmif.h"
#include "drmiffl.h"
#include <windows.h>
#include <iostream.h>
#include "Shlwapi.h"
#undef min


// list of MIF statements processed in this module


mif MIFrd::mifanpile[] = {
  { "PgfAutoNum",        yorn,       imnem, 0,  0,  0, panum },
  { "PgfNumFormat",      stringval,  etext, 0,  0,  0, panform },
  { "PgfNumberFont",     nameval,    etext, 0,  0,  0, panfont },
  { "PgfNumAtEnd",       yorn,       imnem, 0,  0,  0, panpos },
  { "PgfNumString",      stringval,  etext, 2,101,  8, panval },

  { "VariableFormats",   newgroup,   imnem, 0,  7,  0, pseudogroup },
  { "VariableFormat",    newgroup,   group, 2, 90,  0, var_def },
  { "VariableName",      nameval,    etext, 2, 91,  1, var_tag },
  { "VariableDef",       vdefval,    group, 2, 91,  2, var_form },
  { "Variable",          newgroup,   imnem, 2, 10,  2, ignore },

  { "XRefFormats",       newgroup,   imnem, 0,  8,  0, pseudogroup },
  { "XRefFormat",        newgroup,   group, 2, 95,  0, xref_def },
  { "XRefName",          nameval,    etext, 2, 96,  1, xref_tag },
  { "XRefDef",           vdefval,    etext, 2, 96,  3, xref_form },
  { "XRef",              newgroup,   group, 2, 10,  7, xref_use },
  { "XRefSrcText",       nameval,    etext, 2, 96,  2, xref_text },
  { "XRefSrcFile",       fileval,    etext, 2, 96,  5, xref_file },
  { "XRefEnd",           empty,      etext, 2, 96,  4, xref_val },
  { "XRefSrcIsElem",     yorn,       imnem, 0,  0,  0, xref_elem },
  { "ElementBegin",      newgroup,   group, 0,  0,  0, elem_beg },
	{ "ETag",              nameval,    ename, 2,121,  1, elem_tag },
	{ "ElementEnd",        nameval,    ename, 2,121,  3, elem_end },
  { "Element",           newgroup,   group, 0,  0,  0, elem_all },
  { "Attributes",        newgroup,   group, 0,  0,  0, elem_attr },
  { "Attribute",         newgroup,   group, 0,  0,  0, attr_beg },
  { "AttrName",          nameval,    etext, 2,121,  4, attr_name },
	{ "AttrValue",         stringval,  etext, 2,121,  5, attr_val },

  { "MarkerTypeCatalog", newgroup,   etext, 0,  0,  0, markcat },
  { "Marker",            newgroup,   etext, 0,  0,  0, marker },
  { "MType",             numval,     ilong, 0,  0,  0, marktyp },
  { "MTypeName",         nameval,    etext, 0,  0,  0, markname },
  { "MText",             unameval,   etext, 2,110,  0, marktxt },
  { "MCurrPage",         numval,     ilong, 0,  0,  0, markpg },

  { "DStartPage",        numval,     imnem, 0,  0,  0, pfirst },
  { "DPageNumStyle",     l_numstyle, imnem, 0,  0,  0, pnstyle },
  { "DLinebreakChars",   stringval,  group, 2, 11,  9, bkchars },
  { "VolumeNumStart",    numval,     imnem, 0,  0,  0, volfirst },
  { "VolumeNumStyle",    l_numstyle, imnem, 0,  0,  0, volstyle },
  { "VolumeNumText",     stringval,  imnem, 0,  0,  0, voltext },
  { "ChapterNumStart",   numval,     imnem, 0,  0,  0, chfirst },
  { "ChapterNumStyle",   l_numstyle, imnem, 0,  0,  0, chstyle },
  { "ChapterNumText",    stringval,  imnem, 0,  0,  0, chtext },
  { "SectionNumStart",   numval,     imnem, 0,  0,  0, secfirst },
  { "SectionNumStyle",   l_numstyle, imnem, 0,  0,  0, secstyle },
  { "SectionNumText",    stringval,  imnem, 0,  0,  0, sectext },
  { "SubSectionNumStart",numval,     imnem, 0,  0,  0, ssecfirst },
  { "SubSectionNumStyle",l_numstyle, imnem, 0,  0,  0, ssecstyle },
  { "SubSectionNumText", stringval,  imnem, 0,  0,  0, ssectext },

  { "FNoteStartNum",     numval,     imnem, 2,103,  0, fnfirst },
  { "DFNoteRestart",     l_fnstart,  imnem, 2,101, 10, fnstart },
  { "DFNoteMaxH",        leadval,    ilong, 2, 72,  3, fnhigh },

  { "DFNoteTag",         nameval,    etext, 2,101,  1, fnname },
  { "DFNoteLabels",      stringval,  group, 2,101,  9, fnchars },
  { "DFNoteNumStyle",    l_numstyle, group, 2,101,  4, fnstyle },
  { "DFNoteNumberPos",   l_fnbase,   imnem, 0,  0,  0, fnbase },
  { "DFNoteNumberPrefix", stringval, group, 2,101,  4, fnpre },
  { "DFNoteNumberSuffix", stringval, group, 2,101,  4, fnsuf },
  { "DFNoteAnchorPos",   l_fnbase,   imnem, 0,  0,  0, fntbase },
  { "DFNoteAnchorPrefix", stringval, etext, 0,  0,  0, fntpre },
  { "DFNoteAnchorSuffix", stringval, etext, 0,  0,  0, fntsuf },

  { "DTblFNoteTag",      nameval,    etext, 2,101,  1, tfnname },
  { "DTblFNoteLabels",   stringval,  group, 2,101,  9, fnchars },
  { "DTblFNoteNumStyle", l_numstyle, group, 2,101,  4, fnstyle },
  { "DTblFNoteNumberPos",    l_fnbase,  imnem, 0,  0,  0, fnbase },
  { "DTblFNoteNumberPrefix", stringval, group, 2,101,  4, fnpre },
  { "DTblFNoteNumberSuffix", stringval, group, 2,101,  4, fnsuf },
  { "DTblFNoteAnchorPos",    l_fnbase,  imnem, 0,  0,  0, fntbase },
  { "DTblFNoteAnchorPrefix", stringval, etext, 0,  0,  0, fntpre },
  { "DTblFNoteAnchorSuffix", stringval, etext, 0,  0,  0, fntsuf }
};

size_t MIFrd::mifan_size = (sizeof(mifanpile) / sizeof(mif));

char *MIFrd::wordanpile[] = {
  (char *) (int) l_numstyle, "Arabic", "LCRoman", "UCRoman",
    "LCAlpha", "UCAlpha", "Custom", "TextStyle", NULL,
  (char *) (int) l_fnstart, "PerPage", "PerFlow", NULL,
  (char *) (int) l_fnbase, "FNBaseline", "FNSubscript", "FNSuperscript", NULL,
  NULL
};



// start of code


void
MIFrd::ProcAn(mif *mp)
{
	DCctl *cp = NULL;
	char *chp = NULL;
	long pos = 0;

  switch (mp->spec) {

    case panum:
      PNumUsed = (unc) MIFval;
      break;
    case panform:
      strncpy(PAForm, MIFstring, FStringMax);
      break;
    case panfont:
			PAFontSet = true;
      strncpy(PAFont, MIFstring, FStringMax);
      break;
    case panpos:
      PNumEnd = (unc) MIFval;
      break;
    case panval:
      strncpy(PAVal, MIFstring, FStringMax);
      ProcPAVal();
      break;

    case var_def:
      MIFval = ++VarID;
      MakeDclGroup(mp);
      break;
    case var_tag:
      ProcVarTag(mp);
      break;
    case var_form:
      ProcVarForm(mp);
      break;

    case xref_def:
      XrefID++;
      EndProc[MIFlevel] = EndXrefDef;
      break;
    case xref_tag:
      ProcXrefTag(mp);
      break;
    case xref_form:
      break;
    case xref_use:
      AddText(0);
      InXrefText = true;
      XrefParentGroup = GroupStack[MIFlevel];
			XrefParentLast = XrefParentGroup->last;
      MakeDclGroup(mp);
      EndProc[MIFlevel] = EndXrefStart;
      break;
    case xref_text:
			if (IDRefAttr) {
				IDRefAttr = false;
				break;
			}
			if ((XrefType == 'N')
			 && isdigit(*MIFstring)
			 && isdigit(*(MIFstring + 4))
			 && (*(MIFstring + 5) == ':')) {
				mp->dtype = ilong;
				MIFval = atol(MIFstring);
				XrefTextCtl = MakeCtl(mp, false);
				mp->dtype = etext;
			}
			else
	      XrefTextCtl = MakeCtl(mp, false);
      break;
    case xref_file:
      if (*MIFstring != 0) {
        XrefFileCtl = MakeCtl(mp, false);
				FixXrefFileName(XrefFileCtl);
			}
      break;
    case xref_val:
      if (TextPos)
        AddText(0);
      InXrefText = false;
      // MIFlevel--;   // wrong because xref can contain ParaLine breaks
			if (GroupStack[MIFlevel]->last->ctl == XrefTextCtl) { // empty
				DCgroup *mt = GroupStack[MIFlevel];
	      if (GroupStack[MIFlevel - 1] == mt)
	        GroupStack[MIFlevel - 1] = XrefParentGroup;  // if line break happened
				GroupStack[MIFlevel] = XrefParentGroup;
				XrefParentGroup->last = XrefParentLast;
				XrefParentLast->next = NULL;
				delete mt;
			}
			else {
				ProcXrefInstance(GroupStack[MIFlevel], (DCgroup *)XrefFormList.find(XrefID));
				if (GroupStack[MIFlevel - 1] == GroupStack[MIFlevel])
					GroupStack[MIFlevel - 1] = XrefParentGroup;  // if line break happened
				GroupStack[MIFlevel] = XrefParentGroup;  // put back regular group
			}
			XrefTextCtl = NULL;
      break;

		case xref_elem:
			if (MIFval
			 && XrefTextCtl) {
				chp = (char *) XrefTextCtl->ex();
				char *pos = strchr(chp, ':');
				if (pos) {
					*pos = '\0';
					chp = NewName(chp);
					XrefTextCtl->ex(chp, strlen(chp) + 1);
					DeleteName(chp);
				}
#if 0
				if (strchr(chp, ':') == (chp + ElemIdLen)) {
					*(chp + ElemIdLen) = '\0';
					chp = NewName(chp);
					XrefTextCtl->ex(chp, ElemIdLen + 1);
					DeleteName(chp);
				}
#endif
			}
			break;

		case elem_all:
			BeginElement = true;
      EndProc[MIFlevel] = EndElement;
			break;
		case elem_beg:
			BeginElement = true;
      EndProc[MIFlevel] = EndElementBegin;
			break;
		case elem_tag:
			if (BeginElement
			 && (*MIFstring != 0)) {
				FlushText();
				MakeCtl(mp, true);
			}
			break;
		case elem_end:
			if (*MIFstring != 0) {
				FlushText();
				MakeCtl(mp, true);
			}
			break;
		case elem_attr:
			break;
		case attr_beg:
			if (!BeginElement)
				break;
			ElemAttr = true;
			AttrMark = NULL;
			// FlushText();
      EndProc[MIFlevel] = EndAttribute;
			break;
		case attr_name:
			if (ElemAttr) {
				IDAttr = stricmp(MIFstring, IDAttrName) ? false : true;
				IDRefAttr = stricmp(MIFstring, IDRefAttrName) ? false : true;
				if ((pos = AttrMarkers.getitem(MIFstring)) != 0)
					AttrMark = (char *) pos;
				MakeCtl(mp, true);
			}
			break;
		case attr_val:
			if (ElemAttr
			 && (*MIFstring != 0)) {
				if (AttrMark)
					AttrMarkVal = NewName(MIFstring);
				MakeCtl(mp, true);
				if (IDAttr) {
					mp->min = 110;
					mp->dat = 4;
					MakeCtl(mp, true);
					mp->min = 121;
					mp->dat = 5;
					IDAttr = false;
				}
#if 0
				else if (IDRefAttr) {
					mp->min = 96;
					mp->dat = 2;
					MakeCtl(mp, true);
					mp->min = 121;
					mp->dat = 5;
					if (!InXrefText)
						IDRefAttr = false;
				}
#endif
			}
			break;

		case markcat:
			StartMarkCat();
			break;
    case marker:
			StartMarker();
      break;
    case marktyp:
      MarkType = (uns) MIFval;
      break;
		case markname:
			if (!InMarkCat)
				MarkName = NewName(MIFstring);
			break;
    case marktxt:
      ProcMarker(mp);
      break;
    case markpg:
      break;

    case pfirst:
      FirstPage = MIFval;
      break;
    case pnstyle:
      if (MIFval < 0)
        break;
      PNumStyle = (uns) MIFval;
      break;
    case bkchars:
      GroupStack[MIFlevel]->add(*ProcCharList(mp));
      break;

    case volfirst:
      FirstVol = MIFval;
      break;
    case volstyle:
      if (MIFval < 0)
        break;
      VolStyle = (uns) MIFval;
      break;
		case voltext:
			if (*MIFstring)
				VolText = NewName(MIFstring);
			break;

    case chfirst:
      FirstChap = MIFval;
      break;
    case chstyle:
      if (MIFval < 0)
        break;
      ChapStyle = (uns) MIFval;
      break;
		case chtext:
			if (*MIFstring)
				ChapText = NewName(MIFstring);
			break;

    case secfirst:
      FirstSect = MIFval;
      break;
    case secstyle:
      if (MIFval < 0)
        break;
      SectStyle = (uns) MIFval;
      break;
		case sectext:
			if (*MIFstring)
				SectText = NewName(MIFstring);
			break;

    case ssecfirst:
      FirstSSect = MIFval;
      break;
    case ssecstyle:
      if (MIFval < 0)
        break;
      SSectStyle = (uns) MIFval;
      break;
		case ssectext:
			if (*MIFstring)
				SSectText = NewName(MIFstring);
			break;

    case fnfirst:
      FirstFN = (uns) MIFval;
      break;
    case fnstyle:
      if (MIFval < 0)
        break;
      FNStyle = (uns) MIFval;
      break;
    case fnname:
      MakePNDef();
      strncpy(FNName, MIFstring, FStringMax);
      break;
    case tfnname:
      FNType = 0;
      MakeFNProp(); // make text footnote groups
      MakeFNDef();  // while data is still there
      FNType = 1;
      FirstFN = 1;  // table footnotes start with 1
      FNReset = 3;  // and restart for each table
      strncpy(TFNName, MIFstring, FStringMax);
      break;
    case fnstart:
      FNReset = MIFval + 1;
      break;
    case fnhigh:
      FNHigh = (uns) MIFval;
      break;
    case fnchars:
      FNCharGroup = ProcCharList(mp);
      break;
    case fnbase:
      if (MIFval < 0)
        break;
      FNBase = (uns) MIFval;
      break;
    case fnpre:
      strncpy(FNPre, MIFstring, FStringMax);
      break;
    case fnsuf:
      strncpy(FNSuf, MIFstring, FStringMax);
      break;
    case fntbase:
      if (MIFval < 0)
        break;
      FNTBase = (uns) MIFval;
      break;
    case fntpre:
      strncpy(FNTPre, MIFstring, FStringMax);
      break;
    case fntsuf:
      strncpy(FNTSuf, MIFstring, FStringMax);
      break;

    default:
      assert(0);
      break;
  }
}



// variables


uns MIFrd::VarID;
DCnlist MIFrd::VarNameList;

vdata MIFrd::VarTime[] = {
  { "Current Date",      12, 0, 1 },
  { "Creation Date",     13, 0, 2 },
  { "Modification Date", 17, 0, 3 },
  { NULL,                 0, 0, 0 }
};

vdata MIFrd::VarBlocks[] = {
  { "paranumonly",     11, 30, 1 },
  { "paranum",          7, 30, 2 },
  { "paratag",          7, 30, 3 },
  { "paratext",         8, 30, 4 },
  { "curpagenum",      10, 21, 1 },
  { "pagenum",          7, 30, 5 },
  { "lastpagenum",     11, 21, 2 },
  { "tblsheetnum",     11, 22, 1 },
  { "tblsheetcount",   13, 22, 2 },
  { "filename",         8, 20, 1 },
  { "fullfilename",    12, 20, 2 },
  { "second00",         8, 15, 1 },
  { "second",           6, 15, 2 },
  { "minute00",         8, 14, 1 },
  { "minute",           6, 14, 2 },
  { "hour01",           6, 13, 1 },
  { "hour24",           6, 13, 3 },
  { "hour",             4, 13, 2 },
  { "ampm",             4, 12, 1 },
  { "AMPM",             4, 12, 2 },
  { "daynum01",         8,  6, 3 },
  { "daynum",           6,  6, 4 },
  { "dayname",          7,  6, 2 },
  { "shortdayname",    12,  6, 1 },
  { "monthnum01",      10,  4, 3 },
  { "monthnum",         8,  4, 4 },
  { "monthname",        9,  4, 2 },
  { "shortmonthname",  14,  4, 1 },
  { "year",             4,  3, 2 },
  { "shortyear",        9,  3, 1 },
  { "startrange",      10, 40, 3 },
  { "endrange",         8, 40, 4 },
  { "singlepage",      10, 40, 2 },
  { "nopage",           6, 40, 1 },
  { "marker1",          7, 23, 1 },
  { "marker2",          7, 23, 2 },
  { "chapnum",          7, 24, 1 },
  { "volnum",           6, 24, 2 },
  { "sectionnum",      10, 24, 3 },
  { "subsectionnum",   13, 24, 4 },
  { NULL,               0,  0, 0 }
};


void
MIFrd::ProcVarTag(mif *mp)
{
  if (CurrPG == psg_var) {
    VarNameList.addnew(MIFstring, VarID);
    MakeCtl(mp);
    return;
  }
  AddText(0);
  DCctl *cp = new DCctl(imnem, 2, 10, 2, VarNameList.getnum(MIFstring));
  GroupStack[MIFlevel]->add(*cp);
}


void
MIFrd::ProcVarForm(mif *mp)
{
  char *vn;
  DCctl *cp = NULL;

  if (CurrPG == psg_var) {
    vn = VarNameList.find(VarID);
    for (size_t i = 0; VarTime[i].p != NULL; i++) {
      if (!strncmp(vn, VarTime[i].p, VarTime[i].len)) {
        cp = new DCctl(imnem, 2, 91, 3, i + 1);
        GroupStack[MIFlevel]->add(*cp);
        break;
      }
    }
  }
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, mp->dat));
  GroupStack[++MIFlevel] = gp;
  ProcVXForm(gp, true);
  GroupStack[--MIFlevel]->add(*gp);
}


void
MIFrd::ProcVXForm(DCgroup *gp, bool isVar)
{
  static char fmt[RStringMax];
  size_t pos = 0;
  int i;
  char *vn, *vp;
  char ch;
  DCctl *cp = NULL;
  uns fntnum;

  GenericTab = true;
  for (vn = MIFstring; *vn; vn++) {
    ch = *vn;
    if (ch == '\\') {
      ch = *++vn;
      if (ch == 't')
        AddText('\t');
      else if (ch == 'x')
        ProcHex(&vn);
			else if (ch == '\\') { // literal backslash
				ch = *++vn;
				switch (ch) {
					case ' ':  // nonbreaking space code (UNIX)
						AddText(0xA0);
						break;
					case '<':  // literal start delimiter
					case '>':  // literal end delimiter
						AddText(ch);
						break;
					case 't':  // unknown tab
						AddText(0);  // put out any text so far
						cp = new DCctl(imnem, 2, 41, 8, 0);
						gp->add(*cp);
						break;
					default:
						AddText('\\');
						--vn;
						break;
				}
			}
      else AddText(((uns) ch) & 0xFF);
    }
    else if (ch == '<') {  // format or variable
      ch = *++vn;
      if (ch == '$') {  // system variable
        vn++;
        if ((i = FindVarBlock(vn)) >= 0) {
          vn += VarBlocks[i].len;
          AddText(0);  // put out any text so far
					if ((VarBlocks[i].pre == 30)
					 && (isVar == false))
	          cp = new DCctl(imnem, 2, 97, 35, VarBlocks[i].val);
					else
	          cp = new DCctl(imnem, 2, 97, VarBlocks[i].pre, VarBlocks[i].val);
          gp->add(*cp);
          if (*vn == '[') {
            vn++;
            uns dval = 31;
            if ((*vn == '+') && (*(vn + 1) == ',')) {
              vn += 2;
              dval = 32;
            }
            while (*vn
						 && (*vn != ']')
						 && (*vn != '>'))
              fmt[pos++] = *vn++;
            if (*vn == ']')
              vn++;
            //if (*vn == '>')
            //  vn++;
						if (*vn == '\0')
							vn--;
            cp = new DCctl(ename, 2, 97, dval);
            fmt[pos++] = '\0';
            cp->ex(fmt, pos);
            gp->add(*cp);
            pos = 0;
          }
        }
        else {  // unrecognized system variable
          vn--;
          AddText('<');
          AddText('$');
        }
      }
      else { // character format tag
        vp = vn;
        while (*vp && (*vp != '>'))
          vp++;
        if (*vp == '>') {
          *vp = '\0';
					if ((vp == vn)  // means def para font
					 || ((vp == (vn + 1))
					  && ((*vn == ' ')
					   || (*vn == '/')))
					 || (!_strnicmp(vn, "default", 7)))
            fntnum = 0;  // means revert to last init format
          else if ((fntnum = (uns) CFormNameList.getnum(vn)) == 0) {  // unidentified font
            *vp = '>';
            vn--;
            AddText('<');
            continue;
          }
          // need to include font call(s)
          AddText(0);  // put out any text so far
          cp = new DCctl(imnem, 2, 82, 2, fntnum);  // always a char format
          gp->add(*cp);
					SetSymFont(fntnum);
          vn = vp;
        }
        else { // literal unmatched <
          vn--;
          AddText('<');
        }
      }
    }
		else if ((ch < 0)
		 && (MIFversion > 6)
		 && ::IsUTF8((unc *) vn)) {
			AddText(::UTF8((unc **) &vn));
			vn--;
		}
    else AddText(((uns) ch) & 0xFF);
  }
  AddText(0);    // add the last txt ctl with content
  GenericTab = false;
}


int
MIFrd::FindVarBlock(char *vn)
{
  for (size_t i = 0; VarBlocks[i].p != NULL; i++) {
    if (!strncmp(vn, VarBlocks[i].p, VarBlocks[i].len))
      return i;
  }
  return -1;
}



// cross-references


uns MIFrd::XrefID;
DCnlist MIFrd::XrefNameList;
DCvlist MIFrd::XrefFormList;
bool MIFrd::XrefEmpty;
bool MIFrd::InXrefText;
DCgroup *MIFrd::XrefParentGroup;
DCitem *MIFrd::XrefParentLast;
DCctl *MIFrd::XrefFileCtl = NULL;
DCctl *MIFrd::XrefTextCtl = NULL;

void
MIFrd::ProcXrefTag(mif *mp)
{
  if (CurrPG == psg_xref) {
    if (*MIFstring)
      XrefNameList.addnew(MIFstring, XrefID);
    else XrefEmpty = true;
    return;
  }
  AddText(0);
	XrefID = (uns) XrefNameList.getnum(MIFstring);
  DCctl *cp = new DCctl(imnem, 2, 96, 3, XrefID);
  GroupStack[MIFlevel]->add(*cp);
}


void
MIFrd::EndXrefDef(void)
{
  if (XrefEmpty) {
    XrefID--;
    XrefEmpty = false;
    return;
  }
  DCgroup *gp = new DCgroup(DCctl(group, 2, 95, XrefID));
  DCctl *cp = new DCctl(etext, 2, 96, 1);
  char *chp = XrefNameList.find(XrefID);
  assert(chp);
  cp->ex(chp, strlen(chp) + 1);
  gp->add(*cp);
  DCgroup *fgp = new DCgroup(DCctl(group, 2, 96, 3));
  GroupStack[++MIFlevel] = fgp;
  ProcVXForm(fgp);
  --MIFlevel;
	XrefFormList.add(fgp, XrefID);
  gp->add(*fgp);
  GroupStack[MIFlevel]->add(*gp);
}


void
MIFrd::EndXrefStart(void)
{
//  MIFlevel++;  // wrong because ParaLine breaks can happen within xref
  // XrefParentGroup is already saved
	IDRefAttr = false;

	// ensure file ctl precedes text ctl
	if (XrefFileCtl) {
	  GroupStack[MIFlevel]->add(*XrefFileCtl);
		XrefFileCtl = NULL;
	}

	if (XrefTextCtl) {
	  GroupStack[MIFlevel]->add(*XrefTextCtl);
		//XrefTextCtl = NULL;
	}

  GroupStack[MIFlevel - 1] = GroupStack[MIFlevel]; // continue xref content
}


void
MIFrd::ProcXrefInstance(DCgroup *instg, DCgroup *formg)
{
	// insert group ref_cont start/end for content generated by xref tokens

	if (!instg || !formg)
		return;

	DCitem *ii = instg->first;
	DCitem *fi = formg->first;
	DCitem *nii = ii;
	DCitem *nfi = fi;

	if (!ii || !fi)
		return;

	DCctl *ic = ii->ctl;
	DCctl *fc = fi->ctl;
	DCctl *cc = NULL;
	DCctl *nic = NULL;
	DCctl *nfc = NULL;
	unl tok = 0;
	char *mp = NULL;
	char *ip = NULL;
	char *fp = NULL;
	char *xp = NULL;
	bool skipped = false;
	bool fstep = true;
	bool istep = true;
	bool seektab = false;
	bool endgr = false;
	bool fonce = false;
	bool infstr = false;
	unl fs = 0;

	// advance in ic to end of prolog, at (text ref_prop id) (2, 96, 2)
	while (ic) {
		if ((ic->maj() == 2)
		 && (ic->min() == 96)
		 && (ic->dat() == 2))  // ref_prop id
			skipped = true;
		ii = ii->next;
		if (!ii) {
			ic = NULL;
			break;
		}
		ic = ii->ctl;
		if (skipped)
			break;
	}

	// if form has ref_token, endgroup and start ref_cont group
	// if form has tab unknown, endgroup and match to next tab_pos
	// if in group and form has text obj lit, and inst has longer one,
	//   see if inst one includes form one at end; if so, insert new
	//   text obj lit for start, endgroup, and trim start of inst one
	// if in group and form matches inst, endgroup
	// note that inst ref_cont group is usually just one text obj lit 

	// init for loop
	nfi = fi;
	nii = ii;

	while (nfi && nii) {
		if (nfi && !infstr) {
			fi = nfi;
			fc = fi->ctl;	
			if ((nfi = fi->next) != NULL)
				nfc = nfi->ctl;
		}
		if (nii) {
			ii = nii;
			ic = ii->ctl;	
			if ((nii = ii->next) != NULL)
				nic = nii->ctl;
		}

		// tab processing
		if ((fc->maj() == 2)
		 && (fc->min() == 41)
		 && (fc->dat() == 8)) {  // (text tab_pos unknown )
			if (tok) {  // close prev one now
				cc = new DCctl(endgroup, 2, 98, (uns) tok);
				instg->insert(*cc, *ic);
				tok = 0;
			}
			if ((ic->maj() != 2)
			 || (ic->min() != 41))
				nfi = fi;  // stay on this line in format
			continue;    // skip in inst to next text tab_pos
		}

		// token processing
		if ((fc->dt() == imnem)
		 && (fc->maj() == 2)
		 && (fc->min() == 97)) {  // ref_token
			unl ptok = tok;  // previous open token
			tok = 0;
			if (fc->dat() == 35) {  // xref
				tok = fc->siz();
				// (dcl def 1 "num_only")
				// (dcl def 2 "num_str")
				// (dcl def 3 "tag")
				// (dcl def 4 "text")
				// (dcl def 5 "page")
			}
			else if (fc->dat() == 24) {  // docnum
				tok = fc->siz() + 5;
				// (dcl def 1 "chapnum")
				// (dcl def 2 "volnum")
			}
			if (tok) {
				// insert before ic 98 with dat():
				// (dcl def 1 "num_only")
				// (dcl def 2 "num_str")
				// (dcl def 3 "tag")
				// (dcl def 4 "text")
				// (dcl def 5 "page")
				// (dcl def 6 "chapnum")
				// (dcl def 7 "volnum")
				if (tok != ptok) {
					if (ptok) {  // close prev one now
						cc = new DCctl(endgroup, 2, 98, (uns) ptok);
						instg->insert(*cc, *ic);
					}
					cc = new DCctl(group, 2, 98, (uns) tok);
					instg->insert(*cc, *ic);
				}
			}
			else
				tok = ptok;
			// look for end of group; may be match by next inst ctl to
			// next form ctl, or scan inst looking for matching ctl
			nii = ii;  // stay on current inst item
			continue;
		}

		// text processing
		if ((fc->maj() == 2)
		 && (fc->min() == 10)
		 && (fc->dat() == 1)
		 && (ic->maj() == 2)
		 && (ic->min() == 10)
		 && (ic->dat() == 1)) {
			if ((fc->dt() == etext)
			 && (ic->dt() == etext)) {
				ip = (char *) ic->ex();
				if (!infstr) {
					fp = (char *) fc->ex();
					fs = fc->siz();
				}
				else {  // at next inst str
					infstr = false;
					if ((nfi = fi->next) != NULL)
						nfc = nfi->ctl;
				} 
				if (fs <= ic->siz()) { // fc is substring
					if (!strcmp(ip, fp)) { // full match
						if (tok) {  // token ends before fc content
							cc = new DCctl(endgroup, 2, 98, (uns) tok);
							instg->insert(*cc, *ic);
							tok = 0;
						}
						continue;
					}
					if ((mp = strstr(ip, fp)) != NULL) {
						if (mp == ip) { // fc is at start, separate it
							if (tok) {  // token ends before fc content
								cc = new DCctl(endgroup, 2, 98, (uns) tok);
								instg->insert(*cc, *ic);
								tok = 0;
							}
							if (*(ip + strlen(fp)) == '\0')
								continue;
							cc = new DCctl(etext, 2, 10, 1);
							cc->ex(ip, strlen(fp));
							instg->insert(*cc, *ic);
							ic->ex(NewName(ip + strlen(fp), strlen(ip) - strlen(fp)));
						}
						else { // later on, separate first part
							cc = new DCctl(etext, 2, 10, 1);
							cc->ex(ip, mp - ip);
							instg->insert(*cc, *ic);
							if (tok) {  // token ends at start of fc content
								cc = new DCctl(endgroup, 2, 98, (uns) tok);
								instg->insert(*cc, *ic);
								tok = 0;
							}
							ic->ex(NewName(mp));
						}
						nii = ii;  // match, stay with remaining ic text
						continue;
					}
				}
				else {  // fc longer than ic, must be line break
					while (*fp == *ip) {
						fp++;
						ip++;
						fs--;
					}
					// fp is next text to match, fs is length
					// advance ic past disc hyphen if any and line break
					// find rest of fp in next ip string
					//nfi = fi;  // stay on this line in format
					infstr = true;
					continue;    // skip in inst to next string
				}
			}
			else if (fc->dt() == imnem) { 
				if (ic->dt() == imnem) {  // see if matching char
					if (fc->siz() == ic->siz()) {
						if (tok) {  // token ends before fc content
							cc = new DCctl(endgroup, 2, 98, (uns) tok);
							instg->insert(*cc, *ic);
							tok = 0;
						}
						continue;
					}
				}
				else if (ic->dt() == etext) {  // look in str for char
					ip = (char *) ic->ex();
					if ((mp = strchr(ip, fc->siz())) != NULL) {
						if (mp == ip) { // at start, separate it
							if (tok) {  // token ends before fc content
								cc = new DCctl(endgroup, 2, 98, (uns) tok);
								instg->insert(*cc, *ic);
								tok = 0;
							}
							if (*(mp + 1) == '\0')
								continue;
							cc = new DCctl(imnem, 2, 10, 1);
							cc->siz(fc->siz());
							instg->insert(*cc, *ic);
							ic->ex(NewName(mp + 1));
						}
						else if (*(mp + 1) == '\0') { // at end
							ic->ex(NewName(ip, mp - ip));
							if (nii) {
								ii = nii;
								ic = ii->ctl;
							}
							if (tok) {  // token ends before fc content
								cc = new DCctl(endgroup, 2, 98, (uns) tok);
								instg->insert(*cc, *ic);
								tok = 0;
							}
							cc = new DCctl(imnem, 2, 10, 1);
							cc->siz(fc->siz());
							instg->insert(*cc, *ic);
							continue;
						}
						else { // later on, separate first part
							cc = new DCctl(etext, 2, 10, 1);
							cc->ex(ip, mp - ip);
							instg->insert(*cc, *ic);
							if (tok) {  // token ends at start of fc content
								cc = new DCctl(endgroup, 2, 98, (uns) tok);
								instg->insert(*cc, *ic);
								tok = 0;
							}
							cc = new DCctl(imnem, 2, 10, 1);
							cc->siz(fc->siz());
							instg->insert(*cc, *ic);
							ic->ex(NewName(mp + 1));
						}
						nii = ii;  // match, stay with remaining ic text
						continue;
					}
				}
			}
			nfi = fi; // str or char not in inst str, stay with fc
			continue;
		}

		// format processing
		if ((fc->maj() == 2)
		 && (fc->min() == 82)
		 && (fc->dat() == 2)) {
			if ((ic->maj() == 2)
			 && (ic->min() == 82)
			 && (ic->dat() == 2)
			 && (ic->siz() == fc->siz())) {
				if (tok) {  // token ends before matching format
					cc = new DCctl(endgroup, 2, 98, (uns) tok);
					instg->insert(*cc, *ic);
					tok = 0;
				}
				continue;
			}
			nfi = fi;
			continue;
		}
	}

	if (tok) {
		cc = new DCctl(endgroup, 2, 98, (uns) tok);
		instg->add(*cc);
	}
}

// previous loop:

#if 0
	while (ic && fc) {
		if (seektab) {
			if ((ic->maj() == 2)
			 && (ic->min() == 41)) {
				seektab = false;
				fstep = true;
			}
		}
		else if (fc->cmp(*ic) == 0) { // match, end group
			if (tok)
				endgr = true;
			fstep = true;
			istep = true;
		}
		else if ((fc->dt() == imnem)
		 && (fc->maj() == 2)
		 && (fc->min() == 97)) {  // ref_token
			unl ptok = tok;
			tok = 0;
			if (fc->dat() == 35) {  // xref
				tok = fc->siz();
				// (dcl def 1 "num_only")
				// (dcl def 2 "num_str")
				// (dcl def 3 "tag")
				// (dcl def 4 "text")
				// (dcl def 5 "page")
			}
			else if (fc->dat() == 24) {  // docnum
				tok = fc->siz() + 5;
				// (dcl def 1 "chapnum")
				// (dcl def 2 "volnum")
			}
			if (tok) {
				// insert before ic 98 with dat():
				// (dcl def 1 "num_only")
				// (dcl def 2 "num_str")
				// (dcl def 3 "tag")
				// (dcl def 4 "text")
				// (dcl def 5 "page")
				// (dcl def 6 "chapnum")
				// (dcl def 7 "volnum")
				if (tok != ptok) {
					if (ptok) {  // close prev one now
						cc = new DCctl(endgroup, 2, 98, (uns) ptok);
						instg->insert(*cc, *ic);
					}
					cc = new DCctl(group, 2, 98, (uns) tok);
					instg->insert(*cc, *ic);
				}
			}
			else
				tok = ptok;
			// look for end of group; may be match by next inst ctl to next
			// form ctl, or scan down inst looking for matching ctl, correcting
			// for soft line breaks and reflowed text obj lits.  But if next
			// form ctl is another ref_token, big trouble; where is boundary
			// in inst?
			fonce = true;

			// peek at next form ctl, is it text found in current inst ctl?
			nic = NULL;
			if (((nfi = fi->next) != NULL)
			 && ((nfc = nfi->ctl) != NULL)
			 && (nfc->maj() == 2)
			 && (nfc->min() == 10)
			 && (nfc->dat() == 1)
			 && (ic->maj() == 2)
			 && (ic->min() == 10)
			 && (ic->dat() == 1)) {
				if ((nfc->dt() == etext)
				 && (ic->dt() == etext)) { // text obj lit strings
					if (((ic->siz() > nfc->siz())  // find form str in inst str
						&& ((mp = strstr((ip = (char *) ic->ex()), (char *) nfc->ex())) != NULL))
					 || ((ic->siz() <= nfc->siz())  // see if inst str broken by line soft
						&& ((nii = ii->next) != NULL) 
						&& ((nic = nii->ctl) != NULL)
						&& (nic->dt() == imnem)
						&& (nic->maj() == 2)
						&& (nic->min() == 11)
						&& (nic->dat() == 10)
						&& (nic->siz() == 0)  // text break line soft broke string
						&& ((nii = nii->next) != NULL) 
						&& ((nic = nii->ctl) != NULL)
						&& (nic->dt() == etext)
						&& (nic->maj() == 2)
						&& (nic->min() == 10)
						&& (nic->dat() == 1)  // rest of it is here
						&& (nfc->siz() > nic->siz())
						&& ((mp = strstr((ip = (char *) nfc->ex()), (char *) nic->ex())) != NULL)
						&& ((xp = NewName(ip, mp - ip)) != NULL)  // frag of format txt in ic
						&& ((mp = strstr((ip = (char *) ic->ex()), xp)) != NULL))) {
						// inst str contains form str, mp - ip is len of new str to insert.
						cc = new DCctl(etext, 2, 10, 1);
						cc->ex(ip, mp - ip);
						instg->insert(*cc, *ic);
						cc = new DCctl(endgroup, 2, 98, (uns) tok);
						instg->insert(*cc, *ic);
						tok = 0;
						ic->ex(ip = NewName(mp));
						DeleteName(ip);
						fstep = true;
						fonce = false;
						fi = nfi;
						fc = nfc;
						if (nic) {
							ii = nii;
							ic = nic;
						}
					}
				}
				else if ((nfc->dt() == imnem)
				 && (ic->dt() == imnem)  // text obj lit special chars
				 && (ic->siz() == nfc->siz())) {
					fi = nfi;
					fc = nfc;
					fstep = true;
					fonce = false;
					endgr = true;
				}
				else if ((nfc->dt() == imnem)  // special char in format
				 && (ic->dt() == etext) // text obj lit strings
				 &&	((mp = strchr(ip = (char *) ic->ex(), (int) nfc->siz())) != NULL)) {
					// inst str contains spec char
					cc = new DCctl(etext, 2, 10, 1);
					cc->ex(ip, mp - ip);
					instg->insert(*cc, *ic);
					cc = new DCctl(endgroup, 2, 98, (uns) tok);
					instg->insert(*cc, *ic);
					tok = 0;
					cc = new DCctl(*nfc);
					instg->insert(*cc, *ic);
					ic->ex(ip = NewName(mp + 1));
					DeleteName(ip);
					fi = nfi;
					fc = nfc;
					nfi = fi->next;
					nfc = nfi->ctl;
					fstep = true;
					istep = false;
				}
			}
		}

		else if ((fc->maj() == 2)
		 && (fc->min() == 41)
		 && (fc->dat() == 8)) {  // (text tab_pos unknown )
			if (tok) {  // close prev one now
				cc = new DCctl(endgroup, 2, 98, (uns) tok);
				instg->insert(*cc, *ic);
				tok = 0;
			}
			// move forward to next, skip in inst to after next text tab_pos
			if ((ic->maj() == 2)
			 && (ic->min() == 41)) {
				seektab = false;
				fstep = true;
			}
			else {
				seektab = true;
				fstep = false;
			}
		}

		else if ((fc->maj() == 2)
		 && (fc->min() == 82)
		 && (fc->dat() == 2)) {  // (text format inherit ... )
			fstep = true;
			istep = false;
		}

		else if ((fc->dt() == etext)
		 && (fc->maj() == 2)
		 && (fc->min() == 10)
		 && (fc->dat() == 1)
		 && (ic->dt() == etext)
		 && (ic->maj() == 2)
		 && (ic->min() == 10)
		 && (ic->dat() == 1)) { // text obj lit strings
			if (tok) {
				if (((ic->siz() > fc->siz())
					&& ((mp = strstr((ip = (char *) ic->ex()), (char *) fc->ex())) != NULL))
				 || ((ic->siz() <= nfc->siz())
					&& ((nii = ii->next) != NULL) 
					&& ((nic = nii->ctl) != NULL)
					&& (nic->dt() == imnem)
					&& (nic->maj() == 2)
					&& (nic->min() == 11)
					&& (nic->dat() == 10)
					&& (nic->siz() == 0)  // text break line soft broke string
					&& ((nii = ii->next) != NULL) 
					&& ((nic = nii->ctl) != NULL)
					&& (nic->dt() == etext)
					&& (nic->maj() == 2)
					&& (nic->min() == 10)
					&& (nic->dat() == 1)  // rest of it is here
					&& ((mp = strstr((ip = (char *) fc->ex()), (char *) nic->ex())) != NULL)
					&& ((xp = NewName(ip, mp - ip)) != NULL)  // frag of format txt in ic
					&& ((mp = strstr((ip = (char *) ic->ex()), xp)) != NULL))) {
					// inst str contains form str, mp - ip is len of new str to insert.
					cc = new DCctl(etext, 2, 10, 1);
					cc->ex(ip, mp - ip);
					instg->insert(*cc, *ic);
					cc = new DCctl(endgroup, 2, 98, (uns) tok);
					instg->insert(*cc, *ic);
					tok = 0;
					ic->ex(ip = NewName(mp));
					DeleteName(ip);
					fstep = true;
				}
			}	
			else { // see if fc is at start of ic
				if ((ic->siz() > fc->siz())
				 && ((mp = strstr(ip = (char *) ic->ex(), fp = (char *) fc->ex())) != NULL)
				 && (mp == ip)) {
					cc = new DCctl(etext, 2, 10, 1);
					cc->ex(fp);
					instg->insert(*cc, *ic);
					ic->ex(ip = NewName(ip + strlen(fp)));
					istep = false;
					fstep = true;
				}
			}
		}

#if 0
		else if ((ic->dt() == imnem)
		 && (ic->maj() == 2)
		 && (ic->min() == 10)
		 && (ic->dat() == 1)) { // text obj lit spec char
		}
#endif

		else if ((ic->dt() == imnem)
		 && (ic->maj() == 2)
		 && (ic->min() == 11)
		 && (ic->dat() == 10)
		 && (ic->siz() == 0)) { // text break line soft
			nii = ii->next;
			if (nii)
				nic = nii->ctl;
			if (nic
			 && (fc->cmp(*nic) == 0)) {
				ii = nii;
				ic = nic;
			}
			else if ((nic->maj() == 2)
			 && (nic->min() == 10)
			 && (nic->dat() == 1)) {
				nii = nii->next;
				if (nii)
					nic = nii->ctl;
				if (nic
				 && (fc->cmp(*nic) == 0)) {
					ii = nii;
					ic = nic;
					if (tok)
						endgr = true;
					fstep = true;
					istep = true;
				}
			}
		}

		if (endgr) {
			cc = new DCctl(endgroup, 2, 98, (uns) tok);
			instg->insert(*cc, *ic);
			tok = 0;
			endgr = false;
		}

		if (!ii || !fi)
			break;

		if (fstep) {
			if (!(fi = fi->next))
				break;
			fc = fi->ctl;
		}
		if (fonce) {
			fstep = false;
			fonce = false;
		}

		if (istep) {
			if (!(ii = ii->next))
				break;
			ic = ii->ctl;
		}
		else
			istep = true;

	}
#endif



void MIFrd::FixXrefFileName(DCctl *xfname)
{
	if (!xfname)
		return;

	char *oname = (char *) xfname->ex();
	char *nname = FixReference(oname);
	if (nname != oname) {
		xfname->ex(nname);
		DeleteName(nname);
	}
}


char *MIFrd::FixReference(const char *hrefattr)
{
	if (!hrefattr)
		return NULL;

	char nref[MAX_PATH];

	// determine if reference is an absolute path or URI
	// if so, use it as it is.
	if (!PathIsRelative(hrefattr)
	 || PathIsURL(hrefattr))
		return NewName(hrefattr);


	char *dclpath = DCLFile->getBasePath();
	if (!*dclpath)
		dclpath = "./";
	if (_fullpath(nref, dclpath, MAX_PATH))
		dclpath = NewName(nref);
	else
		return NewName(hrefattr);

	// dclpath is the full path to the file that will contain the ref

	char *xmlpath = MIFFile->getBasePath();
	if (!*xmlpath)
		xmlpath = "./";
	if (_fullpath(nref, xmlpath, MAX_PATH))
		xmlpath = NewName(nref);
	else
		return NewName(hrefattr);

	// xmlpath is the full path to the file we are reading the ref from
	char *hrefpath = NULL;
	if (*hrefattr == '#') {
		PathCombine(nref, xmlpath, MIFFile->getFileName());
		hrefpath = NewName(nref, strlen(nref) + strlen(hrefattr));
		strcat(hrefpath, hrefattr);
	}
	else {
		PathCombine(nref, xmlpath, hrefattr);
		hrefpath = NewName(nref);
	}

	DeleteName(xmlpath);

	if (_fullpath(nref, hrefpath, MAX_PATH)) {
		DeleteName(hrefpath);
		hrefpath = NewName(nref);
	}
	else
		return NewName(hrefattr);

	// hrefpath is now the full path to the referenced item

	char *hrp = NULL;

#if 0
	if (UseFullPath) {
		for (hrp = hrefpath; *hrp; hrp++) {
			if (*hrp == '\\')
				*hrp = '/';
		}

		return hrefpath;
	}
#endif

	if (PathRelativePathTo(nref, dclpath, FILE_ATTRIBUTE_DIRECTORY,
	     hrefpath, FILE_ATTRIBUTE_NORMAL)) {
		DeleteName(dclpath);
		DeleteName(hrefpath);
	}
	else
		return NewName(hrefattr);

	// nref is now the path to the referenced item from the DCL file

	for (hrp = nref; *hrp; hrp++) {
		if (*hrp == '\\')
			*hrp = '/';
	}

	// see if this is a path to another doc we are processing
	// if so, we need to replace it with an internal reference ****

	return NewName(nref);
}



// elements


bool MIFrd::BeginElement = false;
bool MIFrd::ElemAttr = false;
bool MIFrd::IDAttr = false;
bool MIFrd::IDRefAttr = false;
char *MIFrd::IDAttrName = "Id";
char *MIFrd::IDRefAttrName = "Idref";

void
MIFrd::EndElement(void)
{
	BeginElement = false;
}

void
MIFrd::EndElementBegin(void)
{
	BeginElement = false;
}

void
MIFrd::EndAttribute(void)
{
	if (AttrMark)
		ProcAttrMarker();
	ElemAttr = false;
}



// markers

bool MIFrd::InMarker = false;
bool MIFrd::InHiddenMarker = false;
uns MIFrd::MarkType = 0;	// original Frame marker number
uns MIFrd::MarkHyperType = 0;	// mapped hypertext marker number
bool MIFrd::InMarkCat = false;  // in MarkerTypeCatalog
char *MIFrd::MarkName = NULL;
DCgroup *MIFrd::IndexGroup = NULL;

DCnlist MIFrd::AttrMarkers;
char *MIFrd::AttrMark = NULL;
char *MIFrd::AttrMarkVal = NULL;

uns MIFrd::MarkTList[MarkTypes] = {
 6, // 0   header/footer $1
 6, // 1   header/footer $2
 1, // 2   index
 3, // 3   comment
 3, // 4   subject
 3, // 5   author
 3, // 6   glossary
 3, // 7   equation
 5, // 8   hypertext
 4, // 9   xref
 0  // 10  cond text
};

char *MIFrd::MarkNames[MarkTypes] = {
 "Header/Footer $1",
 "Header/Footer $2",
 "Index",
 "Comment",
 "Subject",
 "Author",
 "Glossary",
 "Equation",
 "Hypertext",
 "Cross-Ref",
 "Conditional Text"
};

char *MIFrd::MarkHyperNames[MarkHyperTypes] = {
 "openObjectIDfitwin",
 "openObjectID",
 "gotoObjectIDfitwin",
 "gotoObjectID",
 "openpage",
 "gotopage",
 "openlinkfitwin",
 "openlink",
 "gotolinkfitwin",
 "gotolink",
 "newlink",
 "message",
 "alert",
 "alerttitle",
 "popup"
};


void
MIFrd::StartMarkCat(void)
{
	InMarkCat = true;
	EndProc[MIFlevel] = EndMarkCat;
}


void
MIFrd::EndMarkCat(void)
{
	InMarkCat = false;
}

void
MIFrd::StartMarker(void)
{
	InMarker = true;
	MarkType = 0;
	MarkHyperType = 0;
	IndexGroup = NULL;

	EndProc[MIFlevel] = EndMarker;
}


void
MIFrd::EndMarker(void)
{
	InMarker = false;
	IndexGroup = NULL;
}


void
MIFrd::ProcAttrMarker(void)
{
	InMarker = true;
	MarkType = 0;
	MarkHyperType = 0;
	IndexGroup = NULL;

	MarkName = AttrMark;
	GetMarkType();
	MIFstring = AttrMarkVal;

	mif *mp = new mif;
	mp->name = "MText";
	mp->mtype = unameval;
	mp->dtype = etext;
	mp->maj = 2;
	mp->min = 110;
	mp->dat = 0;
	mp->spec = marktxt;

	ProcMarker(mp);

	InMarker = false;
	IndexGroup = NULL;
	AttrMark = NULL;
	AttrMarkVal = NULL;
}


void
MIFrd::GetMarkType(void)
{
	long i = 0;
	bool matched = false;

	MarkHyperType = 0;
	MarkType = 12; // custom, not special, is default
	for (i = 0; i < (MarkTypes - 1); i++) { // exclude cond text
		if (!stricmp(MarkNames[i], MarkName)) {
			MarkType = (uns) i;
			matched = true;
			break;
		}
	}
	if (!matched) {
		for (i = 0; i < MarkHyperTypes; i++) {
			if (!strnicmp(MarkHyperNames[i], MarkName, strlen(MarkHyperNames[i]))) {
				MarkType = 8;
				MarkHyperType = i + 1;
				break;
			}
		}
	}
}


void
MIFrd::ProcMarker(mif *mp)
{
	// do mapping of marker usage per [Markers] info for MarkName
	// permit remap to nothing (delete) or to a different name
	if (!MarkName || !*MarkName) {
		if (MarkType < MarkTypes)
			MarkName = MarkNames[MarkType];
		else if (MarkType == 17)
			MarkName = "HTML Macro";
		else {
			MarkName = NewName("Type", NumLenMax + 5);
			sprintf(MarkName + 4, "%d", MarkType);
		}
	}

	uns i = 0;

	if ((MarkType != 10)  // conditional
	 && IniFile
	 && IniFile->Section("Markers")
	 && IniFile->Find(MarkName)
	 && IniFile->ListRight()) {
		char *item = NULL;
		bool matched = false;
		while ((item = IniFile->ListItem()) != NULL) {
			matched = false;
			MarkName = NewName(item);
			GetMarkType();
#if 0
			MarkHyperType = 0;
			MarkType = 12; // custom, not special, is default
			for (i = 0; i < (MarkTypes - 1); i++) { // exclude cond text
				if (!stricmp(MarkNames[i], MarkName)) {
					MarkType = i;
					matched = true;
					break;
				}
			}
			if (!matched) {
				for (i = 0; i < MarkHyperTypes; i++) {
					if (!strnicmp(MarkHyperNames[i], MarkName, strlen(MarkHyperNames[i]))) {
						MarkType = 8;
						MarkHyperType = i + 1;
						break;
					}
				}
			}
#endif
			// for each name in list, ProcMarkText
			ProcMarkText(mp);
		}
	}
	else {
		for (i = 0; i < MarkHyperTypes; i++) {
			if (!strnicmp(MarkHyperNames[i], MarkName, strlen(MarkHyperNames[i]))) {
				MarkType = 8;
				MarkHyperType = i + 1;
				break;
			}
		}
		ProcMarkText(mp);
	}
}


void
MIFrd::ProcMarkText(mif *mp)
{
  DCctl *cp = NULL;
  char *chp = NULL;
  size_t len, tlen;
	long id = 0;

  uns mark = (MarkType < MarkTypes) ? MarkTList[MarkType] : 3;
  if (mark == 0) {  // hidden conditional text
    ProcHiddenMarker();
    return;
  }

  AddText(0);

	if (MarkType == DirectiveMark) {  // DCL directive
		ProcDirectiveMarker();
		return;
	}

	if ((ALinkMarkerType != 0)
	 && (MarkType == ALinkMarkerType)) {  // subject marker
	  cp = new DCctl(etext, 2, 110, 6);
		cp->ex(MIFstring);
    GroupStack[MIFlevel]->add(*cp);
		return;
	}

	if (((ExtraIXMarkerType != 0)
		&& (MarkType == ExtraIXMarkerType))   // extended index marker
	 || (ExtraIXMarkerName
	  && *ExtraIXMarkerName
		&& MarkName
	  && !stricmp(MarkName, ExtraIXMarkerName))) {
		mark = 1;
		if (ExtraIXPrefix
		 && *ExtraIXPrefix) {
			chp = NewName(strlen(MIFstring) + strlen(ExtraIXPrefix));
			strcpy(chp, ExtraIXPrefix);
			strcat(chp, MIFstring);
			strcpy(MIFstring, chp);
			DeleteName(chp);
			chp = NULL;
		}
	}

#if 0
	if (MarkName) {
		DeleteName(MarkName);
		MarkName = NULL;
	}
#endif

  if (mark == 1) {  // index marker
    DCgroup *gp = new DCgroup(DCctl(group, mp->maj, mp->min, 1));
    GroupStack[++MIFlevel] = gp;
    ProcVXForm(gp);
    GroupStack[--MIFlevel]->add(*gp);
		IndexGroup = gp;
    return;
  }

  if (mark == 6) {	// header/footer
    cp = new DCctl(mp->dtype, mp->maj, 111, MarkType + 1);
    tlen = strlen(MIFstring) + 1;
    cp->ex(MIFstring, tlen);
    GroupStack[MIFlevel]->add(*cp);
    return;
  }

	if (mark == 4) {   // xref markers 
		if ((XrefType == 'N')
		 && isdigit(*MIFstring)
		 && isdigit(*(MIFstring + 4))
		 && (*(MIFstring + 5) == ':')) {	// conforming to "nnnnn:" pattern
			cp = new DCctl(ilong, 2, 110, 4, atol(MIFstring));
			GroupStack[MIFlevel]->add(*cp);
			return;
		}

		if (*MIFstring == '\0')  // remove empty markers
			return;

		if (RemoveWordTocMarkers
		 && !strncmp(MIFstring, "_Toc", 4))  // or Word TOC markers
			return;
	}

	if (mark == 5) {  // hypertext markers, parse
		if (MarkHyperType) { // pre-parsed by being mapped in [Markers]
			chp = MIFstring;
			if (MarkHyperType < 11)   // filenames, before newlink
				ParseHyperFileName(chp);
			if (MarkHyperType < 5) {  // ObjectID
				if (isdigit(*chp) // char after colon
				 && (*(chp + 1) == ' ')
				 && isdigit(*(chp + 2)))
					id = atol(chp + 2);
			}
			switch (MarkHyperType) {

				case 1:  // openObjectIDfitwin
					cp = new DCctl(imnem, 2, 112, 1, 1);
					GroupStack[MIFlevel]->add(*cp);
					// fall through
				case 2: // openObjectID
					cp = new DCctl(ilong, 2, 112, 5, id);
					break;

				case 3: // gotoObjectIDfitwin
					cp = new DCctl(imnem, 2, 112, 1, 1);
					GroupStack[MIFlevel]->add(*cp);
					// fall through
				case 4: // gotoObjectID
					cp = new DCctl(ilong, 2, 112, 4, id);
					break;

				case 5: // openpage
				case 6: // gotopage
					if (isdigit(*chp)) // char after colon
						id = atol(chp);
					else if (_strnicmp(chp, "firstpage", 9))
						id = 1; 
					else if (_strnicmp(chp, "lastpage", 8))
						id = 0; 
					cp = new DCctl(ilong, 2, 112, 11, id);
					break;

				case 7: // openlinkfitwin
					cp = new DCctl(imnem, 2, 112, 1, 1);
					GroupStack[MIFlevel]->add(*cp);
					// fall through
				case 8: // openlink
					//if (isdigit(*chp)) { // char after colon
					//	id = atol(chp);
					//	cp = new DCctl(ilong, 2, 112, 5, id);
					//}
					//else {
						cp = new DCctl(etext, 2, 112, 5);
						cp->ex(chp);
					//}
					break;

				case 9: // gotolinkfitwin
					cp = new DCctl(imnem, 2, 112, 1, 1);
					GroupStack[MIFlevel]->add(*cp);
					// fall through
				case 10: // gotolink
					//if (isdigit(*chp)) { // char after colon
					//	id = atol(chp);
					//	cp = new DCctl(ilong, 2, 112, 4, id);
					//}
					//else {
						cp = new DCctl(etext, 2, 112, 4);
						cp->ex(chp);
					//}
					break;

				case 11: // newlink
					cp = new DCctl(etext, 2, 112, 2);
					cp->ex(chp);
					break;

				case 12:  // message
					while (*chp && isspace(*chp)) // strip leading spaces
					chp++;
					if (*chp == '\0')  // empty
						return;
					cp = new DCctl(etext, 2, 112, 7);
					cp->ex(chp);
					break;

				case 13: // alert
					cp = new DCctl(etext, 2, 112, 6);
					cp->ex(MIFstring);
					break;

				case 14: // alerttitle
					cp = new DCctl(etext, 2, 112, 13);
					cp->ex(MIFstring);
					break;

				case 15: // popup menu
					cp = new DCctl(etext, 2, 112, 14);
					cp->ex(MIFstring);
					break;

				default:
					break;
			}

			GroupStack[MIFlevel]->add(*cp);
			return;
		}
		if (!_strnicmp("openObjectIDfitwin", MIFstring, 18)) {
			ParseHyperFileName(chp = MIFstring + 19);
		  cp = new DCctl(imnem, 2, 112, 1, 1);
	    GroupStack[MIFlevel]->add(*cp);
			if (isdigit(*chp) // char after colon
			 && (*(chp + 1) == ' ')
			 && isdigit(*(chp + 2)))
				id = atol(chp + 2);
			cp = new DCctl(ilong, 2, 112, 5, id);
		}
		else if (!_strnicmp("openObjectID", MIFstring, 12)) {
			ParseHyperFileName(chp = MIFstring + 13);
			if (isdigit(*chp) // char after colon
			 && (*(chp + 1) == ' ')
			 && isdigit(*(chp + 2)))
				id = atol(chp + 2);
			cp = new DCctl(ilong, 2, 112, 5, id);
		}
		else if (!_strnicmp("gotoObjectIDfitwin", MIFstring, 18)) {
			ParseHyperFileName(chp = MIFstring + 19);
		  cp = new DCctl(imnem, 2, 112, 1, 1);
	    GroupStack[MIFlevel]->add(*cp);
			if (isdigit(*chp) // char after colon
			 && (*(chp + 1) == ' ')
			 && isdigit(*(chp + 2)))
				id = atol(chp + 2);
			cp = new DCctl(ilong, 2, 112, 4, id);
		}
		else if (!_strnicmp("gotoObjectID", MIFstring, 12)) {
			ParseHyperFileName(chp = MIFstring + 13);
			if (isdigit(*chp) // char after colon
			 && (*(chp + 1) == ' ')
			 && isdigit(*(chp + 2)))
				id = atol(chp + 2);
			cp = new DCctl(ilong, 2, 112, 4, id);
		}
		else if (!_strnicmp("gotopage", MIFstring, 8)) {
			ParseHyperFileName(chp = MIFstring + 9);
			if (isdigit(*chp)) // char after colon
				id = atol(chp);
			cp = new DCctl(ilong, 2, 112, 11, id);
		}
		else if (!_strnicmp("openlinkfitwin", MIFstring, 14)) {
			ParseHyperFileName(chp = MIFstring + 15, true);
		  cp = new DCctl(imnem, 2, 112, 1, 1);
	    GroupStack[MIFlevel]->add(*cp);
			//if (isdigit(*chp)) { // char after colon
			//	id = atol(chp);
			//	cp = new DCctl(ilong, 2, 112, 5, id);
			//}
			//else {
				cp = new DCctl(etext, 2, 112, 5);
				cp->ex(chp);
			//}
		}
		else if (!_strnicmp("openlink", MIFstring, 8)) {
			ParseHyperFileName(chp = MIFstring + 9, true);
			//if (isdigit(*chp)) { // char after colon
			//	id = atol(chp);
			//	cp = new DCctl(ilong, 2, 112, 5, id);
			//}
			//else {
				cp = new DCctl(etext, 2, 112, 5);
				cp->ex(chp);
			//}
		}
		else if (!_strnicmp("gotolinkfitwin", MIFstring, 14)) {
			ParseHyperFileName(chp = MIFstring + 15);
		  cp = new DCctl(imnem, 2, 112, 1, 1);
	    GroupStack[MIFlevel]->add(*cp);
			//if (isdigit(*chp)) { // char after colon
			//	id = atol(chp);
			//	cp = new DCctl(ilong, 2, 112, 4, id);
			//}
			//else {
				cp = new DCctl(etext, 2, 112, 4);
				cp->ex(chp);
			//}
		}
		else if (!_strnicmp("gotolink", MIFstring, 8)) {
			ParseHyperFileName(chp = MIFstring + 9);
			//if (isdigit(*chp)) { // char after colon
			//	id = atol(chp);
			//	cp = new DCctl(ilong, 2, 112, 4, id);
			//}
			//else {
				cp = new DCctl(etext, 2, 112, 4);
				cp->ex(chp);
			//}
		}
		else if (!_strnicmp("newlink", MIFstring, 7)) {
			ParseHyperFileName(chp = MIFstring + 8);
		  cp = new DCctl(etext, 2, 112, 2);
			cp->ex(chp);
		}
		else if (!_strnicmp("message url", MIFstring, 11)) {
			chp = MIFstring + 12;
			while (*chp && isspace(*chp)) // strip leading spaces
				chp++;
			if (*chp == '\0')  // empty
				return;
		  cp = new DCctl(etext, 2, 112, 7);
			cp->ex(chp);
		}
		else if (!_strnicmp("message openfile", MIFstring, 16)) {
			chp = MIFstring + 17;
			while (*chp && isspace(*chp)) // strip leading spaces
				chp++;
			if (*chp == '\0')  // empty
				return;
			char *cstr = chp;
			char *colon = NULL;
			if (((colon = strchr(chp, ':')) != NULL)
			 && (colon == chp + 1)
			 && isalpha(*chp)) {  //put protocol on absolute paths
				cstr = NewName(strlen(chp) + 9);
				strcpy(cstr, "file:///");
				strcat(cstr, chp);
			}
		  cp = new DCctl(etext, 2, 112, 7);
			cp->ex(cstr);
		}
		else { // type unknown, includes alert
		  cp = new DCctl(etext, 2, 112, 6);
			cp->ex(MIFstring);
		}
    GroupStack[MIFlevel]->add(*cp);
    return;
	}

	if ((MarkType == 11)  // no longer fixed in FM6
	 && (OutType != 'S')) {
		if ((Mark11 == 'M')
		 && (!_strnicmp("midtopic", MIFstring, 8))) {
			ParseHyperFileName(chp = MIFstring + 9);
			cp = new DCctl(etext, 2, 112, 10);
			cp->ex(chp);
			GroupStack[MIFlevel]->add(*cp);
			return;
		}
		else if (Mark11 == 'F') {
			ParseHyperFileName(chp = MIFstring);
			cp = new DCctl(etext, 2, 112, 10);
			cp->ex(chp);
			GroupStack[MIFlevel]->add(*cp);
			return;
		}
	}

	if (MarkType == 17) { // HTML macro (even in FM6)
	  cp = new DCctl(etext, 2, 112, 8);
		cp->ex(MIFstring);
	  GroupStack[MIFlevel]->add(*cp);
		return;
	}

  cp = new DCctl(mp->dtype, mp->maj, mp->min, mark);

	if (MarkName
	 && !stricmp(MarkName, "TopicAlias")) {
		cp->min(112);
		cp->dat(16);
		tlen = strlen(MarkName) + strlen(MIFstring) + 2;
		chp = NewName(MarkName, tlen);
		strcat(chp, ":");
		strcat(chp, MIFstring);
    cp->ex(chp, tlen);
    DeleteName(chp);
	}
  else if (mark == 3) {	// other lists, including custom types
	  // MarkTypes over 10, except 17, no longer descriptive in FM6
    len = strlen(MarkName);
    tlen = strlen(MIFstring) + len + 2;
    chp = NewName(tlen);
    strcpy(chp, MarkName);
    chp[len] = ':';
    chp[len + 1] = '\0';
    strcpy(&chp[len + 1], MIFstring);
    cp->ex(chp, tlen);
    DeleteName(chp);
		if (!stricmp(MarkName, "M2GSetting")
		 || !stricmp(MarkName, (FormType == 'H') ? "HTMSetting" : "RTFSetting"))
			CheckMarkerSetting(MIFstring);
  }
  else cp->ex(MIFstring, strlen(MIFstring) + 1);

  GroupStack[MIFlevel]->add(*cp);
}


void
MIFrd::CheckMarkerSetting(char *txt)
{
	char *sect = NULL;
	char *key = NULL;
	char *val = NULL;

	// [section]key=val for globals, or [section]val
	while (*txt && isspace(*txt))
		txt++;
	if (*txt == '[') {
		sect = txt + 1;
		while (*txt && (*txt != ']'))
			txt++;
		if (*txt)
			txt++;
	}
	while (*txt && isspace(*txt))
		txt++;

	if (!sect
	 || !DCini::SetSect(sect, false))
		return;  // invalid section name, forget it

	if (!*txt)
		return;  // nothing after section, ignore

	if (DCini::CurrSect->sectype == set) {  // get key and value
		key = txt;
		if ((val = strchr(txt, '=')) != NULL) {
			*val++ = '\0';
			DCini::ProcMarkSect(sect, key, val);  // global setting, do now
			return;
		}
		else  // key but no value
			return;
	}
	else // section with key prohibited, = is plain character
		val = txt;

#if 0
	// this is scan phase, save object setting for write phase
	setting *s = new setting;  
	s->sect = sect;
	s->key = key;
	s->val = val;
	if (DCini::CurrSect->sectype == tabl) {  // table setting
		TblSetList.add(s, (long) Tbl);
		return;
	}
	if (DCini::CurrSect->sectype == graphic) {  // graphic setting
		if (!GraphSetList)
			GraphSetList = new DClist;
		GraphSetList->add((long) s);  // retrieve at next graphic
		return;
	}
	if (DCini::CurrSect->sectype == xrstyle) {  // xref setting
		if (!XrefSetList)
			XrefSetList = new DClist;
		XrefSetList->add((long) s);  // retrieve at next xref
		return;
	}
	SetList.add(s, (long) cp);  // whatever is left is for Curr
#endif
}


void
MIFrd::ParseHyperFileName(char* &str, bool open)
{
  DCctl *cp = NULL;
	char *chs = str;
  char *che = str + strlen(str);
	char *chp = NULL;
	char *chn = NULL;
	char *fname = NULL;
	short fnlen = 0;

	while (*chs && isspace(*chs)) // strip leading spaces
		chs++;

	while (che > chs) {          // and trailing spaces
		if (isspace(*--che))
			*che-- = '\0';
		else {     // not a space, leave alone
			che++;   // and point at nul terminator
			break;
		}
	}

	if ((che > chs)  // some non-space content
	 && (((chp = strrchr(chs, ':')) != NULL)  // get rightmost colon
	  || open)) {  // or if openlink, default is filename not dest

		if ((chp == NULL)        // no colon, openlink, is file name
		 || (chp == (chs + 1)))  // drive letter colon, is file name
			fnlen = che - chs;
		else if (chp == (che - 1))  // colon at end, is file name
			fnlen = chp - chs;
		else {  // colon in middle, separator, file and dest names
			fnlen = chp - chs;
			chn = chp + 1;
		}

		if (fnlen) {  // filename is non-empty
			cp = new DCctl(etext, 2, 112, 3);
			fname = NewName(chs, fnlen);
			cp->ex(fname);
			GroupStack[MIFlevel]->add(*cp);
		}

		if (chn) {
			while (*chn && isspace(*chn)) // strip any more spaces
				chn++;
			str = chn;
		}
		else  // must be empty dest string
			str = che;
	}
	else  // no colon, not openlink, so no filename
		str = chs;
}



// DCL directive markers

char *MIFrd::DMAppName = NULL;
char *MIFrd::DMSectionName = NULL;
char *MIFrd::DMPropertyName = NULL;
char *MIFrd::DMPropertyValue = NULL;
bool MIFrd::LocalApp = false;
DCgroup *MIFrd::GraphicsDirectives = NULL;


void
MIFrd::ProcDirectiveMarker(void)
{
  DCctl *cp = NULL;
	char *ncp;
	char *lcp;
	uns len;
	DCgroup *gp = GroupStack[MIFlevel];

	// parse MIFstring for directive elements

	// check for scope prefix at front
	if (!_strnicmp(MIFstring, "G:", 2)) {     // graphic
		if (GraphicsDirectives == NULL)
			GraphicsDirectives = new DCgroup;
		gp = GraphicsDirectives;
	}
	else if (!_strnicmp(MIFstring, "F:", 2))	// file
		gp = GroupList[psg_base];

	// application name is of the form "dwrtf.ini"
	if (((ncp = strchr(MIFstring, '.')) != NULL)
	 && (ncp > MIFstring)
	 && !_strnicmp(ncp, ".ini", 4)) {
		len = 1;
		ncp--;
		while	(ncp > MIFstring) {
		  if (!isalnum(*ncp)) {
				len--;
				ncp++;
				break;
			}
			len++;
			ncp--;
		}
		if (len > 0) {
			if (DMAppName != NULL)
				DeleteName(DMAppName);
			DMAppName = NewName(ncp, len);
			if ((len == 5)
			 && !_strnicmp(ncp, "drmif", 5))  // for this file
				LocalApp = true;
			else LocalApp = false;
			if (LocalApp == false) {
				cp = new DCctl(etext, 1, 70, 1);
				cp->ex(DMAppName, len + 1);
				gp->add(*cp);
			}
		}
	}

	// section name is of the form "[Section]"
	if (((ncp = strchr(MIFstring, '[')) != NULL)
	 && ((lcp = strchr(MIFstring, ']')) != NULL)
	 && (lcp > (ncp + 1))) {
		len = lcp - ++ncp;
		if (DMSectionName != NULL)
			DeleteName(DMSectionName);
		DMSectionName = NewName(ncp, len);
		if (LocalApp == false) {
			cp = new DCctl(etext, 1, 70, 2);
			cp->ex(DMSectionName, len + 1);
			gp->add(*cp);
		}
	}

	// property name ends with equal sign
	// and value begins with it
	if ((ncp = strchr(MIFstring, '=')) != NULL) {
		if (ncp > MIFstring) { // '=' is not at start of line
			len = 0;
			lcp = ncp - 1;
			while	(lcp > MIFstring) {
				if (!isalnum(*lcp)) {
					lcp++;
					break;
				}
				len++;
				lcp--;
			}
			if (len > 0) {
				if (DMPropertyName != NULL)
					DeleteName(DMPropertyName);
				DMPropertyName = NewName(lcp, len);
				if (LocalApp == false) {
					cp = new DCctl(etext, 1, 70, 3);
					cp->ex(DMPropertyName, len + 1);
					gp->add(*cp);
				}
			}
		}
		if (*(ncp + 1) != '\0') {	// '=' is not at end of line
			lcp = ++ncp;
			while (*lcp)
				lcp++;
			len = lcp - ncp;
			if (len > 0) {
				if (DMPropertyValue != NULL)
					DeleteName(DMPropertyValue);
				DMPropertyValue = NewName(ncp, len);
				if (LocalApp == false) {
					cp = new DCctl(etext, 1, 70, 4);
					cp->ex(DMPropertyValue, len + 1);
					gp->add(*cp);
				}
			}
		}	 
	}

	if (LocalApp == true) {  // update local settings
		if (!_stricmp(DMSectionName, "Options")) {
			if (!_stricmp(DMPropertyName, "OutputType")) {
				OutType = toupper(*DMPropertyValue);
				if (OutType != 'S') {
					XrefType = 'N';
					ObjectIDs = 'A';
				}
			}
			//else if (!_stricmp(DMPropertyName, "ObjectIDs")) {
			//	ObjectIDs = toupper(*DMPropertyValue);
			//}
			else if (!_stricmp(DMPropertyName, "XrefType")) {
				XrefType = toupper(*DMPropertyValue);
			}
			else if (!_stricmp(DMPropertyName, "DirectiveMarker")) {
				DirectiveMark = (uns) atol(DMPropertyValue);
			}
			else if (!_stricmp(DMPropertyName, "ImportGraphics")) {
				ImpGraph = toupper(*DMPropertyValue);
				if (ImpGraph == 'E')
					ExFiles = true;
			}
			else if (!_stricmp(DMPropertyName, "EpsiUsage")) {
				EpsiUsage = toupper(*DMPropertyValue);
				if ((EpsiUsage == 'R')
				 || (EpsiUsage == 'E'))
					KeepEpsi = true;
				if ((EpsiUsage == 'R')
				 || (EpsiUsage == 'P'))
					EpsiPreview = true;
			}
		}
		else if (!_stricmp(DMSectionName, "Graphics")) {
			if (!_stricmp(DMPropertyName, "UseFrameImage")) {
				switch (toupper(*DMPropertyValue)) {
					case 'T':
					case 'Y':
					case '1':
						UseFrameImage = true;
						break;
					case 'F':
					case 'N':
					case '0':
						UseFrameImage = false;
						break;
					default:
						break;
				}
			}
		}
	}
}


// processing at end of doc group, before page images


void
MIFrd::EndDocGroup()
{
  MakeFNProp();

  MakePNDef();
  MakeFNDef(12); // table footnotes
  MakeANDefs();
}



// format autonumber and frame content


bool MIFrd::PNumUsed;
bool MIFrd::PNumEnd;
bool MIFrd::PTSepUsed;
bool MIFrd::PBSepUsed;
char MIFrd::PTSep[FStringMax];
char MIFrd::PBSep[FStringMax];
char MIFrd::PAForm[FStringMax];
char MIFrd::PAFont[FStringMax];
char MIFrd::PAVal[FStringMax];
bool MIFrd::PAFontSet = false;
DCclist MIFrd::RFrameRefList;
DCnlist MIFrd::RFrameNameList;
uns MIFrd::RFRefNum = 0;
uns MIFrd::PARefNum = 0;
uns MIFrd::AnumDefID = 0;
DCclist MIFrd::AnumRefList;
DCclist MIFrd::AnumFNRefList;
DCvlist MIFrd::AnumStructList;
DCilist MIFrd::AnumPFormList;
DCilist MIFrd::AnumDefList;
DCclist MIFrd::AnumDefCList;
DCclist MIFrd::AnumLevCList;
DCilist MIFrd::AnumNameList;
bool MIFrd::AnumsDefined;
DClist MIFrd::ChapnumRefList;
DClist MIFrd::ChapnumStructList;
DClist MIFrd::VolnumRefList;
DClist MIFrd::VolnumStructList;
DClist MIFrd::SectnumRefList;
DClist MIFrd::SectnumStructList;
DClist MIFrd::SSectnumRefList;
DClist MIFrd::SSectnumStructList;


void
MIFrd::ProcPFCont()
{
  DCgroup *gp = NULL;

  // produce prefix
  if ((*PTSep || (PTSepUsed && (CurrPG != psg_pgf)))
	 || (PNumUsed && *PAForm && !PNumEnd)) {
    gp = new DCgroup(DCctl(group, 2, 81, 3));
    if (*PTSep || (PTSepUsed && (CurrPG != psg_pgf))) {
      MakePFrame(gp, PTSep);
			PTSepUsed = false;
		}
    if (PNumUsed && *PAForm && !PNumEnd)
      MakePADef(gp);
    GroupStack[MIFlevel]->add(*gp);
  }

  // produce suffix
  if ((*PBSep || (PBSepUsed && (CurrPG != psg_pgf)))
	 || (PNumUsed && *PAForm && PNumEnd)) {
    gp = new DCgroup(DCctl(group, 2, 81, 4));
    if (*PBSep || (PBSepUsed && (CurrPG != psg_pgf))) {
      MakePFrame(gp, PBSep);
			PBSepUsed = false;
		}
    if (PNumUsed && *PAForm && PNumEnd)
      MakePADef(gp);
    GroupStack[MIFlevel]->add(*gp);
  }
}


void
MIFrd::MakePFrame(DCgroup *gp, char *fname)
{
  DCctl *cp = new DCctl(ilong, 3, 21, 4, 0);
  if (*fname && !cp->siz(FrameNameList(fname))) {
    RFrameRefList.add(cp, ++RFRefNum);
    RFrameNameList.addnew(fname, RFRefNum);
  }
  gp->add(*cp);
}


void
MIFrd::MakePADef(DCgroup *gp)
{
  // at present we have PAForm and PAFont, but they will be gone soon
  // build anum structs; later retrieve these and construct the anumdefs

  DCctl *cp = NULL;
	DCctl *fncp = NULL;
	DCctl *ccp = NULL;
  char ch;
  uns val;
  anum *a = new anum;
  alevel *al = NULL;
  long anumid = 0;
  long fntid = 0;

  AddText(0);  // clear any buffered text
  DCgroup *fgp = new DCgroup(DCctl(group, 2, 101, 4));
  GroupStack[++MIFlevel] = fgp;
  GenericTab = true;

  a->seq = 0;

#if 0
  if (*PAFont) {
    a->fnt = strcpy(NewName(strlen(PAFont) + 1), PAFont);
    if (AnumsDefined)
      fntid = CFormNameList(a->fnt);
    fncp = new DCctl(imnem, 2, 82, 2, fntid);
    gp->add(*fncp);
  }
  else a->fnt = NULL;
#endif

  if (PAFontSet)  // new char format (font) name was given
    a->fnt = (*PAFont != '\0') ? NewName(PAFont) : NULL;
	else if ((CurrPG != psg_pgf)  // not a para form def
	 && CurrPForm) {  // an override, so get default font
	  long refnum = AnumPFormList(CurrPForm);  // def anum for format
		if (refnum) {  // format defined an anum, get its font name
			anum *defan = (anum *) AnumStructList.find(refnum);
			if (defan)   // use same name for current override
				a->fnt = defan->fnt;
		}
	}
  else  // no font specified, uses regular para font
		a->fnt = NULL;

	if (a->fnt) {  // put out ctl for char format to use
		if (AnumsDefined) // past document props, so list is filled
			fntid = CFormNameList.getnum(a->fnt);
		fncp = new DCctl(imnem, 2, 82, 2, fntid);
		gp->add(*fncp);   // else will update fncp->siz() later
	}


  char *p = PAForm;
  if (p[1] == ':') {  // series label
    a->name = p[0];
    p += 2;
  }
  else a->name = '0';

  a->levels = 0;
  for (; *p; p++) {
    if (*p == '\\') {
      ch = *++p;
      if (ch == '\\') {
        if (*(p + 1) == 't') {
          ch = *++p;
          AddText('\t');
        }
        else AddText('\\');
      }
      else if (ch == 't')
        AddText('\t');
      else if (ch == 'x')
        ProcHex(&p);
    }
    else if (*p == '<') { // counter
			if (*(p + 1) == '$') { // no, var
				AddText(0);  // close text ctl
				char *tok = p + 2;
				for (p = tok ; *p && (*p != '>') ; p++)
					;
				//int len = p - tok;
				int val = 0;
				if (!_strnicmp(tok, "chapnum", 7))
					val = 1;
				else if (!_strnicmp(tok, "volnum", 6))
					val = 2;
				else if (!_strnicmp(tok, "sectionnum", 10))
					val = 3;
				else if (!_strnicmp(tok, "subsectionnum", 13))
					val = 4;
				if (val) {
					if (++a->levels == 1)
						al = (a->next = new alevel);
					else al = (al->next = new alevel);
					al->lnum = a->levels;
					al->lval = val + 8;
					al->next = NULL;
					al->incr = false;
					ccp = new DCctl(imnem, 2, 102, val + 8, a->levels);
					//ccp = new DCctl(imnem, 2, 97, 24, val);
					fgp->add(*ccp);  // add token ctl
					al->show = true;
					al->set = true;
					al->setval = 0;  // get value later
				}
			}
			else {
				if (++a->levels == 1)
					al = (a->next = new alevel);
				else al = (al->next = new alevel);
				al->lnum = a->levels;
				al->next = NULL;
				al->incr = al->set = al->show = false;
				AddText(0);  // close text ctl
				ccp = new DCctl(imnem, 2, 102, 0, a->levels);
				fgp->add(*ccp);  // add token ctl

				ch = *++p;
				if (ch == '>')
					ccp->dat(al->lval = 1);  // invisible
				else {
					if (ch == ' ') {
						ch = *++p;
						ccp->dat(al->lval = 1);  // invisible
					}
					else {
						al->show = true;
						if (isdigit(ch)) {
							ccp->dat(al->lval = 2);  // arabic
							al->set = true;
							val = 0;
							for (; isdigit(ch); ch = *++p)
								val = (val * 10) + (ch - '0');
							//p--;
							al->setval = val;
						}
						else switch (ch) {
							case '+':
								al->incr = true;
							case 'n':
							case '#':
								ccp->dat(al->lval = 2);  // arabic
								break;
							case 'r':
								ccp->dat(al->lval = 3);  // lower roman
								break;
							case 'R':
								ccp->dat(al->lval = 4);  // upper roman
								break;
							case 'a':
								ccp->dat(al->lval = 5);  // lower alpha
								break;
							case 'A':
								ccp->dat(al->lval = 6);  // upper alpha
								break;
							default:
								break;
						}
						ch = *++p;
					}
					if (ch == '=') { // setting counter, get number
						al->set = true;
						val = 0;
						for (ch = *++p; isdigit(ch); ch = *++p)
							val = (val * 10) + (ch - '0');
						al->setval = val;
					}
					else if (ch == '+') {  // incrementing counter
						al->incr = true;
						al->incval = 1;     // get incr val if structured
						ch = *++p;
						if (isdigit(ch)) {  // also a common error, <n+1>
							val = 0;
							for (; isdigit(ch); ch = *++p)
								val = (val * 10) + (ch - '0');
							al->incval = val;
						}
					}
				}
			}
    }
		else if ((*p < 0)
		 && (MIFversion > 6)
		 && ::IsUTF8((unc *) p)) {
			AddText(::UTF8((unc **) &p));
			p--;
		}
    else AddText(*p);
  }

  AddText(0);
  GenericTab = false;
  --MIFlevel; // format group is complete

  AnumStructList.add( (void *) a, ++PARefNum);
  if (AnumsDefined) {
    anumid = AnumNameList(a->name);
    AnumDefList.add(anumid, PARefNum);
    AnumPFormList.add(CurrPForm, PARefNum);
  }
  else {
    if (a->fnt)
      AnumFNRefList.add(fncp, PARefNum);
    AnumPFormList.add(FormatID, PARefNum);
  }

  if (a->levels == 0) {  // not a real anum, just prefix text
    gp->merge(*fgp);
    return;
  }

  // make up the anumber group for the format
  DCgroup *ngp = new DCgroup(DCctl(group, 2, 10, 3));
  cp = new DCctl(ilong, 2, 101, 3, anumid);
  if (!AnumsDefined)
    AnumRefList.add(cp, PARefNum);
  ngp->add(*cp);

  ngp->add(*fgp);

  // make any level set and inc ctls
  for (al = a->next; al; al = al->next) {
    if (al->set) {
      cp = new DCctl(ilong, 2, 103, al->setval, al->lnum);
			if (al->lval == 9) { // chapnum
				ChapnumRefList.add((long) cp);
				ChapnumStructList.add((long) al);
			}
			else if (al->lval == 10) {  // volnum
				VolnumRefList.add((long) cp);
				VolnumStructList.add((long) al);
			}
			else if (al->lval == 11) {  // sectionnum
				SectnumRefList.add((long) cp);
				SectnumStructList.add((long) al);
			}
			else if (al->lval == 12) {  // subsectionnum
				SSectnumRefList.add((long) cp);
				SSectnumStructList.add((long) al);
			}
      ngp->add(*cp);
    }
    if (al->incr) {
      cp = new DCctl(ilong, 2, 101, 7, al->lnum);
      ngp->add(*cp);
			if (al->incval != 1) {
				cp = new DCctl(ilong, 2, 101, 11, al->incval);
				ngp->add(*cp);
			}
    }
  }
  gp->add(*ngp);
}


void
MIFrd::MakeANDefs(void)
{
  // retrieve the anum structs and build corresponding anumdefs
  // associate refnum with actual anumdef, plug in right number in cp

  anum *a;
  uns anumid;
  DCgroup *gp = NULL;
  DCctl *cp = NULL;
  static char nm[2];

  for (int i = 1; i <= PARefNum; i++) {
    a = (anum *) AnumStructList.find(i);
    assert(a);
    if (a->fnt) { // update anum font ctl in format def with real format ID
			long fnum = CFormNameList.getnum(a->fnt);
			if (!fnum
			 && (AnumCharNotInCatalog.getitem(a->fnt) == 0)) {
				long pfid = AnumPFormList.find(i);
				char *pn = FormNameList.find(pfid);
				AnumCharNotInCatalog.add(a->fnt, ++AnumCharNotInCatalogCount);
				LogEvent(logwarn, 1, "Char format for ", (pn ? pn : ""),
					" autonumber not in catalog: ",	a->fnt);
			}
      AnumFNRefList.find(i)->siz(fnum);
		}
    if (a->levels == 0)  // not real anum, don't make stream def
      continue;

    if ((anumid = (uns) AnumNameList(a->name)) == 0) { // not made yet
      gp = new DCgroup(DCctl(group, 2, 100, anumid = ++AnumDefID));

      cp = new DCctl(ename, 2, 101, 1);
      *nm = a->name;
      cp->ex(nm, 2);
      gp->add(*cp);

      GroupList[psg_anum]->add(*gp);
      AnumDefList.add(AnumDefID, i);
      AnumDefCList.add(cp, AnumDefID);
      AnumNameList.add(a->name, AnumDefID);

      cp = new DCctl(ilong, 2, 101, 7, a->levels);
      gp->add(*cp);
      AnumLevCList.add(cp, AnumDefID);
    }
    else {
			AnumDefList.add(anumid, i);  // already exists, refer to it
			cp = AnumLevCList.find(anumid);
			if (a->levels > cp->siz())
				cp->siz(a->levels);
		}

    // update anum stream ID ctl in format def with real anum ID
    AnumRefList.find(i)->siz(anumid);
  }
  AnumsDefined = true;
}


DCnlist MIFrd::AnumCharNotInCatalog;
long MIFrd::AnumCharNotInCatalogCount = 0;

void
MIFrd::ProcPAVal(void)
{
  DCctl *cp = NULL;
	DCctl *fcp = NULL;
  DCgroup *gp = NULL;
	DCgroup *vp = NULL;
  uns aref;
  anum *a;
	char *fntname = NULL;
  char ch;
	bool syfont = SymFont;
	long fval;

  if ((aref = (uns) AnumPFormList(CurrPForm)) == 0)  // never defined in Pgf,
    gp = vp = new DCgroup(DCctl(group, 2, 101, 8));  // use value string only

  else { // identify appropriate anumdef, put out complete anum ref
    a = (anum *) AnumStructList.find(aref);
    assert(a);

		// handle any font overrides
    if (a->fnt)
			fntname = a->fnt;
		if (*PAFont)
			fntname = PAFont;

		if (fntname) {
			if ((fval = CFormNameList.getnum(fntname)) != 0) {
				fcp = new DCctl(imnem, 2, 82, 2, fval);
				SetSymFont(fval);
			}
			else {
				if (AnumCharNotInCatalog.getitem(fntname) == 0) {
					AnumCharNotInCatalog.add(fntname, ++AnumCharNotInCatalogCount);
					LogEvent(logwarn, 1, "Char format used in anumber but not in catalog: ",
						fntname);
				}
				SymFont = false;
			}
		}
		else
			SymFont = false;

    if (a->levels) {  // real autonumber
      gp = new DCgroup(DCctl(group, 2, 10, 3));
      if (a->fnt)
        gp->add(*fcp);

      cp = new DCctl(ilong, 2, 101, 3, AnumDefList.find(aref));
      gp->add(*cp);

      cp = new DCctl(ilong, 2, 101, 6, ++(a->seq));
      gp->add(*cp);

      // make any level set and inc ctls
      for (alevel *al = a->next; al; al = al->next) {
        if (al->set) {
          cp = new DCctl(ilong, 2, 103, al->setval, al->lnum);
          gp->add(*cp);
        }
        if (al->incr) {
          cp = new DCctl(ilong, 2, 101, 7, al->lnum);
          gp->add(*cp);
					if (al->incval != 1) {
						cp = new DCctl(ilong, 2, 101, 11, al->incval);
						gp->add(*cp);
					}
        }
      }
      vp = new DCgroup(DCctl(group, 2, 101, 8));  // for value string
      gp->add(*vp);
    }
    else {  // generated text
      vp = gp = new DCgroup(DCctl(group, 2, 83, ParaStarting ? 1 : 2));
      if (fntname)
        gp->add(*fcp);
    }
  }
  AddText(0);  // purge buffer
  GroupStack[++MIFlevel] = vp;
  
  // process escapes in value string
	if (!PNumEnd)
		LineTabCount = 0;
  for (char *p = PAVal; *p; p++) {
    if (*p == '\\') {
      ch = *++p;
      if (ch == 't')
        AddText('\t');
      else if (ch == 'x')
        ProcHex(&p);
    }
		else if ((*p < 0)
		 && (MIFversion > 6)
		 && ::IsUTF8((unc *) p)) {
			AddText(::UTF8((unc **) &p));
			p--;
		}
    else AddText(*p);
  }

  AddText(0);
  GroupStack[--MIFlevel]->add(*gp);

  if (fntname) {  // restore the char properties
    fcp = new DCctl(imnem, 2, 82, 2, 0);
	  GroupStack[MIFlevel]->add(*fcp);
  }
	SymFont = syfont;
}



// page number processing


long MIFrd::FirstPage = 0;
uns MIFrd::PNumStyle = 0;
char MIFrd::NumStyles[NumStyleMax] = { 2, 3, 4, 5, 6, 7, 8 };
long MIFrd::FirstVol = 0;
uns MIFrd::VolStyle = 0;
char *MIFrd::VolText = NULL;
long MIFrd::FirstChap = 0;
uns MIFrd::ChapStyle = 0;
char *MIFrd::ChapText = NULL;
long MIFrd::FirstSect = 0;
uns MIFrd::SectStyle = 0;
char *MIFrd::SectText = NULL;
long MIFrd::FirstSSect = 0;
uns MIFrd::SSectStyle = 0;
char *MIFrd::SSectText = NULL;


void
MIFrd::MakePNDef()
{
  static bool def_made = false;

  if (def_made)
    return;
  def_made = true;

  // make page num definition
	MakeNumDef(FirstPage, 1, PNumStyle);

	if (FirstChap || ChapText)
		MakeNumDef(FirstChap, 8, ChapStyle, ChapText);
	if (FirstChap) {
		ChapnumRefList.all(SetChap);
		ChapnumStructList.all(SetChapStruct);
	}

	if (FirstVol || VolText)
		MakeNumDef(FirstVol, 9, VolStyle, VolText);
	if (FirstVol) {
		VolnumRefList.all(SetVol);
		VolnumStructList.all(SetVolStruct);
	}

	if (FirstSect || SectText)
		MakeNumDef(FirstSect, 10, SectStyle, SectText);
	if (FirstSect) {
		SectnumRefList.all(SetSect);
		SectnumStructList.all(SetSectStruct);
	}

	if (FirstSSect || SSectText)
		MakeNumDef(FirstSSect, 10, SSectStyle, SSectText);
	if (FirstSSect) {
		SSectnumRefList.all(SetSSect);
		SSectnumStructList.all(SetSSectStruct);
	}
}

void
MIFrd::SetChap(long i)
{
	if (i == 0)
		return;

	DCctl *cp = (DCctl *) i;
	cp->dat((uns) FirstChap);
}


void
MIFrd::SetChapStruct(long i)
{
	if (i == 0)
		return;

	alevel *al = (alevel *) i;
	al->setval = (uns) FirstChap;
}


void
MIFrd::SetVol(long i)
{
	if (i == 0)
		return;

	DCctl *cp = (DCctl *) i;
	cp->dat((uns) FirstVol);
}


void
MIFrd::SetVolStruct(long i)
{
	if (i == 0)
		return;

	alevel *al = (alevel *) i;
	al->setval = (uns) FirstVol;
}


void
MIFrd::SetSect(long i)
{
	if (i == 0)
		return;

	DCctl *cp = (DCctl *) i;
	cp->dat((uns) FirstSect);
}


void
MIFrd::SetSectStruct(long i)
{
	if (i == 0)
		return;

	alevel *al = (alevel *) i;
	al->setval = (uns) FirstSect;
}


void
MIFrd::SetSSect(long i)
{
	if (i == 0)
		return;

	DCctl *cp = (DCctl *) i;
	cp->dat((uns) FirstSSect);
}


void
MIFrd::SetSSectStruct(long i)
{
	if (i == 0)
		return;

	alevel *al = (alevel *) i;
	al->setval = (uns) FirstSSect;
}



void
MIFrd::MakeNumDef(long val, long typ, uns sty, char *txt)
{
  DCgroup *gp = new DCgroup(DCctl(group, 2, 100, ++AnumDefID));

  DCctl *cp = new DCctl(imnem, 2, 101, 2, typ);
  gp->add(*cp);

  if (sty < NumStyleMax) {
		DCgroup *fgp = new DCgroup(DCctl(group, 2, 101, 4));
		cp = new DCctl(imnem, 2, 102, NumStyles[sty], 1L);
		fgp->add(*cp);
		gp->add(*fgp);
	}

	if (sty > 4) {
		cp = new DCctl(ename, 2, 103, (uns) val);
		if (txt)
			cp->ex(txt, strlen(txt) + 1);
	}
	else
		cp = new DCctl(ilong, 2, 103, (uns) val, 1L);
  gp->add(*cp);

  GroupList[psg_anum]->add(*gp);
}




// footnote number processing


uns MIFrd::FirstFN;
uns MIFrd::FNStyle;
char MIFrd::FNName[FStringMax];
char MIFrd::TFNName[FStringMax];
uns MIFrd::FNReset;
uns MIFrd::FNHigh;
DCgroup *MIFrd::FNCharGroup = NULL;
uns MIFrd::FNBase;
char MIFrd::FNPre[FStringMax];
char MIFrd::FNSuf[FStringMax];
uns MIFrd::FNTBase;
char MIFrd::FNTPre[FStringMax];
char MIFrd::FNTSuf[FStringMax];


void
MIFrd::MakeFNProp()
{
  DCctl *cp = NULL;

  char *fnname = FNType ? TFNName : FNName;
  DCgroup *gp = new DCgroup(DCctl(group, 2, 70, FNType + 1));

  // make footnote property definition
  if (FormNameList.getnum(fnname))
    cp = new DCctl(ilong, 2, 72, 1, FormNameList.getnum(fnname));
  else if (FormTagNameList.getnum(fnname))
    cp = new DCctl(ilong, 2, 72, 1, FormTagNameList.getnum(fnname));
  else {
    cp = new DCctl(ename, 2, 72, 4);
    cp->ex(fnname, strlen(fnname) + 1);
  }
  gp->add(*cp);

  cp = new DCctl(ilong, 2, 72, 5, 0);
  if (!cp->siz(FrameNameList(fnname))) {
    RFrameRefList.add(cp, ++RFRefNum);
    RFrameNameList.addnew(fnname, RFRefNum);
  }
  gp->add(*cp);

  cp = new DCctl(ilong, 2, 72, 2, FNType + 2);
  gp->add(*cp);

  cp = new DCctl(ilong, 2, 72, 3, FNHigh);
  gp->add(*cp);

  GroupList[psg_fnote]->add(*gp);
}


void
MIFrd::MakeFNDef(long typ)
{
  // make footnote num definition
  DCgroup *gp = new DCgroup(DCctl(group, 2, 100, ++AnumDefID));

  DCctl *cp = new DCctl(imnem, 2, 101, 2, typ);
  gp->add(*cp);

  DCgroup *fgp = new DCgroup(DCctl(group, 2, 101, 4));
  MakeFNFormat(fgp, FNTBase, FNTPre, FNTSuf);
  gp->add(*fgp);

  fgp = new DCgroup(DCctl(group, 2, 101, 5));
  MakeFNFormat(fgp, FNBase, FNPre, FNSuf);
  gp->add(*fgp);

  cp = new DCctl(ilong, 2, 103, FirstFN, 1);
  gp->add(*cp);

  gp->add(*FNCharGroup);

  cp = new DCctl(imnem, 2, 101, 10, FNReset);
  gp->add(*cp);

  GroupList[psg_anum]->add(*gp);
}


void
MIFrd::MakeFNFormat(DCgroup *fgp, uns Base, char *Pre, char *Suf)
{
  DCctl *cp = NULL;

  GroupStack[++MIFlevel] = fgp;
  GenericTab = true;

  if (Base) {
    cp = new DCctl(imnem, 2, 15, 6, Base);
    fgp->add(*cp);
  }

  if (Pre && *Pre)
    ConvertFString(Pre);

  assert(FNStyle < NumStyleMax);
  cp = new DCctl(imnem, 2, 102, NumStyles[FNStyle], 1);
  fgp->add(*cp);

  if (Suf && *Suf)
    ConvertFString(Suf);

  GenericTab = false;
  --MIFlevel;
}


void
MIFrd::ConvertFString(char *p)
{
  char ch;

  AddText(0);
  for (; *p; p++) {
    if (*p == '\\') {
      ch = *++p;
      if (ch == '\\')  // doubled backslash
	      ch = *++p;
      if (ch == 't')
        AddText('\t');
      else if (ch == 'x')
        ProcHex(&p);
    }
		else if ((*p < 0)
		 && (MIFversion > 6)
		 && ::IsUTF8((unc *) p)) {
			AddText(::UTF8((unc **) &p));
			p--;
		}
    else AddText(*p);
  }
  AddText(0);
}


DCgroup *
MIFrd::ProcCharList(mif *mp)
{
  unc ch = 0;
	uns uch = 0;

  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, mp->dat));
  GroupStack[++MIFlevel] = gp;

  for (unc *chp = (unc *) MIFstring; *chp; chp++) {
    if ((ch = *chp) != '\\') {
			if ((ch > 0x7F)
			 && (MIFversion > 6)
			 && ::IsUTF8(chp)) {
				AddText(::UTF8(&chp));
				chp--;
			}
      else if (ch != ' ') {
        AddText(ch);
      }
      AddText(0);
    }
    else if ((ch = *++chp) == 'x') {
      ProcHex((char **) &chp);
      AddText(0);
    }
    // ignore other backslash sequences
  }
  --MIFlevel;
  return gp;
}



// end of drmifan.cc


