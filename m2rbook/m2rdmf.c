/* m2rdmf.c is the FDK designer panel for MultiFile
 * Last edited on 1/8/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


CompFileT *CurrCompFile;


CompFileT *DesCompFiles;
CompFileT *CurrCompFile;
CompFileT DefCompFile = {
	"", False, False, False, False, 
	NULL, False, NULL, False, NULL, False, NULL, False, NULL, False, NULL,
	NULL 
};
CompFileT FirstCompFile = {
	"", False, False, False, False, 
	NULL, False, NULL, False, NULL, False, NULL, False, NULL, False, NULL,
	NULL 
};
CompFileT *LastCompFile = &FirstCompFile;
CompFileT ClipboardCompFile;
BoolT CompFileChanged = False;

StringT *DesCompFileList = NULL;
IntT CurrCompFileNum = 0;
IntT DesCompFileCount = 0;
IntT DesCompFileAlloc = 0;
BoolT CompFileListChanged = False;

F_StringsT CompFileStrings;

CompFileT *LastDefCompFile = NULL;


VoidT InitDesignerCompFiles(VoidT)
{
	ReadCompFilesFromIni(CurrIni, True);	
	InitCompFileList();
}


VoidT GetBookCompFiles(F_ObjHandleT bookId)
{
	F_ObjHandleT compId = 0;

	StringT chap = NULL;
	StringT basename = NULL;
	IntT bctype = FV_BK_NOT_GENERATABLE;
	StringT px = NULL;

	compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
	while (compId) {
		chap = F_ApiGetString(bookId, compId, FP_Name);
		basename = BaseFileName(chap);
		F_StrFree(chap);
		AddCompFile(basename);

		bctype = F_ApiGetInt(bookId, compId, FP_BookComponentType);
		if (bctype == FV_BK_TOC)
			LastCompFile->is_TOC = True;
		else if (bctype == FV_BK_INDEX_STAN)
			LastCompFile->is_IX = True;

		px = F_ApiGetString(bookId, compId, FP_PagePrefix);
		if (px && *px)
			LastCompFile->word_sect = px;

		compId = F_ApiGetId(bookId, compId, FP_NextComponentInBook);
	}
}


VoidT GetDocCompFile(F_ObjHandleT docId)
{
	StringT chap;
	StringT basename;

	chap = F_ApiGetString(FV_SessionId, docId, FP_Name);
	basename = BaseFileName(chap);
	F_StrFree(chap);
	AddCompFile(basename);
}


VoidT AddCompFile(StringT sname)
{
	CompFileT *curr = NULL;

	curr = (CompFileT *) F_Alloc(sizeof(CompFileT), NO_DSE);
	if (curr) {
		F_ClearPtr(curr, sizeof(CompFileT));
		curr->name = sname;
	}
	LastCompFile->next = curr;
	LastCompFile = curr;
	DesCompFileCount++;
}


BoolT InCompFileList(StringT sname)
{
	CompFileT *item = FirstCompFile.next;

	while (item) {
		if (!F_StrICmp(sname, item->name))
			return True;
		item = item->next;
	}
	return False;
}


VoidT InitCompFileList(VoidT)
{
	IntT i = 0;
	CompFileT *curr = NULL;

	if (DesCompFileList
	 && (DesCompFileAlloc < DesCompFileCount)) {
		F_Free(DesCompFileList);
		DesCompFileList = NULL;
	}

	if (DesCompFileList == NULL) {
		DesCompFileList = (StringT *) F_Alloc((DesCompFileCount + 1) * sizeof(StringT), NO_DSE);
		DesCompFileList[0] = "Files";
		DesCompFileAlloc = DesCompFileCount;
	}

	/* make list from files */
	curr = DesCompFiles;
	i = 0;
	while (curr) {
		DesCompFileList[++i] = curr->name;
		curr = (CompFileT *) curr->next;
	}

	/* sort the DesCompFileList entries 1..DesCompFileCount alphabetically */
	if (DesCompFileCount > 1)
		qsort(&DesCompFileList[1], DesCompFileCount, sizeof(StringT), CmpStrings);

	SetNewCompFile(1);  /* put at start of list, sets CurrCompFile and CurrCompFileNum */
	CompFileListChanged = False;

	/* set the global namelist entries to the new info */
	DesNameLists[DES_NAMES_FILES].nameList = DesCompFileList;
	DesNameLists[DES_NAMES_FILES].nameCount = DesCompFileCount;
	DesNameLists[DES_NAMES_FILES].nameSel = CurrCompFileNum;
}


VoidT SetNewCompFile(IntT filenum)
{
	CompFileT *curr = NULL;
	StringT sname = NULL;

	CurrCompFileNum = filenum;
	sname = DesCompFileList[filenum];
	curr = DesCompFiles;
	while (curr
	 && F_StrICmp(sname, curr->name))
		curr = (CompFileT *) curr->next;
	if (curr)
		CurrCompFile = curr;
}


VoidT FreeDesignerCompFiles(VoidT)
{
	CompFileT *last = DesCompFiles;
	CompFileT *curr = NULL;

	if (CompFileChanged)
		WriteCompFilesToIni(CurrIni, True);

	while (last) {
		F_StrFree(last->name);

		// free all the other 10 strings too

		curr = (CompFileT *) (last->next);
		F_Free(last);
		last = curr;
	}

	DesCompFiles = NULL;
	DesCompFileCount = 0;
	CurrCompFile = NULL;
	FirstCompFile.next = NULL;

	F_Free(DesCompFileList);
	DesCompFileList = NULL;
	CurrCompFileNum = 0;
}


CompFileT *FindCompFileInDesCompFiles(StringT sname)
{
	CompFileT *curr = NULL;

	for (curr = DesCompFiles; curr; curr = (CompFileT *) curr->next)
		if (!F_StrICmp(sname, curr->name))
			return curr;

	return NULL;
}


IntT FindCompFileInList(StringT sname)
{
	IntT filenum = 1;

	if (sname && *sname) {
		while (filenum <= DesCompFileCount) {
			if (!F_StrICmp(sname, DesCompFileList[filenum]))
				return filenum;
			filenum++;
		}
	}

	return -1;
}


#define FILE_SECT_COUNT 7

StringT CompFileSections[FILE_SECT_COUNT] = {
	"FileIDs",
	"WordSectionFiles",
	"HelpXrefFiles",
	"BctFileHeads",
	"BrowseStart",
	"BrowsePrefix",
	"FILES"   // keep as last, really in HPJ
};


VoidT ReadCompFilesFromIni(StringT ininame, BoolT confirm)
{
	IntT i = 0;
	StringT sname = NULL;
	StringT rtfname = NULL;
	StringT hpjname = NULL;
	CompFileT *curr = NULL;

	if (confirm
	 && CompFileChanged
	 && (F_ApiAlert(DiscardChanges, FF_ALERT_NO_DEFAULT) == -1))
		return;

	TrueFalseIni(ininame, SubOptions, "RemoveUnusedStyles",
							 &CurrDesText.rem_unused_sty);

	for (i = 0; i < FILE_SECT_COUNT - 1; i++)
		ReadCompFilesFromIniSect(ininame, i);

	/* last sect is in HPJ */
	hpjname = GetHpjName(ininame);
	GetIniSect(hpjname, CompFileSections[i]);
	while ((sname = GetIniSectKey()) != NULL) {
		rtfname = sname;
		sname = BaseFileName(rtfname);
		F_StrFree(rtfname);
		curr = FindCompFileInDesCompFiles(sname);
		if (!curr) {
			AddCompFile(F_StrCopyString(sname));
			curr = LastCompFile;
		}
		curr->compile = True;
	}

	CompFileListChanged = True;
	CompFileChanged = False;
}


VoidT ReadCompFilesFromIniSect(StringT ininame, IntT sectnum)
{
	CompFileT *curr = NULL;
	StringT section = CompFileSections[sectnum];
	StringT sname = NULL;

	/* scan DesCompFiles list for match, or add to end, and set props */
	GetIniSect(ininame, section);
	while ((sname = GetIniSectKey()) != NULL) {
		curr = FindCompFileInDesCompFiles(sname);
		if (!curr) {
			AddCompFile(F_StrCopyString(sname));
			curr = LastCompFile;
		}
		switch (sectnum) {
			case 0: // FileIDs
				GetIniString(ininame, section, sname, &curr->fileID);
				if (!IniBufEmpty())
					curr->fileID_set = True;
				break;
			case 1: // WordSectionFiles
				GetIniString(ininame, section, sname, &curr->word_sect);
				if (!IniBufEmpty())
					curr->word_sect_set = True;
				break;
			case 2: // HelpXrefFiles
				GetIniString(ininame, section, sname, &curr->dest_hlp);
				if (!IniBufEmpty())
					curr->dest_hlp_set = True;
				break;
			case 3: // BctFileHeads
				GetIniString(ininame, section, sname, &curr->bct_first);
				if (!IniBufEmpty())
					curr->bct_first_set = True;
				break;
			case 4: // BrowseStart
				GetIniString(ininame, section, sname, &curr->browse_start);
				if (!IniBufEmpty())
					curr->browse_start_set = True;
				break;
			case 5: // BrowsePrefix
				GetIniString(ininame, section, sname, &curr->browse_px);
				if (!IniBufEmpty())
					curr->browse_px_set = True;
				break;
			default:
				break;
		}
	}
}


VoidT WriteCompFilesToIni(StringT ininame, BoolT confirm)
{
	CompFileT *curr = NULL;
	StringT hpjname = NULL;
	StringT rtfname = NULL;
	StringT basehlp = NULL;

	if (!CompFileChanged)
		return;

	hpjname = GetHpjName(ininame);

	for (curr = DesCompFiles; curr; curr = (CompFileT *) curr->next) {
		SetIniStr(ininame, CompFileSections[0], curr->name,
											 curr->fileID_set ? curr->fileID : NULL);
		SetIniStr(ininame, CompFileSections[1], curr->name,
											 curr->word_sect_set ? curr->word_sect : NULL);
		SetIniStr(ininame, CompFileSections[2], curr->name,
											 curr->dest_hlp_set ? curr->dest_hlp : NULL);
		SetIniStr(ininame, CompFileSections[3], curr->name,
											 curr->bct_first_set ? curr->bct_first : NULL);
		SetIniStr(ininame, CompFileSections[4], curr->name,
											 curr->browse_start_set ? curr->browse_start : NULL);
		SetIniStr(ininame, CompFileSections[5], curr->name,
											 curr->browse_px_set ? curr->browse_px : NULL);

		if (curr->is_TOC)
			curr->compile = CurrDesText.use_fm_toc;
		else if (curr->is_IX)
			curr->compile = CurrDesText.use_fm_ix;
		else if (curr->dest_hlp_set
		 && CurrDesText.hlp_name
		 && *CurrDesText.hlp_name) {
			basehlp = BaseFileName(CurrDesText.hlp_name);
			if (!F_StrICmp(curr->dest_hlp, basehlp))
				curr->compile = True;
			F_StrFree(basehlp);
		}
		else
			curr->compile = False;

		rtfname = F_StrNew(F_StrLen(curr->name) + 9);
		F_Sprintf(rtfname, ".\\%s.rtf ;", (StringT) curr->name);
		SetIniStr(hpjname, CompFileSections[FILE_SECT_COUNT - 1], rtfname,
											 curr->compile ? "" : NULL);
		F_StrFree(rtfname);
	}
}


StringT GetHpjName(StringT defname)
{
	if (CurrDesText.hpj_name
	 && *CurrDesText.hpj_name
	 && FileExists(CurrDesText.hpj_name))
		return CurrDesText.hpj_name;
	return defname;
}


/**************************************************************/
/* MultiFile designer page */


/* m2rdmf items */

#define DLG_DMF_FID_CK  18
#define DLG_DMF_FID_BOX 19
#define DLG_DMF_SEC_CK  20
#define DLG_DMF_SEC_BOX 21
#define DLG_DMF_BRS_CK  22
#define DLG_DMF_BRS_BOX 23
#define DLG_DMF_BRP_CK  24
#define DLG_DMF_BRP_BOX 25
#define DLG_DMF_HLP_CK  26
#define DLG_DMF_HLP_BOX 27
#define DLG_DMF_BCT_CK  28
#define DLG_DMF_BCT_BOX 29

#define DLG_DMF_FID_USE 32
#define DLG_DMF_FID_GR  33

#define DLG_DMF_TOC_USE 35
#define DLG_DMF_IX_USE  36

#define DLG_DMF_IDN_RAD 38
#define DLG_DMF_IDR_RAD 39
#define DLG_DMF_IDA_RAD 40

#define DLG_DMF_HLP_DEF 42
#define DLG_DMF_HLP_SUF 44




/* MultiFile dialog events */

VoidT ProcMultiFileDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DMF_FID_CK:
			EnableDlgItem(dlgId, DLG_DMF_FID_BOX, GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DMF_FID_BOX:
			DesignerChanged = True;
			break;

		case DLG_DMF_SEC_CK:
			EnableDlgItem(dlgId, DLG_DMF_SEC_BOX, GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DMF_SEC_BOX:
			DesignerChanged = True;
			break;

		case DLG_DMF_BRS_CK:
			EnableDlgItem(dlgId, DLG_DMF_BRS_BOX, GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DMF_BRS_BOX:
			DesignerChanged = True;
			break;

		case DLG_DMF_BRP_CK:
			EnableDlgItem(dlgId, DLG_DMF_BRP_BOX, GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DMF_BRP_BOX:
			DesignerChanged = True;
			break;

		case DLG_DMF_HLP_CK:
			EnableDlgItem(dlgId, DLG_DMF_HLP_BOX, GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DMF_HLP_BOX:
			DesignerChanged = True;
			break;

		case DLG_DMF_BCT_CK:
			EnableDlgItem(dlgId, DLG_DMF_BCT_BOX, GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DMF_BCT_BOX:
			DesignerChanged = True;
			break;

		case DLG_DMF_FID_USE:
		case DLG_DMF_FID_GR:
			DesignerChanged = True;
			break;

		case DLG_DMF_TOC_USE:
		case DLG_DMF_IX_USE:
			DesignerChanged = True;
			break;

		case DLG_DMF_IDN_RAD:
		case DLG_DMF_IDR_RAD:
		case DLG_DMF_IDA_RAD:
			DesignerChanged = True;
			break;

		case DLG_DMF_HLP_DEF:
		case DLG_DMF_HLP_SUF:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetMultiFileDesigner(F_ObjHandleT dlgId)
{
	EnableItems(dlgId, DLG_DMF_SEC_CK, DLG_DMF_SEC_BOX, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableItems(dlgId, DLG_DMF_BRS_CK, DLG_DMF_HLP_SUF, (CurrPropSetNum == DES_PROPSET_HELP));
	EnableDlgItem(dlgId, DLG_DMF_FID_BOX, CurrCompFile->fileID_set);
	if (CurrPropSetNum == DES_PROPSET_WORD)
		EnableDlgItem(dlgId, DLG_DMF_SEC_BOX, CurrCompFile->word_sect_set);
	if (CurrPropSetNum == DES_PROPSET_HELP) {
		EnableDlgItem(dlgId, DLG_DMF_BRS_BOX, CurrCompFile->browse_start_set);
		EnableDlgItem(dlgId, DLG_DMF_BRP_BOX, CurrCompFile->browse_px_set);
		EnableDlgItem(dlgId, DLG_DMF_HLP_BOX, CurrCompFile->dest_hlp_set);
		if (CurrDesText.cnt_type == 1)
			EnableDlgItem(dlgId, DLG_DMF_BCT_BOX, CurrCompFile->bct_first_set);
		else
			EnableItems(dlgId, DLG_DMF_BCT_CK, DLG_DMF_BCT_BOX, False);
	}

	SetDlgState(dlgId, DLG_DMF_FID_CK, CurrCompFile->fileID_set);
	SetDlgText(dlgId, DLG_DMF_FID_BOX, CurrCompFile->fileID);

	SetDlgState(dlgId, DLG_DMF_SEC_CK, CurrCompFile->word_sect_set);
	SetDlgText(dlgId, DLG_DMF_SEC_BOX, CurrCompFile->word_sect);

	SetDlgState(dlgId, DLG_DMF_BRS_CK, CurrCompFile->browse_start_set);
	SetDlgText(dlgId, DLG_DMF_BRS_BOX, CurrCompFile->browse_start);

	SetDlgState(dlgId, DLG_DMF_BRP_CK, CurrCompFile->browse_px_set);
	SetDlgText(dlgId, DLG_DMF_BRP_BOX, CurrCompFile->browse_px);

	SetDlgState(dlgId, DLG_DMF_HLP_CK, CurrCompFile->dest_hlp_set);
	SetDlgText(dlgId, DLG_DMF_HLP_BOX, CurrCompFile->dest_hlp);

	SetDlgState(dlgId, DLG_DMF_BCT_CK, CurrCompFile->bct_first_set);
	SetDlgText(dlgId, DLG_DMF_BCT_BOX, CurrCompFile->bct_first);

	SetDlgState(dlgId, DLG_DMF_FID_USE, CurrDesText.use_file_ids);
	SetDlgState(dlgId, DLG_DMF_FID_GR, CurrDesText.use_graph_fids);

	SetDlgState(dlgId, DLG_DMF_TOC_USE, CurrDesText.use_fm_toc);
	SetDlgState(dlgId, DLG_DMF_IX_USE, CurrDesText.use_fm_ix);

	SetDlgState(dlgId, DLG_DMF_IDN_RAD + CurrDesText.object_ids, True);

	SetDlgText(dlgId, DLG_DMF_HLP_DEF, CurrDesText.hlp_xref_default);
	SetDlgText(dlgId, DLG_DMF_HLP_SUF, CurrDesText.hlp_xref_suf);

	DesignerChanged = False;
}


VoidT GetMultiFileDesigner(F_ObjHandleT dlgId)
{
	CurrCompFile->fileID_set = GetDlgState(dlgId, DLG_DMF_FID_CK);
	CurrCompFile->fileID = GetDlgText(dlgId, DLG_DMF_FID_BOX);

	CurrCompFile->word_sect_set = GetDlgState(dlgId, DLG_DMF_SEC_CK);
	CurrCompFile->word_sect = GetDlgText(dlgId, DLG_DMF_SEC_BOX);

	CurrCompFile->browse_start_set = GetDlgState(dlgId, DLG_DMF_BRS_CK);
	CurrCompFile->browse_start = GetDlgText(dlgId, DLG_DMF_BRS_BOX);

	CurrCompFile->browse_px_set = GetDlgState(dlgId, DLG_DMF_BRP_CK);
	CurrCompFile->browse_px = GetDlgText(dlgId, DLG_DMF_BRP_BOX);

	CurrCompFile->dest_hlp_set = GetDlgState(dlgId, DLG_DMF_HLP_CK);
	CurrCompFile->dest_hlp = GetDlgText(dlgId, DLG_DMF_HLP_BOX);

	CurrCompFile->bct_first_set = GetDlgState(dlgId, DLG_DMF_BCT_CK);
	CurrCompFile->bct_first = GetDlgText(dlgId, DLG_DMF_BCT_BOX);

	CurrDesText.use_file_ids = GetDlgState(dlgId, DLG_DMF_FID_USE);
	CurrDesText.use_graph_fids = GetDlgState(dlgId, DLG_DMF_FID_GR);

	CurrDesText.use_fm_toc = GetDlgState(dlgId, DLG_DMF_TOC_USE);
	CurrDesText.use_fm_ix = GetDlgState(dlgId, DLG_DMF_IX_USE);

	CurrDesText.object_ids = GetRadValue(dlgId, DLG_DMF_IDN_RAD,
																							DLG_DMF_IDA_RAD);

	CurrDesText.hlp_xref_default = GetDlgText(dlgId, DLG_DMF_HLP_DEF);
	CurrDesText.hlp_xref_suf = GetDlgText(dlgId, DLG_DMF_HLP_SUF);

	TextChanged = True;
	CompFileChanged = True;
	DesignerChanged = False;
}


VoidT FreeMultiFileDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdmf.c */

