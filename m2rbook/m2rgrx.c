/* m2rgrx.c is the graphics export FDK module for mif2rtf
 * Last edited on 12/25/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"

/* globals */

StringT CurrGrxFile = NULL;
StringT RefGrName = NULL;
StringT RefGrID = NULL;
StringT RefGrDpi = NULL;
StringT GrGenSect = "GrGen";
StringT GrDpiSect = "GrDpi";
StringT GrUseSect = "GrUse";


/* graphics export functions */


IntT ExportGraphics(F_ObjHandleT docId, StringT filebase, IntT grFormat, IntT grDPI, IntT grType)
{
	/* export graphic files in the specified format */
	IntT grnum = 0;

	StringT format = NULL;
	StringT suffix = NULL;
	StringT filename = NULL;
	StringT fname = NULL;
	StringT str = NULL;

	F_ObjHandleT graph = 0;
	F_ObjHandleT frame = 0;
	F_ObjHandleT first = 0;
	F_ObjHandleT last = 0;
	IntT unique = 0;
	IntT filepos = 0;
	IntT dpi = 96;
	IntT refdpi = 0;
	UIntT otype = 0;

	static IntT fcodes[] = {KBD_PUTINLINE};

	/* formats are 5 four-char groups, then a name up to 31 chars 
	 * groups are: 0001, vendor, format, platform, filter version 
   * vendor may be PGRF, FAPI, FFLT, IMAG, etc. 
	 * formats include WMF , GIF , DIB , TIFF, JPEG, PNG, etc.
	 * IMAG has 1st 4, but "DIB " is misset to "DIB" in maker.ini
	 * platforms are WINT, WIN3, WIN4, UNIX, etc.
	 * for GIF: 0001IMAGGIF WINT
	 */

	switch (grFormat + 1) {
		case 1: // disabled in protest of Unisys 
			// format = GetFormatHint("GIF", "IMAG");
			// suffix = "gif";
			break;
		case 2: 
			format = GetFormatHint("DIB", "IMAG");
			suffix = "bmp";
			break;
		case 3: 
			format = GetFormatHint("TIFF", "IMAG");
			suffix = "tif";
			break;
		case 4: 
			format = GetFormatHint("WMF", "IMAG");
			suffix = "wmf";
			break;
		case 5: 
			format = GetFormatHint("JPEG", "IMAG");
			suffix = "jpg";
			break;
		case 6: 
			format = GetFormatHint("PNG", "IMAG");
			suffix = "png";
			break;
		case 7: 
			format = GetFormatHint("EPS", "IMAG");
			suffix = "eps";
			break;
		case 8: 
			format = GetFormatHint("PICT", "IMAG");
			suffix = "pct";
			break;
		case 9: 
			format = GetFormatHint("CGM", "IMAG");
			suffix = "cgm";
			break;
		case 10: 
			format = GetFormatHint("IGES", "IMAG");
			suffix = "igs";
			break;
		case 11: // put back by popular demand 
			format = GetFormatHint("GIF", "IMAG");
			suffix = "gif";
			break;
		case 12: // added for FM 7.1
			format = GetFormatHint("SVG", "FRAM");
			suffix = "svg";
			break;
		case 13: // added for testing
			format = GetFormatHint("PDF", "FRAM");
			suffix = "pdf";
			break;

		default:
			//format = "0001IMAGJPEGWINT";
			//suffix = "jpg";
			//break;
			M2GLogEvent(logwarn, 1, "Unknown GraphicExportFormat",
			 NULL, NULL, NULL, NULL);
			return 0;
	}

	if (!format)
		return 0;
	filepos = F_StrLen(filebase);
	filename = F_StrNew(filepos + 14);
	F_StrCpy(filename, filebase);

	// graph = F_ApiGetUniqueObject(docId, FO_UnanchoredFrame, 172857);
	//  fails with FA_errno == FE_NameNotFound
	// graph = F_ApiGetNamedObject(docId, FO_UnanchoredFrame, "Head2");
	//  works, so ref frames are really not in graphics chain
	// str = F_ApiGetString(docId, graph, FP_Name);

	/* for each equation, get unique ID and make up filename */
	graph = F_ApiGetId(FV_SessionId, docId, FP_FirstGraphicInDoc);
	frame = 0;
	while (graph != 0) {
		// str = F_ApiGetString(docId, graph, FP_Name);
		FA_errno = FE_Success;
		dpi = grDPI;
		otype = F_ApiGetObjectType(docId, graph);
		unique = F_ApiGetInt(docId, graph, FP_Unique);
		unique &= GrFileNumMask;
		if (grType == GT_EQN) {  /* write only for eqns */
			if (otype == FO_Math) {
#if 0
			 && !IsHiddenFrame(docId, graph)  /* unless hidden */
#endif
				frame = F_ApiGetId(docId, graph, FP_FrameParent);
				otype = F_ApiGetObjectType(docId, frame);
				if (!IsNeededGraphic(docId, frame, otype))
					frame = 0;
			}
		}
		else if (grType == GT_ALL) { /* get all anchored frames */
		//if ((grType == GT_ALL) && !frame) { /* get all anchored frames */
			if (IsNeededGraphic(docId, graph, otype)) {
#if 0
			if (((otype == FO_AFrame) /* includes those for equations */
			  && !IsHiddenFrame(docId, graph)) /* unless hidden */
			 || ((otype == FO_UnanchoredFrame) /* include named ref frames */
			  && IsRefFrame(docId, graph)))
#endif
				frame = graph;
				first = F_ApiGetId(docId, frame, FP_FirstGraphicInFrame);
				if (first != 0) {
					last = F_ApiGetId(docId, frame, FP_LastGraphicInFrame);
					if ((first == last)  /* frame contains only one equation obj */
					 && (F_ApiGetObjectType(docId, first) == FO_Math))
						dpi = EqDpiValue;
					else while (first) {
						if (F_ApiGetObjectType(docId, first) == FO_Inset) {
							fname = F_ApiGetString(docId, first, FP_InsetFile);
							if (fname  /* ensure any referenced graphics are present */
							 && *fname) {
								if (!FileExists(fname)) {
									if (RefGrName) {
										M2GLogEvent(logwarn, 1, "Missing imported graphic ",
										 fname, " for ", RefGrName, NULL);
										str = F_StrNew(F_StrLen(RefGrName) + 3);
										F_StrCpy(str, RefGrName);
										F_StrCat(str, " *");
										SetRawIniStr(CurrGrxFile, GrGenSect, RefGrID, str);
										F_StrFree(str);
									}
									else {
										M2GLogEvent(logwarn, 1, "Missing imported graphic ",
										 fname, NULL, NULL, NULL);
									}
									F_StrFree(fname);
									frame = 0;
									break;
								}
								F_StrFree(fname);
							}
						}
						first = F_ApiGetId(docId, first, FP_NextGraphicInFrame);
					}
				}
			}
		}
		if (frame) {
			unique = F_ApiGetInt(docId, (PT_rtf(FormType) && WriteEqns) ? graph : frame, FP_Unique);
			unique &= GrFileNumMask;
			F_Sprintf(filename + filepos, "%0.*lx.%s", GrFileNumLen, unique, suffix);
			if ((GraphicsFirst || RefGrName)
#if 0
			 || (RefGrName
		  // check not needed because already handled in IsNeededGraphic
			  && !_stricmp(RefGrName, filename + F_StrLen(CurrDir))))
#endif
			 && (FA_errno == FE_Success)) {
				if (RefGrDpi
				 && *RefGrDpi
				 && ((refdpi = atoi(RefGrDpi)) >= 50)
				 && (refdpi <= 1200))
					dpi = refdpi;
				if (RefGrName && *RefGrName) {
					F_StrFree(filename);
					filename = F_StrNew(F_StrLen(CurrDir) + F_StrLen(RefGrName) + 2);
					F_StrCpy(filename, CurrDir);
					F_StrCat(filename, RefGrName);
				}
				ExportGraphic(docId, frame, filename, format, dpi);
				grnum++;
			}
			frame = 0;
		}
		graph = F_ApiGetId(docId, graph, FP_NextGraphicInDoc);
	}

	F_StrFree(filename);
	return grnum;
}


BoolT IsNeededGraphic(F_ObjHandleT docId, F_ObjHandleT graph, UIntT otype)
{
	F_ObjHandleT frame;
	F_ObjHandleT first;
	F_ObjHandleT last;
	F_ObjHandleT hidden;
	F_ObjHandleT hframe;
	StringT str = NULL;
	IntT unique = 0;

	if (RefGrName) {
		F_StrFree(RefGrName);
		RefGrName = NULL;
	}
	if (RefGrID) {
		F_StrFree(RefGrID);
		RefGrID = NULL;
	}

	if ((otype != FO_AFrame)
	 && (otype != FO_UnanchoredFrame)
	 && (otype != FO_TextFrame))
		return False;

	if (!GraphicsFirst) { /* if not in list, return False */
		unique = F_ApiGetInt(docId, graph, FP_Unique);
		RefGrID = F_StrNew(10);
		F_Sprintf(RefGrID, "%ld", unique);
		GetRawIniString(CurrGrxFile, GrGenSect, RefGrID, &RefGrName);
		if (RefGrName == NULL)
			return False;
		RefGrDpi = NULL;
		GetRawIniString(CurrGrxFile, GrDpiSect, RefGrID, &RefGrDpi);
		str = &RefGrName[F_StrLen(RefGrName) - 1];
		while (*str && (str > RefGrName)) {
			if ((*str != '*')
			 && (*str != ' '))
				break;
			str--;
		}
		*(str + 1) = '\0';  /* trim off any error indicator */
	}

	frame = F_ApiGetId(docId, graph, FP_FrameParent);

	/* always exclude hidden graphics */
	if (((hidden = F_ApiGetId(FV_SessionId, docId, FP_HiddenPage)) != 0)
	 && ((hframe = F_ApiGetId(docId, hidden, FP_PageFrame)) != 0)
	 && (frame == hframe))
		return False;

	if (otype == FO_AFrame)
		return True;

	/* must be a top-level graphic on its ref or master page */

	/* check all reference pages if excluding ref graphics */
	if (!WriteRefGraphics) {
		first = F_ApiGetId(FV_SessionId, docId, FP_FirstRefPageInDoc);
		last = F_ApiGetId(FV_SessionId, docId, FP_LastRefPageInDoc);
		while (first) {
			if (frame == F_ApiGetId(docId, first, FP_PageFrame))
				return False;
 			if (first == last)
				break;
			first = F_ApiGetId(docId, first, FP_PageNext);
		}
	}

	/* check all master pages if excluding master-page graphics */
	if (!WriteMpgGraphics) {
		first = F_ApiGetId(FV_SessionId, docId, FP_FirstMasterPageInDoc);
		last = F_ApiGetId(FV_SessionId, docId, FP_LastMasterPageInDoc);
		while (first) {
			if (frame == F_ApiGetId(docId, first, FP_PageFrame))
				return False;
			if (first == last)
				break;
			first = F_ApiGetId(docId, first, FP_PageNext);
		}
	}

	return True;  // go ahead and write it
}


#if 0

BoolT IsHiddenFrame(F_ObjHandleT docId, F_ObjHandleT graph)
{
	F_ObjHandleT frame;
	F_ObjHandleT hidden;

	frame = F_ApiGetId(docId, graph, FP_FrameParent);
	hidden = F_ApiGetId(FV_SessionId, docId, FP_HiddenPage);
	if (hidden
	 && (frame == F_ApiGetId(docId, hidden, FP_PageFrame)))
		return True;
	return False;
}


BoolT IsRefFrame(F_ObjHandleT docId, F_ObjHandleT graph)
{
	F_ObjHandleT frame;
	F_ObjHandleT first;
	F_ObjHandleT last;

	StringT fname;

	/* must be a top-level graphic on its ref or master page */
	frame = F_ApiGetId(docId, graph, FP_FrameParent);

	/* check all reference pages */
	if (WriteRefGraphics) {
		first = F_ApiGetId(FV_SessionId, docId, FP_FirstRefPageInDoc);
		last = F_ApiGetId(FV_SessionId, docId, FP_LastRefPageInDoc);
		while (first) {
			if (frame == F_ApiGetId(docId, first, FP_PageFrame)) {
				if (((fname = F_ApiGetString(docId, graph, FP_Name)) == NULL)
				 || (*fname == '\0'))
					return False;  /* on ref page but not named, unusable */
				return True;
			}
			if (first == last)
				break;
			first = F_ApiGetId(docId, first, FP_PageNext);
		}
	}

	/* check all master pages */
	if (WriteMpgGraphics) {
		first = F_ApiGetId(FV_SessionId, docId, FP_FirstMasterPageInDoc);
		last = F_ApiGetId(FV_SessionId, docId, FP_LastMasterPageInDoc);
		while (first) {
			if (frame == F_ApiGetId(docId, first, FP_PageFrame))
				return True;
			if (first == last)
				break;
			first = F_ApiGetId(docId, first, FP_PageNext);
		}
	}

	return False;
}



VoidT ScanAFrames(F_ObjHandleT docId)
{
	F_ObjHandleT frame;
	F_ObjHandleT graph;
	F_ObjHandleT last;
	IntT objType;

	frame = F_ApiGetId(FV_SessionId, docId, FP_FirstGraphicInDoc);
	while (frame != 0) {
		if (F_ApiGetObjectType(docId, frame) == FO_AFrame) {
			graph = F_ApiGetId(docId, frame, FP_FirstGraphicInFrame);
			last = F_ApiGetId(docId, frame, FP_LastGraphicInFrame);
			if (graph == last) {
				if ((objType = F_ApiGetObjectType(docId, graph)) == FO_Inset)
					ProcInset(docId, graph, True);
				else if (objType == FO_Math)
					EquationsFound = True;
			}
			else { /* more than one object in frame */
				while (graph != 0) {
					if ((objType = F_ApiGetObjectType(docId, graph)) == FO_Inset)
						ProcInset(docId, graph, False);
					else if (objType == FO_Math)
						EquationsFound = True;
					graph = F_ApiGetId(docId, graph, FP_NextGraphicInFrame);
				}
			}
		}
		frame = F_ApiGetId(docId, frame, FP_NextGraphicInDoc);
	}
}


VoidT ProcInset(F_ObjHandleT docId, F_ObjHandleT inset, BoolT Single)
{
	StringT hint = NULL;
	StringT filename = NULL;
	IntT unique = 0;

	hint = F_ApiGetString(docId, inset, FP_ImportHint);
	format = F_StrNew(5);
	F_StrCpyN(format, hint + 8, 5);
	if (!F_StrICmp(format, "DIB ")
	 || !F_StrICmp(format, "WMF "))
		; /* readable, nothing to do */
	else if (Single
	 && ((FormType < 4)  /* Word */
	  || ((FormType == 5)
	   && !F_StrICmp(format, "GIF "))))
		; /* let Word or HCW handle it */
	else { /* not filter readable, make .bmp */
		filename = F_ApiGetString(docId, inset, FP_InsetFile);
		// if copied in, = "", so figure out filter's name for export
		if ((filename == NULL)
		 || (*filename == '\0')) {
			unique = F_ApiGetInt(docId, inset, FP_Unique);
			bmpname = F_StrNew(13);
			F_Sprintf(bmpname, "%0.8lx.bmp", unique);
			if (filename)
				F_StrFree(filename);
		}
		else { // imported by reference
			unique = 0;
			bmpname = NewFileExt(filename, ".bmp");

		// [GraphFiles] filename = bmpname, with forward slashes in pathnames
		// stored only once in list, later filter converts those listed
		// or just add tif=bmp, filter converts all of type
		// or just have dwrtf look for .bmp or .wmf and use it if found

		}
		ExportGraphic(docId, inset, bmpname, GetFormatHint("DIB", "IMAG"), GrDpiValue);
		if (unique)
			F_StrFree(bmpname);
	}
	F_StrFree(hint);
	F_StrFree(format);
}

#endif


StringT GetFormatHint(StringT oldFormat, StringT oldVendor)
{
#if 1
// was ifndef FM50VER
#define FORMLEN 4
	UCharT format[FORMLEN + 1];
	UCharT vendor[FORMLEN + 1];
	StringT newFormat = NULL;
	StringT newVendor = NULL;
	UIntT i;
	F_StringsT exportFilters;
	BoolT match = False;
	BoolT formMatch = False;
	UIntT formMatchVal = 0;
#endif
	StringT result = NULL;

	if (!oldFormat)
		oldFormat = "GIF ";

#if 0
// was ifdef FM50VER
	result = F_StrCopyString(oldFormat);
#else
	exportFilters = F_ApiGetStrings(FV_SessionId, FV_SessionId, FP_ExportFilters);
	
	if (exportFilters.len == 0) {
		M2GLogEvent(logwarn, 1, "Graphic export filters missing",
		 NULL, NULL, NULL, NULL);
		return NULL;
	}

	F_StrCpyN(format, oldFormat, FORMLEN + 1);
	for (i = F_StrLen(format); i < FORMLEN; ++i)
		format[i] = ' ';
	format[FORMLEN] = '\0';

	if (!oldVendor
	 || (F_StrLen(oldVendor) != 4))
		vendor[0] = 0;
	else
		F_StrCpyN(vendor, oldVendor, FORMLEN + 1);

	for (i = 0; i < exportFilters.len; ++i)	{
		newFormat = F_GetValueFromHint(exportFilters.val[i], FV_Hint_FormatId);
		if ((match = F_StrIEqual(format, newFormat)) == True) {
			formMatch = True;
			formMatchVal = i;
		}
		F_Free(newFormat);
		if (match && oldVendor)	{
			newVendor = F_GetValueFromHint(exportFilters.val[i], FV_Hint_VendorId);
			match = F_StrIEqual(vendor, newVendor);
			F_Free(newVendor);
		}
		if (match) {
			result = F_StrCopyString(exportFilters.val[i]);
			break;
		}
	}

	if (!match) {
		if (formMatch)
			result = F_StrCopyString(exportFilters.val[formMatchVal]);
		else {
			M2GLogEvent(logwarn, 1, "Graphic export filter not listed, using ",
			 format, NULL, NULL, NULL);
			result = F_StrCopyString(format);
		}
	}

	F_ApiDeallocateStrings(&exportFilters);
#endif

	return(result);
}


VoidT ExportGraphic(F_ObjHandleT docId, F_ObjHandleT graphic,
										 StringT fileName, StringT format, IntT grDPI)
{
	F_PropValsT script;
	F_PropValsT *returnp = NULL;
	StringT str = NULL;
	IntT i = 0;

	FA_errno = FE_Success;

	script = F_ApiGetExportDefaultParams();
	if (script.len == 0) {
		M2GLogEvent(logwarn, 1, "Graphic export not supported",
		 NULL, NULL, NULL, NULL);
		return;
	}

	i = F_ApiGetPropIndex(&script, FS_ExportObjectId);
	script.val[i].propVal.u.ival = graphic;
	i = F_ApiGetPropIndex(&script, FS_GraphicDpi);
	script.val[i].propVal.u.ival = grDPI;
#if 0
// was ifdef FM50VER
	i = F_ApiGetPropIndex(&script, FS_ExportFileFormat);
#else
	i = F_ApiGetPropIndex(&script, FS_ExportFileTypeHint);
#endif
	script.val[i].propVal.u.sval = F_StrCopyString(format);

	returnp = NULL;

	M2GLogEvent(loginfo, 2, "Started graphic export for ",
	 fileName, NULL, NULL, NULL);

	F_ApiExport(docId, fileName, &script, &returnp);

	if (FA_errno != FE_Success)	{
		str = F_StrNew(36);
		str = _itoa(FA_errno, str, 10);
		M2GLogEvent(logwarn, 1, "Graphic export failed for ",
		 fileName, ", error ", str, NULL);
		F_StrFree(str);
		str = NULL;
		F_Printf(NULL, EqnFailMsg, fileName, format);
		F_ApiPrintExportStatus(returnp);
		F_Printf(NULL, "\n");

		//F_ApiAlert(EqnFailure, FF_ALERT_CONTINUE_NOTE);

		if (RefGrName) {
			str = F_StrNew(F_StrLen(RefGrName + 4));
			F_StrCpy(str, RefGrName);
			F_StrCat(str, " **");
			SetRawIniStr(CurrGrxFile, GrGenSect, RefGrID, str);
			//F_StrFree(str);
		}
	}

	F_ApiDeallocatePropVals(&script);
	F_ApiDeallocatePropVals(returnp);
}



/* End of m2rgrx.c */

