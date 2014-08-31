// drxmlel.cpp is the code for dcl XML element processing, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcelem.h"
#include "dcmacr.h"
#include "drxml.h"
#ifdef DITA
#include "expat.h"
#endif
#ifdef UDOC
#include "mxlparser.h"
#include "drxmlud.h"
#endif
#include "drxmlfl.h"
#include <windows.h>
#include <iostream.h>
#include "Shlwapi.h"
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <direct.h>
#include <stdlib.h>
#include <ctype.h>

#undef min


DCnlist XMLrd::CurrElems;
DCvlist XMLrd::CurrEInfo;
long XMLrd::CurrDepth = 0;
long XMLrd::CurrRootDepth = 0;
long XMLrd::CurrBranchDepth = 0;
long XMLrd::CurrElemDepth = 0;
char *XMLrd::CurrElemName = NULL;
DCelem *XMLrd::CurrElemInfo = NULL;
DCelem *XMLrd::PrevElemInfo = NULL;
long XMLrd::CurrElemNum = 0;
bool XMLrd::InTitleElem = false;
DCgroup *XMLrd::TitleGroup = NULL;
bool XMLrd::ReadTopicTitle = false;
bool XMLrd::TopicTitleStarting = false;
bool XMLrd::InAnumElem = false;
long XMLrd::InList = 0;
long XMLrd::InListItem = 0;
long XMLrd::InPairList = 0;
long XMLrd::InPairListTerm = 0;
long XMLrd::InPairListDef = 0;
DClist XMLrd::PairListLevels;
bool XMLrd::InCodeblock = false;
bool XMLrd::UseCodeblockPI = true;
DCgroup *XMLrd::AnumGroup = NULL;
DCgroup *XMLrd::PrePrologText = NULL;
bool XMLrd::KeepPrologText = false;

bool XMLrd::SkipToElement = false;
bool XMLrd::EndShortTag = false;

bool XMLrd::EndAtElement = false;
char *XMLrd::EndingElemName = NULL;
long XMLrd::EndingElemDepth = 0;

//bool XMLrd::SkipThisElement = false;
//char *XMLrd::SkippingElemName = NULL;
//long XMLrd::SkippingElemDepth = 0;


DCnlist XMLrd::BlockElems;
long XMLrd::BlockElemCount = 0;
long XMLrd::BlockElemsStarted = 0;
DCilist XMLrd::BlockAttrNumStartList;
DCilist XMLrd::BlockAttrNumEndList;
DCnlist XMLrd::BlockAttrNameList;
DCnlist XMLrd::BlockAttrValList;
long XMLrd::BlockAttrNum;
DCvlist XMLrd::BlockElemInfo;
//DCvlist XMLrd::RootElemInfo;

DCnlist XMLrd::InlineElems;
long XMLrd::InlineElemCount = 0;
long XMLrd::InlineElemsStarted = 0;
DCilist XMLrd::InlineAttrNumStartList;
DCilist XMLrd::InlineAttrNumEndList;
DCnlist XMLrd::InlineAttrNameList;
DCnlist XMLrd::InlineAttrValList;
long XMLrd::InlineAttrNum;
DCvlist XMLrd::InlineElemInfo;

bool XMLrd::IsFirst = true;
char *XMLrd::CurrLang = "en";
DCvlist XMLrd::LangStack;
long XMLrd::LangLevel = 0;
DCilist XMLrd::LangDepthStack;
DCnlist XMLrd::LangAttrStack;
DCnlist XMLrd::ElemClasses;

bool XMLrd::InTextref = false;
DCvlist XMLrd::PreTextRefGroups;
DCgroup *XMLrd::TextRefGroup;
char *XMLrd::TextRefIDs = NULL;
trtyp XMLrd::TextRefType = trnone;
DCnlist XMLrd::VarLibPaths;
long XMLrd::VarLibCount = 0;

qutyp XMLrd::QueryRefType = qunone;
DCvlist XMLrd::PreQueryRefGroups;
DCgroup *XMLrd::QueryRefGroup = NULL;
DCelem *XMLrd::QueryParent = NULL;
char *XMLrd::Query = NULL;
char *XMLrd::QueryResultsPath = NULL;
DCnlist XMLrd::QueryResult;


void
XMLrd::StartElement(const char *name, const char **atts, long depth)
{
	bool isRoot = false;
	bool wasInDoc = false;
	const char **attp = atts;
	static long lastDepth = 0;
	char *outclass = NULL;
	DCctl *cp = NULL;

	const char *classatt = NULL;
	const char *idatt = NULL;
	const char *hrefatt = NULL;
	const char *typeatt = NULL;
	const char *keyrefatt = NULL;
	const char *keysatt = NULL;
	const char *nameatt = NULL;
	const char *valueatt = NULL;
	const char *langatt = NULL;

	const char *conrefatt = NULL;
	const char *conendatt = NULL;
	const char *conactatt = NULL;
	const char *conkeyrefatt = NULL;
	const char *outputclass = NULL;
	const char *scope = NULL;
	const char *format = NULL;
	const char *impatt = NULL;
	const char *archatt = NULL;
	const char *printatt = NULL;
	const char *datatypeatt = NULL;

	const char *idrefsatt = NULL;
	const char *idrefatt = NULL;
	const char *projatt = NULL;
	const char *shapeatt = NULL;
	const char *coordsatt = NULL;
	const char *groupsatt = NULL;
	const char *outputatt = NULL;
	const char *queryatt = NULL;
	const char *resultatt = NULL;

	attrp alist[] = {
	 {"class", &classatt },
	 {"id", &idatt },
	 {"href", &hrefatt },
	 {"type", &typeatt },
	 {"keyref", &keyrefatt },
	 {"keys", &keysatt },
	 {"name", &nameatt },
	 {"value", &valueatt },
	 {"xml:lang", &langatt },

	 {"conref", &conrefatt },
	 {"conrefend", &conendatt },
	 {"conaction", &conactatt },
	 {"conkeyref", &conkeyrefatt },
	 {"outputclass", &outputclass },
	 {"scope", &scope },
	 {"format", &format },
	 {"importance", &impatt },
	 {"ditaarch:DITAArchVersion", &archatt },
	 {"print", &printatt },
	 {"datatype", &datatypeatt },  // for data elements

	 // for uDoc
	 {"lang", &langatt },
	 {"src", &hrefatt },
	 {"key", &keyrefatt },
	 {"idrefs", &idrefsatt },
	 {"idref", &idrefatt },
	 {"project", &projatt },
	 {"shape", &shapeatt },
	 {"coords", &coordsatt },
	 {"subjects", &groupsatt },
	 {"output", &outputatt },
	 {"query", &queryatt },
	 {"results", &resultatt },

	 {NULL, NULL }
	};

#ifdef DITA
	if (!strcmp(name, "dita"))   // wrapper for topics
		return;                    // does not increase level
#endif

#ifdef UDOC
	//if (UMXL) {  // handle short tags
		if (XMLud::CheckUMXLElemName(name, epShort)) {
			switch (*name) {
				case 'c':  // cell
					if (!InTable) {
						EndShortTag = true;
						return;
					}
					name = "cell";
					if (InTableCell) 
						EndElement(name, CurrDepth);
					depth = CurrDepth + 1;
					EndShortTag = true;
					break;

				case 'r':  // row
					if (!InTable) {
						EndShortTag = true;
						return;
					}
					name = "row";
					if (InTableTitle) 
						EndElement("title", CurrDepth);
					else {
						if (InTableCell) 
							EndElement("cell", CurrDepth);
						if (InTableRow) 
							EndElement(name, CurrDepth);
					}
					depth = CurrDepth + 1;
					EndShortTag = true;
					break;

				case 't':  // title
					if (!InTable
					 && !InPairList) {
						EndShortTag = true;
						return;
					}
					name = "title";
					if (InPairList
					 && (InPairList == InList)) {
						if (InPairListTerm == InPairList) // second t in pair list
							EndElement(name, CurrDepth);
						InPairListTerm = InPairList;
					}
					else if (InTable)
						InTableTitle = true;
					depth = CurrDepth + 1;
					EndShortTag = true;
					break;

				case 'l':  // list item
					if (!InList) {
						EndShortTag = true;
						return;
					}
					name = "li";
					if (InList == InPairList) {
						if (InPairListTerm == InPairList) {
							EndElement("title", CurrDepth);
							InPairListTerm--;
						}
						if (InPairListDef == InPairList) {
							EndElement("p", CurrDepth);
							InPairListDef--;
						}
					}
					if (InListItem == InList) 
						EndElement(name, CurrDepth);
					depth = CurrDepth + 1;
					EndShortTag = true;
					break;

				case 'd':  // def in pair list
					if (!InPairList
					 || (InPairList != InList)) {
						EndShortTag = true;
						return;
					}
					name = "p";
					if (InPairListTerm == InPairList) {
						EndElement("title", CurrDepth);
						InPairListTerm--;
					}
					if (InPairListDef == InPairList) {
						EndElement("p", CurrDepth);
						InPairListDef--;
					}
					depth = CurrDepth + 1;
					EndShortTag = true;
					break;

				default:
					EndShortTag = true;
					return;
			}
		}
		else if (InTable
		 || InList) {

			if (XMLud::CheckUMXLElemName(name, epTabCell)) {
				if (InTableCell) 
					EndElement("cell", CurrDepth);
				depth = CurrDepth + 1;
			}
			else if (XMLud::CheckUMXLElemName(name, epTabRow)) {
				if (InTableTitle) 
					EndElement("title", CurrDepth);
				if (InTableCell) 
					EndElement("cell", CurrDepth);
				if (InTableRow) 
					EndElement("row", CurrDepth);
				depth = CurrDepth + 1;
			}
			else if (XMLud::CheckUMXLElemName(name, epTabCol)) {
				if (InTableTitle) 
					EndElement("title", CurrDepth);
				depth = CurrDepth + 1;
			}
			else if (XMLud::CheckUMXLElemName(name, epTitle)) {
				if (InTable)
					InTableTitle = true;
				depth = CurrDepth + 1;
			}
			else if (XMLud::CheckUMXLElemName(name, epDesc)) {
				if (InTableTitle) 
					EndElement("title", CurrDepth);
				depth = CurrDepth + 1;
			}
			else if (XMLud::CheckUMXLElemName(name, epListItem)) {
				if ((QueryRefType == qunone)
				 && InList) {
					if (InPairList == InList) {
						FlushText();
						if (InPairListTerm == InPairList) {
							EndElement("title", CurrDepth);
							InPairListTerm--;
						}
						if (InPairListDef == InPairList) {
							EndElement("p", CurrDepth);
							InPairListDef--;
						}
					}
					if (InListItem == InList) 
						EndElement("li", CurrDepth);
					depth = CurrDepth + 1;
				}
			}
#if 0
			else if (!stricmp(name, "p")) {
				if (InListTitle) {
					EndElement("title", CurrDepth);
					InListTitle = false;
				}
				depth = CurrDepth + 1;
			}
#endif
		}
	//}
#endif

	ProcAttrs(atts, alist);


#ifdef DITA
	if (archatt) {
		char *per = strrchr(archatt, '.');
		if (per
		 && isdigit(*(per + 1)))
		XMLrfile::DITAver = *(per + 1) - '0';
	}

	if (NextOutputclass) {
		if (!outputclass)
			outputclass = NextOutputclass;
		NextOutputclass = NULL;
	}
#endif

	if (langatt) {
		DCirfile *lfile = NULL;
		if (!*langatt) { // use lang at bottom of stack
			if (LangLevel
			 && ((lfile = (DCirfile *) LangStack.find(1)) != NULL)) {
				CurrLang = LangAttrStack.find(1);
				RestoreLangFile(lfile);
				LangStack.add(lfile, ++LangLevel);
				LangFile = lfile;
				LanguageText = lfile->getFilePath();
				LangDepthStack.add(depth, LangLevel);
				LangAttrStack.add(CurrLang, LangLevel);
			}
		}
		else if (strcmp(langatt, CurrLang)) {
			char *lid = NewName(langatt);
			char *ltext = NULL;
			lfile = GetLangFile(&ltext, &lid);
			if (lfile) {
				LanguageText = ltext;
				CurrLang = lid;
				LangStack.add(lfile, ++LangLevel);
				LangFile = lfile;
				LangDepthStack.add(depth, LangLevel);
				LangAttrStack.add(CurrLang, LangLevel);
			}
		}
		else   // repeated lang att, ignore
			langatt = NULL;
	}

	//if (UMXL) {
#ifdef UDOC
		outputclass = classatt;
		classatt = name;
#endif
	//}
	//else {
#ifdef DITA
		if (!classatt) {  // error in 1.2 DTDs
			if ((strlen(name) == 1)
			 && ((*name == 'b')
				|| (*name == 'i'))) {
				if (*name == 'b')
					classatt = "+ topic/ph hi-d/b";
				else if (*name == 'i')
					classatt = "+ topic/ph hi-d/i";
			}
			else if (!stricmp(name, "abbreviated-form")) {
				classatt = "+ topic/term abbrev-d/abbreviated-form ";
			}
			else if (IniFile->Section("ElementClasses")
			 && IniFile->Find(name)) {
				classatt = IniFile->StrRight();
				//  do some bulletproofing here ****
			}
			else if (ElemClasses(name))
				classatt = NewName((char *) ElemClasses(name));
			else {
				LogEvent(logerr, 1, "No class attribute for element ", name);
				classatt = "- topic/data ";
			}
		}
		if (!ElemClasses(name))
			ElemClasses.add(CurrElemName, (long) NewName(classatt));
#endif
	//}

	CurrDepth = depth;

#ifdef DITA
	if (RecordShortdesc
	 && RecList
	 && (!strcmp(name, "shortdesc")
	  || (classatt
	   && strstr(classatt, "shortdesc")))) {
		RecordingElements = true;
		RecElemDepth = depth;
		RecordShortdesc = false;
	}

	if (RecordingElements)
		RecElemStart(name, atts, depth);
#endif

	// determine if root

	if (!DocRoot) {
		//if (UMXL) {  // first elem is root
#ifdef UDOC
			DocRoot = NewName(name);
			XMLrfile::CurrRoot = DocRoot;
			InMap = XMLud::CheckUMXLElemName(name, epRoot || (epMap << 8)); 
#endif
		//}else 
#ifdef DITA
		if (!strcmp(XMLrfile::CurrRoot, "dita")) {
			DocRoot = NewName(name);
			XMLrfile::CurrRoot = DocRoot;
		}
		else DocRoot = XMLrfile::CurrRoot;
		InMap = CheckElemName(name, classatt, epMap);
#endif
		isRoot = true;
	}
	else if (!strcmp(name, DocRoot)) {  // nested topic of same kind
		if (ExcludeDepth
		 && (depth > ExcludeDepth))
			return;
		isRoot = wasInDoc = true;
	}
#ifdef DITA
	else if (CheckElemName(name, classatt, epRoot)) { // nested topic of another kind
		if (ExcludeDepth
		 && (depth > ExcludeDepth))
			return;
		DocRoot = NewName(name);
		isRoot = wasInDoc = true;
	}
#endif
#ifdef UDOC
	else if (XMLud::CheckUMXLElemName(name, epRoot)) {
		if (ExcludeDepth
		 && (depth > ExcludeDepth))
			return;
		isRoot = wasInDoc = true;
	}
#endif

#ifdef UDOC
	if (  //UMXL && 
	 (wasInDoc
	  || InTextref)) {  // substitute div
		if (!strcmp(name, "doc")
		 || !strcmp(name, "map")
	   || !strcmp(name, "lib"))
	  // || !strcmp(name, "variable"))
			name = classatt = "div";
		isRoot = wasInDoc = false;
	}
#endif

	CurrElemName = NewName(name);
	CurrElems.replace(CurrElemName, depth);

	// root identified, check for skip of topic or skip to element

	if (isRoot) {
		CurrRootDepth = depth;
		if (CurrBranchDepth >= CurrRootDepth)
			CurrBranchDepth = 0;

		//if (UMXL) {
#ifdef UDOC
			if ((CurrDocRef->chunkby == cbdoc)
			 || SplitTopicFiles)
				ChunkDoc = true;
#endif
		//}
		//else {
#ifdef DITA
			if (ProcessingConrefs) {
				if (DocID) {  // finding conref, require match to topic ID
					if (!idatt
					 || strcmp(DocID, idatt)) {
						//SkipTopic = true;
						ConrefTopic = false;
						//SkipNesting++;  // determine whether to include nested topics
														// by CurrDocChunk
						//return;
					}
					else
						ConrefTopic = true;
				}
				else  // no topic ID, use whole doc
					ConrefTopic = true;
			}
			// determine chunking status
			else if (!InMap
			 && CurrDocRef) {  // in doc referenced from map
				//if (SplitTopicFiles)
				//	ChunkTopic = true;
				//else 
				switch (CurrDocRef->chunkby) {
					case cbdoc:    // include whole doc in one chunk
						// add Chunk marker to start of first topic title only
						// include CurrDocRef->copyto name, else source doc name
						if (!ChunkDocSet
						 && !DisableChunking)
							ChunkDoc = true;
						break;
					case cbtopic:  // make each topic a separate chunk
						// add Chunk marker to start of each topic title
						// include CurrDocRef->copyto name, else topic id as name
						ChunkTopic = true;
						break;
					default:
						if (SplitTopicFiles)
							ChunkTopic = true;
						break;
				}
				switch (CurrDocRef->chunksel) {
					case csdoc:    // all topics in the current doc
						// even if DocID, don't skip anything
						break;
					case cstopic:  // only topic matching id
						// if DocID doesn't match, skip it
						if (DocID
						 && idatt
						 && !strcmp(DocID, idatt))  // matches, use it
							break; 
						if (!DocID
						 && FirstTopic)             // no ID, use this one
							break;
						SkipTopic = true;
						return;
					case csbranch: // topic matching id plus all nested in it
						// if DocID doesn't match, and not nested in DocID topic, skip
						if ((DocID
							&& idatt
							&& !strcmp(DocID, idatt))  // matches, use it
						 || (!DocID
							&& FirstTopic))            // no ID, use this one
							CurrBranchDepth = CurrRootDepth;
						if (CurrBranchDepth)
							break;
						SkipTopic = true;
						return;
					default:  // no select specified, go by DocID
						if (DocID
						 && (!idatt
							|| strcmp(DocID, idatt))) {
							SkipTopic = true;
							return;
						}
						break;  // matched ID, use it
				}
				switch (CurrDocRef->chunkto) {
					case ctcont:   // include all parts in one chunk
						// our default behavior anyway
						if (!LastChunkToCont) {
							LastChunkToCont = true;
							if (!DisableChunking)
								SplitThisTopic = true;
						}
						break;
					case ctnav:    // make TOC only (Eclipse)
						// ignore this for now
						break;
					default:
						if (LastChunkToCont) {
							LastChunkToCont = false;
							if (!DisableChunking)
								SplitThisTopic = true;
						}
						break;
				}
			}
			else if (CurrMapRef) {  // start of map referenced from map
				if (SplitTopicFiles)
					ChunkTopic = true;
				// does chunking from topicref apply, or not?
			}
			else {  // in initial doc or map
				// use chunking policy set in ini
				if (ChunkSel
				 && !stricmp(ChunkSel, "topic")
				 && ((DocID
					 && (!idatt
						|| strcmp(DocID, idatt)))
					|| (!DocID
					 && !FirstTopic))) {
					SkipTopic = true;
					return;
				}
				if (ChunkSel
				 && !stricmp(ChunkSel, "branch")) {
					if (!CurrBranchDepth
					 && (((DocID
						 && (!idatt
							|| strcmp(DocID, idatt)))
						|| (!DocID
						 && !FirstTopic)))) {
						SkipTopic = true;
						return;
					}
					CurrBranchDepth = CurrRootDepth;
				}
				if (SplitTopicFiles)
					ChunkTopic = true;
				else {  // default is ChunkSel not set, or == "document"
					if ((ChunkBy
						&& !stricmp(ChunkBy, "document"))
					 && !ChunkDocSet
					 && !DisableChunking)
						ChunkDoc = true;
					if (ChunkBy
					 && !stricmp(ChunkBy, "topic"))
						ChunkTopic = true;
				}
			}
#endif
		//}
		SkipTopic = false;
		FirstTopic = false;
	}

	if (SkipTopic)  // non-root element
		return;

	CurrElemDepth = (CurrDepth - CurrRootDepth) + 1;

	// check for conditional exclusion

	if (ExcludeDepth
	 && (depth > ExcludeDepth))
		return;

	ExcludeDepth = 0;

#ifdef UDOC
	if (!XMLud::CheckConds(atts)
#endif
#ifdef DITA
	if (CondAttrsExclude(atts)
#endif

	 || (printatt
	  && !strstr(classatt, "keydef")  // which somehow gets print=no ****
	  && ((PrintProject && !strcmp(printatt, "no"))
	   || (!PrintProject && !strcmp(printatt, "printonly"))))) {
		if (isRoot)
			SkipTopic = true;
		else
			ExcludeDepth = depth;
		return;
	}

#ifdef UDOC
	if (XMLud::InOutput
	 && !XMLud::UseOutput) {
		ExcludeDepth = depth;
		return;
	}
#endif

	// using this element, continue processing

	CheckFlags(atts);

	if (outputclass)
		outclass = CheckOutputClass(outputclass);

	if (isRoot) {
#ifdef DITA
		if (CheckElemName(name, classatt, epMap)
#endif
#ifdef UDOC
		if (XMLud::CheckUMXLElemName(name, epMap)
#endif
		) {
			InMap = true;
			InReltable = false;
			if (!MapCount) {  // initial map
				MapCount = 1;
				StartPage();
			}
			else if (ProcessingConrefs
			 && !PageStarted)
				StartPage();
			if (!CurrMapInfo)
				SetMapInfo(FixReference(XMLFile->getFileName()));
		}
		else {
			if (wasInDoc) {  // nested topic
				EndPage();
				CurrDocLevel++;
				if (!ProcessingConrefs)
					ProcNestedTopicMapGroup(depth, idatt);
			}
			if (InMap) { //end of maps, start of docs
				TextNum = 2;
				InMap = false;
			}
			if (!CurrDocInfo) {        // direct processing of topic
#ifdef DITA
				if (!ProcessingConrefs
				 && UseReplacementMap()) { // use map, either existing or new
					XMLrfile::AbortParse();
					DocRoot = NULL;
					CurrElems.remove(depth);
					return;  // cancel operation
				}
				else
#endif
					SetDocInfo(FixReference(XMLFile->getFileName()));
			}
			StartPage();
			InTitleElem = true;
			SetTopicInfo(name, classatt);
			CurrTopicInfo->mref = CurrDocRef;
			if (!CurrDocRef->reftop)
				CurrDocRef->reftop = CurrTopicInfo;
#if 0
			InTaskTopic = strstr(classatt, "task") ? true : false;
#endif
		}
		DocRoots.add(DocRoot, CurrDocLevel);
		InterpolatedP = 0;
		InterpolatedPName = NULL;
		TextBlockElemLevel = 0;
		TextBlockElemName = NULL;
	}
#ifdef UDOC
	else {  // not root, see if block with text allowed
		if (InterpolatedP
		 && InterpolatedPName
		 && (InterpolatedP == depth - 1)
		 && (XMLud::CheckUMXLElemName(name, epPara)
		  || !XMLud::CheckUMXLElemName(name, epText))) {
			CloseInterP = true;
			CurrDepth = TextBlockElemLevel;
			EndParaTextShorthand();
			EndElement(InterpolatedPName, CurrDepth);
			FormatUsed.remove(CurrDepth);
			FormatInline.remove(CurrDepth);
			AllowTextLevels.remove(CurrDepth);
			CurrElems.remove(depth - 1);
			CurrElems.changeid(depth - 1, depth);
			CurrDepth = --depth;
			InterpolatedP = 0;
			InterpolatedPName = NULL;
			TextBlockElemLevel = 0;
			DeleteName(TextBlockElemName);
			TextBlockElemName = NULL;
			CloseInterP = false;
		}
		if (!TextBlockElemLevel
		 && !XMLud::InMXMLIndexEntry) {
			if (XMLud::CheckUMXLElemName(name, epText, epInline)) {
				TextBlockElemLevel = depth;
				TextBlockElemName = NewName(name);
			}
			else if (XMLud::CheckUMXLElemName(name, epInline, epIndex)) {
				InterpolatedP = depth;
				InterpolatedPName = "p";
				CurrElems.remove(depth);
				StartElement(InterpolatedPName, NULL, depth);
				depth = ++CurrDepth;
				CurrElemDepth++;
				CurrElems.add(name, depth);
			}
		}
	}
#endif

	if (InMap) {
//	 && !(UMXL && XMLud::CheckUMXLElemName(name, epVar, 0, true))
		SetMapElemInfo(name, classatt, idatt, outclass, hrefatt,
		 conrefatt, conendatt, conactatt, keyrefatt, conkeyrefatt,
		 idrefatt, idrefsatt, depth, isRoot);
		CurrElemInfo->setAttrs(atts);
		CurrElemInfo->isEmpty = true;
#ifdef UDOC
		if (projatt)
			CurrElemInfo->project = NewName(projatt);
		if (outputatt)
			CurrElemInfo->output = NewName(outputatt);
		if (typeatt)
			CurrElemInfo->type = NewName(typeatt);
#endif
		if (keysatt)
			CurrElemInfo->keys = NewName(keysatt);
#if 0
		if (CurrElemInfo->checkType(epNote)
		 && !CurrElemInfo->getAttr("type"))
			CurrElemInfo->addAttr("type", "note");
#endif

		if (CurrMapRef) {
			DCmapelem *me = (DCmapelem *) CurrElemInfo;
			if (CurrMapRef->flags) {  // inherit flags from mapref
				if (!me->flags)
					me->flags = new DClist(CurrMapRef->flags);
				else {
					DClist *cmr = CurrMapRef->flags;
					while ((cmr = cmr->next) != NULL)
						me->flags->add(cmr->id);
				}
			}
			if (CurrMapRef->groups) {  // inherit groups from mapref
				if (!me->groups)
					me->groups = new DCnlist;
				me->groups->addlist(CurrMapRef->groups);
			}
		}
		StartMapElement(name, atts, depth, isRoot);
		SetFilterClasses();
		return;
	}


	// in topic
	// root gets eleminfo too; sets IsInline
	SetTopicElemInfo(name, classatt, idatt, outclass, hrefatt,
		conrefatt, conendatt, conactatt, keyrefatt, conkeyrefatt,
		idrefatt, idrefsatt, depth, isRoot, (depth > lastDepth));
	CurrElemInfo->setAttrs(atts);
	CurrElemInfo->isEmpty = true;
#ifdef UDOC
	if (projatt)
		CurrElemInfo->project = NewName(projatt);
	if (outputatt)
		CurrElemInfo->output = NewName(outputatt);
	if (typeatt)
		CurrElemInfo->type = NewName(typeatt);
#endif
	if (keysatt)
		CurrElemInfo->keys = NewName(keysatt);
#if 0
	if (CurrElemInfo->checkType(epNote)
	 && !CurrElemInfo->getAttr("type"))
		CurrElemInfo->addAttr("type", "note");
#endif
	CurrElemInfo->eltyp = estopic;
	if (langatt)
		CurrElemInfo->lang = NewName(langatt);
	else
		CurrElemInfo->lang = GetParentLang(CurrElemInfo);

	if (isRoot) {
		FindGlossAbbrReset(CurrElemInfo);
		if (UsePartFiles
		 && !ProcessingConrefs) {
			CheckPartStart(CurrElemInfo);
			CurrTopicInfo->part = DCLPartFile;
		}
		if (CurrDocRef) {
			if (CurrDocRef->flags) {  // inherit flags from topicref
				DClist *flist = (DClist *) CondFlagLevels.find(CurrDepth);
				if (!flist) {
					flist = new DClist(CurrDocRef->flags);
					CondFlagLevels.add(flist, CurrDepth);
				}
				else {
					DClist *cdr = CurrDocRef->flags;
					while ((cdr = cdr->next) != NULL)
						flist->add(cdr->id);
				}
			}
			if (CurrDocRef->groups) {  // inherit groups from docref
				for (long i = 1; i <= CurrDocRef->groups->count(); i++)
					SetOneTopicAlink(CurrDocRef->groups->find(i), CurrTopicInfo);
			}
		}

		if (!CurrTopicInfo->mref->userel)
			CurrTopicInfo->userel = false;
	}

	//if (UMXL) {

#ifdef UDOC
		if (CurrElemInfo->checkType(epOut)) {  // output-dependent setting in doc
			XMLud::SetOutputType(CurrElemInfo->type);
			return;
		}

		if (CurrElemInfo->checkType((epDef << 8) | epKey)) {
			// keydef in doc
			ProcKeydef(CurrElemInfo);
			return;
		}

		if (CurrElemInfo->checkType((epDef << 16) | (epRef << 8) | epElem)) {
			// references one or more element definitons
			XMLud::ReadElementDefinitions(CurrElemInfo->href,
			                      NewName(idrefsatt ? idrefsatt : idrefatt));
			return;
		}

		if (CurrElemInfo->checkType((epFile << 16) | (epRef << 8) | epRel)) {
			// related topics, set group membership
			if (groupsatt) {
				DCnlist *groups = new DCnlist;
				groups->addlist(groupsatt);
				for (long i = 1; i <= groups->count(); i++)
					SetOneTopicAlink(groups->find(i), CurrTopicInfo);
				delete groups;
			}
			return;
		}

		if (CurrElemInfo->checkType(epCond)) {  // condition settings
			XMLud::ProcCondition(CurrElemInfo);
			return;
		}

		if (CurrElemInfo->checkType((epDef << 8) | epVar)) {
			// content is a variable definition
			// most likely brought in by a var reference, just a wrapper
			return;
		}
#endif
	//}

	if (CurrElemInfo->keyref) {
		FlushText();
		SetElemText();
		CurrElemInfo->isEmpty = true;
		ResolveKeyref((DCtopelem *) CurrElemInfo);
	}

#ifdef DITA
	if (ConrefTopic) {
		if (ProcessingT2TConrefs
		 && CurrConrefT2TReplacement) {
			if (isRoot) {
				if (CurrConrefT2TReplacement->action)
					CurrConrefT2TReplacement->destset = CurrTopicInfo;
				else
					CurrConrefT2TReplacement->srcset = CurrTopicInfo;
			}
			if ((!DocElement  // conref replaces topic
				&& isRoot)
			 || (DocElement   // conref replaces element
				&& idatt
				&& !strcmp(DocElement, idatt))) {
				if (CurrConrefT2TReplacement->action)
					CurrConrefT2TReplacement->destelem = CurrElemInfo;
				else
					CurrConrefT2TReplacement->srcelem = CurrElemInfo;
				AnumElemList = &CurrConrefT2TReplacement->anums;
				LOFItemList = &CurrConrefT2TReplacement->lofitems;
				LOTItemList = &CurrConrefT2TReplacement->lotitems;
			}
		}
		else if (ProcessingM2TConrefs
		 && CurrConrefM2TReplacement) {
			if (isRoot) {
				if (CurrConrefM2TReplacement->action)
					CurrConrefM2TReplacement->destset = CurrTopicInfo;
				else
					CurrConrefM2TReplacement->srcset = CurrTopicInfo;
			}
			if (DocElement   // conref replaces element
			 && idatt
			 && !strcmp(DocElement, idatt)) {
				if (CurrConrefM2TReplacement->action)
					CurrConrefM2TReplacement->destelem = CurrElemInfo;
				else
					CurrConrefM2TReplacement->srcelem = CurrElemInfo;
				AnumElemList = &CurrConrefM2TReplacement->anums;
				LOFItemList = &CurrConrefM2TReplacement->lofitems;
				LOTItemList = &CurrConrefM2TReplacement->lotitems;
			}
		}
		else if (ProcessingT2MConrefs
		 && CurrConrefT2MReplacement) {
			if (isRoot) {
				if (CurrConrefT2MReplacement->action)
					CurrConrefT2MReplacement->destset = CurrTopicInfo;
				else
					CurrConrefT2MReplacement->srcset = CurrTopicInfo;
			}
			if (DocElement   // conref replaces element
			 && idatt
			 && !strcmp(DocElement, idatt)) {
				if (CurrConrefT2MReplacement->action)
					CurrConrefT2MReplacement->destelem = CurrElemInfo;
				else
					CurrConrefT2MReplacement->srcelem = CurrElemInfo;
				AnumElemList = &CurrConrefT2MReplacement->anums;
				LOFItemList = &CurrConrefT2MReplacement->lofitems;
				LOTItemList = &CurrConrefT2MReplacement->lotitems;
			}
		}
	}
#endif

	if (!DocCount)
		NextDocNum = CurrDocNum = DocCount = 1;

	// artifact after previous closing tag
	if ((PendingLen == 1)
	 && (*PendingText == ' ')
	 && !IsInline
	 && !IsPreform) {
		PendingText = NULL;
		PendingLen = 0;
	}
	SetStringProps(depth);

	// ensure no old formats left on stack
	FormatUsed.remove(depth);
	FormatInline.remove(depth);
	AllowTextLevels.remove(depth);

	//if (UMXL) {
#ifdef UDOC
		if (IsInline) {
			if (!CurrElemInfo->checkType(epBreak))
				FormatInline.add(depth);
			if (PendingText)
				FlushText();
		}
		else {
			if (CurrElemInfo->checkType(epText)) { // starting a text block
				PendingText = NULL;
				PendingLen = 0;
			}
			CurrElemInfo->isfirst = IsFirst = (depth > lastDepth);
			lastDepth = depth;
		}
#endif
	//}
	//else 
#ifdef DITA
	if (!InImageMap) {
		if (PendXref)
			StartXrefContent();

		if (IsInline) {
			// inline elem, flush any text for enclosing block elem
			// but first make sure block was started
			FormatInline.add(depth);
			if (!InFootnote
			 && !CurrElemInfo->checkType(epFtn)) {
				if (!PendingText)
					FlushText();
				DCelem *einfo = NULL;
				long lev = depth - 1;
				while (lev && FormatInline.find(lev))
					lev--;  // skip current inlines
				if (((einfo = (DCelem *) CurrEInfo.find(lev)) != NULL)
				 && einfo->checkType(epPern)
				 && !ParaStarted) {
					StartPern = true;
					PernDepth = lev;
				}
			}
			if (PendingText)
				FlushText();
		}
		else {	// block elem, shouldn't be any text waiting
			// unless in an epPern elem
			DCelem *einfo = NULL;
			long lev = depth - 1;
			while (lev && FormatInline.find(lev))
				lev--;  // skip current inlines
			if (((einfo = (DCelem *) CurrEInfo.find(lev)) != NULL)
			 && einfo->checkType(epPern)) {
				if (PendingText) {
					if ((PendingLen == 1)
					 && (*PendingText == ' ')) {
						//DeleteName(PendingText);
						PendingText = NULL;
						PendingLen = 0;
					}
					else
						FlushText();
				}
				if (ParaStarted) {
					EndPara();
					ForcedClose.add(einfo->elform, depth);
				}
			}
			CurrElemInfo->isfirst = IsFirst = (depth > lastDepth);
			lastDepth = depth;
		}
	}
#endif

	if (!IsInline
	 && CurrElemInfo->parent) {  // see if first of its type under parent
		IsFirst = false;
		DCelem *elp = CurrElemInfo->parent;
		DClist *cl = elp->children;
		DCelem *el = NULL;
		while ((cl = cl->next) != NULL) {
			el = (DCelem *) cl->id;
			if (el == CurrElemInfo) {
				IsFirst = true;
				break;
			}
			if (!stricmp(el->elemname, CurrElemInfo->elemname))
				break;
		}
		CurrElemInfo->isfirst = IsFirst;
	}

	if (CurrElemInfo->checkType(epDraft)
	 && !KeepDraftComments) {
		ExcludeDepth = depth;
		return;
	}

	//if (UMXL) {
#ifdef UDOC
		if (CurrElemInfo->checkType((epRef << 8) | epElem)) {
			SetTextRef(CurrElemInfo, trtext);  // block textref
			return;
		}

		if (CurrElemInfo->checkType((epInline << 16) | (epText << 8) | epVar)) {
			// content is a reference to a variable def for transclusion
			// if no src or key, default to x2g\lib\stdvars.mxl for idref
			SetTextRef(CurrElemInfo, trvar);  // var
			return;
		}

	  if (CurrElemInfo->checkType((epRef | (epDir << 8)))) {
			SetQueryRef(CurrElemInfo, qudir, queryatt, resultatt);
			return;
		}

	  if (CurrElemInfo->checkType((epRef | (epDB << 8)))) {
			SetQueryRef(CurrElemInfo, qudb, queryatt, resultatt);
			return;
		}

	  if (CurrElemInfo->checkType((epRef | (epWeb << 8)))) {
			SetQueryRef(CurrElemInfo, quweb, queryatt, resultatt);
			return;
		}
#endif
	//}

	if (CurrElemInfo->checkType(epPre)
	 && !IsPreform) {
		PreInline = IsInline;
		IsPreform = StartingPre = true;
		PreformDepth = depth;
#ifdef UDOC
		if (XMLud::InMXMLIndexEntry)
			XMLud::InMXMLIndexEntry = InIndex = false;
#endif
	}
	
	if (CurrElemInfo->checkType(epText, epData)) {
		AllowText = true;
		AllowTextLevels.add(depth);
	}
	else
		AllowText = false;

	// structs set and attrs stored, process specific elem types

	if (CurrElemInfo->checkType((epInline << 8) | epBreak)) {
		TextGroup->add(*(new DCctl(imnem, 2, 11, 10, 1)));
		return;
	}

	if (InRelLinks) {
		if (ProcRelatedLinks((DCtopelem *) CurrElemInfo, atts))
			return;
	}

#if 0
	if (CurrElemInfo->checkType(epInline, epIndex)
	 && !ParaStarted) {
			CurrElemInfo->isEmpty = false;
			FlushText();
	}
#endif

	if (CurrElemInfo->checkType((epInline << 8) | epTypo)) {
		if (!ParaStarted) {
				CurrElemInfo->isEmpty = false;
				FlushText();
		}
		if (!SetTypographic(name, true)) { // flush and turn on prop
			// not std, treat as inline format 
			if (!CurrElemInfo->elpath)
				CurrElemInfo->elpath = GetElemPath(depth);
			char *form = GetElemForm(CurrElemInfo, CurrElemInfo->outclass,
			                     CurrElemInfo->elemname, true, depth);
			CurrElemInfo->elform = form;
			CurrElemInfo->formctl = SetFormat(CurrFormat = form, true);
		}
		SetInlineElemTags();
	}

	if (InObject) {
		if (CurrElemInfo->checkType(epParam))
			SetObjectParam(atts);
		else if (CurrElemInfo->checkType(epDesc))
			InObjectDesc = true;
		else
			LogEvent(logwarn, 1, "Unknown element in object: ", name);
	}
	else if (CurrElemInfo->checkType((epInline << 8) | epSeq)) {
		if (CurrElemInfo->checkType(epText)) {  // cascade sequence
			InCascade = true;
		}
#if 0
		else {  // cascade separator, process at end tag
			if (InCascade) {
				if (PendingText)
					FlushText();
				if (CascadeSeparator) {
					char *cs = DCmacr::GetMacro(CascadeSeparator);
					PutTextCtrls(cs, strlen(cs));
				}
				else
					PutTextStrCtl(" | ", 3);
			}
		}
#endif
	}
	else if (CurrElemInfo->checkType(epCascadeSet)) { // menucascade
		InCascade = true;
		CascadeStarted = false;
	}
	else if (CurrElemInfo->checkType(epTmark)) { // tm
		//if (UMXL) {
#ifdef UDOC
			if (CurrElemInfo->checkType((epText << 8) | epInline)) { // tm
				XMLud::ListElemRefStart(CurrElemInfo, TmarkFileName, depth, gentmk);
			}
			else if (CurrElemInfo->checkType(epRef)) {
			}
			else if (CurrElemInfo->checkType(epDef)) {
			}
#endif
		//}
		//else {  // DITA
#ifdef DITA
			// tmtype is tm, reg, or service
			char *tmtype = NULL;
			if ((tmtype = CurrElemInfo->getAttr("tmtype")) != NULL)
				CurrElemInfo->tm = (tmtyp) TrademarkTypes(tmtype);
			// trademark, tmowner, and tmclass also present ****
#endif
		//}
	}
	else if (CurrElemInfo->checkType(epCascadeItem)) { // uicontrol
		if (InCascade) {
			if (CascadeStarted) {
				if (PendingText)
					FlushText();
				if (CascadeSeparator) {
					char *cs = DCmacr::GetMacro(CascadeSeparator);
					PutTextCtrls(cs, strlen(cs));
				}
				else
					PutTextStrCtl(" | ", 3);
			}
			else
				CascadeStarted = true;
		}
	}
	else if (CurrElemInfo->checkType(epTab)) { // tab def or use
		if (IsInline)
			UseTab((char *) name + 1);
		else
			SetTabs(CurrElemInfo);
	}
	else if (CurrElemInfo->checkType(epData, (epIndex << 8) | epMarker)) { // ignore data elems for now
		if (nameatt
		 && !stricmp(nameatt, "debug")) {
			return;
		}
		//else if (UMXL) {
#ifdef UDOC
			if ((CurrElemInfo->checkType(epCSH)
			  || (nameatt
			   && !stricmp(nameatt, "TopicAlias")))
			 && valueatt)
				SetTopicAlias(NewName(valueatt));
			// other data elements automatically become macro variables
#endif
		//} else 
#ifdef DITA
		if ((datatypeatt
		  && !stricmp(datatypeatt, "fm:marker"))   // DITA-FMx marker
		 || !datatypeatt) {  // without the @datatype, for our own use
			if (nameatt
			 && valueatt) {
				if (!stricmp(nameatt, "TopicAlias"))
					SetTopicAlias(NewName(valueatt));
				else if (!stricmp(nameatt, "HHTextPopup")) {
					FlushText();
					if (!ParaStarted)
						SetElemFormat(true);
					DCctl *cp = new DCctl(etext, 2, 110, 3);
					char *str = NewName(nameatt, strlen(nameatt) + strlen(valueatt) + 1);
					strcat(str, ":");
					strcat(str, valueatt);
					cp->ex(str);
					TextGroup->add(*cp);
				}
			}
		}
#endif
	}
#ifdef UDOC
	else if (CurrElemInfo->checkType((epData << 8) | epMarker)) {  // markers
		if (nameatt && valueatt) {
			FlushText();
			if (!ParaStarted)
				SetElemFormat(true);
			DCctl *cp = new DCctl(etext, 2, 110, 3);
			char *str = NewName(nameatt, strlen(nameatt) + strlen(valueatt) + 1);
			strcat(str, ":");
			strcat(str, valueatt);
			cp->ex(str);
			TextGroup->add(*cp);
		}
	}
#endif
	else if (CurrElemInfo->checkType((epMeta << 8) | epRefer)) { // resourceid
		if (idatt)
			SetTopicAlias(NewName(idatt));
	}
	else if (CurrElemInfo->checkType((epMeta << 8) | epGroup)) { // prolog
		//FlushText();
		PrePrologText = TextGroup;
		if (!CurrTopicInfo->metatext)
			CurrTopicInfo->metatext = new DCgroup;
		TextGroup = CurrTopicInfo->metatext;
	}
	else if (CurrElemInfo->checkType(epRel, epMap)) {  // related-links
		if (ParaStarted)
			EndPara();  // wrap up before links
		InRelLinks = true;
		InitRelatedLinks();
		PreLinkTextGroup = TextGroup;
		ProcRelatedLinks((DCtopelem *) CurrElemInfo, atts);
	}
#ifdef UDOC
	else if (CurrElemInfo->checkType((epRef | (epPre << 8)), epInline)) {
#endif
#ifdef DITA
	else if (CurrElemInfo->checkType(epRef | (epPre << 8) | (epText << 16))) { 
#endif
	  // coderef, insert contents of file here ****
		if (!TextGroup)
			return;
		if (CurrElemInfo->href) {
			FlushText();
			CurrElemInfo->isEmpty = false;
			DCctl *cp = new DCctl(ename, 1, 50, 4);
			TextGroup->add(*cp);
			cp->ex(CurrElemInfo->href);
			PendXref = InXref = false;
		}
	}
	else if (CurrElemInfo->checkType(((epPre << 8) | epText), epInline)) {
		// codeblock, force para start
		//StartPara();
		SetElemFormat(false);
	}
	else if (CurrElemInfo->checkType(((epInline << 8) | epRef), (epImage << 8) | epTopic)) {
		// xref, not topicref or imagemap area
		if (CurrElemInfo->href
#ifdef UDOC
		 || (idrefsatt || idrefatt)
#endif
		) {
			if (InImageArea) {
				InImageXref = true;
				ProcImageAreaXref(CurrElemInfo->href, format, scope);
			}
			else {
				//if (UMXL) {
#ifdef UDOC
					if (!CurrElemInfo->project) {
						CurrElemInfo->href = XMLud::MakeUMXLHref(CurrElemInfo->href,
						                            idrefsatt ? idrefsatt : idrefatt);
					}
					if (PathIsURL(CurrElemInfo->href))
						scope="external";

					if (!IsInline
					 && !ParaStarted) {
						SetElemFormat(false);
						StartXref(depth);
					}
					if (strstr(CurrElemInfo->href, ".txt")) {
						CurrElemInfo->href = NewName(hrefatt);  // undo fixup
						scope="external";
					}
#endif
				//}
				PendXref = true;
				XrefDepth = depth;
				XrefHref = CurrElemInfo->href;
				XrefForm = CurrElemInfo->outclass;
				XrefExt = SetXrefScope(scope, XrefHref);
				//XrefExt = scope ? (scopetyp) LinkScopes.getitem(scope) : sclocal;
			}
		}
		else
			XrefHref = NULL;
	}
	else if (CurrElemInfo->checkType(epTable, epRel)) {
		if (PendingText)
			FlushText();
		InDefinitionList = false;
		if (CurrElemInfo->checkType(epDefList)) {
			if (DefinitionListTables
			 && !InTable)
				InDefinitionList = true;
			else
				return;
		}
		InParameterList = false;
		if (CurrElemInfo->checkType(epParmList)) {
			if (ParameterListTables
			 && !InTable)
				InParameterList = true;
			else
				return;
		}
		if (CurrElemInfo->checkType((epStart << 8) | epTable, epRel)) {
			CurrElemInfo->elemtype = table;
			InTitleElem = true;
			StartTable(CurrElemInfo->outclass, CurrElemInfo->classatt, atts);
		}
		else if (CurrElemInfo->checkType(epTabHead))
			TableHead = true;
		else if (CurrElemInfo->checkType(epTabBody))
			TableHead = false;
		else if (CurrElemInfo->checkType(epTabCol)) {
			if (!InTable) {
				LogEvent(logerr, 1, "Table col not in table.");
				return;
			}
			ProcTableCol(atts);
		}
	}
	else if (CurrElemInfo->checkType(epTabRow, epRel)) {
		InDefinitionEntry = InParameterEntry = false;
		if (CurrElemInfo->checkType(epDefEntry)) {
			if (InDefinitionList)
				InDefinitionEntry = true;
			else
				return;
		}
		if (CurrElemInfo->checkType(epParmEntry)) {
			if (InParameterList)
				InParameterEntry = true;
			else
				return;
		}

		if (CurrElemInfo->checkType(epTabHead))
			TableHead = true;
		else if (CurrElemInfo->checkType(epTabBody))
			TableHead = false;
#ifdef DITA
		else if (!TableHead) { // complex table row
			if (CurrElemInfo->outclass
			 && TableFooterClass
			 && !stricmp(CurrElemInfo->outclass, TableFooterClass))
				TableFoot = true;
			else
				TableFoot = false;
		}
#endif

		if (!InTable) {
			LogEvent(logerr, 1, "Table row not in table.");
			return;
		}
		InTableRow = true;
		StartTableRow(atts);

#ifdef DITA
		if (outclass
		 && !TableFoot)
			SetSpecialFormat(sfmt_tr, outclass);
#endif
	}
	else if (CurrElemInfo->checkType(epTabCell, epRel)) {
		InDefinitionTerms = InDefinitionDefs = false;
		if (CurrElemInfo->checkType(epDefTerm)) {
			if (!InDefinitionEntry)
				return;
			InDefinitionTerms = true;
		}
		if (CurrElemInfo->checkType(epDefDef)) {
			if (!InDefinitionEntry)
				return;
			InDefinitionDefs = true;
		}
		InParameterTerms = InParameterDefs = false;
		if (CurrElemInfo->checkType(epParmTerm)) {
			if (!InParameterEntry)
				return;
			InParameterTerms = true;
		}
		if (CurrElemInfo->checkType(epParmDef)) {
			if (!InParameterEntry)
				return;
			InParameterDefs = true;
		}
		if (!InTable
		 || (TableRowMajor && !InTableRow)) {
			LogEvent(logerr, 1, "Table cell not in table row.");
			return;
		}
		InTableCell = true;
		StartTableCell(atts);
		if (outclass)
			SetSpecialFormat(sfmt_tc, outclass);
	}
	else if (CurrElemInfo->checkType(epFig)) {
		if (ParaStarted
		 && PendingText)
			FlushText();
		if (CurrElemInfo->checkType((epGroup << 8) | epRef))  // imagemap
			StartImageMap();
		CurrElemInfo->elemtype = fig;
		InTitleElem = true;
		ProcFigStart();
	}
	else if (CurrElemInfo->checkType(epSect)) {
		CurrElemInfo->elemtype = section;
		InTitleElem = true;
		if (CurrElemInfo->checkType(epTask)) {
			//CurrTaskHeadType = thnone;
			for (uns th = thsteps; th < thlast; th++) {
				if (strstr(CurrElemInfo->classatt, TaskHeadTypes.find(th))) {
					CurrTaskHeadType = (thtyp) th;
					CurrTaskHeadElem = CurrElemInfo;
					CurrTaskHeadDepth = depth;
					CurrTaskHeadUsed = false;
					break; // context, prereq, example, result, postreq
				}
			}
		}
	}
	else if (CurrElemInfo->checkType(epFtn)) {
		CurrElemInfo->elemtype = fn;
		CurrFtnHasID = idatt ? true : false;
		CurrFtnForm = CurrElemInfo->outclass;
		CurrFtnCallout = CurrElemInfo->getAttr("callout");
		InAnumElem = true;
	}
#ifdef UDOC
	else if (CurrElemInfo->checkType((epList << 8) | epDef, epRef)) { // genlist ****
		if (!CurrElemInfo->checkType(epListItem)) {
		}
		else {  // genlist item
		}
	}
#endif
	else if (CurrElemInfo->checkType((epList << 8) | epNumbered)) {
		InList++;   // not a pair list
		InAnumElem = true;  // used for ol items with IDs
		//CurrTaskHeadType = thnone;
		if (CurrElemInfo->checkType(epTask, epSub)
		 && StepsHeadFormat
		 && *StepsHeadFormat) {  // interpolate heading
			CurrTaskHeadType = thsteps;
			CurrTaskHeadUsed = false;
		}
	}
	else if (CurrElemInfo->checkType((epList << 8) | epTask)) {
		// steps-unordered
		//CurrTaskHeadType = thnone;
		if (CurrElemInfo->checkType(epTask, epSub)
		 && StepsHeadFormat
		 && *StepsHeadFormat) {  // interpolate heading
			CurrTaskHeadType = thsteps;
			CurrTaskHeadUsed = false;
		}
	}
	else if (CurrElemInfo->checkType((epStart << 8) | epList, (epDef << 8) | epRef)) {
		if (InList
		 && (InList == InPairList)) {
			if (InPairListTerm == InPairList) {
				EndElement("title", CurrDepth);
				InPairListTerm--;
			}
			if (InPairListDef == InPairList) {
				EndElement("p", CurrDepth);
				InPairListDef--;
			}
		}
		InList++;
		if (CurrElemInfo->checkType(epParmList)) {
			PairListLevels.add(InPairList = InList);
			InPairListTerm = InPairListDef = InPairList - 1;
		}
	}
	else if (CurrElemInfo->checkType(epTask, epListItem)) {  // stepxmp, stepresult
		CurrTaskHeadType = thnone;
		for (uns th = thsteps; th < thlast; th++) {
			if (strstr(CurrElemInfo->classatt, TaskHeadTypes.find(th))) {
				CurrTaskHeadType = (thtyp) th;
				CurrTaskHeadElem = CurrElemInfo;
				CurrTaskHeadDepth = depth;
				CurrTaskHeadUsed = false;
				break;
			}
		}
		//if (CurrElemInfo->checkType(epListItem)
		// && InAnumElem)  // xrefs only allowed to ol lists
		//	CurrElemInfo->elemtype = li;
	}
#ifdef UDOC
	else if (CurrElemInfo->checkType(epListItem)) {
		InListItem = InList;
		//if (UMXL || InAnumElem)  // xrefs only allowed to ol lists
			CurrElemInfo->elemtype = li;
	}
#endif
	else if (CurrElemInfo->checkType(epImage)) {
		if (CurrElemInfo->checkType(epRef)) // area
			StartImageArea(shapeatt, coordsatt);
		else if (CurrElemInfo->checkType(epAlt))
			InAlt = true;
#ifdef DITA
		else if (CurrElemInfo->checkType(epText, epGroup)) // shape
			InImageShape = true;
		else if (CurrElemInfo->checkType(epText)) // coords
			InImageCoords = true;
#endif
		else {
			GraphHref = CurrElemInfo->href;
			FlushText();
			ProcImageStart();
		}
	}
	else if (CurrElemInfo->checkType(epAlt))
		InAlt = true;
	else if (CurrElemInfo->checkType(epDesc)) {   // desc
		// can provide HTML @title for fig, table, xref
		InDesc = true;
	}
	else if (CurrElemInfo->checkType(epPre, epInline))
		InCodeblock = true;
	else if (CurrElemInfo->checkType(epObj)) {
		ObjectDepth = depth;
		StartObject(atts);
	}
#ifdef UDOC
	else if (CurrElemInfo->checkType((epInline << 16) | (epData << 8) | epIndex)) {
		FlushText();
		if (XMLud::InMXMLIndexEntry)
			XMLud::EndUMXLIndexItem();
		XMLud::StartUMXLIndexItem();
	}
#endif
#ifdef DITA
	else if (CurrElemInfo->checkType((epStart << 8) | epIndex)) {
		FlushText();
		StartIndexItem();
	}
#endif
	else if (CurrElemInfo->checkType(epIndex))
		ProcIndexElemStart();
	else if (CurrElemInfo->checkType(epGloss)) {
		if (CurrElemInfo->checkType(epRoot)
#ifdef UDOC
		 || CurrElemInfo->checkType(epDef)
#endif
		)
			InGlossEntry = true;
		else if (CurrElemInfo->checkType((epAbbr << 8) | epInline)) {
			InGlossAbbr = true;
			ProcGlossAbbr();  // DITA, not UMXL
		}
#ifdef UDOC
		else if (CurrElemInfo->checkType((epText << 8) | epInline)) {
			XMLud::ListElemRefStart(CurrElemInfo, GlossaryFileName, depth, gengloss);
		}
#endif
	}
#ifdef UDOC
	else if (CurrElemInfo->checkType(epAbbr)) {
		// UMXL abbr, not DITA ****
		if (CurrElemInfo->checkType(epInline)) {
			XMLud::ListElemRefStart(CurrElemInfo, AbbrFileName, depth, genabbr);
		}
		else if (CurrElemInfo->checkType(epRef)) {
		}
		else if (CurrElemInfo->checkType(epDef)) {
		}
	}
#endif
	else if (CurrElemInfo->checkType(epSDesc)) { // shortdesc
	//else if (CurrElemInfo->checkType(UMXL ? epDesc : epSDesc)) { // shortdesc
		DCgroup *mgp = CurrTopicInfo->mref->mapdesc;
		if (mgp              // if needed and not already provided
		 && !(mgp->first))   // put copy of shortdesc in map group
			SetTopicShortdescInMap(CurrTopicInfo);
	}

#ifdef DITA
	// if element has conref, save its attrs and set up group
	CheckTopicConref(CurrElemInfo, isRoot);
#endif

#if 0
	if (CurrElemInfo->conref
	 || CurrElemInfo->conact) {
		if (CurrElemInfo->conref
		 && FileIsMap(CurrElemInfo->conref)) {
			if (CurrElemInfo->conact)
				SetT2MConrefInfo(true);  //push
			else
				SetM2TConrefInfo(NULL);
		}
		else
			SetConrefInfo(isRoot, (CurrElemInfo->conact != NULL));
	}
	else if (!isRoot
	 && (idatt                  // insurance in case it is conref'd later
	 || CurrElemInfo->keyref    // or in case we need to add content
	 || CurrElemInfo->conkeyref))
		SetElemText();
#endif

	SetElemText();  // for everything, to support conref ranges and push

	if (CurrElemInfo->keyref
	 && CurrElemInfo->href
	 && !PendXref) {
		PendXref = true;
		XrefDepth = depth;
		XrefHref = CurrElemInfo->href;
		XrefForm = CurrElemInfo->outclass;
		XrefExt = SetXrefScope(scope, XrefHref);;
		//XrefExt = scope ? (scopetyp) LinkScopes.getitem(scope) : sclocal;
	}

	if (CurrElemInfo->checkType(epTitle)) {
		if (InPairList
		 && (InPairList == InList)) {
			InPairListTerm = InPairList;
		}
		else {
			if (InTitleElem) { // title to be saved for use in xrefs
				TitleGroup = CurrElemInfo->elemtref = new DCgroup;
				if (!CurrTopicInfo->title) {
					ReadTopicTitle = true;
					TopicTitleStarting = true;
				}
			}

			if (InTable
			 || (InFig
				&& TreatTableFigAsTable
				&& ((CurrElemInfo->outclass
					&& (strstr(CurrElemInfo->outclass, "table")
					 || strstr(CurrElemInfo->outclass, "Table")))
				 || (CurrElemInfo->parent
				  && CurrElemInfo->parent->outclass
					&& (strstr(CurrElemInfo->parent->outclass, "table")
					 || strstr(CurrElemInfo->parent->outclass, "Table")))))) {
				if (InTable) {
					StartTableTitle();
					InTableTitle = true;
				}
				else { // change format name
					if (!CurrElemInfo->elpath)
						CurrElemInfo->elpath = GetElemPath(depth);
					CurrElemInfo->elform = GetElemForm(CurrElemInfo, CurrElemInfo->outclass,
																						 CurrElemInfo->elemname);
					if (!_strnicmp(CurrElemInfo->elform, "figure", 6)) {
						char *form = NewName("Table", strlen(CurrElemInfo->elform));
						strcat(form, CurrElemInfo->elform + 6);
						CurrElemInfo->elform = form;
					}
				}
				if (GenerateLOT)
					InTblTitle = true;
			}
			else if (InFig
			 && GenerateLOF)
				InFigTitle = true;
		}
	}
#ifdef DITA
	else if (CurrElemInfo->xrsrc)
#endif
#ifdef UDOC
	else if (CurrElemInfo->checkType(epTitle)
	 || CurrElemInfo->checkType(epListItem)
	 || CurrElemInfo->checkType(epGloss)
	 || CurrElemInfo->checkType(epAbbr)
	 || CurrElemInfo->checkType(epTmark)
	 || CurrElemInfo->checkType(epFtn))
#endif
		TitleGroup = CurrElemInfo->elemtref = new DCgroup;
	else if (InPairList
	 && (InPairList = InList)
	 && CurrElemInfo->checkType((epPara << 8) | epText, (epTitle << 8) | epShort)) {
		FlushText();
		if (InPairListTerm == InPairList) {
			EndElement("title", CurrDepth);
			InPairListTerm--;
		}
		if (InPairListDef == InPairList)
			EndElement("p", CurrDepth);
		SetElemFormat(false);
		InPairListDef = InPairList;
	}

	SetFilterClasses(); // this handles the table row classes
}


bool XMLrd::OutputclassHasBorderShadeFormats = true;;
char *XMLrd::BorderType = "border";
char *XMLrd::ShadeType = "shade";
char *XMLrd::BorderFmt = NULL;
char *XMLrd::ShadeFmt = NULL;

char **XMLrd::FmtClassTypes[MaxFmtClassType] = {&BorderType, &ShadeType };
char **XMLrd::FmtClassNames[MaxFmtClassType] = {&BorderFmt, &ShadeFmt };
DCnlist XMLrd::OutClassParts;
DCilist XMLrd::OutClassTypes;


char *
XMLrd::CheckOutputClass(const char *outputclass)
{
	if (!outputclass)
		return NULL;

	char *oclass = NewName(outputclass);
	if (!OutputclassHasBorderShadeFormats)
		return oclass;

	char *chp = oclass;
	char *start = NULL;
	long cnt = 0;
	long scnt = 0;
	long rval = 0;
	long i = 0;
	OutClassParts.empty();
	OutClassTypes.empty();
	if (strchr(oclass, ' ')) {  // split up class list
		while (*chp) {
			while (*chp && (*chp == ' '))  // trim leading spaces
				chp++;
			start = chp;
			while (*chp && (*chp != ' '))  // skip to next space
				chp++;
			if (*chp) {      // terminating space
				*chp++ = '\0';
				OutClassParts.addnew(start, ++cnt);
			}
			else if (chp > start)
				OutClassParts.addnew(start, ++cnt);
			if (CheckClassType(OutClassParts.find(cnt)), cnt)
				scnt++;
		}
		DeleteName(oclass);  // all parts are stored
		oclass = NULL;
		if (cnt > scnt) {  // at least one was not a spec format
			for (i = 1; i < cnt; i++) {
				if (!OutClassTypes.find(i)) {  // find first normal one
					oclass = OutClassParts.find(rval = i);
					break;
				}
			}
			// if more not identified, assign to border then shade
			for ( ; i < cnt; i++) {
				if (!OutClassTypes.find(i)) {  // find more normal ones
					if (!BorderFmt)
						BorderFmt = OutClassParts.find(i);
					else if (!ShadeFmt) {
						ShadeFmt = OutClassParts.find(i);
						break;
					}
				}
			}
		}
		if (!oclass)
			oclass = OutClassParts.find(1);  // return first anyway
	}
	else   // just one class name
		CheckClassType(oclass);

	return oclass;
}


bool 
XMLrd::CheckClassType(char *oclass, long cnt)
{
	for (unc typ = 0; typ < MaxFmtClassType; typ++) {
		if (strstr(oclass, *FmtClassTypes[typ])) {
			*FmtClassNames[typ] = oclass;
			if (cnt)
				OutClassTypes.add(typ + 1, cnt);
			return true;
		}				
	}
	return false;
}


long 
XMLrd::GetProcessedDocInfo(const char *dname, bool strict)
{
	DCmlist *dnames = ((ProcessingConrefs && ConrefPull) ?
	                    &ConrefDocNamesProcessed :
		                  &DocNamesProcessed);

	long dcount = dnames->getmult(dname, CurrBranch);
	if (!dcount
	 && !strict)
		dcount = dnames->getmult(dname, NULL);
	if (!dcount) {
		if (ProcessingConrefs
		 && ConrefPull) {
			dnames = &DocNamesProcessed;
			dcount = dnames->getmult(dname, CurrBranch);
		}
		else {
			dnames = &ConrefDocNamesProcessed;
			dcount = dnames->getmult(dname, CurrBranch);
		}
		if (!dcount
		 && !strict)
			dcount = dnames->getmult(dname, NULL);
	}

	char *branch = NULL;
	long dval = 0;

	if (!dcount)
		return 0;

	if (dcount == 1)
		return dnames->multitem(&branch);

	while (dcount--) {
		dval = dnames->multitem(&branch);
		if (!branch || !CurrBranch
		 || !_stricmp(branch, CurrBranch))
			return dval;
	}
	return dval;
}


void 
XMLrd::SetDocInfo(char *ref)
{
	DCmlist *dnames = ((ProcessingConrefs && ConrefPull) ?
	                     &ConrefDocNamesProcessed :
		                   &DocNamesProcessed);

	ParseHref(DocRef = ref, &DocName, &DocID, &DocElement);
	//uns len = strlen(DocName) + 1;
	//DocBasePath = NewName(len);
	//DocBaseName = NewName(len);
	//::GetBaseName(DocName, DocBasePath, DocBaseName);

	if ((CurrDocNum = GetProcessedDocInfo(DocName, true)) != 0) {
		CurrDocInfo = (DCtopdoc *) DocInfos.find(CurrDocNum);
		CurrTopicNum = CurrDocInfo->topiccnt;
	}
	else {
		CurrDocInfo = new DCtopdoc(CurrDocNum = ++NextDocNum);
		DocInfos.add(CurrDocInfo, CurrDocNum);
		dnames->addmult(DocName, CurrDocNum, CurrBranch);
		CurrDocInfo->docname = NewName(DocName);
		CurrTopicNum = 0;
	}

	CurrElemNum = 0;
	CurrTopicInfo = NULL;
	PrevElemInfo = CurrElemInfo = NULL;

	DocRefsProcessed.add(DocRef, CurrDocNum);
}


void 
XMLrd::SetTopicInfo(const char *name, const char *classatt)
{
	CurrTopicInfo = new DCtopic(++CurrTopicNum, PageNum, CurrPageInfo,
		CurrDocNum, CurrDocInfo, CurrDocLevel, CurrBranch);

	CurrElemNum = 0;
	PrevElemInfo = CurrElemInfo = NULL;
	CurrPageInfo->topic = CurrTopicInfo;
	if (!CurrTopicInfo->metatext)
		CurrTopicInfo->metatext = new DCgroup;
}


void
XMLrd::SetTopicElemInfo(const char *name, const char *classatt,
 const char *idatt, const char *outclass, const char *hrefatt,
 const char *conrefatt, const char *conendatt,
 const char *conactatt, const char *keyrefatt,
 const char *conkeyrefatt, const char *idrefatt, const char *idrefsatt,
 long depth, bool isRoot, bool first)
{
	PrevElemInfo = CurrElemInfo;  // NULL for topic elem itself
	CurrElemInfo = new DCtopelem(++CurrElemNum, NewName(name), depth, 
		NewName(classatt), NewName(outclass), CurrTopicNum, CurrTopicInfo);

	CurrEInfo.replace(CurrElemInfo, depth);
	DCelem *parent = NULL;
	if ((depth > 1)
	 && ((parent = (DCelem *) CurrEInfo.find(depth - 1)) != NULL)) {
		CurrElemInfo->parent = parent;
		if (!parent->children)
			parent->children = new DClist();
		parent->children->add((long) CurrElemInfo);
	}

	if (!CurrElemInfo->setType())
		LogEvent(logwarn, 1, "Properties not found for ", CurrElemInfo->elemname);

	if (CurrElemInfo->checkType(epSDesc) // shortdesc in abstract
	//if (CurrElemInfo->checkType(UMXL ? epDesc : epSDesc) // shortdesc in abstract
	 && parent
	 && parent->checkType(epAbstr)) {
		CurrElemInfo->addProp(epInline);
		IsInline = true;
	}
	else
		IsInline = CurrElemInfo->checkType(epInline);
	CurrElemInfo->elinlin = IsInline;

	if (IsInline) {
		CurrElemInfo->elemcount = ++InlineElemCount;
		InlineElemInfo.add(CurrElemInfo, InlineElemCount);
		InlineElems.addnew(name, InlineElemCount);
	}
	else {  // includes root elem
		CurrElemInfo->elemcount = ++BlockElemCount;
		BlockElemInfo.add(CurrElemInfo, BlockElemCount);
		BlockElems.addnew(name, BlockElemCount);
	}

	CurrElemInfo->elemtype = isRoot ? GetTopicType(name, classatt) : other;
	CurrElemInfo->xrsrc = CurrElemInfo->checkType(epXRsrc);
	CurrElemInfo->href = FixReference(hrefatt);
	CurrElemInfo->conref = FixReference(conrefatt);
	CurrElemInfo->endref = FixReference(conendatt);
	CurrElemInfo->conact = NewName(conactatt);
	CurrElemInfo->setKeyrefs(keyrefatt, conkeyrefatt);
	CurrElemInfo->idref = NewName(idrefatt);
	CurrElemInfo->idrefs = NewName(idrefsatt);

	if (idatt) {
		if (isRoot) {  // topicid is also its elemid
			CurrTopicInfo->id = NewName(idatt);
			CurrDocInfo->topicids->add(CurrTopicInfo->id, CurrTopicNum);
			CurrElemInfo->id = CurrTopicInfo->id;
		}
		else {   // keep topicid out of elemid list
			CurrElemInfo->id = NewName(idatt);
			if (CurrTopicInfo)
				CurrTopicInfo->elemids->add(CurrElemInfo->id, CurrElemNum);
			//SetElemText();  // insurance in case it is conref'd later
			if (!IsInline && first)
				CurrElemInfo->isfirst = IsFirst = first;
			CurrElemInfo->elpath = GetElemPath(depth);
		}
	}
	else if (isRoot && GenerateUIDs) {  // otherwise made later with idatt
		char *ref = CurrElemInfo->href ? CurrElemInfo->href : DocRef;
		CurrElemInfo->elemuid = MakeAnchorName(ref, CurrElemInfo);
		CurrElemInfo->id = CurrElemInfo->elemuid;
	}
}



DCvlist XMLrd::ElemTextGroups;


void 
XMLrd::SetElemText(bool conref, bool map)
{
	if (CurrElemInfo->elemtext  // already in effect
	 || !TextGroup)             // really fucked up
		return;

	DCctl *cp = NULL;

	CurrElemInfo->elemtext = new DCgroup;
	CurrElemInfo->lasttext = (TextGroup == PageTextGroup) ? 0 : TextGroup;

	ElemTextGroups.add(CurrElemInfo, (long) CurrElemInfo->elemtext);

	// for conrefs, add text inset ctls here
	if (conref) {
		if (!CurrElemInfo->elinlin
		 && ParaStarted)
			EndPara();
		DCgroup *gp = new DCgroup(DCctl(group, 2, 75, 1));
		TextGroup->add(*gp);
		CurrElemInfo->conrefinset = cp = new DCctl(ename, 2, 75, 3);
		if (map) {
			if (CurrMapConrefInfo
			 && (CurrMapConrefInfo->destelem == CurrElemInfo)
			 && *CurrMapConrefInfo->href)
				cp->ex(*CurrMapConrefInfo->href, strlen(*CurrMapConrefInfo->href));
			if (CurrConrefT2MInfo
			 && (CurrConrefT2MInfo->destelem == CurrElemInfo)
			 && *CurrConrefT2MInfo->href)
				cp->ex(*CurrConrefT2MInfo->href, strlen(*CurrConrefT2MInfo->href));
		}
		else {
			if (CurrConrefT2TInfo
			 && (CurrConrefT2TInfo->destelem == CurrElemInfo)
			 && *CurrConrefT2TInfo->href)
				cp->ex(*CurrConrefT2TInfo->href, strlen(*CurrConrefT2TInfo->href));
			if (CurrConrefM2TInfo
			 && (CurrConrefM2TInfo->destelem == CurrElemInfo)
			 && *CurrConrefM2TInfo->href)
				cp->ex(*CurrConrefM2TInfo->href, strlen(*CurrConrefM2TInfo->href));
		}
		gp->add(*cp);
	}

	TextGroup->add(*(CurrElemInfo->elemtext));

	if (conref) {
#if 0
		if (!CurrElemInfo->elinlin) {
			cp = new DCctl(imnem, 2, 11, 2);  // end para started in inset
			TextGroup->add(*cp);
		}
#endif
		//CurrElemInfo->eignore = cp = new DCctl(endgroup, 1, 30, 1);
		TextGroup->add(*cp);
		CurrElemInfo->einset = cp = new DCctl(imnem, 2, 75, 8);  // end inset
		TextGroup->add(*cp);
	}

	TextGroup = CurrElemInfo->elemtext;
}


void
XMLrd::EndElement(const char *name, long depth)
{
	DCctl *cp = NULL;
	bool textGroupSet = false;
	bool paraend = true;

#ifdef DITA
	if (!name
	 || !depth
	 || !strcmp(name, "dita"))   // wrapper for topics
		return;                    // at end of file
#endif

	if (EndShortTag) {
		EndShortTag = false;
		return;
	}

	CurrDepth = depth;
	CurrElemInfo = (DCelem *) CurrEInfo.find(depth);

	//if (UMXL) {
#ifdef UDOC
#if 0
		// treat empty p tag as short tag, p already started, don't end it ****
		if (CurrElemInfo
		 && CurrElemInfo->isEmpty
		 && (InTable || InList)
		 && !PendingText
		 && !stricmp(CurrElemInfo->elemname, "p"))
			return;
#endif

		if (CurrElemInfo
		 && TextBlockElemLevel
		 && (TextBlockElemLevel == depth)) {
			FlushText();
			EndParaTextShorthand();
			if (InList
			 && (InPairList == InList)
			 && (InPairListDef == InPairList))
				InPairListDef--;
			depth = CurrDepth;
			if (InterpolatedP
			 && (InterpolatedP == depth)) {
				if (CloseInterP
				 || strcmp(CurrElemInfo->elemname, name)) {
					//CurrElemInfo = (DCelem *) CurrEInfo.find(depth);
					InterpolatedP = 0;
					InterpolatedPName = NULL;
				}
				DCelem *cinfo = (DCelem *) CurrEInfo.find(depth - 1);
				if (cinfo
				 && !stricmp(cinfo->elemname, name)) {
					CurrEInfo.remove(depth);
					CurrDepth = depth = depth - 1;
					CurrElemInfo = (DCelem *) CurrEInfo.find(depth);
				}
			}
			DeleteName(TextBlockElemName);
			TextBlockElemName = NULL;
			TextBlockElemLevel = 0;
		}

		if (CurrElemInfo
		 &&	(!strcmp(name, "doc")
			|| !strcmp(name, "map")
			|| !strcmp(name, "lib"))
		 && !strcmp(CurrElemInfo->elemname, "div"))
			name = "div";

		if (CurrElemInfo
		 && stricmp(CurrElemInfo->elemname, name)) {
			long edepth = 0;
			long odepth = depth;
			DCelem *cinfo = NULL;
			for (edepth = depth; edepth > 0; edepth--) {
				if (((cinfo = (DCelem *) CurrEInfo.find(edepth)) != NULL)
				 && !stricmp(cinfo->elemname, name))
					break;
			}
			if (edepth) {  // elem found at higher level
				while (odepth > edepth) {  // close lower elems in order
					if ((cinfo = (DCelem *) CurrEInfo.find(odepth)) != NULL) {
						if (CurrShortPairLevel
						 && (CurrShortDepth.find(CurrShortPairLevel) == CurrDepth))
							EndParaTextShorthand();
						else
							EndElement(cinfo->elemname, odepth);
						if (odepth == CurrDepth)
							odepth--;
						else
							odepth = CurrDepth;
					}
					else
						odepth--;
				}
				CurrDepth = depth = edepth;
			}
			else
				return;
			// go ahead with original call
			CurrElemInfo = (DCelem *) CurrEInfo.find(CurrDepth);
			if (CurrElemInfo)
				name = CurrElemInfo->elemname;
		}
#endif
	//}


#ifdef DITA
	if (RecordingElements) {
		RecElemEnd(name, depth);
		if (RecElemDepth == depth) {
			RecordingElements = false;
			RecList = NULL;
		}
	}
#endif

#ifdef UDOC
	if (CurrElemInfo) {
		if (CurrElemInfo->checkType((epMap << 16) | (epTopic << 8) | epPara)) {  // doctext
			if (CurrDocref) {
				FlushText();
				if (PreDocText) {
					TextGroup = PreDocText;
					PreDocText = NULL;
				}
			}
		}
		else if (!InMap
		 && CurrDocRef
		 && (CurrDocRef->doctextids
		  || CurrDocRef->docstarttext
		  || CurrDocRef->docendtext)) {
			doctxt *dtxs = NULL;
			DCgroup *tgp = CurrElemInfo->elemtext;
			if (!tgp)
				tgp = TextGroup;
			if (CurrElemInfo->id
			 && CurrDocRef->doctextids
			 && ((dtxs = (doctxt *) CurrDocRef->doctextids->getitem(CurrElemInfo->id)) != NULL)) {
				FlushText();
				if (dtxs->dtt == dtxafter)
					tgp->add(*((DCctl *) dtxs->dtgp));
				else if (dtxs->dtt == dtxbefore)
					tgp->insert(*((DCctl *) dtxs->dtgp));
				else if (dtxs->dtt == dtxreplace) {
					tgp->empty();
					tgp->add(*((DCctl *) dtxs->dtgp));
				}
			}
			if (CurrElemInfo->checkType(epTitle)
			 && ReadTopicTitle
			 && CurrDocRef->docstarttext) {
				FlushText();
				tgp->add(*((DCctl *) CurrDocRef->docstarttext));
			}
			else if (CurrElemInfo->checkType((epRoot << 8) | epTopic)
			 && (((DCtopelem *) CurrElemInfo)->topic->mref == CurrDocRef)
			 && CurrDocRef->docendtext) {
				FlushText();
				tgp->add(*((DCctl *) CurrDocRef->docendtext));
			}
		}
	}
#endif

	if (CurrElemInfo
	 && CurrElemInfo->lang
	 && (LangLevel > 1)
	 && (depth == LangDepthStack.find(LangLevel))) { // restore previous lang
		DCirfile *lfile = (DCirfile *) LangStack.find(LangLevel - 1);
		CurrLang = LangAttrStack.find(LangLevel - 1);
		LangStack.remove(LangLevel);
		LangDepthStack.remove(LangLevel);
		LangAttrStack.remove(LangLevel);
		LangLevel--;
		RestoreLangFile(lfile);
		LangFile = lfile;
		LanguageText = lfile->getFilePath();
	}

	if (SkipTopic 
	 || (ExcludeDepth
	  && (depth >= ExcludeDepth))) {

		//if (!strcmp(name, DocRoot)  // decide per CurrDocChunk
		// && SkipNesting)
		//	SkipNesting--;

		IsFirst = false;
		CurrEInfo.remove(depth);
		CurrElems.remove(depth);
		CurrDepth = depth - 1;
		return;
	}

	if (SkipMapRef
	 && (depth < SkipMapRefDepth)) {
		SkipMapRef = false;
		SkipMapRefDepth = 0;
	}

	if (MapDescDepth
	 && (depth < MapDescDepth))
		MapDescDepth = 0;

	//if (UMXL
	// && CurrElemInfo->checkType((epDef << 8) | epVar)) {
		// content is a variable definition
		// most likely brought in by a var reference, just a wrapper
	//	return;
	//}

	bool wasInline = IsInline;

	if (CurrElemInfo) {
		if (CurrElemInfo->keyref
		 && CurrElemInfo->elemtext
		 && CurrElemInfo->isEmpty) {
			if (CurrElemInfo->keytext
			 && CurrElemInfo->keytext->first != NULL) {
				SetElemFormat();
				if (CurrElemInfo->href) {
					XrefHref = CurrElemInfo->href;
					StartXref(depth);
					StartXrefContent();
				}
				CurrElemInfo->elemtext->add(*CurrElemInfo->keytext);
				if (CurrElemInfo->href) {
					EmptyXref = false;
					EndXref();
				}
			}
			else if (PendXref)
				StartXref(depth);
			else if (CurrElemInfo->checkType(epText, epRef))
				LogEvent(logwarn, 1, "Keyref text missing for ", CurrElemInfo->elemname,
					" for key ", CurrElemInfo->keyref);
		}

#ifdef DITA
		if (CurrElemInfo->tm) {
			FlushText();
			switch (CurrElemInfo->tm) {
				case tmtm:
					PutMarkFormat("TradeMark");
					break;
				case tmreg:
					PutMarkFormat("RegMark");
					break;
				case tmsm:
					PutMarkFormat("ServiceMark");
					break;
				default:
					break;
			}
		}
#endif

		if (CurrElemInfo->checkType(epTypo)) {
			FlushText();
			paraend = false;
			if (CurrElemInfo->elform)  // used format for typo
				wasInline = true;
			else {
				SetTypographic(name, false);  // flush and turn off prop
				wasInline = false;
			}
		}

		if (InKeyDef) {
#ifdef UDOC
			if (CurrElemInfo->checkType((epDef << 8) | epKey)
#endif
#ifdef DITA
			if (CurrElemInfo->checkType(((epRef << 16) | (epTopic << 8) | epMap), epMeta)
#endif
			) {
				while (KeyDefForm) {
					SetFormat(NULL, true);
					KeyDefForm--;
				}
				InKeyDef = false;
			}
			else if (CurrElemInfo->checkType(epTypo)
			 || CurrElemInfo->checkType(epTmark))
				KeyDefForm = 0;
			else if (CurrElemInfo->checkType(epText)) {
				while (KeyDefForm) {
					SetFormat(NULL, true);
					KeyDefForm--;
				}
			} 
		}

#if 0
		if (UMXL   // xref with type text
		 && CurrElemInfo->checkType(epRef | (epInline << 8), epTopic | (epElem << 8))
		 && !PendXref
		 && !InImageMap
		 && TextRefType
		 && !stricmp(TextRefType, "text"))
			GetTextRef();
		else 
#endif

		if (CurrElemInfo->checkType((epRef | (epInline << 8)), epTopic | (epPre << 8)) 
		 && PendXref
		 && !InImageMap) {
			//if (PendingText  // prevented empty tags from working
			// && !InXref)
			if (!InXref)
				StartXref(depth);
			else
				PendXref = false;
		}
		else if (PendXref
		 && CurrElemInfo->keyref
		 && CurrElemInfo->href
		 && PendingText
		 && !InXref)
			StartXref(depth);

		if (CurrElemInfo->checkType((epRef | (epInline << 8)), (epImage << 8) | epTopic)
		 && !CurrElemInfo->href)  // a failed xref
			paraend = wasInline = false;

	}

	// ensure elems started
	if (TextGroup) {
		if (BlockElemCount > BlockElemsStarted)
			SetBlockElemTags();
		if (InlineElemCount > InlineElemsStarted)
			SetInlineElemTags();
		if (MapElemCount > MapElemsStarted)
			SetMapElemTags();
	}

	// artifact after previous closing tag
	if ((PendingLen == 1)
	 && (*PendingText == ' ')
	 && !wasInline) {
		PendingText = NULL;
		PendingLen = 0;
	}

	// flush any remaining text for current elem
	// may be more coming for parent
	if (CurrElemInfo) {
		if (TextGroup)
			FlushText();
		if (InMap
		 && !IsInline
		 && InMapDescGroup) {
			EndMapGroup(sqdesc);
			ProcessVariableEnd(CurrElemInfo);
			if (((TextGroup == PreTopicmeta)
			  || (CurrTopicref
			   && (TextGroup == CurrTopicref->mapdesc)))
			 && (((DCmapelem *) (CurrElemInfo->parent))->metatext != NULL))
				TextGroup = ((DCmapelem *) (CurrElemInfo->parent))->metatext;
		}
		else
			ProcessVariableEnd(CurrElemInfo);
	}

	if (CurrTaskHeadDepth >= depth) {
		CurrTaskHeadDepth = 0;
		CurrTaskHeadType = thnone;
		CurrTaskHeadElem = NULL;
		CurrTaskHeadUsed = false;
	}

	ForcedClose.remove(depth + 2);
	ForcedClose.remove(depth + 1);
	FormatUsed.remove(depth + 1);

	
	if (CurrElemInfo
	 && CurrElemInfo->checkType(epTab, epInline)) // tab def
		ClearTabs();

	if (CurrElemInfo
	 && CurrElemInfo->checkType(epPre)
	 && (PreformDepth >= depth)) {
		PreInline = IsPreform = StartingPre = false;
		PreformDepth = 0;
	}

#ifdef UDOC
	if (CurrElemInfo) {
		if (CurrElemInfo->checkType(epOut)) {
			if (XMLud::OutputKeys)
				XMLud::ProcOutputKeys();
			XMLud::InOutput = false;
			XMLud::UseOutput = false;
			XMLud::SourceKeys = false;
			XMLud::OutputKeys = false;
			XMLud::OutputKeyref = NULL;
			XMLud::OutputKeyID = NULL;
		}
		else if (XMLud::InOutput
		 && !XMLud::UseOutput)
			;
		else if (CurrElemInfo->checkType((epRef << 8) | epElem, (epDef << 8) | epInline)
		 && TextRefGroup) {  // textref element
			GetTextRef(trtext);
		}
		else if (CurrElemInfo->checkType((epInline << 16) | (epText << 8) | epVar)) {
			GetTextRef(trvar);
			wasInline = false;
		}
		else if (CurrElemInfo->checkType((epRef << 8) | epKey, epDef)) {  // keyref
			if (!XMLud::InOutput
			 || XMLud::SourceKeys)
				GetTextRef(trkey);
			textGroupSet = true;
			paraend = false;
		}
		else if (CurrElemInfo->checkType((epRef | (epDir << 8)))) {
			GetQueryRef(qudir);
			wasInline = false;
			textGroupSet = true;
			paraend = false;
		}
		else if (CurrElemInfo->checkType((epRef | (epDB << 8)))) {
			GetQueryRef(qudb);
			wasInline = false;
			textGroupSet = true;
			paraend = false;
		}
		else if (CurrElemInfo->checkType((epRef | (epWeb << 8)))) {
			GetQueryRef(quweb);
			wasInline = false;
			textGroupSet = true;
			paraend = false;
		}

		else if (CurrElemInfo->checkType((epRef << 8) | epVar)) {
			textGroupSet = true;
			paraend = false;
		}
		else if (CurrElemInfo->checkType((epDef << 8) | epList)) {
			if (MapLevelAdjust > 0)
				MapLevelAdjust--;
		}
		else if (CurrElemInfo->checkType((epDef << 8) | epVar)) {
		// content is a variable definition
		// most likely brought in by a var reference, just a wrapper
			if (CurrElemInfo->idref
			 && (*CurrElemInfo->idref == '$')) { // system var
				char *txt = NULL;
				if (CurrElemInfo->idref[1] == '$') { // $$ is macro var call
					// put in a code marker
					uns len = strlen(CurrElemInfo->idref) + 8;
					txt = NewName("code:<", len);
					strcat(txt, CurrElemInfo->idref);
					strcat(txt, ">");
					DCctl *cp = new DCctl(etext, 2, 110, 3);
					cp->ex(txt, len);
					TextGroup->add(*cp);
				}
				else if (!stricmp(CurrElemInfo->idref + 1, "date")) {
					const char *dform = "%#c";
					if (CurrElemInfo->type)
						dform = CurrElemInfo->type;
					txt = XMLud::GetSystemTime(dform);
					ProcChar(txt, strlen(txt), CurrDepth);
				}
#if 0
				else if (!stricmp(CurrElemInfo->idref + 1, "mdate")) {
					LPWIN32_FILE_ATTRIBUTE_DATA dat = new WIN32_FILE_ATTRIBUTE_DATA;
					GetFileAttributesEx(filename, GetFileExInfoStandard, dat);
					FILETIME ft = dat->ftLastWriteTime;
					// turn this into a string somehow ****
				}
#endif
			}
			textGroupSet = true;
			paraend = false;
		}
		else if (CurrElemInfo->checkType((epRef << 8) | epGloss)) {
			textGroupSet = true;
			paraend = false;
		}
		else if (CurrElemInfo->checkType((epRef << 8) | epAbbr)) {
			textGroupSet = true;
			paraend = false;
		}
		else if (CurrElemInfo->checkType(epCond))
			XMLud::EndCondition(CurrElemInfo);
		else if (CurrElemInfo->checkType(epListItem)) {
			InListItem--;
			if (InPairList
			 && (InPairList == InList)) {
				InPairListTerm = InPairListDef = InList - 1;;
			}
		}
		else if (CurrElemInfo->checkType(epList)) {
			if (InPairList
			 && (InPairList == InList)) {
				long lnum = PairListLevels.pop();
				if ((lnum == InList)  // get previous level that is a pair list
				 && ((InPairList = PairListLevels.pop()) > 0)) {
					PairListLevels.add(InPairList);  // put back current level
					InPairListTerm = InPairListDef = InPairList - 1;
				}
				else 
					InPairList = InPairListTerm = InPairListDef = 0;
			}
			if (InList > 0)
				InList--;
			InListItem = InList;
		}

		if (stricmp(CurrElemInfo->elemname, name))
			CurrElemInfo->elemname = (char *) name;
		if (CurrElemInfo->checkType((epGroup << 8) | epNolev)) {
			if (CurrElemInfo->checkType(epMap)) {  // end of branch
				char *bname = NULL;
				CurrBranchLevel = 0;
				for (long i = CurrDepth - 1; i > 0; i--) {
					if ((bname = (char *) BranchLevels.find(i)) != NULL) {
						CurrBranchLevel = i;
						BranchLevels.remove(i);
						break;
					}
				}
				CurrBranch = bname;
			}
			textGroupSet = true;
			paraend = false;
		}
	}
#endif

	if (CurrElemInfo
	 && InMapTopicmeta) {
		if (CurrElemInfo->checkType(epNavtitle)) {
		//!strcmp(name, "navtitle")
			FlushText();
			TextGroup = PreNavtitle;
			CurrTopicref->navtitle = MapNavtitle->getText();
			if (CurrTopicref->href
			 && !strcmp(CurrTopicref->href, "none"))
				CurrTopicref->href = CurrTopicref->navtitle;
#if 0
			if (CurrTopicref->navtxt
			 && !strncmp((char *) CurrTopicref->navtxt->ex(), "none",
			             CurrTopicref->navtxt->siz()))
#else
			if (!CurrTopicref->navtxt)
				CurrTopicref->navtxt = PutTextStrCtl(CurrTopicref->navtitle,
				                              strlen(CurrTopicref->navtitle));
			else
#endif
				CurrTopicref->navtxt->ex(CurrTopicref->navtitle);
			TextGroup->merge(*MapNavtitle);
			MapNavtitle = NULL;
		}
	}

	if (CurrElemInfo
	 && InReltable) {
		if (CurrElemInfo->checkType((epRel << 16) | (epMap << 8) | epTable)) {
			InReltable = false;
			TextGroup = PreRelTableTextGroup;
			textGroupSet = true;
		}
		else if (CurrElemInfo->checkType((epTabRow << 8) | epTabBody))
			ProcRelRowEnd();
	}
	else if (TextGroup) {
		if (InXref
		 && !InImageMap
		 && !InGlossTerm
		 && EmptyXref) {
			EndXref(name);
			paraend = wasInline = false;
		}
		else if (IncludeElementTags) {
			cp = new DCctl(ename, 2, 121, 3);
			cp->ex(name);
			TextGroup->add(*cp);
		}
	}

	if (InXref
	 && !InImageMap
	 && !InGlossTerm
	 && (XrefDepth == depth)) {
		EndXref(name);
		//if (CurrLinkInfo)
		//	CurrLinkInfo->fnctl->maj((uns) 0);
		if (CurrXrefInfo)
			CurrXrefInfo->fnctl->maj((uns) 0);
		paraend = wasInline = false;
	}

	if (DocRoot
	 && !strcmp(name, DocRoot)) {
		DocRoots.remove(CurrDocLevel);
		if (--CurrDocLevel > 0)
			DocRoot = (char *) DocRoots.find(CurrDocLevel);
		else {
			CurrDocLevel = 1;
			DocRoot = NULL;
			InReltable = false;
		}
		if (!InMap)
			EndPage();
	}

	//if (!strcmp(name, "dita"))   // wrapper for topics
	//	return;                    // at end of file


	// processing for other element ends like fig

	if (!CurrElemInfo)
		return;

	if (CurrElemInfo->checkType((epRoot << 8) | epMap)) {
		// map or bookmap
#ifdef UDOC
		//if (UMXL)
			XMLud::RestoreCondSet();
		//else
#endif
#ifdef DITA
			RestoreConditionSet();
#endif
	}
	else if (CurrElemInfo->checkType((epList << 8) | epMap, (epText << 8) | epRef)) {
		// booklists, frontmatter, backmatter, bookid
#ifdef UDOC
		//if (UMXL)
			XMLud::RestoreCondSet();
		//else
#endif
#ifdef DITA
			RestoreConditionSet();
#endif
		if (MapLevelAdjust > 0)
			MapLevelAdjust--;
	}
	else if (CurrElemInfo->checkType((epTopic << 16) | (epKey << 8) | epRef)) {
		// keydef
#ifdef UDOC
		//if (UMXL)
			XMLud::RestoreCondSet();
		//else
#endif
#ifdef DITA
			RestoreConditionSet();
#endif
		CheckMapShortdesc((DCmapelem *) CurrElemInfo);
		CurrTopicref = NULL;
	}
	else if (CurrElemInfo->checkType((epTopic << 16) | (epMap << 8) | epMeta)) {
		// topicmeta
		if (InMapTopicmeta) {
			if ((TextGroup == ((DCmapelem *) CurrElemInfo)->metatext)
			 && (PreTopicmeta != NULL)) {
				TextGroup = PreTopicmeta;
				textGroupSet = true;
			}
			InMapTopicmeta = false;
		}
	}
	else if (CurrElemInfo->checkType((epTopic << 8) | epMap)) {
		// topicref, topichead, topicgroup
#ifdef UDOC
		//if (UMXL) {
			XMLud::RestoreCondSet();
			InCodedocref = false;
			InExternDoc = false;
		//}
		//else
#endif
#ifdef DITA
			RestoreConditionSet();
#endif
		CheckMapShortdesc((DCmapelem *) CurrElemInfo);
		CurrTopicref = NULL;
		//if (PreLinkTextGroup) {
		//	TextGroup = PreLinkTextGroup;
		//	PreLinkTextGroup = NULL;
		//}
		CurrRelLinkTopicref = NULL;
	}
	// end of map elements
	else if (CurrElemInfo->checkType(epSDesc, epMap)) { // shortdesc
	//else if (CurrElemInfo->checkType(UMXL ? epDesc : epSDesc, epMap)) { // shortdesc
		if (MapDescNeeded)   // end text cloning now
			EndMapShortdesc();
		//textGroupSet = true;
	}
	else if (CurrElemInfo->checkType(epFtn)) {
		paraend = wasInline = false;
		EndFootnote();
		textGroupSet = true;
	}
	else if (CurrElemInfo->checkType((epInline << 8) | epSeq)) {
		if (CurrElemInfo->checkType(epText)) {  // cascade sequence
			InCascade = false;
		}
		else {  // cascade separator
			if (InCascade) {
				if (PendingText)
					FlushText();
				if (CascadeSeparator) {
					char *cs = DCmacr::GetMacro(CascadeSeparator);
					PutTextCtrls(cs, strlen(cs));
				}
				else
					PutTextStrCtl(" | ", 3);
			}
		}
	}
	else if (CurrElemInfo->checkType(epCascadeSet)) { // menucascade
		InCascade = false;
		CascadeStarted = false;
	}
	else if (CurrElemInfo->checkType(epTitle)) {
#ifdef UDOC
		if (XMLud::InMXMLIndexEntry)
			XMLud::EndUMXLIndexItem();
#endif
		if (InTableTitle)
			EndTableTitle();

		InFigTitle = false;
		ReadTopicTitle = false;
		if (InList
		 && (InPairList == InList)
		 && (InPairListTerm == InPairList))
			InPairListTerm--;
	}
	else if (CurrElemInfo->checkType((epMeta << 8) | epGroup)) {
		TextGroup = PrePrologText; // end of prolog
		PrePrologText = NULL;
		textGroupSet = true;
	}
	else if (CurrElemInfo->checkType((epList << 8) | epNumbered)) {
		InAnumElem = false;  // end of an ol
	}
	else if (CurrElemInfo->checkType(epRel, epMap)) {  // related-links
		InRelLinks = false;
		RelTopicAlinkRefs = NULL;
		//TextGroup = PreLinkTextGroup;
		if (PreLinkTextGroup) {
			TextGroup = PreLinkTextGroup;
			PreLinkTextGroup = NULL;
		}
		textGroupSet = true;
	}
	else if (CurrElemInfo->checkType(((epLink << 8) | epList), epSeq)) {
		InLinkPool = false;
	}
	else if (CurrElemInfo->checkType((epLink << 8) | epList)) {
		InLinkList = false;
	}
	else if (CurrElemInfo->checkType(epLink, epMap)) {
		CurrRelLink = NULL;
		//TextGroup = PreLinkTextGroup;
		if (PreLinkTextGroup) {
			TextGroup = PreLinkTextGroup;
			PreLinkTextGroup = NULL;
		}
		textGroupSet = true;
	}
	else if (CurrElemInfo->checkType((epStart << 8) | epTable, epRel)) {
		if (CurrElemInfo->checkType(epDefList)
		 && !InDefinitionList)
			;
		else if (CurrElemInfo->checkType(epParmList)
		 && !InParameterList)
			;
		else {
			EndTable();
			InTableCell = false;
			InTableRow = false;
			InTableTitle = false;
			textGroupSet = true;
			paraend = wasInline = InParameterList = InDefinitionList = false;
		}
	}
	else if (!InReltable
	 && CurrElemInfo->checkType(epTabRow, epShort)) {
		InDefinitionEntry = InParameterEntry = false;
		if (CurrElemInfo->checkType(epDefEntry)
		 && !InDefinitionList)
		;
		else if (CurrElemInfo->checkType(epParmEntry)
		 && !InParameterList)
			;
		else {
			EndTableRow();
			InTableCell = false;
			InTableRow = false;
		}
	}
	else if (!InReltable
	 && CurrElemInfo->checkType(epTabCol)) {
		EndTableCol();
	}
	else if (!InReltable
	 && CurrElemInfo->checkType(epTabCell, epShort)) {
		if (CurrElemInfo->checkType(epDefTerm)
		 && !InDefinitionTerms)
			;
		else if (CurrElemInfo->checkType(epDefDef)
		 && !InDefinitionDefs)
			;
		else if (CurrElemInfo->checkType(epParmTerm)
		 && !InParameterTerms)
			;
		else if (CurrElemInfo->checkType(epParmDef)
		 && !InParameterDefs)
			;
		else {
			EndTableCell();
			InTableCell = false;
		}
	}
	else if (!InReltable
	 && InTable
	 && TableGroup
	 && TableTitleGroup) {
		EndTableTitle();
		InTableTitle = false;
	}
	else if (CurrElemInfo->checkType(epFig)) {
		if (CurrElemInfo->checkType((epGroup << 8) | epRef)) {// imagemap
			EndImageMap();
			InXref = PendXref = wasInline = false;
		}
		//FlushText();
		ProcFigEnd();
	}
	else if (CurrElemInfo->checkType(epImage)) {
		if (CurrElemInfo->checkType(epRef))  // area
			EndImageArea();
		else if (CurrElemInfo->checkType(epAlt))
			InAlt = false;
		else if (!CurrElemInfo->checkType(epText))
			ProcImageEnd();
		paraend = wasInline = false;
	}
	else if (CurrElemInfo->checkType(epAlt))
		InAlt = false;
	else if (CurrElemInfo->checkType(epDesc)) {
		FlushText();
		if (TopicDesc) {  // **** 
			if (CurrElemInfo->parent)
				CurrElemInfo->parent->tdesc = TopicDesc;
			TopicDesc = NULL;
		}
		InDesc = false;
	}
	else if (CurrElemInfo->checkType(epPre, epInline | (epRef << 8)))
		InCodeblock = false;
	else if (CurrElemInfo->checkType(epObj)) {
		if (depth == ObjectDepth) {
			EndObject();
			ObjectDepth = 0;
		}
	}
#ifdef UDOC
	else if (CurrElemInfo->checkType((epInline << 16) | (epData << 8) | epIndex)) {
		XMLud::EndUMXLIndexItem();
		textGroupSet = true;
		paraend = wasInline = false;
	}
#endif
#ifdef DITA
	else if (CurrElemInfo->checkType((epStart << 8) | epIndex)) {
		EndIndexItem();
		textGroupSet = true;
		paraend = wasInline = false;
	}
#endif
	else if (CurrElemInfo->checkType(epIndex, epList)) {
		ProcIndexElemEnd();
		textGroupSet = true;
		paraend = wasInline = false;
	}
	else if (CurrElemInfo->checkType(epGloss)) {
		if (CurrElemInfo->checkType(epRoot))
			InGlossEntry = false;
#ifdef UDOC
		else if (CurrElemInfo->checkType(epDef))
			InGlossEntry = false;
#endif
		else if (CurrElemInfo->checkType((epAbbr << 8) | epInline))
			InGlossAbbr = false;  //DITA
#ifdef UDOC
		else if (CurrElemInfo->checkType((epText << 8) | epInline)) {
		//else if (InGlossTerm) {
			XMLud::ListElemRefEnd(CurrElemInfo, gengloss);
		}
#endif
		textGroupSet = true;
		paraend = wasInline = false;
	}
	else if (CurrElemInfo->checkType((epAbbr << 8) | epInline)) {	  // UMXL
#ifdef UDOC
		XMLud::ListElemRefEnd(CurrElemInfo, genabbr);
#endif
		textGroupSet = true;
		paraend = false;
	}
#ifdef UDOC
	else if (CurrElemInfo->checkType((epTmark << 8) | epInline)) {
		XMLud::ListElemRefEnd(CurrElemInfo, gentmk);
		textGroupSet = true;
		paraend = false;
	}
#endif

	if (TitleGroup
	 && !IsInline) {
		TitleGroup = NULL;
		InTitleElem = false;
	}

	FormatUsed.remove(depth);
	FormatInline.remove(depth);
	AllowTextLevels.remove(depth);

#ifdef DITA
	// at end of conref replacement
	if (ProcessingT2TConrefs
	 && CurrConrefT2TReplacement
	 && (CurrConrefT2TReplacement->srcelem == (DCtopelem *) CurrElemInfo)) {
		AnumElemList = &ConrefAnumList;
		LOFItemList = &ConrefLOFList;
		LOTItemList = &ConrefLOTList;
		CurrConrefT2TReplacement->didlists = true;
		//SkipTopic = true;
	}

	// at end of element being replaced by conref
	if (CurrElemInfo->conrefnum && !InMap) {
		AnumElemList->append(-(CurrElemInfo->conrefnum));
		if (GenerateLOF)
			LOFItemList->append(-(CurrElemInfo->conrefnum));
		if (GenerateLOT)
			LOTItemList->append(-(CurrElemInfo->conrefnum));
		MarkConrefGenList(-(CurrElemInfo->conrefnum));
	}
#endif

	// restore prev element properties
	IsInline = FormatInline.find(depth - 1);
	AllowText = AllowTextLevels.find(depth - 1);

	if (wasInline //&& !IsInline) **********
	 && TextGroup
	 && !InImageMap
	 && !InReltable) {
		if (!CurrElemInfo->elskip) {
			SetEndTextGroup();
			cp = new DCctl(imnem, 2, 82, 2, 0);
			TextGroup->add(*cp);
		}
		SetFlags(depth - 1);
	}

	CurrFormat = (char *) FormatUsed.find(depth - 1);

	if (!wasInline && !IsInline)
		IsFirst = false;

#if 0
	if (EndAtElement) {  // may be at end of conref or xref content
		if ((EndingElemDepth == depth)
		 && !strcmp(name, EndingElemName))
			// is at end of proc, either end processing or start skipping
			SkipTopic = true;
	}
#endif

	if (!stricmp(CurrElemInfo->elemname, name)
	 && (CurrElemInfo->elemdepth == depth)) {
		if (CurrElemInfo->elemtext  // end of element to be replaced
		 && TextGroup
		 && !textGroupSet)  {
			if (!IsInline
			 && !wasInline
			 && paraend
			 && ParaStarted)
				EndPara();
			TextGroup = CurrElemInfo->lasttext ?
			  CurrElemInfo->lasttext : PageTextGroup;  // restore normal group
		}
		CurrElemInfo = (DCelem *) CurrEInfo.find(depth - 1);
	}
	else
		LogEvent(logwarn, 1, "Current element not on top of stack: ",
			name, " at depth ", itoa(depth, NewName(10), 10));

	CurrEInfo.remove(depth);
	CurrElems.remove(depth);
	CurrDepth--;
}

#ifdef UDOC

void
XMLrd::SetTextRef(DCelem *celem, trtyp typ)
{
	TextRefType = typ;
	FlushText();
	PreTextRefGroups.add(TextGroup, CurrDepth);
	TextRefGroup = new DCgroup();
	TextGroup = TextRefGroup;  // catch the text to use for failure
	TextRefIDs = NewName(celem->idrefs ? celem->idrefs : celem->idref);
}


void
XMLrd::GetTextRef(trtyp typ)
{
	if (typ != TextRefType) {
		LogEvent(logwarn, 1, "Type mismatch for textref");
		TextRefType = typ;
	}

	DCelem *celem = CurrElemInfo;
	DCgroup *tgp = TextGroup;
	long i = 0;
	bool itf = InTextref;
	bool found = false;
	char *refids = TextRefIDs;
	bool isVar = (TextRefType == trvar);
	bool isCond = (TextRefType == trcond);

	FlushText();
	TextGroup = (DCgroup *) PreTextRefGroups.find(CurrDepth);
	DCgroup *ptgp = TextGroup;
	PreTextRefGroups.remove(CurrDepth);
	InTextref = true;
	if (!celem->href) {  // use current file or use default
		if (refids            // try current file first
		 && XMLud::GetUMXLElement(NULL, refids, isVar))
			found = true;
		if (!found
		 && isVar) {              // try var library
			for (i = 1; i <= VarLibCount; i++) { 
				celem->href = VarLibPaths.find(i);
				if (!XMLud::ParsedFiles.getitem(celem->href))
					XMLrd::LogEvent(loginfo, 1, "Reading variables from: ", celem->href);
				found = XMLud::GetUMXLElement(celem->href, refids, true);
				if (found)
					break;
			}
			if (!found) {
				celem->href = XMLud::GetLibPath(XMLud::StdVarLib);
				if (!XMLud::ParsedFiles.getitem(celem->href))
					XMLrd::LogEvent(loginfo, 1, "Reading variables from: ", celem->href);
				found = XMLud::GetUMXLElement(celem->href, refids, true);
			}
		}
		if (!found) {
			LogEvent(logwarn, 1, "No src specified for ",
			 isVar ? "variable " :
			 (isCond ? "condition set " : "textref "),
			 refids ? refids : "and no idref either");
			TextGroup->add(*TextRefGroup);
		}
	}
	else {  // has @src
		if (isVar)
			XMLrd::LogEvent(loginfo, 1, "Reading variable ",
				              (refids ? refids : ""), " from: ", celem->href);
		else if (isCond)
			XMLrd::LogEvent(loginfo, 1, "Reading conditions set ",
				              (refids ? refids : ""), " from: ", celem->href);
		else
			XMLrd::LogEvent(loginfo, 1, "Reading included content from: ", celem->href);
		found = XMLud::GetUMXLElement(celem->href, refids);
		if (TextRefGroup) {
			if (found) {
				delete TextRefGroup;
				TextRefGroup = NULL;
			}
			else
				TextGroup->add(*TextRefGroup);
		}
	}
	if (TextRefIDs) {
		DeleteName(TextRefIDs);
		TextRefIDs = NULL;
	}
	TextRefType = trnone;
	InTextref = itf;
	CurrElemInfo = celem;
	if (!TextGroup)
		TextGroup = tgp;
	else if (TextGroup != ptgp) {
		//ptgp->add(*TextGroup);
		TextGroup = ptgp;
	}
}


void
XMLrd::SetQueryRef(DCelem *elem, qutyp typ,
		               const char *query, const char *results)
{
	QueryRefType = typ;
	FlushText();

	// set variables for query from elem->href and @query
	if (!query) {
		LogEvent(logwarn, 1, "No query text specified");
		QueryRefType = qunone;
		return;
	}

	if (!elem->href) {
		LogEvent(logwarn, 1, "No source specified for query ", query);
		QueryRefType = qunone;
		return;
	}

	Query = NewName(query);
	QueryResultsPath = NewName(results);
	PreQueryRefGroups.add(TextGroup, CurrDepth);
	QueryRefGroup = new DCgroup();
	TextGroup = QueryRefGroup;  // catch the text to use for failure
	QueryParent = elem->parent;
	if (InMap)
		MapLevelAdjust++;

	// run query, which adds results to DCnlist QueryResult
	RunQuery(elem->href, Query, &QueryResult);
}


void
XMLrd::RunQuery(char *src, char *query, DCnlist *dcn)
{
	long i = 0;

	if (QueryRefType == qudir) {
		// get curr dir, set it to src
		int curdrive = _getdrive();
		char buffer[_MAX_PATH];
		_getcwd(buffer, _MAX_PATH);
		if (_chdir(src))
      LogEvent(logwarn, 1, "Unable to locate the directory: ", src);
		else {
			// call findfirst with query
			struct _finddata_t c_file;
			long hFile;
			hFile = _findfirst(query, &c_file);
			char *fname = NULL;
			if (hFile != -1L ) {
				// while found, store full path, call findnext
				fname = NewName(src, strlen(src) + strlen(c_file.name) + 2);
				strcat(fname, "\\");
				strcat(fname, c_file.name);
				dcn->add(fname, ++i);
				while (_findnext(hFile, &c_file) == 0) {
					fname = NewName(src, strlen(src) + strlen(c_file.name) + 2);
					strcat(fname, "\\");
					strcat(fname, c_file.name);
					dcn->add(fname, ++i);
				}
				// call findclose
				_findclose(hFile);
			}
		}
		// restore curr dir
		_chdrive(curdrive);
		_chdir(buffer);
	}
	else {
		// for testing, make dummy items
		char *item00 = NewName("item  ");
		for (i = 1; i < 10; i++) {
			item00[5] = (char) ('0' + i);
			dcn->addnew(item00, i);
		}
		DeleteName(item00);
	}
}


void
XMLrd::GetQueryRef(qutyp typ)
{
	if (QueryRefType == qunone)
		return;

	if (typ != QueryRefType) {
		LogEvent(logwarn, 1, "Type mismatch for query");
		QueryRefType = typ;
	}

	DCelem *celem = CurrElemInfo;
	DCgroup *tgp = TextGroup;
	long i = 0;
	bool isDir = (QueryRefType == qudir);
	bool isDB = (QueryRefType == qudb);
	bool isWeb = (QueryRefType == quweb);

	FlushText();
	TextGroup = (DCgroup *) PreQueryRefGroups.find(CurrDepth);
	PreQueryRefGroups.remove(CurrDepth);

	// if no query results, use included text as default, else discard text
	if (!QueryResult.count()) {
		LogEvent(logwarn, 1, "No results for query of ",
		 isDir ? "directory " :
		 (isDB ? "database " : "Web "), Query, " from ", celem->href);
		TextGroup->add(*QueryRefGroup);
	}
	else {
		// depending on CurrElemInfo->parent, write out result of query
		// using SAX interface; default is an sl with an li for each
		DCnlist *qr = &QueryResult;
		char *ltag = "sl";
		char *itag = "li";
		char **attrs = NULL;
		char *name = QueryResultsPath;
		char *slash = NULL;
		char *parent = NULL;
		long plev = 0;
		long firstp = 1;
		DCnlist parents;

		if (QueryResultsPath) {  // elem path for results
			ltag = NULL;
			itag = NULL;
			slash = strchr(QueryResultsPath, '/');
			while (slash) {
				*slash = '\0';
				parent = slash + 1;
				parents.add(parent, ++plev);
				slash = strchr(parent, '/');
			}
			if (!stricmp(name, "docref")
			 || !stricmp(name, "codedocref")) {
				itag = name;
				attrs = new char*[6];
				attrs[0] = "src";
				attrs[1] = NULL;
				//attrs[2] = "id";
				attrs[2] = NULL;
				attrs[3] = NULL;
				attrs[4] = NULL;
				attrs[5] = NULL;
			}
			else if (!stricmp(name, "p")
			 && plev
			 && !stricmp(parents.find(1), "li")) {
				itag = "li";
				firstp = 2;
			}
			else
				itag = name;
		}
		if (plev) {
			for (i = plev; i >= firstp; i--) {
				ltag = parents.find(i);
				StartElement(ltag, NULL, CurrDepth + 1);
			}
		}
		else if (ltag)
			StartElement(ltag, NULL, CurrDepth + 1);
		//CurrDepth++;
		while ((qr = qr->next) != NULL) {
			if (attrs) {
				//char *bname = NewName(strlen((char *) qr->name));
				//GetBaseName((char *) qr->name, NULL, bname);
				attrs[1] = (char *) qr->name;
				//attrs[3] = bname;
			}
			StartElement(itag, (const char **) attrs, CurrDepth + 1);
			if (!attrs) {
				if (firstp == 2) // interpolate p
					StartElement("p", (const char **) attrs, CurrDepth + 1);
				ProcChar(qr->name, strlen(qr->name), CurrDepth + 1);
				if (firstp == 2) // interpolated p
					EndElement("p", CurrDepth);
			}
			EndElement(itag, CurrDepth);
		}
		//CurrDepth--;
		if (plev) {
			for (i = firstp; i = plev; i++) {
				ltag = parents.find(i);
				EndElement(ltag, CurrDepth);
			}
		}
		else if (ltag)
			EndElement(ltag, CurrDepth);
		QueryResult.clear();
		if (attrs)
			delete [] attrs;
	}

	// clean up
	DeleteName(Query);
	Query = NULL;
	DeleteName(QueryResultsPath);
	QueryResultsPath = NULL;
	QueryRefType = qunone;
	CurrElemInfo = celem;
	if (InMap
	 && (MapLevelAdjust > 0))
		MapLevelAdjust--;
}
#endif

void
XMLrd::PutMarkFormat(char *mark)
{
	if (!mark
	 || !TextGroup)
		return;

	DCgroup *gp = new DCgroup;
	WriteMarkItem(gp, mark);

	if (gp && gp->first)
		TextGroup->add(*gp);
}


bool
XMLrd::CheckElemName(const char *name, const char *classatt,
	                   unl bit, unl neg, bool all)
{
	//if (UMXL)
	//	return XMLud::CheckUMXLElemName(name, bit, neg, all);

	static DCelem *einfo = NULL;

	if (!classatt)
		return false;

	if (!einfo
	 || strcmp(einfo->classatt, classatt)) {
		if (einfo) {
			delete einfo->classatt;
			delete einfo;
		}
		einfo = new DCelem(0);
		einfo->classatt = NewName(classatt);
		einfo->epset = false;
		einfo->epcnt = 0;
		einfo->eprop = NULL;
	}

	// normalize the class attribute
	char *nclass = NormalizeClass(classatt);

	if (!nclass)
		return false;

	char *nptr = nclass + strlen(nclass);
	static char *lastclass = NULL;
	static etype **lastep = NULL;

	etype **ep = NULL;
	bool last = false;

	if (!einfo->epset) {  // have not checked this one before
		if (lastclass
		 && !strcmp(lastclass, nclass)) {  // same class as last
			ep = lastep;                     // use same properties
			last = true;
			DeleteName(nclass);
		}
		else {
			if (lastclass)
				DeleteName(lastclass);
			lastclass = nclass;

			etype *key = new etype;
			key->ename = nclass;

			etype **arr = ElemTypes;
			int cnt = ElemTypesCount;

			ep = (etype **) bsearch(&key, arr, cnt, sizeof(etype *),
																		CompElemTypes);

			if (!ep) {  // not in list, trim class attrs from right
				char *space = NULL;
				while (nptr > nclass) {
					while ((*nptr != ' ')
					 && (nptr > nclass))
						nptr--;             // nptr is at space between parts
					if (nptr > nclass) {  // use classes right to left
						char *elem = NewName(nclass, nptr - nclass);
						key->ename = elem;
						ep = (etype **) bsearch(&key, arr, cnt, sizeof(etype *),
																 CompElemTypes);
						DeleteName(elem);
					}
					if (ep)  // found base class of current class
						break;
					nptr--;  // move off the space
				}
			}
			delete key;
			lastep = ep;
		}

		if (ep) {  // found, store it
			einfo->epcnt = (*ep)->cnt;
			einfo->eprop = (*ep)->eprop;
			einfo->epset = true;
		}
	}

	if (einfo->epset)
		return einfo->checkType(bit, neg, all);
	else {
		if (!last)
			LogEvent(logwarn, 1, "Properties not found for ", einfo->classatt);
		einfo->epset = true;  // avoid another failed lookup
	}

	return false;
}



char *XMLrd::NormalizeClass(const char *eclass)
{
	// normalize the class attribute

	if ((*eclass == '-')
	 || (*eclass == '+'))
		eclass++;

	while (isspace(*eclass))
		eclass++;

	const char *ecend = eclass + strlen(eclass) - 1;

	while (isspace(*ecend))
		ecend--;

	char *nclass = NewName((ecend - eclass) + 1);
	char *nptr = nclass;
	const char *eptr = eclass;
	while (eptr <= ecend) {
		if (isspace(*eptr)) {
			*nptr++ = *eptr++;
		  while (isspace(*eptr))  // make spaces single
				eptr++;
		}
		*nptr++ = *eptr++;
	}
	*nptr = '\0';

	return nclass;
}


int __cdecl
XMLrd::CompElemTypes(const void *elem1, const void *elem2)
{
	const char *name1 = (*((etype **) elem1))->ename;
	const char *name2 = (*((etype **) elem2))->ename;

	return strcmp(name1, name2);
}


char *XMLrd::CascadeSeparator = " | ";
bool XMLrd::InCascade = false;
bool XMLrd::CascadeStarted = false;


char *XMLrd::Quotes = "“ ” ‘ ’";
uns XMLrd::QuoteLevel = 0;
unl XMLrd::QuoteChars[4];


void
XMLrd::SetQuotes(void)
{
	// set QuoteChars from Quotes
	unc *q = (unc *) Quotes;
	if (!q) {
		QuoteChars[0] = '"';
		QuoteChars[1] = '"';
		QuoteChars[2] = '\'';
		QuoteChars[3] = '\'';
		return;
	}
	long i = 0;
	for (i = 0; i < 4; i++) {
		while (*q && (*q == ' '))
			q++;
		if (*q
		 && (!*(q + 1)
		  || (*(q + 1) == ' '))) {
			switch (*q) {
				case 0x91: // singles
					QuoteChars[i] = 0x2018;
					break;
				case 0x92:
					QuoteChars[i] = 0x2019;
					break;

				case 0x93: // doubles
					QuoteChars[i] = 0x201C;
					break;
				case 0x94:
					QuoteChars[i] = 0x201D;
					break;

				default:  // convert on output side
					QuoteChars[i] = *q;
					break;
			}
			q++;
		}
		else if (::IsUTF8(q))
			QuoteChars[i] = ::UTF8(&q);
	}
	if (i == 2) {  // only one pair given, clone it
		QuoteChars[2] = QuoteChars[0];
		QuoteChars[3] = QuoteChars[1];
	}
}


DCctl *
XMLrd::SetTypographic(const char *name, bool start)
{
	DCctl *cp = new DCctl(imnem, 2, 15, 0, start ? 1 : 0);
	if (!strcmp(name, "b")) {
		cp->dat(4);
		cp->siz(start ? 4 : 0);
	}
	else if (!strcmp(name, "i"))
		cp->dat(5);
	else if (!strcmp(name, "u"))
		cp->dat(11);
	else if (!strcmp(name, "q")) {
		// quote; spec doesn't say typographic, but it is
		// cp->dat(17);
		cp->min(10);
		cp->dat(1);
		if (start) {
			QuoteLevel++;
			cp->siz(QuoteChars[(QuoteLevel & 1) ? 0 : 2]);
		}
		else {
			cp->siz(QuoteChars[(QuoteLevel & 1) ? 1 : 3]);
			QuoteLevel--;
		}			
	}
	else if (!strcmp(name, "sup")) {
		cp->dat(6);
		cp->siz(start ? 2 : 0);
	}
	else if (!strcmp(name, "sub"))
		cp->dat(6);
	else  // not typographic after all
		return NULL;

	if (TextGroup)
		TextGroup->add(*cp);
	if (TitleGroup
	 && !InIndex)
		TitleGroup->add(*cp);
	return cp;
}


elemtyp 
XMLrd::GetTopicNameType(const char *name)
{
	if (!strcmp(name, "topic"))
		return topic;
	if (!strcmp(name, "concept"))
		return concept;
	if (!strcmp(name, "task"))
		return task;
	if (!strcmp(name, "reference"))
		return reference;
	if (!strcmp(name, "glossentry"))
		return glossentry;
	return other;
}


elemtyp 
XMLrd::GetTopicType(const char *name, const char *classatt)
{
	// scan classatt for base type
	elemtyp etype = GetTopicNameType(name);

	if (etype != other)
		return etype;

	if (classatt) {  // not in list, use class attrs
		const char *slash = NULL;
		const char *space = NULL;
		const char *classend = classatt + strlen(classatt) - 1;
		while (classend > classatt) {
			while (*classend == ' ')
				classend--;
			slash = classend;
			while ((slash > classatt)
			 && (*slash != '/'))
				slash--;
			if (slash > classatt) {  // use classes right to left
				char *elem = NewName(slash + 1, classend - slash);
				etype = GetTopicNameType(elem);
				DeleteName(elem);
				if (etype != other)
					return etype;
			}
			while ((classend > classatt)
			 && (*classend != ' '))
				classend--;
		}
	}

	return topic;
}



#ifdef DITA
// recording and playing back elements


bool XMLrd::RecordingElements = false;
long XMLrd::RecElemDepth = 0;  // used to identify end of stream
DClist *XMLrd::RecList = NULL;


void 
XMLrd::RecElemStart(const char *name, const char **attp, long depth)
{
	elemdata *el = new elemdata;
	el->name = NewName(name);
	el->depth = depth;

	long count = 1;
	char *str = NULL;
	char **strp = (char **) attp;
	while (*strp++)
		count++;
	el->attrs = strp = (char **) new char[sizeof(char *) * count];
	while (*attp)
		*strp++ = str = NewName(*attp++);
	*strp = NULL;

	parsedata *pd = new parsedata;
	pd->type = parelem;
	pd->pdata = el;
	RecList->add((long) pd);
}


void 
XMLrd::RecElemEnd(const char *name, long depth)
{
	enddata *en = new enddata;
	en->name = NewName(name);
	en->depth = depth;

	parsedata *pd = new parsedata;
	pd->type = parend;
	pd->pdata = en;
	RecList->add((long) pd);
}


void 
XMLrd::RecText(const char *str, long len, long depth)
{
	textdata *tx = new textdata;
	tx->str = NewName(str, len);
	tx->len = len;
	tx->depth = depth;

	parsedata *pd = new parsedata;
	pd->type = partext;
	pd->pdata = tx;
	RecList->add((long) pd);
}


void 
XMLrd::RecPI(const char *target, const char *data)
{
	pidata *pi = new pidata;
	pi->target = NewName(target);
	pi->data = NewName(data);

	parsedata *pd = new parsedata;
	pd->type = parpi;
	pd->pdata = pi;
	RecList->add((long) pd);
}


void 
XMLrd::Playback(DClist *rec, long currdepth)
{
	parsedata *pd = NULL;
	elemdata *el = NULL;
	enddata *en = NULL;
	textdata *tx = NULL;
	pidata *pi = NULL;
	long firstdepth = 0;
	long depth = 0;
	char *str = NULL;
	char **strp = NULL;

	while ((pd = (parsedata *) rec->dequeue()) != NULL) {
		switch (pd->type) {
			case parelem:
				el = (elemdata *) pd->pdata;
				if (!firstdepth)
					firstdepth = el->depth;
				depth = (el->depth - firstdepth) + currdepth;
				StartElement(el->name, (const char **) el->attrs, depth);
				strp = el->attrs;
				while ((str = *strp++) != NULL)
					DeleteName(str);
				delete [] el->attrs;
				DeleteName(el->name);
				delete el;
				break;
			case parend:
				en = (enddata *) pd->pdata;
				depth = (en->depth - firstdepth) + currdepth;
				EndElement(en->name, depth);
				DeleteName(en->name);
				delete en;
				break;
			case partext:
				tx = (textdata *) pd->pdata;
				depth = (tx->depth - firstdepth) + currdepth;
				ProcChar(tx->str, tx->len, depth);
				DeleteName(tx->str);
				delete tx;
				break;
			case parpi:
				pi = (pidata *) pd->pdata;
				ProcPI(pi->target, pi->data);
				DeleteName(pi->target);
				DeleteName(pi->data);
				delete pi;
				break;
			default:
				break;
		}
		delete pd;
	}
}
#endif



// end of drxmlel.cpp

