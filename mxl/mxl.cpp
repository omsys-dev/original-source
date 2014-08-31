// mxl.cpp runs mxlparser.dll to produce JSON of the dta model on stdout
// it either accepts a single filename arg or uses stdin for input


#include "stdafx.h"
#include "mxl.h"


// globals

MxlParser *Parser = NULL;
FILE *ofile = stdout;


// entry point

int main(int argc, char* argv[])
{
	char *fname = NULL;
	char *oname = NULL;
	char *ename = NULL;
	bool sax = false;
	bool model = true;
	bool full = false;
	bool expat = false;

	if (argc == 1) {
		WriteHelp();
		return 0;
	}

	for (int i = 1; i < argc; i++) {
		if ((argv[i][0] == '-')
		 || (argv[i][0] == '/')) {  // switch
			switch(argv[i][1]) {
				case 'o':  // output file
					oname = argv[++i];
					break;
				case 'e':  // error file
					ename = argv[++i];
					break;
				case 's':
					sax = true;
					break;
				case 'n':
					model = false;
					break;
				case 'f':
					full = true;
					break;
				case 'x':
					expat = true;
					break;
				case 'h':
				case '?':
					WriteHelp(false);
					return 0;
				case 'a':
					WriteHelp(true);
					return 0;
				default:
					fputs("Unrecognized option; options are case-sensitive.\n\n", stdout);
					WriteHelp(false);
					return 0;
			}
		}
		else  // source filename
			fname = argv[i];
	}

	if (oname) {
		FILE *tmp = fopen(oname, "w");
		if (tmp)
			ofile = tmp;
	}

	Parser = new MxlParser();

	Parser->SetOptions(sax, model, full);
	if (expat)
		Parser->SetExpatCallbacks(ename);
	else
		Parser->SetCallbacks(ename);
	element *result = Parser->ParseFile(fname, NULL);

	// send result to stdout in JSON format
	if (result) {
		fputs("\n\n", ofile);
		WriteJSON(result, 1);
		fputs("\n\n", ofile);
	}

	return 0;
}


// help

void WriteHelp(bool api)
{
	if (!api) {
		fputs("Usage: mxl [sourcefile  (default is stdin)] [options]\n", stdout);
		fputs("Options:\n", stdout);
		fputs("  -o outputfile  Default is stdout\n", stdout);
		fputs("  -e errorfile   Default is stderr\n", stdout);
		fputs("  -n No content model, otherwise sent as JSON to outputfile\n", stdout);
		fputs("  -s Send SAX messages (as diagnostics) to errorfile\n", stdout);
		fputs("  -f FullXML, report DOCTYPE, CDATA, and PIs as SAX messages\n", stdout);
		fputs("     to errorfile instead of reporting them as errors\n", stdout);
		fputs("  -x Expat callbacks for all SAX messages\n", stdout);
		fputs("  -a Provide brief help on the mxlparser.dll API\n", stdout);
		fputs("  -h or -?, Provide help (this message)\n", stdout);
	}
	else {
		fputs("\nAPI for mxlparser.dll:\n", stdout);
		fputs(" First create an MxlParser with:\n", stdout);
		fputs("  MxlParser *Parser = new MxlParser();\n", stdout);
		fputs(" Optionally, set up options and SAX callbacks:\n", stdout);
		fputs("  Parser->SetOptions(UseSAX, UseModel, FullXML); (all bools)\n", stdout);
		fputs("  Parser->SetCallbacks(ErrFileName, ReportErrorFunc,\n", stdout);
		fputs("    StartTagFunc, EndTagFunc, TextContentFunc, ReportCDataFunc,\n", stdout);
		fputs("    ReportPIFunc, ReportDoctypeFunc, ReportCommentFunc)\n", stdout);
		fputs(" For expat-compatible callbacks, use SetExpatCallbacks instead,\n", stdout);
		fputs("  which has a longer list of callbacks.\n", stdout);
		fputs(" Finally, parse the file:\n", stdout);
		fputs("  element *DataModel = Parser->ParseFile(SourceFileName);\n\n", stdout);
		fputs(" Error messages and comments are sent to ErrFileName (default\n", stdout);
		fputs("  stderr) unless the Report*Func says otherwise.\n", stdout);
		fputs(" If UseSax, the Tag and Text callbacks are used; the stub functions\n", stdout);
		fputs("  for them report the UTF-32 strings in JSON to ErrFileName.\n", stdout);
		fputs(" If UseModel, the data model is returned at the end as a struct\n", stdout);
		fputs("  with all strings in UTF-32 encoding, zero terminated.\n", stdout);
		fputs(" If FullXML, the DOCTYPE, CDATA, and PIs are reported as SAX messages\n", stdout);
		fputs("  instead of errors; they are never in the data model.\n", stdout);
	}
}


// JSON writing

void WriteJSON(element *el, long lev)
{
	long tabs = (lev - 1) * 2;
	long i = 0;
	pair *attr = NULL;
	cont *citem = NULL;
	
	PutTabs(tabs);
	fputs("[ \"", ofile);
	Put32StrJSON(el->name, el->namelen);
	fputs("\",", ofile);

	if (!el->attrcnt)
		fputs(" {}, ", ofile);
	else if (el->attrcnt == 1) {
		fputs(" { \"", ofile);
		attr = el->attrs[0];
		Put32StrJSON(attr->name, attr->namelen);
		fputs("\": \"", ofile);
		Put32StrJSON(attr->val, attr->vallen);
		fputs("\" }, ", ofile);
	}
	else {
		fputs("\n", ofile);
		PutTabs(tabs + 1);
		fputc('{', ofile);
		for (i = 0; i < el->attrcnt; i++) {
			if (!i) 
				fputc('\t', ofile);
			else
				PutTabs(tabs + 2);
			fputc('"', ofile);
			attr = el->attrs[i];
			Put32StrJSON(attr->name, attr->namelen);
			fputs("\": \"", ofile);
			Put32StrJSON(attr->val, attr->vallen);
			fputc('"', ofile);
			if (i < (el->attrcnt - 1))
				fputc(',', ofile);
			fputs("\n", ofile);
		}
		PutTabs(tabs + 1);
		fputs("}, ", ofile);
	}

	if (!el->contcnt)
		fputs(" []", ofile);
	else if ((el->contcnt == 1)
	 && (citem = el->content[0])->cnt) {
		fputs("[ \"", ofile);
		Put32StrJSON((unl *) citem->it, citem->cnt);
		fputs("\" ]", ofile);
	}
	else {
		fputs("\n", ofile);
		PutTabs(tabs + 1);
		fputs("[", ofile);
		for (i = 0; i < el->contcnt; i++) {
			citem = el->content[i];
			if (citem->cnt) {  // text
				if (!i) 
					fputc('\t', ofile);
				else
					PutTabs(tabs + 2);
				fputc('"', ofile);
				Put32StrJSON((unl *) citem->it, citem->cnt);
				fputc('"', ofile);
			}
			else { // element
				if (!i)
					fputs("\n", ofile);
				WriteJSON((element *) citem->it, lev + 1);
			}
			if (i < (el->contcnt - 1))
				fputc(',', ofile);
			fputs("\n", ofile);
		}
		PutTabs(tabs + 1);
		fputs("]", ofile);
	}
	fputs("\n", ofile);
	PutTabs(tabs);
	fputs("]", ofile);
}


void Put32StrJSON(unl *u32arr, long cnt)
{
	unl u32 = 0;

	for (long i = 0; i < cnt; i++) {
		u32 = u32arr[i];
		if (u32 < 0x20) {
			if (u32 == 0x09)
				fputs("\\t", ofile);
			else if (u32 == 0x0A)
				fputs("\\n", ofile);
		}
		else if (u32 > 0x10000) { // surrogate pair needed
			u32 -= 0x10000;
			fprintf(ofile, "\\u%04lX", 0xD800 | ((u32 >> 10) & 0x3FF));
			fprintf(ofile, "\\u%04lX", 0xD900 | (u32 & 0x3FF));
		}
		else if (u32 > 0x7F)
			fprintf(ofile, "\\u%04lX", u32);
		else
			fputc(u32, ofile);
	}
}


// utility functions

void PutTabs(long cnt)
{
	for (long i = 0; i < cnt; i++)
		fputc('\t', ofile);
}


// end of mxl.c