/* m2rsty.c is the FDK style handler for mif2rtf FM 5.5 export 
 * Last edited on 12/15/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/* globals */


DesStyleT *DesStyles = NULL;
IntT DesStyleCount = 0;
DesStyleT *CurrDesStyle = NULL;
DesStyleT FirstDesStyle;
DesStyleT *LastDesStyle = &FirstDesStyle;
DesStyleT ClipboardStyle;
DesStyleT EmptyStyle;
BoolT StylesChanged = False;

StringT *DesStyleList = NULL;
IntT DesListAlloc = 0;
IntT DesListCount = 0;
IntT CurrStyle = 0;
BoolT StyleListChanged = False;

BoolT ShowPara = True;
BoolT ShowChar = True;

F_StringsT StyleStrings;


/* style management */



VoidT InitDesignerStyles(VoidT)
{
	/* now add in the styles already in the .ini file */
	ReadStylesFromIni(CurrIni, True);
	InitStyleList();
}


VoidT InitStyleList(VoidT)
{
	DesStyleT *curr = NULL;

	if (DesStyleList
	 && (DesListAlloc < DesStyleCount)) {
		F_Free(DesStyleList);
		DesStyleList = NULL;
	}

	if (DesStyleList == NULL) {
		DesStyleList = (StringT *) F_Alloc((DesStyleCount + 1) * sizeof(StringT), NO_DSE);
		DesStyleList[0] = "Styles";
		DesListAlloc = DesStyleCount;
	}

	/* make list from appropriate subset of styles */
	DesListCount = 0;
	for (curr = DesStyles; curr; curr = (DesStyleT *) curr->next) {
		if ((ShowPara && curr->para)
		 || (ShowChar && !curr->para))
			DesStyleList[++DesListCount] = curr->name;
	}

	/* sort the DesStyleList entries 1..DesListCount alphabetically */
	if (DesListCount > 1)
		qsort(&DesStyleList[1], DesListCount, sizeof(StringT), CmpStrings);

	SetNewStyle(1);  /* put at start of list, sets CurrStyle and CurrDesStyle */
	StyleListChanged = False;
	StyleStrings.val = DesStyleList;
	StyleStrings.len = DesListCount + 1;

	/* set the global namelist entries to the new info */
	DesNameLists[DES_NAMES_STYLES].nameList = DesStyleList;
	DesNameLists[DES_NAMES_STYLES].nameCount = DesListCount;
	DesNameLists[DES_NAMES_STYLES].nameSel = CurrStyle;
	DesNameLists[DES_NAMES_STYLES].listLab = CurrDesStyle->para ? ParaDesLab : CharDesLab;
}


VoidT GetDocStyles(F_ObjHandleT docId, BoolT isPara)
{
	F_ObjHandleT compId = 0;
	StringT sname = NULL;
	StringT vname = NULL;
	StringT fname = NULL;
	IntT fnum = 0;
	DesStyleT *curr = NULL;

	compId = F_ApiGetId(FV_SessionId, docId, isPara ? FP_FirstPgfFmtInDoc :
																										FP_FirstCharFmtInDoc);

	while (compId) {
		sname = F_ApiGetString(docId, compId, FP_Name);
		if (InDesList(sname))
			F_StrFree(sname);
		else {
			curr = CreateDesignerStyle(sname);
			curr->para = isPara;
			if (isPara
			 || (F_ApiGetInt(docId, compId, FP_UseFontVariation)
			  && F_ApiGetInt(docId, compId, FP_UseFontFamily))) {
				vname = F_ApiGetString(docId, compId, FP_FontVariation);
				if (vname && *vname) {
					if (F_StrICmp(vname, "Regular")) {
						fnum = F_ApiGetInt(docId, compId, FP_FontFamily);
						fname = F_StrNew(F_StrLen(vname) + F_StrLen(FontFamilies.val[fnum]) + 1);
						F_StrCpy(fname, FontFamilies.val[fnum]);
						F_StrCat(fname, vname);
						AddDesFont(fname);
						F_StrFree(fname);
					}
					F_StrFree(vname);
				}
			}
			LastDesStyle->next = curr;
			LastDesStyle = curr;
			DesStyleCount++;
		}
		compId = F_ApiGetId(docId, compId, isPara ? FP_NextPgfFmtInDoc :
																								FP_NextCharFmtInDoc);
	}
}


BoolT InDesList(StringT sname)
{
	DesStyleT *item = FirstDesStyle.next;

	while (item) {
		if (!F_StrICmp(sname, item->name))
			return True;
		item = item->next;
	}
	return False;
}


IntT CmpStrings(const VoidT *p1, const VoidT *p2)
{
	return F_StrICmp(* (StringT *) p1, * (StringT *) p2);
}


DesStyleT *CreateDesignerStyle(StringT sname)
{
	DesStyleT *curr = NULL;

	curr = (DesStyleT *) F_Alloc(sizeof(DesStyleT), NO_DSE);
	if (curr) {
		F_ClearPtr(curr, sizeof(DesStyleT));
		curr->name = sname;
	}
	return curr;
}


VoidT FreeDesignerStyles(VoidT)
{
	DesStyleT *last = DesStyles;
	DesStyleT *curr = NULL;

	if (StylesChanged)
		WriteStylesToIni(CurrIni, True);

	while (last) {
		F_StrFree(last->name);

		// free all the other 10 strings too

		curr = (DesStyleT *) (last->next);
		F_Free(last);
		last = curr;
	}

	DesStyles = NULL;
	DesStyleCount = 0;
	CurrDesStyle = NULL;
	LastDesStyle = NULL;

	F_Free(DesStyleList);
	DesStyleList = NULL;
	DesListAlloc = 0;
	DesListCount = 0;
	CurrStyle = 0;
}


VoidT SetNewStyle(IntT stylenum)
{
	DesStyleT *curr = NULL;
	StringT sname = NULL;

	CurrStyle = stylenum;
	sname = DesStyleList[stylenum];
	curr = DesStyles;
	while (curr
	 && F_StrICmp(sname, curr->name))
		curr = (DesStyleT *) curr->next;
	if (curr)
		CurrDesStyle = curr;
}


DesStyleT *FindStyleInDesStyles(StringT sname)
{
	DesStyleT *curr = NULL;

	for (curr = DesStyles; curr; curr = (DesStyleT *) curr->next)
		if (!F_StrICmp(sname, curr->name))
			return curr;

	return NULL;
}


IntT FindStyleInList(StringT sname)
{
	IntT stylenum = 1;

	if (sname && *sname) {
		while (stylenum <= DesListCount) {
			if (!F_StrICmp(sname, DesStyleList[stylenum]))
				return stylenum;
			stylenum++;
		}
	}

	return -1;
}


/* read and write style info in .ini */

#define STY_SECT_COUNT 3

StringT StyleSections[STY_SECT_COUNT] = {
	"Styles",
	"StyleReplacements",
	"HelpStyles"
};


VoidT ReadStylesFromIni(StringT ininame, BoolT confirm)
{
	IntT i = 0;

	if (confirm
	 && StylesChanged
	 && (F_ApiAlert(DiscardChanges, FF_ALERT_NO_DEFAULT) == -1))
		return;

	TrueFalseIni(ininame, SubOptions, "RemoveUnusedStyles",
							 &CurrDesText.rem_unused_sty);

	for (i = 0; i < STY_SECT_COUNT; i++)
		ReadStylesFromIniSect(ininame, i);

	StyleListChanged = True;
	StylesChanged = False;
}


VoidT ReadStylesFromIniSect(StringT ininame, IntT sectnum)
{
	DesStyleT *curr = NULL;
	StringT section = StyleSections[sectnum];
	StringT sname = NULL;
	StringT props = NULL;
	StringT aname = NULL;


	/* scan DesStyles list for match, or add to end, and set props */
	GetIniSect(ininame, section);
	while ((sname = GetIniSectKey()) != NULL) {
		curr = FindStyleInDesStyles(sname);
		if (!curr) {
			curr = LastDesStyle;
			curr->next = CreateDesignerStyle(F_StrCopyString(sname));
			curr = (DesStyleT *) curr->next;
			curr->para = True;  /* we presume */
			curr->next = NULL;
			LastDesStyle = curr;
			DesStyleCount++;
		}
		aname = Fm2a(sname);
		GetIniString(ininame, section, aname, &props);
		F_Free(aname);
		GetIniStyleProps(ininame, sectnum, props, curr);
	}
}


#if 0
VoidT ParseIniSectLine(StringT *leftp, StringT *rightp, StringT *nextp)
{
	StringT ptr = *nextp;
	StringT delim = NULL;

	if (!ptr) {
		*leftp = NULL;
		*rightp = NULL;
		return;
	}

	if (*ptr == ';') {
		*leftp = NULL;
		*rightp = NULL;
		while (*ptr
				&& (*ptr != '\n'))
			ptr++;
		if (*ptr == '\0')
			*nextp = NULL;
		else
			*nextp = ++ptr;
		return;
	}

	while (*ptr
			&& (*ptr == ' '))
		ptr++;
	if (*ptr == '\0') {
		*leftp = NULL;
		*rightp = NULL;
		*nextp = NULL;
		return;
	}
	*leftp = ptr;

	while (*ptr
			&& (*ptr != '=')
			&& (*ptr != '\n'))
		ptr++;
	if (*ptr == '\0') {
		*leftp = NULL;
		*rightp = NULL;
		*nextp = NULL;
		return;
	}
	if (*ptr == '\n') {
		*leftp = NULL;
		*rightp = NULL;
		*nextp = ++ptr;
		return;
	}
	delim = ptr++;
	*delim = '\0';
	delim--;
	while ((delim > *nextp)
			&& (*delim == ' '))
		delim--;
	*++delim = '\0';

	while (*ptr
			&& (*ptr == ' '))
		ptr++;
	if (*ptr == '\0') {
		*rightp = NULL;
		*nextp = NULL;
		return;
	}
	*rightp = ptr;

	while (*ptr
			&& (*ptr != '\n'))
		ptr++;
	if (*ptr == '\0')
		*nextp = NULL;
	else if (*ptr == '\n')
		*nextp = ++ptr;
}
#endif

#define STY_START_COUNT 12

StringT HelpStyleStart[STY_START_COUNT] = {
 "Topic", "Resume",
 "JumpHot", "PopHot", "PopOver",
 "JumpTarget", "PopContent", "MacroHot",
 "Key", "AKey", "SpKey", "Window"
};

#define STY_HKEY_COUNT 21

StringT HelpStyleKeys[STY_HKEY_COUNT] = {
 "Browse", "Window", "Macro", "Build", "Contents",
 "Refer", "Suffix", "TitleSuf", "File", "SpKey",
 "Key", "AKey", "NoTitle", "Slide",
 "Scroll", "NoScroll", "XScroll", "NoXScroll",
 "Local", "Green", "Uline"
};

#define STY_HSECT_COUNT 10

StringT HelpStyleSects[STY_HSECT_COUNT] = {
 "HelpBrowsePrefixStyles",
 "HelpWindowStyles",
 "HelpMacroStyles",
 "HelpTopicBuildStyles",
 "HelpCntStyles",
 "HelpRefStyles",
 "HelpSuffixStyles",
 "HelpTitleSufStyles",
 "HelpJumpFileStyles",
 "HelpKeywordStyles"
};


VoidT GetIniStyleProps(StringT ininame, IntT sectnum, StringT props, DesStyleT *curr)
{
	DesStyleT *right;
	StringT pPr = NULL;
	StringT term = NULL;
	StringT pval = NULL;
	BoolT first = True;
	IntT i = 0;

	StringT aname = NULL;

	if (props == NULL)
		return;

	aname = Fm2a(curr->name);

	switch (sectnum) {
		case 0:  /* Styles */
			curr->remap = True;
			curr->rem_name = props;
			// add rem_name to list
			right = FindStyleInDesStyles(curr->rem_name);
			if (!right) {
				right = LastDesStyle;
				right->next = CreateDesignerStyle(F_StrCopyString(curr->rem_name));
				right = (DesStyleT *) right->next;
				right->next = NULL;
				LastDesStyle = right;
				DesStyleCount++;
			}
			// else error, should be new name
			right->para = curr->para;  /* we hope */
			break;

		case 1: /* StyleReplacements */
			curr->replace = True;
			curr->rep_name = props; 
			break;
	
		case 2: /* HelpStyles */
			pPr = props;
			term = props;
			first = True;
			curr->helptyp = 0;
			curr->ungreen = True;
			curr->nouline = True;
			while (*pPr) {
				while (*term
						&& (*term != ' '))
					term++;
				if (*term)
					*term++ = '\0';
				if (first) {
					for (i = 0; i < STY_START_COUNT; i++)
						if (!F_StrICmp(pPr, HelpStyleStart[i]))
							break;
					switch (i) {
						case 0:
							curr->helptyp = 1;
							curr->topictyp = 0;
							break;
						case 1:
							curr->helptyp = 1;
							curr->topictyp = 2;
							break;
						case 2:
							curr->helptyp = 2;
							curr->hotsptyp = 0;
							GetIniString(ininame, HelpStyleSects[5], aname,
													 &(curr->fixed_name));
							if (curr->fixed_name)
								curr->fixed = True;
							break;
						case 3:
							curr->helptyp = 2;
							curr->hotsptyp = 1;
							GetIniString(ininame, HelpStyleSects[5], aname,
													 &(curr->fixed_name));
							if (curr->fixed_name)
								curr->fixed = True;
							break;
						case 4:
							curr->helptyp = 2;
							curr->hotsptyp = 3;
							break;
						case 5:
							curr->helptyp = 3;
							break;
						case 6:
							curr->helptyp = 3;
							curr->pop_cont = True;
							break;
						case 7:
							curr->helptyp = 4;
							GetIniString(ininame, HelpStyleSects[2], aname,
													 &(curr->mac_name));
							break;
						case 8:
							curr->usekey = True;
							break;
						case 9:
							curr->akey = True;
							break;
						case 10:
							GetIniString(ininame, HelpStyleSects[9], aname,
													 &(curr->keys));
							break;
						case 11:
							curr->helptyp = 2;
							curr->hotsptyp = 2;
							curr->window = True;
							GetIniString(ininame, HelpStyleSects[1], aname,
													 &(curr->win_name));
							break;
						default:
							break;
					}
					first = False;
				}
				else {
					for (i = 0; i < STY_HKEY_COUNT; i++)
						if (!F_StrICmp(pPr, HelpStyleKeys[i]))
							break;
					if (i < STY_HSECT_COUNT)
						GetIniString(ininame, HelpStyleSects[i], aname, &pval);
					switch (i) {
						case 0:
							curr->browse = True;
							curr->br_px = pval;
							break;
						case 1:
							curr->window = True;
							curr->win_name = pval;
							break;
						case 2:
							curr->mac_name = pval;
							break;
						case 3:
							curr->build = True;
							curr->build_tag = pval;
							break;
						case 4:
							if (!pval)
								break;
							curr->cont = True;
							switch (*pval) {
								case 'V':
									curr->conttyp = 0;
									break;
								case 'H':
									curr->conttyp = 1;
									break;
								case 'T':
									curr->conttyp = 2;
									break;
								case 'B':
									curr->conttyp = 3;
									break;
								default:
									break;
							}
							curr->contlev = F_StrCopyString(pval + 1);
							F_StrFree(pval);
							break;
						case 5: // Refer (Topic only)
							curr->refer = True;
							break;
						case 6: // Suffix
							curr->refer = True;
							curr->ref_sx = pval;
							break;
						case 7:
							curr->titsx = True;
							curr->title_sx = pval;
							break;
						case 8:
							curr->fileref = True;
							curr->file_name = pval;
							break;
						case 9:
							curr->keys = pval;
							break;
						case 10:
							curr->usekey = True;
							break;
						case 11:
							curr->akey = True;
							break;
						case 12:
							curr->notitle = True;
							break;
						case 13:
							curr->topictyp = 1;
							break;
						case 14:
							curr->noscr_used = True;
							curr->noscroll = False;
							break;
						case 15:
							curr->noscr_used = True;
							curr->noscroll = True;
							break;
						case 16:
							curr->xtscr_used = True;
							curr->xtscroll = True;
							break;
						case 17:
							curr->xtscr_used = True;
							curr->xtscroll = False;
							break;
						case 18:
							curr->local = True;
							break;
						case 19:
							curr->ungreen = False;
							break;
						case 20:
							curr->nouline = False;
							break;
						default:
							break;
					}
				}
				pPr = term;
			}

			F_StrFree(props);
			break;

		default:
			F_StrFree(props);
			break;
	}

	F_Free(aname);
}


VoidT WriteStylesToIni(StringT ininame, BoolT confirm)
{
	DesStyleT *curr = NULL;
	UCharT helpStyle[MAX_PROFILE];
	StringT hsPtr = NULL;
	UCharT contType[3];
	UCharT contTypes[4] = { 'V', 'H', 'T', 'B' };
	StringT aname = NULL;
	StringT temp1;


	if (TextChanged)
		SetIniTF(ininame, SubOptions, "RemoveUnusedStyles", CurrDesText.rem_unused_sty);

	if (!StylesChanged)
		return;

	for (curr = DesStyles; curr; curr = (DesStyleT *) curr->next) {
		aname = Fm2a(curr->name);
		if (curr->remap) {
			temp1 = curr->rem_name;
			SetIniStr(ininame, StyleSections[0], aname, temp1);
			//F_Free(temp1);
		}
		else
			SetIniStr(ininame, StyleSections[0], aname, NULL);

		if (curr->remap) {
			temp1 = curr->rep_name;
			SetIniStr(ininame, StyleSections[1], aname, temp1);
			//F_Free(temp1);
		}
		else
			SetIniStr(ininame, StyleSections[1], aname, NULL);

		helpStyle[0] = '\0';
		hsPtr = NULL;
		switch (curr->helptyp) {
			case 1:
				if (curr->topictyp == 0)
					hsPtr = "Topic ";
				else if (curr->topictyp == 1)
					hsPtr = "Topic Slide ";
				else if (curr->topictyp == 2)
					hsPtr = "Resume ";
				break;
			case 2:
				if (curr->hotsptyp == 0)
					hsPtr = "JumpHot ";
				else if (curr->hotsptyp == 1)
					hsPtr = "PopHot ";
				else if (curr->hotsptyp == 2)
					hsPtr = "Window ";
				else if (curr->hotsptyp == 3)
					hsPtr = "PopOver ";
				break;
			case 3:
				hsPtr = curr->pop_cont ? "PopContent " : "JumpTarget ";
				break;
			case 4:
				hsPtr = "MacroHot ";				
				break;
			case 0:
			default:
				break;
		}
		if (hsPtr)
			F_StrCpy(helpStyle, hsPtr);

		if (curr->usekey)
			F_StrCat(helpStyle, "Key ");
		if (curr->akey)
			F_StrCat(helpStyle, "AKey ");
		if (curr->keys
		 && *curr->keys) {
			F_StrCat(helpStyle, "SpKey ");
			SetIniStr(ininame, "HelpKeywordStyles", aname, curr->keys);
		}

		if (curr->helptyp == 1) {
			if (curr->browse) {
				F_StrCat(helpStyle, "Browse ");
				SetIniStr(ininame, "HelpBrowsePrefixStyles", aname,
				                   (curr->br_px && *curr->br_px) ? curr->br_px : NULL);
			}
			if (curr->cont) {
				F_StrCat(helpStyle, "Contents ");
				contType[0] = contTypes[curr->conttyp];
				contType[1] = *curr->contlev;
				contType[2] = '\0';
				SetIniStr(ininame, "HelpCntStyles", aname, contType);
			}
			if (curr->mac_name
			 && *curr->mac_name) {
				F_StrCat(helpStyle, "Macro ");
				SetIniStr(ininame, "HelpMacroStyles", aname, curr->mac_name);
			}
			if (curr->build) {
				F_StrCat(helpStyle, "Build ");
				SetIniStr(ininame, "HelpTopicBuildStyles", aname, curr->build_tag);
			}
			if (curr->window) {
				F_StrCat(helpStyle, "Window ");
				SetIniStr(ininame, "HelpWindowStyles", aname, curr->win_name);
			}
			if (curr->notitle)
				F_StrCat(helpStyle, "NoTitle ");
			else if (curr->titsx) {
				F_StrCat(helpStyle, "TitleSuf ");
				SetIniStr(ininame, "HelpTitleSufStyles", aname, curr->title_sx);
			}
			if (curr->refer) {
				F_StrCat(helpStyle, "Refer ");
				if (curr->ref_sx
				 && *curr->ref_sx) {
					F_StrCat(helpStyle, "Suffix ");
					SetIniStr(ininame, "HelpSuffixStyles", aname, curr->ref_sx);
				}
			}
			if (curr->noscr_used)
				F_StrCat(helpStyle, curr->noscroll ? "NoScroll " : "Scroll ");
			if (curr->xtscr_used)
				F_StrCat(helpStyle, curr->xtscroll ? "XScroll " : "NoXScroll ");
		}
		else if (curr->helptyp == 2) {
			if (curr->window) {
				if (curr->hotsptyp != 2)
					F_StrCat(helpStyle, "Window ");
				SetIniStr(ininame, "HelpWindowStyles", aname, curr->win_name);
			}
			if (curr->fileref) {
				F_StrCat(helpStyle, "File ");
				SetIniStr(ininame, "HelpJumpFileStyles", aname, curr->file_name);
			}
			if (curr->local)
				F_StrCat(helpStyle, "Local ");
			if (curr->fixed)
				SetIniStr(ininame, "HelpRefStyles", aname, curr->fixed_name);
			else if (curr->ref_sx
						&& *curr->ref_sx) {
				F_StrCat(helpStyle, "Suffix ");
				SetIniStr(ininame, "HelpSuffixStyles", aname, curr->ref_sx);
			}
			if (!curr->ungreen)
				F_StrCat(helpStyle, "Green ");
			if (!curr->nouline)
				F_StrCat(helpStyle, "Uline ");
		}
		else if (curr->helptyp == 3) {
			if (curr->local)
				F_StrCat(helpStyle, "Local ");
			if (curr->refer) {
				F_StrCat(helpStyle, "Suffix ");
				SetIniStr(ininame, "HelpSuffixStyles", aname, curr->ref_sx);
			}
			if (curr->cont) {
				F_StrCat(helpStyle, "Contents ");
				contType[0] = contTypes[curr->conttyp];
				contType[1] = *curr->contlev;
				contType[2] = '\0';
				SetIniStr(ininame, "HelpCntStyles", aname, contType);
			}
		}
		else if (curr->helptyp == 4) {
			if (curr->mac_name
			 && *curr->mac_name)
				SetIniStr(ininame, "HelpMacroStyles", aname, curr->mac_name);
			if (!curr->ungreen)
				F_StrCat(helpStyle, "Green ");
			if (!curr->nouline)
				F_StrCat(helpStyle, "Uline ");
		}

		hsPtr = helpStyle + F_StrLen(helpStyle);
		if (*hsPtr == ' ')
			*hsPtr = '\0';
		SetIniStr(ininame, "HelpStyles", aname, *helpStyle ? helpStyle : NULL);
		F_Free(aname);
	}
	StylesChanged = False;
}



/* end of file m2rsty.c */



