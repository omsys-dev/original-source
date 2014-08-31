// drxmltb.cpp is the code for dcl XML table processing, JHG
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



bool XMLrd::InTable = false;
bool XMLrd::InTableTitle = false;
bool XMLrd::InTableRow = false;
bool XMLrd::InTableCell = false;
bool XMLrd::TableHead = false;
bool XMLrd::TableFoot = false;
bool XMLrd::TableColMajor = false;
bool XMLrd::TableRowMajor = true;
long XMLrd::TableDepth = 0;
long XMLrd::TableCol = 0;
long XMLrd::TableColCount = 0;
long XMLrd::ColStraddle = 0;
long XMLrd::TableRow = 0;
long XMLrd::TableRowCount = 0;
long XMLrd::RowStraddle = 0;
long *XMLrd::CurrRowStrad = NULL;
DCilist XMLrd::CurrColStrad;
DCgroup *XMLrd::TableGroup = NULL;
DCgroup *XMLrd::PreTableGroup = NULL;
DCgroup *XMLrd::TableTitleGroup = NULL;
DCgroup *XMLrd::TableRowGroup = NULL;
DCgroup *XMLrd::TableCellGroup = NULL;
DCvlist XMLrd::TableRows;
DCvlist XMLrd::TableColElems;
DClist XMLrd::TableHeadCols;
long XMLrd::TableColTypeDef = 1;
DCilist XMLrd::TableRowTypes;
long XMLrd::TableRowTypeDef = 1;
DCilist XMLrd::TableRowHeights;
long XMLrd::TableRowHeightDef = 0;
DCctl *XMLrd::TableColCtl = NULL;
DCctl *XMLrd::TableWtypeCtl = NULL;
DCctl *XMLrd::TableColWidCtl = NULL;
DCnlist XMLrd::ColNameList;
DCilist XMLrd::TableColWidths;
DCvlist XMLrd::TableColWidthsD;
long XMLrd::TableColWidthLDef = 0;
double XMLrd::TableColWidthDDef = 0.0;
//long XMLrd::DefColWidth = 7200;
bool XMLrd::TableColWidthsPercent = false;
double XMLrd::TableColWidthSum = 0.0;
char *XMLrd::TableWidth = NULL;
char *XMLrd::SimpleTableWidth = NULL;
char *XMLrd::SimpleTableRelCol = NULL;
char *XMLrd::TableFooterClass = "footer";

bool XMLrd::DefinitionListTables = true;
bool XMLrd::InDefinitionList = false;
bool XMLrd::InDefinitionEntry = false;
bool XMLrd::InDefinitionTerms = false;
bool XMLrd::InDefinitionDefs = false;
long XMLrd::DefinitionListTermCount = 0;
long XMLrd::DefinitionListDefCount = 0;

bool XMLrd::ParameterListTables = true;
bool XMLrd::InParameterList = false;
bool XMLrd::InParameterEntry = false;
bool XMLrd::InParameterTerms = false;
bool XMLrd::InParameterDefs = false;
long XMLrd::ParameterListTermCount = 0;
long XMLrd::ParameterListDefCount = 0;


void 
XMLrd::StartTable(const char *outclass, const char *classatt, const char **atts)
{
	DCctl *cp = NULL;
	InTable = true;
	TableHead = false;
	TableFoot = false;
	TableCol = 0;
	TableColCount = 0;
	TableRow = 0;
	TableRowCount = 0;
	GraphDesc = NULL;
	TableRowMajor = true;
	TableColMajor = false;

	// simpletable attributes
	const char *relcolwidth = NULL;
	const char *keycol = NULL;
	const char *refcols = NULL;
	const char *cols = NULL;
	const char *rows = NULL;
	const char *width = NULL;
	const char *order = NULL;
	attrp alist[] = {
	 {"relcolwidth", &relcolwidth },
	 {"keycol", &keycol },
	 {"refcols", &refcols },
	 {"cols", &cols },
		// for UMXL
	 {"rows", &rows },
	 {"width", &width },
	 {"order", &order },
	 {NULL, NULL }
	};
	long lval = 0;
	double dval = 0.0;

	TableColWidthsPercent = false;
	if (atts && *atts) {
		ProcAttrs(atts, alist);
#ifdef UDOC
		//if (UMXL) {
			if (rows)
				TableRowCount = atol(rows);
			if (width)
				TableWidth = NewName(width);
			if (order) {
				if (!stricmp(order, "row")) {
					TableRowMajor = true;
					TableColMajor = false;
				}
				else if (!stricmp(order, "col")) {
					TableColMajor = true;
					TableRowMajor = false;
				}
			}
		//}
#endif
		if (cols)
			TableColCount = atol(cols);
		if (SimpleTableRelCol) {
			if (!relcolwidth)
				relcolwidth = SimpleTableRelCol;
			SimpleTableRelCol = NULL;
		}
		if (InDefinitionList
		 && DefListTableColWidths
		 && !relcolwidth)
			relcolwidth = DefListTableColWidths;
		if (InParameterList
		 && ParamListTableColWidths
		 && !relcolwidth)
			relcolwidth = ParamListTableColWidths;

		if (relcolwidth) {  // set TableColWidths and TableColCount
			char *chp = (char *) relcolwidth;
			char *chnum = NULL;
			TableColCount = 0;
			while (*chp) {
				if (*chp
				 && isspace(*chp))
					chp++;
				chnum = chp;
				TableColCount++;
				while (*chp
				 && (isalnum(*chp)
				  || (*chp == '.')))
					chp++;
				if (*chp
				 && (*chp == '*')) {
					dval = strtod(chnum, &chp);
					if (dval == 0.0)
						dval = 1.0;
					TableColWidthsD.add(new double(dval), TableColCount);
					TableColWidthSum += dval;
					chp++;
				}
				else {
					lval = GetNumericAttr(chnum);
					TableColWidths.add(lval, TableColCount);
				}
			}
			if (TableColWidthSum != 0.0) {
				if (SimpleTableWidth) {  // width set by a PI
					lval = GetNumericAttr(SimpleTableWidth);
					SimpleTableWidth = NULL;
				}
				else {
					lval = 0;
					TableColWidthsPercent = true;
				}
				double *dpt = NULL;
				for (long col = 1; col <= TableColCount; col++) {
					dpt = (double *) TableColWidthsD.find(col);
					if (dpt) {
						dval = (*dpt * 100.0) / TableColWidthSum;
						if (lval)
							dval = (dval * (double) lval) / 100.0;
						TableColWidths.add((long) floor(dval), col);
						delete dpt;
					}
				}
				TableColWidthsD.empty();
				TableColWidthSum = 0.0;
			}
		}
		else if (InDefinitionList
		 || InParameterList) {     // set col widths
			TableColCount = 2;
			if (SimpleTableWidth) {  // width set by a PI
				lval = GetNumericAttr(SimpleTableWidth);
				SimpleTableWidth = NULL;
				TableColWidthsPercent = false;
			}
			else {
				lval = 0;
				TableColWidthsPercent = true;
			}
			for (long col = 1; col <= TableColCount; col++) {
				dval = (col == 1) ? 25.0 : 75.0;
				if (lval)
					dval = (dval * (double) lval) / 100.0;
				TableColWidths.add((long) floor(dval), col);
			}
		}
	}


	PreTableGroup = TextGroup;
	TableDepth = CurrDepth;
	char *tform = GetTableFormat(outclass, classatt);

	if (InFig
	 && !FigTitleAboveTable
	 && FigTextGroup)
		TextGroup = FigTextGroup;
	if (UseTableAnchor) {
		char *aform = NULL;
		if (tform
		 && IniFile->Section("TableAnchorFormats")
		 && IniFile->Find(tform))
			aform = IniFile->StrRight();
		StartPara();
		SetFormat(aform ? aform : TableAnchorFormat);
	}
	if (InFig) {
		if (!FigTitleAboveTable) {
			if (FigTextGroup
			 && FigTextIDGroup)
			FigTextGroup->add(*FigTextIDGroup);
		}
		else {
			if (FigTitleIDGroup
			 && FigTextIDGroup)
			FigTitleIDGroup->add(*FigTextIDGroup);
		}
		//if (!FigTitleAboveTable)
		//	FigTextGroup->add(*FigTextIDGroup);
		//else
		//	FigTitleIDGroup->add(*FigTextIDGroup);
	}
	SetFlags(TableDepth);
	ParaStarted = false;
	PendingText = NULL;
	PendingLen = 0;

	//if (TblFootReset == 3)
	//	CurrTblFootSeq = 0;
	ResetStreams(rtTable);

	// write table header info up to title
	TableGroup = new DCgroup(DCctl(group, 2, 50, 0));
	CurrElemInfo->elemtext = TableGroup;
	CurrElemInfo->lasttext = PreTableGroup;
	SetTableFormat(tform);
	cp = new DCctl(imnem, 2, 58, 1, 1);  // table title placement
	TableGroup->add(*cp);
	cp = new DCctl(imnem, 2, 53, 1, 0);  // table column count
	TableColCtl = cp;
	TableGroup->add(*cp);
	cp = new DCctl(imnem, 2, 53, 7, 0);  // table width type fixed
	TableWtypeCtl = cp;
	TableGroup->add(*cp);
	cp = new DCctl(ilong, 2, 112, 2, ++ItemID);  // loc
	TableGroup->add(*cp);
	cp = new DCctl(elong, 2, 53, 2, 0);  // table column widths
	TableColWidCtl = cp;
	TableGroup->add(*cp);
	TextGroup = TableGroup;
}


void 
XMLrd::EndTable(void)
{
	if (!TableGroup)
		return;

	// close last cell and row, and add to PageGroup
	//if (TextGroup == TableCellGroup)
	//	EndTableCell();
	//if (TextGroup == TableRowGroup)
	//	EndTableRow();
	//EndTableTitle();

	if (TableColMajor) {
		DCvlist *dvp = &TableRows;
		while ((dvp = dvp->next) != NULL)
			TableGroup->add(*(TableRowGroup = (DCgroup *) dvp->vp));
	}

	if (!TableColCount)
		TableColCount = TableCol;

	if (CurrRowStrad) {
		//delete [] CurrRowStrad;
		CurrRowStrad = NULL;
	}


	TableColCtl->siz(TableColCount);

	long *widths = new long[TableColCount];
	if (TableColWidthSum != 0.0) {  // calculate percents
		double *dpt = NULL;
		double dval = 0.0;
		long lval = 0;
		if (SimpleTableWidth) {  // width set by a PI
			lval = GetNumericAttr(SimpleTableWidth);
			SimpleTableWidth = NULL;
			TableColWidthsPercent = false;
		}
		else
			TableColWidthsPercent = true;
		for (long col = 1; col <= TableColCount; col++) {
			dpt = (double *) TableColWidthsD.find(col);
			if (dpt) {
				dval = (*dpt * 100.0) / TableColWidthSum;
				if (lval)
					dval = (dval * (double) lval) / 100.0;
				TableColWidths.add((long) floor(dval), col);
				delete dpt;
			}
		}
		TableColWidthsD.empty();
		TableColWidthSum = 0.0;
	}
	if (TableColWidthsPercent)
		TableWtypeCtl->siz(1);

	if (TableColWidths.count()) {
		for (long i = 0; i < TableColCount; i++)
			widths[i] = TableColWidths.find(i + 1);
	}
	else {
		for (long i = 0; i < TableColCount; i++)
			widths[i] = 1; //DefColWidth;
	}
	TableColWidCtl->ex(widths, TableColCount * sizeof(long));
	delete [] widths;

	TableColWidCtl = TableColCtl = NULL;
	TableGroup->dat((uns) TableRow);

	if (InFig
	 && !FigTitleAboveTable
	 && FigTextGroup) {
		TextGroup = FigTextGroup;
		FigTextGroup->add(*TableGroup);
		SetFlags(TableDepth - 1);
		TextGroup = PreTableGroup;
		ParaStarted = true;  // fig title is open
	}
	else {
		//PageGroup->add(*TableGroup);
		TextGroup = PreTableGroup;
		SetTableDesc("TableTitle", TextGroup);
		TextGroup->add(*TableGroup);
		SetFlags(TableDepth - 1);
		ParaStarted = true;  // leave anchor para open
	}

	ColNameList.empty();
	CurrColStrad.empty();
	TableColWidths.empty();
	TableRows.empty();
	TableRowHeights.empty();
	TableColElems.empty();
	TableHeadCols.empty();
	TableRowTypes.empty();
	TableColWidthsD.empty();

	TableColWidthsPercent = false;
	TableColWidthSum = 0.0;
	TableColWidthDDef = 0.0;
	TableColWidthLDef = 0;
	TableRowHeightDef = 0;
	TableColTypeDef = 1;
	TableRowTypeDef = 1;
	TableColCount = 0;
	TableGroup = NULL;
	TableDepth = 0;
	InTable = false;
}


void 
XMLrd::SetTableDesc(char *title, DCgroup *gp)
{
	if (GraphDesc) {
		DCctl *cp = new DCctl(etext, 2, 110, 3);
		uns len = strlen(GraphDesc) + 12;
		char *txt = NewName(title, len);
		strcat(txt, ":");
		strcat(txt, GraphDesc);
		cp->ex(txt, len);
		gp->add(*cp);
		GraphDesc = NULL;
	}
}


void 
XMLrd::ProcTableCol(const char **atts)
{
	char *cpos = NULL;
	const char *colnum = NULL;
	const char *colname = NULL;
	const char *colwidth = NULL;
	const char *coltype = NULL;
	const char *colpos = NULL;
	attrp alist[] = {
	 {"colnum", &colnum },
	 {"colname", &colname },
	 {"colwidth", &colwidth },
	 {"width", &colwidth },
	 {"type", &coltype },
	 {"pos", &colpos },
	 {NULL, NULL }
	};
	long coltyp = TableColTypeDef;

	//EndTableTitle();
	TableColElems.add(CurrElemInfo, ++TableCol);

	if (TableColMajor) {
		TableRow = 1;
		while (CurrColStrad.find(TableRow) >= TableCol)
			TableRow++;
	}
	else if (TableRowMajor)
		TableColCount = TableCol;

	if (atts && *atts)
		ProcAttrs(atts, alist);

	if (colnum) {
		long val = strtol(colnum, &cpos, 10);
		if (val != TableCol)
			LogEvent(logwarn, 1, "Wrong colnum in colspec: ", colnum);
	}
	if (colname)
		ColNameList.addnew(colname, TableCol);

	if (colwidth)
		ProcTableColWidth(colwidth);
	else if (TableColWidthsPercent) {
		if (TableColWidthDDef != 0.0) {
			TableColWidthsD.add(new double(TableColWidthDDef), TableCol);
			TableColWidthSum += TableColWidthDDef;
		}
	}
	else if (TableColWidthLDef != 0)
		TableColWidths.add(TableColWidthLDef, TableCol);

	if (coltype) {
		if (!stricmp(coltype, "head")) {
			coltyp = 2;
			TableHeadCols.add(TableCol);
		}
	}
	else if (coltyp == 2)
		TableHeadCols.add(TableCol);

	if (colpos)   // specifies which cols properties apply to
		ProcPosList(colpos, false, coltyp, colwidth);
}


void 
XMLrd::ProcPosList(const char *aval, bool row, long ttyp, const char *tsize)
{
	if (*aval == '*') {
		if (ttyp) {
			if (row)
				TableRowTypeDef = ttyp;
			else
				TableColTypeDef = ttyp;
		}
		if (tsize && *tsize) {
			if (row)
				TableRowHeightDef = GetNumericAttr(tsize);
			else {
				long tcol = TableCol;
				TableCol = 0;  // to set defaults
				ProcTableColWidth(tsize);
				TableCol = tcol;
			}
			// otherwise rowheight
		}
		return;
	}

	DCnlist *plist = new DCnlist();
	plist->addlist(aval);
	long pcnt = plist->count();

	char *pval = NULL;
	char *sep = NULL;
	long start = 0;
	long end = 0;
	DClist vals;

	for (long i = 1; i <= pcnt; i++) {
		pval = plist->find(i);
		start = end = 0;
		if ((sep = strchr(pval, '-')) != NULL) {  // range
			*sep = '\0';
			start = atol(pval);
			end = atol(sep + 1);
			*sep = '-';
			for (long j = start; j <= end; j++)
				vals.add(j);
		}
		else   // single item
			vals.add(atol(pval));
	}

	// copy and store the atts list, except for pos, and use it 
	// for all the rows or cols in the DCvlist of nums
	// but really, that's just type and height/width	
	double dval = 0.0;
	long lval = 0;
	DClist *valp = &vals;
	long lastid = 0;

	if (row) {
		if (tsize)
			lval = GetNumericAttr(tsize);
		
		while ((valp = valp->next) != NULL) {
			if (ttyp)
				TableRowTypes.add(ttyp, valp->id);
			if (lval)
				TableRowHeights.add(lval, valp->id);
		}
	}
	else {
		if (tsize) {
			if (TableColWidthsPercent) {
				char *cpos = NULL;
				dval = strtod(tsize, &cpos);
				if (dval == 0.0)
					dval = 1.0;
				while ((valp = valp->next) != NULL) {
					TableColWidthsD.add(new double(dval), lastid = valp->id);
					TableColWidthSum += dval;
				}
			}
			else {
				lval = GetNumericAttr(tsize);
				while ((valp = valp->next) != NULL)
					TableColWidths.add(lval, lastid = valp->id);
			}
		}
		if (ttyp == 2) {
			valp = &vals;
			while ((valp = valp->next) != NULL)
				TableHeadCols.add(lastid = valp->id);
		}
		if (TableRowMajor) {
			if (lastid > TableColCount)
				TableColCount = lastid;
		}
	}
}


void 
XMLrd::ProcTableColWidth(const char *colwidth)
{
	char *cpos = NULL;
	size_t len = strlen(colwidth);

	if (len && (colwidth[len - 1] == '*')) {
		double dval = 0.0;
		TableColWidthsPercent = true;
		dval = strtod(colwidth, &cpos);
		if (dval == 0.0)
			dval = 1.0;
		if (!TableCol)
			TableColWidthDDef = dval;
		else {
			TableColWidthsD.add(new double(dval), TableCol);
			TableColWidthSum += dval;
		}
	}
	else {
		long lval = 0;
		lval = GetNumericAttr(colwidth);
		if (!TableCol)
			TableColWidthLDef = lval;
		else
			TableColWidths.add(lval, TableCol);
	}
}


void 
XMLrd::EndTableCol(void)
{
	// add any proc needed for col-major layouts ****
	if (TableColMajor) {
		if (TableRow)
			TableRow--;
	}
}


void 
XMLrd::StartTableTitle(void)
{
	// write table title up to para start
	TableTitleGroup = new DCgroup(DCctl(group, 2, 58, 3));
	TextGroup = TableTitleGroup;
}


void 
XMLrd::EndTableTitle(void)
{
	if (!TableTitleGroup)
		return;

	// close title and add to TableGroup
	TextGroup = TableTitleGroup;
	EndPara();
	TableGroup->add(*TableTitleGroup);
	TableTitleGroup = NULL;
	TextGroup = TableGroup;
	InTblTitle = false;
	InTableTitle = false;
}


void 
XMLrd::StartTableRow(const char **atts)
{
	const char *rowheight = NULL;
	const char *rowtype = NULL;
	const char *rowpos = NULL;
	attrp alist[] = {
	 {"height", &rowheight },
	 {"type", &rowtype },
	 {"pos", &rowpos },
	 {NULL, NULL }
	};
	long height = 0;
	long rowtyp = TableRowTypes.find(TableRow + 1);
	if (!rowtyp)
		rowtyp = TableRowTypeDef;

#ifdef DITA
	//if (!UMXL)
		rowtyp = TableHead ? 2 : (TableFoot ? 3 : 1);
#endif
	//else {
#ifdef UDOC
		TableHead = TableFoot = false;
		if (atts && *atts)
			ProcAttrs(atts, alist);

		if (rowtype) {
			rowtyp = 1;
			if (!stricmp(rowtype, "head"))
				rowtyp = 2;
			else if (!stricmp(rowtype, "foot"))
				rowtyp = 3;
			TableRowTypes.add(rowtyp, TableRow + 1);
		}
		else if (((rowtyp = TableRowTypes.find(TableRow + 1)) == 0)
		 && TableRowTypeDef)
			TableRowTypes.add(rowtyp = TableRowTypeDef, TableRow + 1);

		if (rowheight) {
			height = GetNumericAttr(rowheight);
			if (height)
				TableRowHeights.add(height, TableRow + 1);
		}
		else if ((height = TableRowHeights.find(TableRow + 1)) == 0)
			TableRowHeights.add(height = TableRowHeightDef, TableRow + 1);

		if (rowpos) { // specifies which rows properties apply to
			ProcPosList(rowpos, true, rowtyp, rowheight);
		}
#endif
	//}


	// write table row info up to first cell
	//EndTableTitle();

	if (TableRowMajor) {
		//EndTableRow();
		TableRow++;
		TableRowGroup = new DCgroup(DCctl(group, 2, 54, (uns) rowtyp));
		//									TableHead ? 2 : TableFoot ? 3 : 1));
		TableRows.add(TableRowGroup, TableRow);
		if (!CurrRowStrad)
			CurrRowStrad = new long[TableColCount];
		else {
			for (long i = 0; i < TableColCount; i++) {
				if (CurrRowStrad[i] > 0)
					CurrRowStrad[i]--;
			}
		}
		TextGroup = TableRowGroup;
		TableCol = 0;
	}
	else if (TableColMajor) {   // row elem may or may not be present at all
		TableRow++;
		if ((TableRowGroup = (DCgroup *) TableRows.find(TableRow)) == NULL) {
			TableRowGroup = new DCgroup(DCctl(group, 2, 54, (uns) rowtyp));
			//										TableHead ? 2 : TableFoot ? 3 : 1));
			TableRows.add(TableRowGroup, TableRow);
		}
	}

	if (height) {
		DCctl *cp = new DCctl(ilong, 2, 55, 4, height);
		TableRowGroup->add(*cp);
	}

	CurrElemInfo->elemtext = TableRowGroup;
	CurrElemInfo->lasttext = TableGroup;
	SetRowFlags(CurrDepth, TableRowGroup);

	if (InDefinitionEntry) {
		DefinitionListTermCount = 0;
		DefinitionListDefCount = 0;
	}
	else if (InParameterEntry) {
		ParameterListTermCount = 0;
		ParameterListDefCount = 0;
	}
}


void 
XMLrd::EndTableRow(void)
{
	//if (TextGroup == TableCellGroup)
	//	EndTableCell();

	if (!TableRowGroup)
		return;

	// close last cell and row, and add to TableGroup
	//EndTableCell();
	if (TableRowMajor) {
		if (TableCol < TableColCount) {  // row straddle at end
			AddTableCells(TableColCount - TableCol);
			TableCol = TableColCount;
		}
		SetTableDesc("RowTitle", TableGroup);
		if (TableGroup)
			TableGroup->add(*TableRowGroup);
	}
	if (TableGroup) {
		TableRowGroup = NULL;
		TextGroup = TableGroup;
	}
}


void 
XMLrd::GetTableRow(void)
{
	// set correct row group for this cell in col major order

	if ((TableRowGroup = (DCgroup *) TableRows.find(TableRow)) == NULL) {
		// row not defined, see if set by pos
		long rowtyp = 1;
		if (((rowtyp = TableRowTypes.find(TableRow)) == 0)
		 && ((rowtyp = TableRowTypeDef) == 0))
			rowtyp = 1;

		TableRowGroup = new DCgroup(DCctl(group, 2, 54, (uns) rowtyp));
		TableRows.add(TableRowGroup, TableRow);
	}
	TableHead = TableFoot = false;
	switch (TableRowGroup->dat()) {
		case 1:
			break;
		case 2:
			TableHead = true;
			break;
		case 3:
			TableFoot = true;
			break;
		default:
			break;
	}
}


void 
XMLrd::StartTableCell(const char **atts)
{
	const char *align = NULL;
	const char *valign = NULL;
	const char *colname = NULL;
	const char *namest = NULL;
	const char *nameend = NULL;
	const char *morerows = NULL;
	const char *colspan = NULL;
	const char *rowspan = NULL;
	attrp alist[] = {
	 {"align", &align },
	 {"valign", &valign },
	 {"colname", &colname },
	 {"namest", &namest },
	 {"nameend", &nameend },
	 // for UMXL
	 {"colspan", &colspan },
	 {"rowspan", &rowspan },
	 {NULL, NULL }
	};

	//long align = 0;
	//long valign = 0;
	DCctl *cp = NULL;

	if (InDefinitionEntry) {
		if (InDefinitionTerms) {
			if (++DefinitionListTermCount > 1)  // multiple terms
				return;
		}
		else {  // in definition
			if (++DefinitionListDefCount > 1)  // multiple definitions
				return;
		}
	}
	else if (InParameterEntry) {
		if (InParameterTerms) {
			if (++ParameterListTermCount > 1)  // multiple terms
				return;
		}
		else {  // in definition
			if (++ParameterListDefCount > 1)  // multiple definitions
				return;
		}
	}

	if (TableRowMajor) {
		if (!TableCol)
			SetTableDesc("RowTitle", TableGroup);
		//else
		//	EndTableCell();
		++TableCol;
		while (CurrRowStrad[TableCol - 1] > 0)  // add straddled cells
			AddTableCells(1);
	}
	else if (TableColMajor) {
		if (TableCol == 1)
			SetTableDesc("RowTitle", TableGroup);
		//else
		//	EndTableCell();
		while (CurrColStrad.find(TableRow) >= TableCol)
			TableRow++;
		GetTableRow();
	}

	// write table cell group with cell props
	TableCellGroup = new DCgroup(DCctl(group, 2, 56, 1));
	TextGroup = TableCellGroup;
	CurrElemInfo->elemtext = TableCellGroup;
	CurrElemInfo->lasttext = TableRowGroup;
	ColStraddle = RowStraddle = 0;

	// add cell props: straddles, rules, and margins
	if (atts && *atts) {
		ProcAttrs(atts, alist);
		if (colname)
			ProcCellCol(colname);
		if (namest)
			ProcCellCol(namest);
		//if (UMXL) {
#ifdef UDOC
			if (colspan) {
				if ((ColStraddle = atol(colspan)) > 1) {
					ColStraddle--;  // use DITA rule
					cp = new DCctl(ilong, 2, 57, 1, ColStraddle + 1);
					TextGroup->add(*cp);
					if (TableColMajor) {
						CurrColStrad.replace(TableCol + ColStraddle, TableRow);
					}
				}
			}
			if (rowspan) {
				RowStraddle = atol(rowspan);
				if (RowStraddle > 1) {
					cp = new DCctl(ilong, 2, 57, 2, RowStraddle);
					TextGroup->add(*cp);
					if (TableRowMajor) {
						for (long i = 0; i <= ColStraddle; i++)
							CurrRowStrad[(TableCol - 1) + i] = RowStraddle;
					}
					else if (TableColMajor) {
						for (long j = 1; j < RowStraddle; j++)
							CurrColStrad.replace(TableCol + ColStraddle, TableRow + j);
					}
				}
			}
		//}
		//else {
#endif
#ifdef DITA
			if (nameend) {
				ColStraddle = ColNameList.getitem(nameend) - TableCol;
				cp = new DCctl(ilong, 2, 57, 1, ColStraddle + 1);
				TextGroup->add(*cp);
			}
			if (morerows) {
				cp = new DCctl(ilong, 2, 57, 2, atol(morerows) + 1);
				TextGroup->add(*cp);
			}
		//}
#endif
		if (align) {
			cp = new DCctl(imnem, 2, 31, (uns) Align.getitem(align), 0);
			TextGroup->add(*cp);
		}
#ifdef UDOC
		else {  //if (UMXL)
			cp = new DCctl(imnem, 2, 31, TableHead || TableFoot ? 2 : 1, 0);
			TextGroup->add(*cp);
		}
#endif
		if (valign) {
			cp = new DCctl(imnem, 2, 57, 19, Valign.getitem(valign) - 1);
			TextGroup->add(*cp);
		}
#ifdef UDOC
		else {  //if (UMXL)
			cp = new DCctl(imnem, 2, 57, 19, TableHead || TableFoot ? 0 : 1);
			TextGroup->add(*cp);
		}
#endif
	}
#ifdef UDOC
	else {  //if (UMXL)
		cp = new DCctl(imnem, 2, 31, TableHead || TableFoot ? 2 : 1, 0);
		TextGroup->add(*cp);
		cp = new DCctl(imnem, 2, 57, 19, TableHead || TableFoot ? 0 : 1);
		TextGroup->add(*cp);
	}
#endif
}


void 
XMLrd::ProcCellCol(const char *name)
{
	long col = 0;

	if ((col = ColNameList.getitem(name)) > TableCol) {
		while (TableCol < col) {  // empty cells for row straddles
			StartPara();
			SetUniqueID();
			EndPara();
			TableRowGroup->add(*TableCellGroup);
			TableCellGroup = new DCgroup(DCctl(group, 2, 56, 1));
			TextGroup = TableCellGroup;
			++TableCol;
		}
	}
}


void 
XMLrd::AddTableCells(long count)
{
	while (count--) {
		TableCellGroup = new DCgroup(DCctl(group, 2, 56, 1));
		TextGroup = TableCellGroup;
		++TableCol;
		StartPara();
		SetUniqueID();
		EndPara();
		TableRowGroup->add(*TableCellGroup);
		TableCellGroup = NULL;
	}
}


void 
XMLrd::EndTableCell(void)
{
	if (!TableCellGroup)
		return;

	// close cell and add to RowGroup
	TextGroup = TableCellGroup;
	if (ParaStarted)
		EndPara();
	SetTableDesc("CellTitle", TextGroup);

	if (TableRowGroup) {
		TableRowGroup->add(*TableCellGroup);

		if (TableRowMajor) {
			if (ColStraddle) {
				AddTableCells(ColStraddle);
				ColStraddle = 0;
			}
		}
		else if (TableColMajor) {
			long row = TableRow;
			long col = TableCol;
			if (ColStraddle)  // add empty cells to current row
				AddTableCells(ColStraddle);
			TableCol = col;
			while (RowStraddle > 1) { // add empty cells to additional rows
				TableRow++;
				RowStraddle--;
				GetTableRow();
				AddTableCells(ColStraddle + 1);
				TableCol = col;
			}
			ColStraddle = RowStraddle = 0;
			//TableRow = row;
			TableCol = col;
			TableRow++;
			//GetTableRow();
		}

		TableCellGroup = NULL;
		TextGroup = TableRowGroup;
	}
}



DCnlist XMLrd::TableFormatList;
long XMLrd::TableFormCount = 0;
long XMLrd::TableFormUseCount = 0;
//char *XMLrd::BaseTableFormat = "TableForm";
char *XMLrd::NormalTableFormat = "NormalTable";
char *XMLrd::SimpleTableFormat = "SimpleTable";
char *XMLrd::ChoiceTableFormat = "ChoiceTable";
char *XMLrd::PropertiesTableFormat = "PropTable";
char *XMLrd::DefListTableFormat = "DefListTable";
char *XMLrd::DefListTableColWidths = NULL;
char *XMLrd::ParamListTableFormat = "ParamListTable";
char *XMLrd::ParamListTableColWidths = NULL;
bool XMLrd::UseTableAnchor = true;
char *XMLrd::TableAnchorFormat = "TableAnchor";
DCclist XMLrd::TableFormats;


char *
XMLrd::GetTableFormat(const char *outclass, const char *classatt)
{
	//char nstr[NumLenMax];
	char *form = NULL;

	// if outputclass, use that as table format
	if (!outclass
	 || !*outclass) {  // if not, get a default outputclass
		if (UMXL)
			outclass = NormalTableFormat;
		else if (InDefinitionList)
			outclass = DefListTableFormat;
		else if (InParameterList)
			outclass = ParamListTableFormat;
		else if (strstr(classatt, "choicetable"))
			outclass = ChoiceTableFormat;
		else if (strstr(classatt, "properties"))
			outclass = PropertiesTableFormat;
		else if (strstr(classatt, "simpletable"))
			outclass = SimpleTableFormat;
		else
			outclass = NormalTableFormat;
	}

	if (IniFile->Section("TableOutclassMaps")
	 && IniFile->Find(outclass))
		form = IniFile->StrRight();
	else
		form = NewName(outclass);

		//sprintf(nstr, "%0.2d", TableFormCount + 1);
		//form = NewName(BaseTableFormat, strlen(BaseTableFormat) + NumLenMax);
		//strcat(form, nstr);

	return form;
}


void
XMLrd::SetTableFormat(char *form)
{
	long fnum = TableFormatList.getitem(form);
	if (!fnum)
		fnum = StoreTableFormat(form);

	DCctl *cp = new DCctl(imnem, 2, 82, 3, fnum);  // table format num
	TableFormats.add(cp, ++TableFormUseCount);
	TableGroup->add(*cp);
}


long 
XMLrd::StoreTableFormat(const char *form)
{
	TableFormatList.addsorted(NewName(form), ++TableFormCount);
	return TableFormCount;
}




// end of drxmltb.cpp


