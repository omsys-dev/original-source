// dwhtmpp.cpp is the code for dcl HTM private profiles, JHG
// Copyright (c) 1996-1999 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcfilp.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwhtm.h"
#include "dwhtmfl.h"



bool HTMwr::ListMissingRefs = false;
bool HTMwr::CheckAllRefs = true;
bool HTMwr::ExtRefUsed = false;
char HTMwr::RefBuf[RefBufLen];
char *HTMwr::LinkID = NULL;
char *HTMwr::LinkRefFile = NULL;
DCvlist HTMwr::LinkFixupList;
long HTMwr::LinkFixupNum = 0;
link **HTMwr::FixupArray = NULL;
DCnlist HTMwr::IntRefList;
DClist *HTMwr::IntRefPend = NULL;

long HTMwr::IntRefCount = 0;
long HTMwr::ExtRefCount = 0;
long HTMwr::FilesFixed = 0;



void
HTMwr::WriteRefCount(void)
{
	if (!RefFileName)
		return;

	char nstr[NumStrMax] = { "" };
	DCpfile::putval("Versions", "IntRefCount", ltoa(IntRefCount, nstr, 10), RefFileName);
	DCpfile::putval("Versions", "ExtRefCount", ltoa(ExtRefCount, nstr, 10), RefFileName);
	DCpfile::putval("Versions", "FilesFixed", ltoa(FilesFixed, nstr, 10), RefFileName);
}


void
HTMwr::SetIntRef(char *txt)
{
	IntRefCount++;
	IntRefList.add(txt, Extracting ? ExtrID : SplitID);
	if (HXMwr::HXref) 
		HXMwr::DITASetIntRef(txt);
}

void
HTMwr::ProcIntRef(char *txt, long sid)
{
	if ((txt == NULL)
	 || (*txt == '\0'))
		return;

	bool ditaset = false;
	char sidRef[NumStrMax];
	sprintf(sidRef, "%ld", sid ? sid : 1);

	// process jump dest within current file, aname, during write phase
	// record it with its current SplitID (or 1 if not split)
	// in our own .ref in [IntRefs]

	DCpfile::getval("IntRefs", txt, "", RefBuf, RefBufLen, RefFileName);

	if (*RefBuf == '\0') {  // not in file, so not referenced yet, just write it
		DCpfile::putval("IntRefs", txt, sidRef, RefFileName);
		if (HXMwr::HXref)
			HXMwr::DITAProcIntRef(txt, true);
		//DCpfile::pclose(RefFileName);
		return;
	}
	else if (!CheckAllRefs
	 && !strncmp(RefBuf, sidRef, strlen(sidRef))) {  // unchanged
		if (!HXMwr::HXref
		 || !HXMwr::DITAProcIntRef(txt, false)) { // DITA info also unchanged
			//DCpfile::pclose(RefFileName);
			return;
		}
		ditaset = true;
	}

	// value has changed, update referencing files
	DCpfile::putval("IntRefs", txt, sidRef, RefFileName);

	if (HXMwr::HXref && !ditaset)  // different sid, not updated yet
		HXMwr::DITAProcIntRef(txt, false);  // update DITA info now

	// determine our new file name and its length
	char *fname = NewName(sid ? SplitNameList.find(sid) : CurrXFileName);
	size_t flen = strlen(fname);

	char *flink = NULL;
	size_t fllen = 0;
	if (HXMwr::HXref && HXMwr::DITA)
		flink = HXMwr::DITASetIntLink(txt, fname);
	else {
		flink = NewName(fname, strlen(fname) + strlen(txt) + 2);
		strcat(flink, "#");
		strcat(flink, txt);
	}
	fllen = strlen(flink);


	char *lptr = NULL;
	char *rptr = NULL;

	if (DCpfile::getkeys("ExtRefs", RefFileName)) {
		while (DCpfile::getnextkey(&lptr, &rptr)) {
	;		if (rptr 
			 && !_stricmp(txt, rptr)) {  // match to our ref
				link *ln = new link;
				LinkFixupList.add(ln, ++LinkFixupNum);
				ln->lid = NewName(lptr);
				ln->ref = NewName(txt);
				ln->nfile = fname;
				ln->nlen = flen;
				ln->nlink = flink;
				ln->nllen = strlen(flink);
				DCpfile::getval("ExtLinks", lptr, "", RefBuf, RefBufLen, RefFileName);
				ln->llink = NewName(RefBuf);
				DCpfile::getval("ExtPaths", lptr, "", RefBuf, RefBufLen, RefFileName);
				ln->lpath = NewName(RefBuf);
				DCpfile::getval("ExtFiles", lptr, "", RefBuf, RefBufLen, RefFileName);
				ln->lfile = NewName(RefBuf);
				DCpfile::getval("ExtRefFiles", lptr, "", RefBuf, RefBufLen, RefFileName);
				ln->rfile = NewName(RefBuf);
				DCpfile::getval("ExtLocs", lptr, "", RefBuf, RefBufLen, RefFileName);
				ln->lloc = atol(RefBuf);
				DCpfile::getval("ExtLens", lptr, "", RefBuf, RefBufLen, RefFileName);
				ln->llen = atol(RefBuf);
				DCpfile::getval("ExtProps", lptr, "", RefBuf, RefBufLen, RefFileName);
				ln->props = atol(RefBuf);
				if ((ln->props & 4) == 4) {  // use only hash
					char *lntxt = ln->nlink;
					char *pound = strchr(lntxt, '#');
					if (pound) {
						ln->nllen = strlen(pound + 1);
						ln->nlink = NewName(pound + 1, ln->nllen);
					}
				}
				else if ((ln->props & 1) == 1) {  // trim off hash and following
					char *lntxt = ln->nlink;
					char *pound = strchr(lntxt, '#');
					if (pound) {
						*pound = '\0';
						ln->nllen = pound - lntxt;
						ln->nlink = NewName(lntxt, ln->nllen);
					}
				}
			}
			lptr = rptr = NULL;
		}
	}
}


void
HTMwr::FixupFileRefs(void)
{
	if (LinkFixupNum == 1)
		FixOneFileRef(LinkFixupList.find(1), 1);
	else if (LinkFixupNum) {
		FixupArray = new (link *[LinkFixupNum]);

		LinkFixupList.all(SetFileRef);
		qsort(FixupArray, (size_t) LinkFixupNum, sizeof(void *), CompRef);
		for (int i = 0; i < LinkFixupNum; i++)
			FixOneFileRef(FixupArray[i], i + 1);
	}

	WriteRefCount();
	DCpfile::pflushall();  // done with ref files
}


void
HTMwr::SetFileRef(void *vp, long num)
{
	if (num)
		FixupArray[num - 1] = (link *) vp;
}


int
HTMwr::CompRef(const void *p1, const void *p2)
{
	link *l1 = *(link **)p1;
	link *l2 = *(link **)p2;

	int ret = _stricmp(l1->lpath, l2->lpath);
	if (ret == 0)
		ret = _stricmp(l1->lfile, l2->lfile);
	if (ret == 0)
		ret = (l1->lloc < l2->lloc) ?  -1 : 1;

	return ret;
}


void
HTMwr::FixOneFileRef(void *vp, long num)
{
	if (!num)  // setup call
		return;

	FilesFixed++;

	link *lp = (link *) vp;

	char nstr[NumStrMax] = { "" };

	// update loc and len from source
	char *rname = NewName(lp->lpath, strlen(lp->lpath) + strlen(lp->rfile) + 2);
	if (rname[strlen(rname) - 1] != '/')
		strcat(rname, "/");
	strcat(rname, lp->rfile);

	DCpfile::getval("LinkFiles", lp->lid, "", RefBuf, RefBufLen, rname);
	if (*RefBuf)
		lp->lfile = NewName(RefBuf);
	DCpfile::getval("LinkLocs", lp->lid, "", RefBuf, RefBufLen, rname);
	if (*RefBuf)
		lp->lloc = atol(RefBuf);
	DCpfile::getval("LinkLens", lp->lid, "", RefBuf, RefBufLen, rname);
	if (*RefBuf)
		lp->llen = atol(RefBuf);
	DCpfile::getval("LinkProps", lp->lid, "", RefBuf, RefBufLen, rname);
	if (*RefBuf)
		lp->props = (unl) atol(RefBuf);
	DCpfile::getval("LinkPads", lp->lid, "", RefBuf, RefBufLen, rname);
	if (*RefBuf)
		lp->lpad = atol(RefBuf);

	char *ltxt = NULL;
	char *lptr = NULL;
	long plen = 0;

	DCpfile::getval("Links", lp->lid, "", RefBuf, RefBufLen, rname);
	if (*RefBuf) {
		if ((lp->props & 4) == 4) {  // use only hash
			char *lntxt = lp->nlink;
			char *pound = strchr(lntxt, '#');
			if (pound) {
				lp->nllen = strlen(pound + 1);
				lp->nlink = NewName(pound + 1, lp->nllen);
			}
		}
		else if ((lp->props & 1) == 1) {  // trim off hash and following
			char *lntxt = lp->nlink;
			char *pound = strchr(lntxt, '#');
			if (pound) {
				*pound = '\0';
				lp->nllen = pound - lntxt;
				lp->nlink = NewName(lntxt, lp->nllen);
			}
		}
		ltxt = NewName(RefBuf);
		char *hash = strrchr(ltxt, '#');
		if (hash)
			*hash = '\0';
		if (((lptr = strrchr(ltxt, '/')) == NULL)
		 && ((lptr = strrchr(ltxt, '\\')) == NULL))
			lptr = ltxt;
		else
			lptr++;  // point after path separator
		if (hash)
			*hash = '#';

		plen = lptr - ltxt;
		
		if ((lp->nllen == (lp->llen - plen))
		 && !_strnicmp(lptr, lp->nlink, lp->nllen)) {  // new name matches old
			if (lptr[strlen(lptr) - 1] == '*') {  // remove error flags
				char *chp = lptr + strlen(lptr);
				while (--chp > lptr) {
					if ((*chp != '*')
					 && (*chp != ' '))
						break;
				}
				chp[1] = '\0';
				DCpfile::putval("Links", lp->lid, ltxt, rname);
			}
			DeleteName(ltxt);
			DeleteName(rname);  // no correction needed
			return;
		}
	}
	else {
		LogEvent(logwarn, 1, "No entry for link ", lp->lid, " in ", rname);
		DeleteName(rname);
		return;
	}


	char *fname = NewName(lp->lpath, strlen(lp->lpath) + strlen(lp->lfile) + 2);
	if (fname[strlen(fname) - 1] != '/')
		strcat(fname, "/");
	strcat(fname, lp->lfile);

	if (!FixItem(fname, lp->lloc + plen, lptr, lp->nlink,
		       lp->llen - plen, lp->nllen, lp->lpad, AllowPadding)) {
		//LogEvent(logwarn, 1, "Cannot correct ", fname, " ref ", ltxt);
		DeleteName(fname);
		DeleteName(ltxt);
		DeleteName(rname);
		return;
	}

	sprintf(nstr, "%ld", lp->nllen + plen);
	DCpfile::putval("ExtLens", lp->lid, nstr, RefFileName);
	DCpfile::putval("LinkLens", lp->lid, nstr, rname);

	sprintf(nstr, "%ld", lp->lloc);
	DCpfile::putval("ExtLocs", lp->lid, nstr, RefFileName);
	DCpfile::putval("LinkLocs", lp->lid, nstr, rname);

	sprintf(nstr, "%ld", lp->props);
	DCpfile::putval("ExtProps", lp->lid, nstr, RefFileName);
	DCpfile::putval("LinkProps", lp->lid, nstr, rname);

	// add path NewName(ltxt, plen) to front of nlink
	char *newlink = NewName(ltxt, strlen(ltxt) + strlen(lp->nlink));
	newlink[plen] = '\0';
	strcat(newlink, lp->nlink);
	DCpfile::putval("ExtLinks", lp->lid, newlink, RefFileName);
	DCpfile::putval("Links", lp->lid, newlink, rname);
	DeleteName(newlink);
	DeleteName(ltxt);

	// fix up locs to correct for changes during rewrite
	if (!AllowPadding
	 || (Padded < 0)) {
		DCpfile::putval("ExtPads", lp->lid, "0", RefFileName);
		DCpfile::putval("LinkPads", lp->lid, "0", rname);

		long adjust = lp->nllen - ((lp->llen - plen) + lp->lpad);
		if (adjust != 0)
			FixRefLocs(rname, lp->lfile, (long) lp->lloc + plen, adjust);
	}
	else {
		sprintf(nstr, "%ld", Padded);
		DCpfile::putval("ExtPads", lp->lid, nstr, RefFileName);
		DCpfile::putval("LinkPads", lp->lid, nstr, rname);
	}
	Padded = 0;


	DeleteName(fname);
	DeleteName(rname);
}


bool HTMwr::AllowPadding = false;
long HTMwr::Padded = 0;


bool
HTMwr::FixItem(char *fn, long loc, char *oname, char *name,
	            long olen, long len, long pad, bool allowpad)
{

	FILE *fp = NULL;
	long epos = 0;
	FilesFixed++;
	Padded = 0;

	if (((fp = fopen(fn, "r+b")) == NULL)
	 || (fseek(fp, 0L, SEEK_END) != 0)
	 || ((epos = ftell(fp)) == -1)) {
		LogEvent(logwarn, 1, "Cannot open ", fn, " to change ref ", oname);
		if (fp)
			fclose(fp);
		return false;
	}

	char *fbuf = NewName(olen + 2);

	if ((fseek(fp, loc, SEEK_SET) != 0)
	 || (fread(fbuf, 1L, olen, fp) != (size_t) olen)) {
		DeleteName(fbuf);
		LogEvent(logwarn, 1, "Cannot read ", fn, " ref ", oname);
		fclose(fp);
		return false;
	}

#if 0
	if ((*fbuf == '"')
	 || (*fbuf == '/')) { // wrote return instead of space, fix offset
		LogEvent(logwarn, 1, "Adjusted start pos in ", fn, " for ", oname);
		memmove(fbuf, fbuf + 1, olen);
		loc = loc + 1;
		if (fbuf[olen] == '"')
			olen--;
	}
	fbuf[olen] = '\0';
#endif

	//(len == olen) && 
	if (_strnicmp(oname, fbuf, olen)) {  // verify text matches .ref
		fbuf[olen] = '\0';
		LogEvent(logwarn, 1, "Mismatch in ", fn, " for ", oname);
		LogEvent(logcont, 1, " for ", fbuf);
		DeleteName(fbuf);
		fclose(fp);
		return false;
	}

	if (len == olen) {
		if ((fseek(fp, loc, SEEK_SET) != 0) // go back to point of change
		 || (fwrite(name, 1L, len, fp) != (size_t) len)) {  // new name
			LogEvent(logwarn, 1, "Cannot overwrite ", fn, " ref ", name);
			DeleteName(fbuf);
			fclose(fp);
			return false;
		}
		Padded = pad;  // unchanged
	}
	else if (allowpad
	 && (len <= (olen + pad))) {  // new value is smaller than avail space
		if ((fseek(fp, loc, SEEK_SET) != 0) // go back to point of change
		 || (fwrite(name, 1L, len, fp) != (size_t) len)) {  // new name
			LogEvent(logwarn, 1, "Cannot overwrite ", fn, " ref ", name);
			DeleteName(fbuf);
			fclose(fp);
			return false;
		}
		long padlen = ((olen + pad) - len) + 1;  // include closing quote
		char *padstr = NewName("\"", padlen);
		memset(padstr + 1, ' ', padlen - 1);
		if (fwrite(padstr, 1L, padlen, fp) != (size_t) padlen) {  // padding
			LogEvent(logwarn, 1, "Cannot write padding for ", fn, " ref ", name);
			DeleteName(padstr);
			DeleteName(fbuf);
			fclose(fp);
			return false;
		}
		DeleteName(padstr);
		Padded = padlen - 1;  // do not include quote
	}
	else {  // will not fit
		size_t tlen = epos - (loc + olen + pad);
		char *tbuf = new char[tlen];
		char *nstr = new char[20];
		char *nstr2 = new char[20];
		if (fread(tbuf, 1L, tlen, fp) != tlen) {  // read rest of file after padding
			LogEvent(logwarn, 1, "Cannot read rest of file after ", fn, " ref ", name);
			LogEvent(logcont, 1, "len ", ltoa(tlen, nstr, 10), " to ", ltoa(epos, nstr2, 10));
			DeleteName(fbuf);
			delete [] tbuf;
			delete [] nstr;
			delete [] nstr2;
			fclose(fp);
			return false;
		}
		else if (fseek(fp, loc, SEEK_SET) != 0) { // go back to point of change
			LogEvent(logwarn, 1, "Cannot return to start of ", fn, " ref ", name);
			DeleteName(fbuf);
			delete [] tbuf;
			fclose(fp);
			return false;
		}
		else if (fwrite(name, 1L, len, fp) != (size_t) len) {  // add new name
			LogEvent(logwarn, 1, "Cannot write the new ", fn, " ref ", name);
			DeleteName(fbuf);
			delete [] tbuf;
			fclose(fp);
			return false;
		}
		else if (fwrite(tbuf, 1L, tlen, fp) != (size_t) tlen) {  // rewrite the rest
			LogEvent(logwarn, 1, "Cannot rewrite rest of file after ", fn, " ref ", name);
			DeleteName(fbuf);
			delete [] tbuf;
			fclose(fp);
			return false;
		}
		if (len < (olen + pad)) {  // white out trailing end of file, doesn't truncate
			tlen = (olen + pad) - len;
			delete [] tbuf;
			tbuf = new char[tlen];
			memset(tbuf, ' ', tlen);
			fwrite(tbuf, 1L, tlen, fp);
		}
		delete [] tbuf;
		Padded = -1;
	}

	fclose(fp);
	DeleteName(fbuf);
	return true;
}


void
HTMwr::FixRefLocs(char *rname, char *fn, long loc, long adjust)
{
	long val = 0;
	char nstr[NumStrMax]; 
	char *lptr = NULL;
	char *rptr = NULL;

	DCpfile::getkeys("LinkLocs", rname);
	while (DCpfile::getnextkey(&lptr, &rptr)) {
		DCpfile::getval("LinkFiles", lptr, "", RefBuf, RefBufLen, rname);
		if (!_stricmp(RefBuf, fn)) {
			if ((val = atol(rptr)) > loc) {
				sprintf(nstr, "%ld", val + adjust);
				DCpfile::putval("LinkLocs", lptr, nstr, rname);
			}
		}
	}
	DCpfile::releasekeys("LinkLocs", rname);

	FixFSRefLocs(rname, "PrevNameLoc", "PrevNameID", fn, loc, adjust);
	FixFSRefLocs(rname, "PrevTitleLoc", "PrevTitleID", fn, loc, adjust);
	FixFSRefLocs(rname, "NextNameLoc", "NextNameID", fn, loc, adjust);
	FixFSRefLocs(rname, "NextTitleLoc", "NextTitleID", fn, loc, adjust);

	//delete [] linkBuf;
}


void
HTMwr::FixFSRefLocs(char *rname, char *locsect, char *idsect,
	 char *fn, long loc, long adjust)
{
	long val = 0;
	char nstr[NumStrMax]; 
	char *lptr = NULL;
	char *rptr = NULL;

	DCpfile::getkeys(locsect, rname);
	while (DCpfile::getnextkey(&lptr, &rptr)) {
		DCpfile::getval(idsect, lptr, "", RefBuf, RefBufLen, rname);
		strcpy(nstr, RefBuf);
		DCpfile::getval("SplitFileNames", nstr, "", RefBuf, RefBufLen, rname);
		if (!_stricmp(RefBuf, fn)) {
			//DCpfile::getval(locsect, lptr, "", RefBuf, RefBufLen, rname);
			if ((val = atol(rptr)) > loc) {
				sprintf(nstr, "%ld", val + adjust);
				DCpfile::putval(locsect, lptr, nstr, rname);
			}
		}
	}
	DCpfile::releasekeys(locsect, rname);
}


char *
HTMwr::ProcExtRef(char *rtxt, bool hyp, bool ext, bool dita)
{
	// ext = false if SecWin or TOC/IX entry
	// dita = true if possible in DITA
	// return char * to possibly modified rtxt, which can be reallocated

	char *argt = rtxt;
	char *txt = rtxt + 1; // skip starting #
	int partID = 0;
	int err = 0;
	char *cpt = NULL;
	int currID = Extracting ? ExtrID : SplitID;

	ExtRefUsed = false;
	ExtRefCount++;

	if (!RefFile)
		return rtxt;

	// process jump ahref
	// if XrefFile not NULL or empty, is to external file;
	// else is to current file before splitting
	// modify XrefFile to point to proper split file name either way
	char **fn = hyp ? &HyperJumpFileName : &XrefFile;

	if (LinkID)
		DeleteName(LinkID);
	if (hyp ? HyperLinkID : XrefLinkID)
		LinkID = NewName(hyp ? HyperLinkID : XrefLinkID);
	else
		LinkID = NewName(txt);

	char *errfn = NewName(strlen(CurrFileName) + 6);
	sprintf(errfn, "0 %s **", CurrFileName);

	RefBuf[0] = '\0';

	if ((*fn == NULL)
	 || (**fn == '\0')) {  // in this file

		if (*txt == '\0')
			strcpy(RefBuf, "1");  // always first part
		else  // look in our own [IntRefs], get PartID = int on right
			DCpfile::getval("IntRefs", txt, "", RefBuf, RefBufLen, RefFileName);

		// if missing, add with PartID of 0, inc count of missing refs
		//   for our name under [IntFiles], 
		if ((*RefBuf == '\0')
		 || ((partID = atoi(RefBuf)) == 0)) { // also if present but 0
			DCpfile::putval("IntRefs", txt, errfn, RefFileName);
			SetMissingRef(CurrFileName, txt, err = 4);
			if (HXMwr::HXref && dita)
				HXMwr::DITAProcExtRef();  // just set defaults
		}
		else {   // found in [IntRefs]
			if (partID == 1) {    // if ref is to unsplit pert of file
				if (currID == 0)   // and current part is unsplit
					*fn = NULL;
				else                // if ref is from split to unsplit part
					*fn = NewName(CurrXFileName);
			}
			else { // get name for part from our own internal list (always safe)
				if (partID != currID)
					*fn = NewName(SplitNameList.find(partID));
				else  // if it's our own current part name, not needed
					*fn = NULL;
			}
			if (HXMwr::HXref && dita)
				rtxt = HXMwr::DITAProcExtRef(txt, RefFileName);
		}
		//LinkID = NULL;
	}
	else {  // in the file identified by *fn

		char *other = GetRefFileName(fn); // may change *fn if RemoveFilePaths

		if (other == NULL)  {
			err = 1;  // abject failure, leave target file name unchanged
			LogEvent(logwarn, 1, "Cannot find ref file for ", *fn);
		}
		else { // in other ref file, follow same process above to get new XrefFile
			if (*txt == '\0')
				strcpy(RefBuf, "1");  // always first part
			else DCpfile::getval("IntRefs", txt, "", RefBuf, RefBufLen, other);
			if ((*RefBuf == '\0')
			 || ((partID = atoi(RefBuf)) == 0)) { // also if present but 0
				DCpfile::putval("IntRefs", txt, errfn, other);
				SetMissingRef(*fn, txt, err = 2);
				// leave name unchanged, need to run filter on other file first
				if (HXMwr::HXref && dita)
					HXMwr::DITAProcExtRef();  // just set defaults
			}
			else {
				if (partID != 1) { // ref is to split part of other file
					// find partID in [SplitFileNames], get str on right for XrefFile
					char *partStr = NewName(NumStrMax);
					sprintf(partStr, "%ld", partID);
					DCpfile::getval("SplitFileNames", partStr, "", RefBuf, RefBufLen, other);

					if (*RefBuf != '\0') { // update name in XrefFile or HyperJumpFileName
						if (RemoveFilePaths) {  // get rid of any pathname components
							cpt = strrchr(RefBuf, '/');	// remove any path
							if (!cpt)
								cpt = strrchr(RefBuf, '\\');
							*fn = NewName(cpt ? cpt + 1 : RefBuf);
						}
						else if ((*RefBuf != '.') && (*RefBuf != '/') && (*RefBuf != '\\')) {
							char *fnp = *fn;
							cpt = strrchr(fnp, '/');	// find original path
							if (!cpt)
								cpt = strrchr(fnp, '\\');
							if (cpt) {
								size_t len = (cpt - fnp) + 1;
								*fn = NewName(strlen(RefBuf) + 1 + len);
								strncpy(*fn, fnp, len);
								strcpy(*fn + len, RefBuf);
							}
							else
								*fn = NewName(RefBuf);
						}
						else
							*fn = NewName(RefBuf);
					}
					else   // split but partID missing, leave original name
						SetMissingRef(*fn, partStr, err = 3);
				}
				else {  // ref to unsplit part of file
					if (RemoveFilePaths) {  // get rid of any pathname components
						strcpy(RefBuf, *fn);
						cpt = strrchr(RefBuf, '/');	// remove any path
						if (!cpt)
							cpt = strrchr(RefBuf, '\\');
						*fn = NewName(cpt ? cpt + 1 : RefBuf);
					}
				}
				if (HXMwr::HXref && dita)
					rtxt = HXMwr::DITAProcExtRef(txt, other);
			}
		}

			// *fn is now correct filename for txt
		if (ext) {
			extlink *ln = new extlink;
			if (other) {
				ln->lid = NewName(LinkID);
				ln->reftxt = NewName(txt);
				ln->rfile = other;
				ln->lloc = 0; // not known yet
				if (!hyp
				 && XrefInCurrDBFile) {
					ln->llen = strlen(rtxt + 1);
					ln->props |= 4;  // part of same file after assembly
				}
				else if (NoLocations || XrefNoRef || Curr.noref) {
					ln->llen = strlen(*fn);
					ln->props = 1;  // 1 for filename only ****
				}
				else {
					ln->llen = strlen(*fn) + strlen(rtxt);  // rtxt includes the hash itself
					ln->props = 0;
				}
				if (hyp)  // otherwise xref
					ln->props |= 2;
				ExtRefList.add(ln, ++ExtRefNum);
				ExtRefPartList.add(ln, (currID ? currID : 1));
				ExtRefUsed = true;
			}
			// in our own [Links], put link=basename#txt
			char *ref = NULL;
			if (!hyp
			 && XrefInCurrDBFile) {
				ref = NewName(strlen(rtxt + 1) + err + 2);
				strcpy(ref, rtxt + 1);  // anchor name only
			}
			else if ((*(rtxt + 1) == '\0')
			 || (other
			  && ((ln->props & 1) == 1))) {
				ref = NewName(strlen(*fn) + err + 2);
				strcpy(ref, *fn);  // file name only
			}
			else {
				ref = NewName(strlen(*fn) + strlen(rtxt + 1) + err + 3);
				sprintf(ref, "%s#%s", *fn, rtxt + 1);
			}
			if (err) {
				char *refp = ref + strlen(ref);
				*refp = ' ';
				while (err--)
					*++refp = '*';
				*++refp = '\0';
			}
			ln->lnktxt = ref;

			DCpfile::putval("Links", LinkID, ref, RefFileName);
			DCpfile::putval("LinkRefs", LinkID, txt, RefFileName);
			DCpfile::putval("LinkFiles", LinkID, *fn, RefFileName);
		}
	}

	DeleteName(errfn);
	if (txt != (rtxt + 1))
		DeleteName(txt - 1); // original link

	return rtxt;
}


void
HTMwr::WriteRefFileInfo(DCfile *wf, size_t fnpos, long divused)
{
	// write these later when ready to write name to file
	if (!ExtRefUsed)
		return;

	extlink *ln = (extlink *) ExtRefList.find(ExtRefNum);
	if (ln == NULL)
		return;

	if (Extracting) {
		if (ExtrScriptDiv && (divused == ExtrScriptDiv)) { // store pos rel to start of div
			ExtScriptRefList.append(ExtrID, ExtRefNum);
			ln->lloc = fnpos + ((DCwfile *)wf)->LenDiv();
			return;
		}
		else if (ExtrObjectDiv && (divused == ExtrObjectDiv)) {
			ExtObjectRefList.append(ExtrID, ExtRefNum);
			ln->lloc = fnpos + ((DCwfile *)wf)->LenDiv();
			return;
		}
	}
	else {
		if (ScriptDiv && (divused == ScriptDiv)) { // store pos rel to start of div
			ExtScriptRefList.append(SplitID ? SplitID : 1, ExtRefNum);
			ln->lloc = fnpos + ((DCwfile *)wf)->LenDiv();
			return;
		}
		else if (ObjectDiv && (divused == ObjectDiv)) {
			ExtObjectRefList.append(SplitID ? SplitID : 1, ExtRefNum);
			ln->lloc = fnpos + ((DCwfile *)wf)->LenDiv();
			return;
		}
	}

	if (TblDivType) {
		TblDivRefList.append(ExtRefNum, TblDivType);
		ln->lloc = fnpos + ((DCwfile *)wf)->LenDiv();
	}
	else if (FigureDiv && (divused == FigureDiv)) {
		FigureDivRefList.add(ExtRefNum);
		ln->lloc = fnpos + ((DCwfile *)wf)->LenDiv();
	}
	else if (MapCalloutDiv && (divused == MapCalloutDiv)) {
		MapCalloutDivRefList.add(ExtRefNum);
		ln->lloc = fnpos + ((DCwfile *)wf)->LenDiv();
	}
	else {  // normal, about to write to file
		ln->lloc = fnpos + wf->tell();
	}
}


void
HTMwr::FixOneDivRef(DCwfile *wf, long lnum)
{
	if (lnum == 0)  // init call not used
		return;

	// called when div is about to be written to file, from list.all
	extlink *ln = (extlink *) ExtRefList.find(lnum);
	ln->lloc += wf->tell();  // now we know where the name is really
}


DCvlist HTMwr::ExtRefList;
DCvlist HTMwr::ExtRefPartList;
DCilist HTMwr::ExtScriptRefList;
DCilist HTMwr::ExtObjectRefList;
long HTMwr::ExtRefNum = 0;
char *HTMwr::ExtRefFileName = NULL;


void
HTMwr::PutExtRefInfo(DCwfile *wf)
{
	// write to local and remote ref files at end of file part
	ExtRefFileName = Extracting ? wf->getAuxName() : wf->getFileName();
	ExtRefPartList.execu(Extracting ? ExtrID : (SplitID ? SplitID : 1), PutOneExtRef);
	if (!Extracting)
		ExtRefPartList.empty();
}

void
HTMwr::PutOneExtRef(void *vp)
{
	if (vp == NULL)  // init call not used
		return;

	extlink *ln = (extlink *) vp;

	if (!ln->rfile)
		return;

	char *nstr = NewName(NumStrMax);

	if (!(ln->lid)
	 || (*(ln->lid) == '\0')) {
		//LogEvent(logwarn, 1, "Empty ID for ext ref ", ExtRefFileName, 
		//         " at ", ltoa(ln->lloc, nstr, 10));
		return;
	}
	if (!(ln->llen)
	 || !(ln->lloc)) {
		//LogEvent(logwarn, 1, "Bad ext ref at ", ltoa(ln->lloc, nstr, 10),
		//         " length ", ltoa(ln->llen, nstr, 10));
		//LogEvent(logcont, 1, "for ", ExtRefFileName);
		return;
	}

	if (!stricmp(ln->rfile, RefFileName))
		DCpfile::releasekeys("ExtRefs", RefFileName);

	DCpfile::putval("ExtRefs", ln->lid, ln->reftxt, ln->rfile);
	DCpfile::putval("ExtLinks", ln->lid, ln->lnktxt, ln->rfile);
	DCpfile::putval("ExtPaths", ln->lid, CurrXFilePath, ln->rfile);
	DCpfile::putval("ExtFiles", ln->lid, ExtRefFileName, ln->rfile);
	DCpfile::putval("LinkFiles", ln->lid, ExtRefFileName, RefFileName);
	DCpfile::putval("ExtRefFiles", ln->lid, RefFileName, ln->rfile);

	sprintf(nstr, "%ld", ln->llen);
	DCpfile::putval("ExtLens", ln->lid, nstr, ln->rfile);
	DCpfile::putval("LinkLens", ln->lid, nstr, RefFileName);

	sprintf(nstr, "%ld", ln->lloc);
	DCpfile::putval("ExtLocs", ln->lid, nstr, ln->rfile);
	DCpfile::putval("LinkLocs", ln->lid, nstr, RefFileName);

	DCpfile::putval("ExtPads", ln->lid, "0", ln->rfile);
	DCpfile::putval("LinkPads", ln->lid, "0", RefFileName);

	sprintf(nstr, "%ld", ln->props);
	DCpfile::putval("ExtProps", ln->lid, nstr, ln->rfile);
	DCpfile::putval("LinkProps", ln->lid, nstr, RefFileName);

	DeleteName(ln->lid);
	DeleteName(ln->reftxt);
	DeleteName(ln->lnktxt);
	DeleteName(nstr);
}


void
HTMwr::SetMissingRef(char *fname, char *txt, int err)
{
	static bool first = true;

	if (ListMissingRefs) {
		if (first) {
			fprintf(stderr, "\nMissing refs to:\n");
			first = false;
		}
		fprintf(stderr, "%s : %s err %d\n", fname, txt, err);
	}
}


void
HTMwr::ProcFileNameRef(char *ref, char *fn)
{
	// add name for SplitID part
	DCpfile::getval("SplitFileNames", ref, "", RefBuf, RefBufLen, RefFileName);
	if (!strcmp(RefBuf, fn))
		return;

	DCpfile::putval("SplitFileNames", ref, fn, RefFileName);
}


// list of graphics to be generated by the FDK

void
HTMwr::PutGraphGenInfo(void)
{
	GrGenList.all(PutGrGenItem);
	DCpfile::pclose(GrxFileName);
}


void
HTMwr::PutGrGenItem(char *name, long genid)
{
	if (!name || !genid)
		return;

	char *idstr = NewName(NumStrMax);
	sprintf(idstr, "%ld", genid);
	DCpfile::putval("GrGen", idstr, name, GrxFileName);

	if ((name = GrDpiList.find(genid)) != NULL)
		DCpfile::putval("GrDpi", idstr, name, GrxFileName);
}



bool HTMwr::RefFile = false;
char *HTMwr::RefFileName = NULL;


void
HTMwr::SetUpRefFile(void)
{

	FILE *fp = fopen(RefFileName, "rb");
	if (fp) {  // file exists, ensure it is writable
		fclose(fp);
		if ((fp = fopen(RefFileName, "rwb")) != NULL) {
			fclose(fp);
			DCpfile::getval("Versions", "RefFileVer", "1", RefBuf, RefBufLen, RefFileName);
			if (*RefBuf == '3')
				RefFile = true;
			else if ((fp = fopen(RefFileName, "wb")) != NULL) // create file
				RefFile = WriteEmptyRefFile(fp, CurrFileName, RefFileName);
		}
	}
	else if ((fp = fopen(RefFileName, "wb")) != NULL)  // create file
		RefFile = WriteEmptyRefFile(fp, CurrFileName, RefFileName);

	RefFileList.add(RefFileName, 1);
	RefFileBaseList.add(CurrFileName, 1);
	NextRefFileNum = 2;
	DCpfile::psetup(RefFileName);

	// make new graphics file each time
	if (FMSource) {
		_unlink(GrxFileName);
		GrxFile = false;
		if ((fp = fopen(GrxFileName, "wt")) != NULL)  // create file
			GrxFile = WriteEmptyGrxFile(fp, CurrFileName);
		DCpfile::psetup(GrxFileName);
	}
	else if (UsePartFiles) {
		_unlink(GrcFileName);
		GrcFile = false;
		if ((fp = fopen(GrcFileName, "wt")) != NULL)  // create file
			GrcFile = WriteEmptyGrcFile(fp, CurrFileName);
		DCpfile::psetup(GrcFileName);
	}
}


bool
HTMwr::WriteEmptyRefFile(FILE *fp, char *fn, char *fname)
{
	// ensure the cache doesn't overwrite our new file later
	//DCpfile::pflushall();
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) DCpfile::ProfileFiles.getfile(fname)) != NULL) {
		pf->pflush();
		DCpfile::ProfileFiles.remove((long) pf);
	}

	bool ret = false;

	fputs("; reference file for ", fp);
	fputs(fn, fp);
	fputs("\n\n", fp);
	fputs("[Versions]\nRefFileVer=3\n\n", fp);

	fputs("[Splits]\n", fp);
	fputs("FirstID=0\n", fp);
	fputs("FirstTitle=\n", fp);
	fputs("LastID=0\n", fp);
	fputs("LastTitle=\n", fp);
	fputs("PrevRef=\n", fp);
	fputs("PrevName=\n", fp);
	fputs("PrevNameLen=0\n", fp);
	fputs("PrevTitle=\n", fp);
	fputs("PrevTitleLen=0\n", fp);
	fputs("NextRef=\n", fp);
	fputs("NextName=\n", fp);
	fputs("NextNameLen=0\n", fp);
	fputs("NextTitle=\n", fp);
	fputs("NextTitleLen=0\n\n", fp);

	fputs("[PrevNameID]\n\n", fp);
	fputs("[PrevNameLoc]\n\n", fp);
	fputs("[PrevTitleID]\n\n", fp);
	fputs("[PrevTitleLoc]\n\n", fp);
	fputs("[NextNameID]\n\n", fp);
	fputs("[NextNameLoc]\n\n", fp);
	fputs("[NextTitleID]\n\n\n", fp);
	fputs("[NextTitleLoc]\n\n\n", fp);

	fputs("[SplitFileNames]\n", fp);
	fputs("1=", fp);
	fputs(fn, fp);
	fputs(".", fp);
	fputs(XrefFileSuffix, fp);
	fputs("\n\n", fp);
	fputs("[IntRefs]\n\n", fp);

	fputs("[DITATopicID]\n\n", fp);
	fputs("[DITATopicType]\n\n", fp);
	fputs("[DITAElemID]\n\n", fp);
	fputs("[DITAElemType]\n\n", fp);

	fputs("[Links]\n\n", fp);
	fputs("[LinkRefs]\n\n", fp);
	fputs("[LinkFiles]\n\n", fp);
	fputs("[LinkLocs]\n\n", fp);
	fputs("[LinkLens]\n\n", fp);
	fputs("[LinkPads]\n\n", fp);
	fputs("[LinkProps]\n\n", fp);

	fputs("[ExtLinks]\n\n", fp);
	fputs("[ExtRefs]\n\n", fp);
	fputs("[ExtPaths]\n\n", fp);
	fputs("[ExtFiles]\n\n", fp);
	fputs("[ExtRefFiles]\n\n", fp);
	fputs("[ExtLocs]\n\n", fp);
	fputs("[ExtLens]\n\n", fp);
	fputs("[ExtPads]\n\n", fp);
	fputs("[ExtProps]\n\n", fp);

	if (fputs("[End]\n;end of ref file\n\n", fp) != EOF)
		ret = true;
	fclose(fp);
	return ret;
}


bool HTMwr::GrxFile = false;
char *HTMwr::GrxFileName = NULL;


bool
HTMwr::WriteEmptyGrxFile(FILE *fp, char *fn)
{
	bool ret = false;

	fputs("; graphics file for ", fp);
	fputs(fn, fp);
	fputs("\n\n", fp);
	fputs("[GrGen]\n\n", fp);
	fputs("[GrDpi]\n\n", fp);
	fputs("[GrUse]\n\n", fp);
	if (fputs("[End]\n;end of grx file\n\n", fp) != EOF)
		ret = true;
	fclose(fp);
	return ret;
}


bool HTMwr::GrcFile = false;
char *HTMwr::GrcFileName = NULL;


bool
HTMwr::WriteEmptyGrcFile(FILE *fp, char *fn)
{
	bool ret = false;

	fputs("; graphics copy file for ", fp);
	fputs(fn, fp);
	fputs("\n\n", fp);
	fputs("[GrOptions]\n", fp);
	fputs("GrCount=0\n\n", fp);
	fputs("[GrPaths]\n\n", fp);
	fputs("[GrFixedPaths]\n\n", fp);
	if (fputs("[End]\n;end of grc file\n\n", fp) != EOF)
		ret = true;
	fclose(fp);
	return ret;
}

void
HTMwr::PutGraphCopyInfo(void)
{
	GraphFileCopyList.all(PutGrCopyItem);
	char *cnt = NewName(NumStrMax);
	sprintf(cnt, "%ld", GraphCopyCount);
	DCpfile::putval("GrOptions", "GrCount", cnt, GrcFileName);
	DCpfile::pclose(GrcFileName);
}


void
HTMwr::PutGrCopyItem(char *name, long id)
{
	if (!name || !id)
		return;

	char *idstr = NewName(NumStrMax);
	sprintf(idstr, "%ld", id);
	DCpfile::putval("GrPaths", idstr, name, GrcFileName);

	if ((name = GraphFileFixedCopyList.find(id)) != NULL)
		DCpfile::putval("GrFixedPaths", idstr, name, GrcFileName);
}




DCnlist HTMwr::RefFileList; // full ref file paths, indexed by
DCnlist HTMwr::RefFileBaseList; // value assigned to base name
uns HTMwr::NextRefFileNum = 1;


char *
HTMwr::GetRefFileName(char **fn)
{
	uns rfnum = 0;
	char *rfname = NULL;

	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	char *rpath = NULL;
	char rdrive[_MAX_DRIVE];
	char rdir[_MAX_DIR];

	_splitpath(*fn, drive, dir, fname, ext);
	
	if (RemoveFilePaths) {  // get rid of any pathname components
		_makepath(path_buffer, "", "./", fname, ext );
		*fn = NewName(path_buffer);
		*drive = '\0';
		strcpy(dir, "./");
	}
	else if ((*dir == '\0')
	 || ((*dir != '.') && (*dir != '/') && (*dir != '\\'))) {
		memmove(dir + 2, dir, strlen(dir) + 1);
		dir[0] = '.';
		dir[1] = '/';
	}

	// check RefFileList by basename, if missing find it
	if (((rfnum = (uns) RefFileBaseList(*fn)) != 0)
	 && ((rfname = RefFileList.find(rfnum)) != NULL))
		return rfname;


	FILE *fp = NULL;

	while (fp == NULL) {  // not there

		// look in dir per [RefFiles] section
		if (IniFile
		 && IniFile->Section("RefFiles")
		 && IniFile->Find(fname)) {
			char ch = 0;
			rpath = IniFile->StrRight();
			size_t len = strlen(rpath);
			if (((ch = rpath[len - 1]) != '/')
			 && (ch != '\\')) {
				rpath = NewName(rpath, len + 1);
				rpath[len] = strchr(rpath, '/') ? '/' : '\\';
			}
			_splitpath(rpath, rdrive, rdir, NULL, NULL);
			_makepath(path_buffer, rdrive, rdir, fname, "ref" );
			if ((fp = fopen(path_buffer, "rb")) != NULL) {
				DCpfile::getval("Versions", "RefFileVer", "1", RefBuf, RefBufLen, path_buffer);
				if (*RefBuf != '3') {
					fclose(fp);
					if ((fp = fopen(path_buffer, "wb")) != NULL) { // create file
						if (!WriteEmptyRefFile(fp, fname, path_buffer))
							fp = NULL;
					}
				}
				break;
			}
			// create file in ref dir if no .ref file there
			if ((fp = fopen(path_buffer, "wb")) != NULL) { // create file
				if (!WriteEmptyRefFile(fp, fname, path_buffer))
					fp = NULL;
				break;
			}

			// if impossible to write there, keep trying other places
		}

		// look in original dir next
		_makepath(path_buffer, drive, dir, fname, "ref" );
		if ((fp = fopen(path_buffer, "rb")) != NULL) {
			DCpfile::getval("Versions", "RefFileVer", "1", RefBuf, RefBufLen, path_buffer);
			if (*RefBuf != '3') {
				fclose(fp);
				if ((fp = fopen(path_buffer, "wb")) != NULL) { // create file
					if (!WriteEmptyRefFile(fp, fname, path_buffer))
						fp = NULL;
				}
			}
			break;
		}
		// create file in orig dir if .mif file there
		_makepath(path_buffer, drive, dir, fname, "mif" );
		if ((fp = fopen(path_buffer, "rb")) != NULL) {
			fclose(fp);
			_makepath(path_buffer, drive, dir, fname, "ref" );
			if ((fp = fopen(path_buffer, "wb")) != NULL) { // create file
				if (!WriteEmptyRefFile(fp, fname, path_buffer))
					fp = NULL;
				break;
			}
		}

		// look in current directory
		_makepath(path_buffer, "", "./", fname, "ref" );
		if ((fp = fopen(path_buffer, "rb")) != NULL) {
			DCpfile::getval("Versions", "RefFileVer", "1", RefBuf, RefBufLen, path_buffer);
			if (*RefBuf != '3') {
				fclose(fp);
				if ((fp = fopen(path_buffer, "wb")) != NULL) { // create file
					if (!WriteEmptyRefFile(fp, fname, path_buffer))
						fp = NULL;
				}
			}
			break;
		}

		// try to think of more places, possibly using the .prjs.for dirs


		// create file in curr dir even if no .mif file there as last resort
		if ((fp = fopen(path_buffer, "wb")) != NULL) { // create file
			if (!WriteEmptyRefFile(fp, fname, path_buffer))
				fp = NULL;
			break;
		}

		break;  // give up
	}

	if (fp) {
		fclose(fp);
		rfname = NewName(path_buffer);
		RefFileList.add(rfname, NextRefFileNum);
		RefFileBaseList.add(NewName(*fn), NextRefFileNum++);
	}

	return rfname;
}


bool HTMwr::StartSplit = false;
char *HTMwr::PrevRefFile = NULL;
char *HTMwr::NextRefFile = NULL;
char *HTMwr::FirstTitle = NULL;
char *HTMwr::LastTitle = NULL;


void
HTMwr::ProcSplitRefInfo(void)
{
	if (!CurrFSPos)
		return;

	// set the [Split] values that will be needed by other files

	DCpfile::getval("Splits", "FirstID", "", RefBuf, RefBufLen, RefFileName);
	if (strcmp(RefBuf, FirstSplitIDRef))
		DCpfile::putval("Splits", "FirstID", FirstSplitIDRef, RefFileName);
	FirstTitle = StartSplit ? SplitTitleList.find(FirstSplitID) : CurrXFileTitle;
	if (FirstTitle) {
		DCpfile::getval("Splits", "FirstTitle", "", RefBuf, RefBufLen, RefFileName);
		if (strcmp(RefBuf, FirstTitle))
			DCpfile::putval("Splits", "FirstTitle", FirstTitle, RefFileName);
	}

	DCpfile::getval("Splits", "LastID", "", RefBuf, RefBufLen, RefFileName);
	if (strcmp(RefBuf, LastSplitIDRef))
		DCpfile::putval("Splits", "LastID", LastSplitIDRef, RefFileName);
	LastTitle = LastSplitID ? SplitTitleList.find(LastSplitID) : CurrXFileTitle;
	if (LastTitle) {
		DCpfile::getval("Splits", "LastTitle", "", RefBuf, RefBufLen, RefFileName);
		if (strcmp(RefBuf, LastTitle))
			DCpfile::putval("Splits", "LastTitle", LastTitle, RefFileName);
	}

	// get the [Split] values needed for this file

	if (PrevFSBase) {
		char *prevbase = NewName(PrevFSBase);
		PrevRefFile = GetRefFileName(&prevbase);
		if (!stricmp(RefFileName, PrevRefFile))
			LogEvent(logwarn, 1, "Reference to nonexistent previous file");
		else {
			DCpfile::getval("Splits", "LastID", "", RefBuf, RefBufLen, PrevRefFile);
			if (*RefBuf) {
				char *prevend = NewName(RefBuf);
				//char *titletype = strcmp(prevend, "1") ? "LastTitle" : "StartTitle";
				DCpfile::getval("Splits", "LastTitle", "", RefBuf, RefBufLen, PrevRefFile);
				if (*RefBuf)
					PrevFSTitle = NewName(RefBuf);
				DCpfile::getval("SplitFileNames", prevend, "", RefBuf, RefBufLen, PrevRefFile);
				if (*RefBuf)
					PrevFSName = NewName(RefBuf);
				DeleteName(prevend);
			}
		}
	}
	if (NextFSBase) {
		char *nextbase = NewName(NextFSBase);
		NextRefFile = GetRefFileName(&nextbase);
		if (!stricmp(RefFileName, NextRefFile))
			LogEvent(logwarn, 1, "Reference to nonexistent next file");
		else {
			DCpfile::getval("Splits", "FirstID", "", RefBuf, RefBufLen, NextRefFile);
			if (*RefBuf) {
				char *nextbegin = NewName(RefBuf);
				DCpfile::getval("Splits", "FirstTitle", "", RefBuf, RefBufLen, NextRefFile);
				if (*RefBuf)
					NextFSTitle = NewName(RefBuf);
				DCpfile::getval("SplitFileNames", nextbegin, "", RefBuf, RefBufLen, NextRefFile);
				if (*RefBuf)
					NextFSName = NewName(RefBuf);
				DeleteName(nextbegin);
			}
		}
	}
}


void
HTMwr::SetSplitRefInfo(void)
{
	if (!CurrFSPos)
		return;

	// set info on usage of names and titles from next and prev files
	long i = 0;
	long cnt = 0;
	long val = 0;
	long sid = 0;
	char numstr[NumStrMax];
	char valstr[NumStrMax];
	bool prevused = false;
	bool nextused = false;

	if (((cnt = PrevFSNameUsage.count()) > 0)
	 && PrevFSName) {
		prevused = true;
		DCpfile::putval("Splits", "PrevName", PrevFSName, RefFileName);
		_ltoa(strlen(PrevFSName), valstr, 10);
		DCpfile::putval("Splits", "PrevNameLen", valstr, RefFileName);
		for (i = 1; i <= cnt; i++) {
			_ltoa(i, numstr, 10);
			sid = PrevFSNameUsage.number(i, &val);
			_ltoa(sid, valstr, 10);
			DCpfile::putval("PrevNameID", numstr, valstr, RefFileName);
			_ltoa(val, valstr, 10);
			DCpfile::putval("PrevNameLoc", numstr, valstr, RefFileName);
		}
	}
	else {
		DCpfile::putval("Splits", "PrevName", "", RefFileName);
		DCpfile::putval("Splits", "PrevNameLen", "0", RefFileName);
	}

	if (((cnt = PrevFSTitleUsage.count()) > 0)
	 && PrevFSTitle) {
		prevused = true;
		DCpfile::putval("Splits", "PrevTitle", PrevFSTitle, RefFileName);
		_ltoa(strlen(PrevFSTitle), numstr, 10);
		DCpfile::putval("Splits", "PrevTitleLen", numstr, RefFileName);
		for (i = 1; i <= cnt; i++) {
			_ltoa(i, numstr, 10);
			sid = PrevFSTitleUsage.number(i, &val);
			_ltoa(sid, valstr, 10);
			DCpfile::putval("PrevTitleID", numstr, valstr, RefFileName);
			_ltoa(val, valstr, 10);
			DCpfile::putval("PrevTitleLoc", numstr, valstr, RefFileName);
		}
	}
	else {
		DCpfile::putval("Splits", "PrevTitle", "", RefFileName);
		DCpfile::putval("Splits", "PrevTitleLen", "0", RefFileName);
	}

	if (((cnt = NextFSNameUsage.count()) > 0)
	 && NextFSName) {
		nextused = true;
		DCpfile::putval("Splits", "NextName", NextFSName, RefFileName);
		_ltoa(strlen(NextFSName), numstr, 10);
		DCpfile::putval("Splits", "NextNameLen", numstr, RefFileName);
		for (i = 1; i <= cnt; i++) {
			_ltoa(i, numstr, 10);
			sid = NextFSNameUsage.number(i, &val);
			_ltoa(sid, valstr, 10);
			DCpfile::putval("NextNameID", numstr, valstr, RefFileName);
			_ltoa(val, valstr, 10);
			DCpfile::putval("NextNameLoc", numstr, valstr, RefFileName);
		}
	}
	else {
		DCpfile::putval("Splits", "NextName", "", RefFileName);
		DCpfile::putval("Splits", "NextNameLen", "0", RefFileName);
	}
		
	if (((cnt = NextFSTitleUsage.count()) > 0)
	 && NextFSTitle) {
		nextused = true;
		DCpfile::putval("Splits", "NextTitle", NextFSTitle, RefFileName);
		_ltoa(strlen(NextFSTitle), numstr, 10);
		DCpfile::putval("Splits", "NextTitleLen", numstr, RefFileName);
		for (i = 1; i <= cnt; i++) {
			_ltoa(i, numstr, 10);
			sid = NextFSTitleUsage.number(i, &val);
			_ltoa(sid, valstr, 10);
			DCpfile::putval("NextTitleID", numstr, valstr, RefFileName);
			_ltoa(val, valstr, 10);
			DCpfile::putval("NextTitleLoc", numstr, valstr, RefFileName);
		}
	}
	else {
		DCpfile::putval("Splits", "NextTitle", "", RefFileName);
		DCpfile::putval("Splits", "NextTitleLen", "0", RefFileName);
	}

	if (prevused) {      // set prev ref file info, where we used Last
		if (!stricmp(RefFileName, PrevRefFile))  // long as it's not us
			LogEvent(logwarn, 1, "Reference to nonexistent previous file");
		else {
			DCpfile::putval("Splits", "NextRef", RefFileName, PrevRefFile);

			// that file may use our Firsts, so check it
			DCpfile::getval("Splits", "NextName", "", RefBuf, RefBufLen, PrevRefFile);
			if (*RefBuf) {  // empty NextName means not used
				char *nextname = NewName(RefBuf);
				char *firstname = StartSplit ? SplitNameList.find(FirstSplitID) : CurrXFileName; 
				long nextlen = 0;
				long firstlen = 0; 
				if (strcmp(firstname, nextname)) { // mismatch, update other file's NextName
					DCpfile::getval("Splits", "NextNameLen", "", RefBuf, RefBufLen, PrevRefFile);
					nextlen = atol(RefBuf);
					firstlen = strlen(firstname);
					FixFSItems(PrevRefFile, "NextNameID", "NextNameLoc", nextname, firstname,
										 nextlen, firstlen);
					DCpfile::putval("Splits", "NextName", firstname, PrevRefFile);
					_ltoa(firstlen, numstr, 10);
					DCpfile::putval("Splits", "NextNameLen", numstr, PrevRefFile);
				}
				DeleteName(nextname);
				DCpfile::getval("Splits", "NextTitle", "", RefBuf, RefBufLen, PrevRefFile);
				if (*RefBuf) {
					char *nexttitle = NewName(RefBuf);
					if (strcmp(nexttitle, FirstTitle)) {  // mismatch, update other file's NextTitle
						DCpfile::getval("Splits", "NextTitleLen", "", RefBuf, RefBufLen, PrevRefFile);
						nextlen = atol(RefBuf);
						firstlen = strlen(FirstTitle);
						FixFSItems(PrevRefFile, "NextTitleID", "NextTitleLoc", nexttitle, FirstTitle,
											 nextlen, firstlen);
						DCpfile::putval("Splits", "NextTitle", FirstTitle, PrevRefFile);
						_ltoa(firstlen, numstr, 10);
						DCpfile::putval("Splits", "NextTitleLen", numstr, PrevRefFile);
					}
					DeleteName(nexttitle);
				}
			}
		}
	}

	if (nextused) {      // set next ref file info, where we used First
		if (!stricmp(RefFileName, NextRefFile))  // long as it's not us
			LogEvent(logwarn, 1, "Reference to nonexistent next file");
		else {
			DCpfile::putval("Splits", "PrevRef", RefFileName, NextRefFile);

			// that file may use our Lasts, so check it
			DCpfile::getval("Splits", "PrevName", "", RefBuf, RefBufLen, NextRefFile);
			if (*RefBuf) {  // empty PrevName means not used
				char *prevname = NewName(RefBuf);
				char *lastname = LastSplitID ? SplitNameList.find(LastSplitID) : CurrXFileName;
				long prevlen = 0;
				long lastlen = 0;
				if (strcmp(lastname, prevname)) { // mismatch, update other file's PrevName
					DCpfile::getval("Splits", "PrevNameLen", "", RefBuf, RefBufLen, NextRefFile);
					prevlen = atol(RefBuf);
					lastlen = strlen(lastname);
					FixFSItems(NextRefFile, "PrevNameID", "PrevNameLoc", prevname, lastname,
										 prevlen, lastlen);
					DCpfile::putval("Splits", "PrevName", lastname, NextRefFile);
					_ltoa(lastlen, numstr, 10);
					DCpfile::putval("Splits", "PrevNameLen", numstr, NextRefFile);
				}
				DeleteName(prevname);
				DCpfile::getval("Splits", "PrevTitle", "", RefBuf, RefBufLen, NextRefFile);
				if (*RefBuf) {
					char *prevtitle = NewName(RefBuf);
					if (strcmp(prevtitle, LastTitle)) {  // mismatch, update other file's PrevTitle
						DCpfile::getval("Splits", "PrevTitleLen", "", RefBuf, RefBufLen, NextRefFile);
						prevlen = atol(RefBuf);
						lastlen = strlen(LastTitle);
						FixFSItems(NextRefFile, "PrevTitleID", "PrevTitleLoc", prevtitle, LastTitle,
											 prevlen, lastlen);
						DCpfile::putval("Splits", "PrevTitle", LastTitle, NextRefFile);
						_ltoa(lastlen, numstr, 10);
						DCpfile::putval("Splits", "PrevTitleLen", numstr, NextRefFile);
					}
					DeleteName(prevtitle);
				}
			}
		}
	}
}


void
HTMwr::FixFSItems(char *ref, char *idsect, char *locsect,
	                char *oname, char *name, long olen, long len)
{
	// for each entry in sect in ref, get filename and offset, verify oname
	// replace oname with name in each file, update all other offsets in ref
	// for that file including those for xrefs in LinkLocs

	long knum = 1;
	char kstr[NumStrMax] = "1";
	char *id = NULL;
	char *fn = NULL;
	long loc = 0;
	long pad = 0;

	DCpfile::getval(idsect, kstr, "", RefBuf, RefBufLen, ref);
	while (*RefBuf) {  // entries are present
		if (!id || strcmp(id, RefBuf)) { // new file
			id = NewName(RefBuf);
			DCpfile::getval("SplitFileNames", id, "", RefBuf, RefBufLen, ref);
			fn = NewName(RefBuf);
		}
		DCpfile::getval(locsect, kstr, "", RefBuf, RefBufLen, ref);
		loc = atol(RefBuf);

		// process this key
		if (FixItem(fn, loc, oname, name, olen, len, pad, false)
		 && (len != olen))
			FixRefLocs(ref, fn, loc, len - olen);

		// try next key
		_ltoa(++knum, kstr, 10);
		DCpfile::getval(idsect, kstr, "", RefBuf, RefBufLen, ref);
	}
}



// .prj file handling


char *HTMwr::PrjFileName = NULL;
char *HTMwr::IDFileName = NULL;
char *HTMwr::FrameBook = NULL;
bool HTMwr::UseLocalFileID = false;


char *
HTMwr::GetFileID(char *fn)
{
	if (!fn || (*fn == '\0'))
		return "";

	char *name = BaseName(fn);
	char *px = NULL;

	if (XMLSource
	 || (!IDFileName
	  && !FindIDFile()))
		return "xx";

	*RefBuf = '\0';  // clear any previous content
	DCpfile::getval("FileIDs", name, "", RefBuf, RefBufLen, IDFileName);
	if (*RefBuf == '\0') {
		DCpfile::getval("FDK", "NextFileID", "", RefBuf, RefBufLen, IDFileName);
		if (*RefBuf == '\0')
			strcpy(RefBuf, "aa");

		// check to see if next ID is already in use, ugly reverse lookup
		while (FileIDUsed(RefBuf)) {
			if (!IncAlpha(RefBuf))
				strcat(RefBuf, "a");
		}
		px = NewName(RefBuf);
		if (!IncAlpha(RefBuf))
			strcat(RefBuf, "a");
		DCpfile::putval("FDK", "NextFileID", RefBuf, IDFileName);
		DCpfile::putval("FileIDs", name, px, IDFileName);
	}
	else
		px = NewName(RefBuf);

	DeleteName(name);
	return px;
}


char *
HTMwr::BaseName(char *fn, size_t len)
{
	char *name = NewName(fn, len);

  char *cpt = strrchr(name, '/');	// remove any path
	char *bpt = strrchr(name, '>'); // and any Frame path codes
	if (bpt > cpt)
		cpt = bpt++;
	if (!cpt)
		cpt = name;
	else
		cpt++;

	StripSuf(name);

	char *base = NewName(cpt);
	DeleteName(name);
	return base;
}


bool
HTMwr::IncAlpha(char *str)
{
	if (!str || (*str == '\0'))
		return false;

	bool done = false;

	for (char *cpt = str + strlen(str) - 1; (!done) && (cpt >= str); cpt--) {
		switch (*cpt) {
			case 'z':
				*cpt = 'a';
				break;
			case 'Z':
				*cpt = 'A';
				break;
			case '9':
				*cpt = '0';
				break;
			case 'k':  // skip lower-case L
				*cpt = 'm';
				done = true;
				break;
			case 'N':  // and upper-case O
				*cpt = 'P';
				done = true;
				break;

			default:
				if (isalnum(*cpt)) {
					(*cpt)++;
					done = true;
				}
				break;			
		}
	}

	return done;  // false for overflow
}


bool
HTMwr::FileIDUsed(char *id)
{
	char *fid = NewName(id);
	char *buf = new char[MaxIniLen];
	char *bpt = buf;
	bool rval = false;

	DCpfile::getval("FileIDs", NULL, "", buf, MaxIniLen, IDFileName);
	while (*bpt) {
		DCpfile::getval("FileIDs", bpt, "", RefBuf, RefBufLen, IDFileName);
		if (!stricmp(fid, RefBuf)) {
			rval = true;
			break;
		}
		while (*bpt)
			bpt++;
		bpt++;
	}

	strcpy(id, fid);  // restore original
	delete [] buf;
	return rval;
}


bool
HTMwr::FindIDFile(void)
{
	FILE *fp = NULL;

	// try current dir for mif2go.ini
	IDFileName = NewName("./mif2go.ini");
	if ((fp = fopen(IDFileName, "rb")) != NULL) {
		fclose(fp);
		return true;
	}	
	DeleteName(IDFileName);

	// try parent directory
	IDFileName = NewName("../mif2go.ini");
	if ((fp = fopen(IDFileName, "rb")) != NULL) {
		fclose(fp);
		return true;
	}	
	DeleteName(IDFileName);

	// try dir containing template, if any
	// give up
	return false;
}



// end of dwhtmpp.cpp

