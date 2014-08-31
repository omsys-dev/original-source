/* m2rstr.c has the localization strings for mif2rtf FM 5.5 FDK
 * Last edited on 11/30/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* for m2rbook.c */

StringT SaveFileStr = "Save Using Mif2Go...";
StringT SetupFileStr = "Set Up Mif2Go Export...";
StringT ConvDesStr = "Conversion Designer...";
StringT WashFileStr = "Wash Via MIF";
StringT WashAllFileStr = "Wash All Files in Book Via MIF";

StringT DestExt = NULL;
StringT RtfExt = ".rtf";
StringT DocExt = ".doc";
StringT HtmExt = ".htm";
StringT HtmlExt = ".html";
StringT XmlExt = ".xml";
StringT DITAExt = ".dita";
StringT DocBookExt = ".ent";
StringT PrjExt = ".prj";
StringT IniExt = ".ini";
StringT MifExt = ".mif";
StringT GrxExt = ".grx";
StringT DclExt = ".dcl";
StringT PdfExt = ".pdf";

StringT ChooseDestFileStr = "Choose the destination file";
StringT ExportFileCancelled = "Export cancelled.";
StringT ChooseDestDirStr = "Choose the destination directory";

StringT ChooseTemplateFileStr = "Choose the FrameMaker template file";
StringT ChooseWordTemplateFileStr = "Choose the Word template DOT file";
StringT ChooseCSSFileStr = "Choose the CSS file to use or create";

StringT SetupCompleted = "Setup completed.";

StringT ProjStartStr = ";DCL project file for %s\n";
StringT IniStartStr = ";DCL .ini file created %s; for %s\n";


/* for m2rdes.c */
StringT ApplyChanges = "Properties changed. Apply changes?";
StringT DiscardChanges = "Properties changed. Discard changes?";
StringT MacroHotspot = "Content of macro for macro hotspot:";
StringT TopicEntryMacro = "Content of topic entry macro:";
StringT ParaDesLab = "Paragraph Tag:";
StringT CharDesLab = "Character Tag:";


/* for m2rutil.c */

StringT StdIni = NULL;
StringT StdRtfIni = "mif2rtf.ini";
StringT StdHtmIni = "mif2htm.ini";
StringT StdPdfIni = "m2gpdf.ini";
StringT StdMulIni = "m2gmul.ini";
StringT IniMissing = ".ini missing, running File | Set Up Mif2go instead.";
StringT RtfIniMissing = ".ini missing, running File | Set Up mif2go instead.";
StringT HtmIniMissing = ".ini missing, running File | Set Up mif2go instead.";
StringT PdfIniMissing = ".ini missing, running File | Set Up mif2go instead.";
StringT MulIniMissing = ".ini missing, running File | Set Up mif2go instead.";
StringT DoBookSetup = "Set Up for current Book?";
StringT ReplacingOldIni = "Replacing current %s with new %s.";

StringT FileTypeErr = "Mif2Go only creates RTF, XML, and HTML files.";
StringT FilePathErr = "Mif2Go file path invalid.";
StringT NoDialogUse = "Running Mif2Go using defaults. Continue?";

StringT InitDialogID = "m2rinit";
StringT InitDialogErr = "Error %ld for init dialog. Continue?";

StringT BookProcessDone = NULL;
StringT FileProcessDone = NULL;
StringT M2GProjectDone = "Mif2Go project finished.";
StringT RtfBookProcessDone = "Mif2Go finished creating files.";
StringT RtfFileProcessDone = "Mif2Go finished creating file.";
StringT HtmBookProcessDone = "Mif2Go finished creating files.";
StringT HtmFileProcessDone = "Mif2Go finished creating file.";
StringT DclBookProcessDone = "Mif2Go finished creating files.";
StringT DclFileProcessDone = "Mif2Go finished creating file.";
StringT MifBookProcessDone = "Mif2Go finished creating files.";
StringT MifFileProcessDone = "Mif2Go finished creating file.";
StringT PdfBookProcessDone = "Mif2Go finished creating files.";
StringT PdfFileProcessDone = "Mif2Go finished creating file.";
StringT MulProcessDone = "Mif2Go finished creating all files.";

StringT ReportOK1 = NULL;
StringT RtfReportOK1 = "Mif2Go created file: %s\n";
StringT HtmReportOK1 = "Mif2Go created file: %s\n";
StringT DclReportOK1 = "Mif2Go created file: %s\n";
StringT ReportOK2 = "in: %ld min %0.2ld seconds at: %s\n";

StringT ReportFail1 = NULL;
StringT RtfReportFail1 = "Mif2Go failed for file: %s\n";
StringT HtmReportFail1 = "Mif2Go failed for file: %s\n";
StringT DclReportFail1 = "Mif2Go failed for file: %s\n";
StringT ReportFailCode = "code %ld at: %s";
StringT ReportFailTypeCode = "type %ld, code %ld, at: %s";

StringT ReportMIFFail = NULL;
StringT RtfReportMIFFail = "Mif2Go could not write %s\n";
StringT HtmReportMIFFail = "Mif2Go could not write %s\n";
StringT DclReportMIFFail = "Mif2Go could not write %s\n";

StringT ReportFailWrite = "Cannot write to destination file; may be in use.\n";
StringT ReportFailType1 = "Could not run DCL filter or other program.";
StringT ReportFailType2 = "Could not get program exit code.";
StringT ReportFailType3 = "Could not allocate process structures.";
StringT ReportFailType4 = "system() function failed.";
StringT ReportFailType5 = "Could not write MIF file.";
StringT ReportFailType6 = "Cancelled by user.";
StringT ReportFailType7 = "Could not run help compiler.";
StringT ReportFailTypeX = "Unknown internal error.";

StringT ReportFailCode1 = "Arguments unacceptable";
StringT ReportFailCode2 = "Name incorrect";
StringT ReportFailCode3 = "Memory unavailable";
StringT ReportFailCode4 = "Cannot open file to read";
StringT ReportFailCode5 = "File format incorrect";
StringT ReportFailCode6 = "Error during file read";
StringT ReportFailCode7 = "Cannot open file to write";
StringT ReportFailCode8 = "Error during file write";
StringT ReportFailCode9 = "Error in STDDEF.DCL";
StringT ReportFailCode10 = "Incorrect file type";
StringT ReportFailCode11 = "Could not run DLL, corrupted.";
StringT ReportFailCode12 = "Could not run DLL, missing.";

StringT UserCancelled = NULL;
StringT RtfUserCancelled = "Mif2Go cancelled by user.";
StringT HtmUserCancelled = "Mif2Go cancelled by user.";
StringT DclUserCancelled = "Mif2Go cancelled by user.";

StringT TemplateFailed = "Cannot open import template file. Continue?";
StringT TemplateFail2 = "Error while importing template file. Continue?";
StringT TemplateFail3 = "Error while updating xrefs. Continue?";
StringT TemplateFail4 = "Error while updating variables. Continue?";
StringT TemplateFail5 = "Error while reformatting file. Continue?";

StringT WriteFailed = "Cannot write output file; may be in use. Continue?";
StringT EqnFailure = "FM graphic filter failed during export.";
StringT EqnFailMsg = "FrameMaker failed to export %s\n as %s: ";
StringT CntFailure = "Mif2Go failed to write .cnt file.";

StringT MiscFailure = NULL;
StringT RtfMiscFailure = "Error creating output; see Console for details. Continue?";
StringT HtmMiscFailure = "Error creating output; see Console for details. Continue?";
StringT DclMiscFailure = "Error creating output; see Console for details. Continue?";
StringT PdfMiscFailure = "Error creating output; see Console for details. Continue?";

StringT StatOpening = "Opening file...";
StringT StatTemplate = "Importing template...";
StringT StatConvVars = "Converting variables...";
StringT StatSetConds = "Setting conditions...";
StringT StatEqWriting = "Writing equations...";
StringT StatGrWriting = "Writing graphics...";

StringT StatConverting = NULL;
StringT RtfStatConverting = "Running Mif2Go...";
StringT HtmStatConverting = "Running Mif2Go...";
StringT DclStatConverting = "Making ASCII DCL...";
StringT PdfStatConverting = "Making Adobe PDF...";

StringT StatSavingMIF = "Saving MIF...";
StringT StatClean = "Cleaning directories...";
StringT StatList = "Preparing Help files...";
StringT StatDITA = "Writing DITA maps...";
StringT StatDocBook = "Writing DocBook book...";
StringT StatCheck = "Checking links...";
StringT StatWrap = "Gathering files...";
StringT StatCompile = "Compiling Help...";
StringT StatJHFTS = "Creating FTS files...";
StringT StatOHJFTS = "Creating FTS file...";
StringT StatJar = "Making jar file...";
StringT StatArchive = "Archiving...";
StringT StatShip = "Putting in shipping directory...";

VoidT SetStrings(IntT typ)
{
	if (PT_htm(typ)) {
		GrFormat = 4;  /* make default JPEG */
		DestExt = (typ == PT_XML) ? XmlExt :
		         ((typ == PT_DITA) ? DITAExt :
		         ((typ == PT_DCBK) ? DocBookExt : HtmExt));
		StdIni = StdHtmIni;
		IniMissing = HtmIniMissing;
		BookProcessDone = HtmBookProcessDone;
		FileProcessDone = HtmFileProcessDone;
		ReportOK1 = HtmReportOK1;
		ReportFail1 = HtmReportFail1;
		ReportMIFFail = HtmReportMIFFail;
		UserCancelled = HtmUserCancelled;
		MiscFailure = HtmMiscFailure;
		StatConverting = HtmStatConverting;
	}
	else if (PT_rtf(typ)) {
		GrFormat = 3;  /* make default WMF */
		DestExt = RtfExt;
		StdIni = StdRtfIni;
		IniMissing = RtfIniMissing;
		BookProcessDone = RtfBookProcessDone;
		FileProcessDone = RtfFileProcessDone;
		ReportOK1 = RtfReportOK1;
		ReportFail1 = RtfReportFail1;
		ReportMIFFail = RtfReportMIFFail;
		UserCancelled = RtfUserCancelled;
		MiscFailure = RtfMiscFailure;
		StatConverting = RtfStatConverting;
	}
	else if (typ == PT_DCL) {
		GrFormat = 3;  /* make default WMF */
		DestExt = DclExt;
		StdIni = StdHtmIni;
		IniMissing = HtmIniMissing;
		BookProcessDone = DclBookProcessDone;
		FileProcessDone = DclFileProcessDone;
		ReportOK1 = DclReportOK1;
		ReportFail1 = DclReportFail1;
		ReportMIFFail = DclReportMIFFail;
		UserCancelled = DclUserCancelled;
		MiscFailure = DclMiscFailure;
		StatConverting = DclStatConverting;
	}
	else if (typ == PT_MIF) {
		GrFormat = 3;  /* make default WMF */
		DestExt = RtfExt;
		StdIni = StdRtfIni;
		IniMissing = RtfIniMissing;
		BookProcessDone = MifBookProcessDone;
		FileProcessDone = MifFileProcessDone;
		ReportOK1 = DclReportOK1;
		ReportFail1 = DclReportFail1;
		ReportMIFFail = DclReportMIFFail;
		UserCancelled = DclUserCancelled;
		MiscFailure = DclMiscFailure;
		StatConverting = DclStatConverting;
	}
	else if (typ == PT_PDF) {
		DestExt = PdfExt;
		StdIni = StdPdfIni;
		IniMissing = PdfIniMissing;
		BookProcessDone = PdfBookProcessDone;
		FileProcessDone = PdfFileProcessDone;
		ReportOK1 = DclReportOK1;
		ReportFail1 = DclReportFail1;
		ReportMIFFail = DclReportMIFFail;
		UserCancelled = DclUserCancelled;
		MiscFailure = PdfMiscFailure;
		StatConverting = PdfStatConverting;
	}
	else if (typ == PT_MUL) {
		StdIni = StdMulIni;
		IniMissing = MulIniMissing;
		BookProcessDone = MulProcessDone;
		FileProcessDone = MulProcessDone;
		ReportOK1 = DclReportOK1;
		ReportFail1 = DclReportFail1;
		ReportMIFFail = DclReportMIFFail;
		UserCancelled = DclUserCancelled;
	}
}

#if 0

#include "fstrlist.h"

NativeIntT CmpTestFn(const ConStringT *s1, const ConStringT *s2)
{
	return (F_StrCmp(*s1, *s2));
}

VoidT SortTest(void)
{
	StringListT list;
	IntT i;

	list = F_StrListNew((UIntT)3, (UIntT)1);
	F_StrListAppend(list, (StringT)"Clothos");
	F_StrListAppend(list, (StringT)"Lachesis");
	F_StrListAppend(list, (StringT)"Atropos");

	F_StrListSort(list, &CmpTestFn);

	for(i = 0; i < F_StrListLen(list); i++)
		F_Printf(NULL, "%s ", F_StrListGet(list, i));
}
#endif


/* end of m2rstr.c */

