
DCnlist RTFwr::CodeStoreParaList;
DCnlist RTFwr::CodeStoreDivList;
char *RTFwr::CodeStoreVar = NULL;
long RTFwr::CodeStoreDiv = 0;
long RTFwr::PreCodeStoreDiv = 0;

// in HTMIniSects
 { "HtmlStyleCodeStore", pcstyle, "StyleSet", NULL, false, str, &Curr.cstore },

// in ISecHtmlStyles
 { "CodeStore", spec, &Curr.cstore, CodeStoreSet, 1 },

void
RTFwr::CodeStoreSet(char *)
{
	Curr.cstore = NewName(Curr.stag);
}

// in ProcHtmlKey
		case 71: // CodeStore
			Curr.cstore = NewName(Curr.stag);
			break;

// in CheckForm
		if (cform->cstore)
			CodeStoreParaList.add(cform->cstore, ParaRefNum);

// in StartPara
	if (!FootDiv || !InFootnote)
		CodeStoreVar = CodeStoreParaList.find(ParaRefNum);


// in WriteParaProps
	if (CodeStoreVar) {  // start diversion for macro var
		DCnlist *dcn = NULL;
		PreCodeStoreDiv = wf->GetDiv();
		if ((CodeStoreDiv = CodeStoreDivList.getitem(CodeStoreVar, &dcn)) != 0) {  // existing div
			wf->DeleteDiv(CodeStoreDiv);
			dcn->id = CodeStoreDiv = wf->NewDiv();
		}
		else {
			CodeStoreDiv = wf->NewDiv();
			CodeStoreDivList.add(CodeStoreVar, CodeStoreDiv);
		}
		CodeStoreVar = NULL;
	}
// at end of WriteFlow, CloseFileBlocks, and EndSpecialBlocks
// and in EndPara in replacement and Comment code

	if (CodeStoreDiv) {  // end any pending div
		wf->SelectDiv(PreCodeStoreDiv);
		CodeStoreDiv = 0;
	}

// in EndPara before suffix frame and macro after
	if (CodeStoreDiv   // end any pending div
	 && (!FootDiv || !InFootnote)) {
		wf->SelectDiv(PreCodeStoreDiv);
		CodeStoreDiv = 0;
	}


// macro funcs
bool
RTFwr::WriteCodeStoreVar(DCwfile *df, char *mn)
{
	HTMwfile *wf = (HTMwfile *) df;
	long div = 0;
	long cdiv = 0;

	if ((div = CodeStoreDivList.getitem(mn)) != 0) {
		cdiv = wf->GetDiv();
		wf->WriteDiv(div);
		wf->SelectDiv(cdiv);
		return true;
	}
	return false;
}


char *
RTFwr::GetCodeStoreVar(DCwfile *df, char *vp)
{
	HTMwfile *wf = (HTMwfile *) df;
	long div = 0;
	long cdiv = 0;
	char *var = NULL;

	if ((div = CodeStoreDivList.getitem(vp)) != 0) {
		cdiv = wf->GetDiv();
		wf->SelectDiv(div);
		var = wf->ReadDiv();
		wf->SelectDiv(cdiv);
		return NewName(var);
	}
	return NULL;
}

