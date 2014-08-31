// dcfilp.cpp contains code for the dcl profile file class, JHG
// Copyright (c) 2001 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dcnode.h"
#include "dclist.h"
#include "dcfile.h"
#include "dcfilr.h"
#include "dcfilw.h"
#include "dcfilp.h"
#include <windows.h>

#pragma warning ( disable : 4237 4710 )

DCnlist DCpfile::ProfileFiles;
bool DCpfile::ending = false;
char *DCpfile::histitem = NULL;
char *DCpfile::valbuf = NULL;
unl DCpfile::valsize = 0;

// list structures; all items in presentation order
// sect dcnlist: name = NULL,    id = keys list with one item, heading list
//            or name = sect,    id = keys list
// heading dclist: id = starting blank lines (optional)
//                   or starting comments (ending at first blank line)
// keys dcnlist: name = NULL,    id = head list (optional)
//            or name = set key, id = line list 
// head dclist:  id = char * to starting comment (0 or more)
//							   or char * to empty str for blank line(s)
//               then char * to [section] (only one)
//                 or char * to comment or macro (0 or more)
// line dclist:  id = char * to comment or macro (0 or more)
//							   or char * to empty str for blank line(s)
//                 or char * to "= value" (only one)


// read one item from internal lists using PPS protocol

unl DCpfile::pread(const char *sect, const char *key, char *val, const char *defval, unl vsize)
{
	DCnlist *keys = NULL;
	unl len = 0;
	char *nm = NULL;
	DClist *lines = NULL;

	// if sect is NULL, copy all sects to val
	if (sect == NULL) {
		valbuf = val;
		valsize = vsize;
		sections.all(dolist);
		*valbuf = '\0';
		return (valbuf - val);
	}
	// look for sect, found
	else if ((keys = (DCnlist *) sections(sect)) != NULL) {
		// if key is NULL, copy all keys in sect to val
		if (key == NULL) {
			valbuf = val;
			valsize = vsize;
			keys->all(dolist);
			*valbuf = '\0';
			return (valbuf - val);
		}
		// if key is empty, copy all lines including comments
		else if (*key == '\0') { 
			if (*sect == ';')  // reading deleted section for restore
				secthead = NewName(sect + 1); // use restored name
			else
				secthead = NewName(sect); // needed by doline
			valbuf = val;
			valsize = vsize;
			keys->all(doline);
			*valbuf = '\0';
			return (valbuf - val);
		}
		// if key found, copy value up to vsize to val
		else if ((lines = (DClist *) (keys->getitem(key))) != NULL) {
			while ((lines = lines->next) != NULL) {
				if (((nm = (char *) lines->id) != NULL)
				 && (*nm == '=')) {
					nm++;
					if ((len = strlen(nm)) >= vsize)
						len = vsize - 1;
					strncpy(val, nm, len);
					val[len] = '\0';
					return len;
				}
			}
			*val = '\0';
			return 0; // empty value
		}
	}
	// if sect or key not found, copy defval to val
	if ((len = strlen(defval)) >= vsize)
		len = vsize - 1;
	strncpy(val, defval, len);
	val[len] = '\0';
	return len;
}


DCilist *DCpfile::pgetkeys(const char *sect)
{
	DCilist *klist = (DCilist *) keylists(sect);
	if (klist)
		return klist;

	klist = new DCilist();
	keylists.add(sect, (long) klist);

	DCnlist *keys = (DCnlist *) sections(sect);

	if (!keys)
		return NULL;

	char *name = NULL;
	char *line = NULL;
	DClist *lines = NULL;

	while ((keys = keys->next) != NULL) {
		name = (char *) keys->name;
		if (!name
		 || (*name == '\0')
		 || (*name == ';'))
			continue;

		lines = (DClist *) keys->id;
		while ((lines = lines->next) != NULL) {
			line = (char *) lines->id;
			if (*line == '=') {
				line = line + 1;
				while (*line == ' ')
					line++;
				klist->append((long) NewName(line), (long) NewName(name));
				break;
			}
		}
	}

	return klist;
}


void DCpfile::prelkeys(const char *sect)
{
	DCilist *klist = (DCilist *) keylists(sect);
	if (klist) {
		keylists.remove((long) klist);
		klist->all(delkeys);
		klist->empty();
		delete klist;
	}
}

void DCpfile::delkeys(long v, long i)
{
	if (v)
		DeleteName((char *) v);
	if (i)
		DeleteName((char *) i);
}



// produce list of nul-delimited settings

void DCpfile::dolist(const char *key, long)
{
	if (!valsize
	 || !key    // exclude non-keys (blanks, comments, macros)
	 || (*key == '\0')
	 || (*key == ';'))
		return;

	addval(key, false);
}


char *DCpfile::secthead = NULL;


// produce all section lines, \n delimited

void DCpfile::doline(const char *key, long vp)
{
	if (!valsize
	 || (vp == NULL))
		return;

	DClist *lines = (DClist *) vp;
	char *line = NULL;
	char *str = NULL;

	while ((lines = lines->next) != NULL) {
		line = (char *) lines->id;

		if (*line == ']') {
			str = NewName("[", strlen(secthead) + strlen(line) + 1);
			strcat(str, secthead);
			strcat(str, line);
			addval(str);
		}
		else if (*line == '=') {
			str = NewName(key, strlen(key) + strlen(line));

			strcat(str, line);
			addval(str);
		}
		else   // anything else (blank, comment or macro)
			addval(line);
	}
}


// add a single item to the return value buffer

void DCpfile::addval(const char *str, bool nl)
{
	unl len = str ? strlen(str) : 0;

	if (!valsize)
		return;

	if (len >= valsize)
		len = valsize - 1;

	if (str
	 && *str)
		strncpy(valbuf, str, len);

	valbuf[len] = nl ? '\n' : '\0';
	valbuf += len + 1;
	valsize -= len + 1;
}


// write one item to internal lists using the PPS protocol

bool DCpfile::pwrite(const char *sect, const char *key, const char *val)
{
	DCnlist *keys = NULL;
	DClist *lines = NULL;
	char *str = NULL;
	char *line = NULL;

	DCnlist *item = NULL;
	char *nm = NULL;
	long ni = 0;

	// look for sect, then key
	if (!sect) // API call flushes cache, ignore it
		return false;

	if (moverr    // already done
	 && changed)  // successfully
		return true;

	changed = true;

	// if sect not found, add to sections
	if ((keys = (DCnlist *) sections(sect)) == NULL) {
		sections.addnew(sect, (long) (keys = new DCnlist));
		lines = new DClist;
		lines->add((long) NewName(""));  // starting blank line
		lines->add((long) NewName("]")); // flag for section name
		keys->add(NULL, (long) lines); // for section head
		ni = 0;
		if (val
		 && strcmp(val, " ")) {  // used when adding section
			if (key) {  // add setting too
				//keys->addnew(key, (long) NewName(val));
				keys->addnew(key, (long) (lines = new DClist));
				str = NewName("=", strlen(val) + 1);
				strcat(str, val);
				lines->add(ni = (long) str);
			}
			else { // comment or macro line, add to head
				//keys->add(NULL, (long) NewName(val));
				lines->add(ni = (long) NewName(val));
			}
			if (histitem)
				insertsethist(lines, ni);
		}
		else if (key) { // key with no value in new section
			//keys->addnew(key, (long) NewName(" "));
			str = NewName("= ");
			keys->addnew(key, (long) (lines = new DClist));
			lines->add(ni = (long) str);
			if (histitem)
				insertsethist(lines, ni);
		}
		else if (histitem)
			insertsecthist(keys);

		return true;
	}

	// section was found
	// if key and val are NULL, comment out section (was: empty list of keys)
	if (!key) {
		if (!val) {
			char *nsect = NewName(";", strlen(sect) + 1);
			strcat(nsect, sect);
			sections.replace(nsect, (long) keys);
			if (histitem)
				insertsecthist(keys);
			//keys->empty();
			//sections.remove((long) keys);
		}
		else if (!strcmp(val, " ")) {  // adding section that is already there
			if (histitem)
				insertsecthist(keys);
		}
		else { // comment or macro line
			//keys->add(NULL, (long) NewName(val));
			if (keys->next == NULL)  // shouldn't happen
				keys->add(NULL, (long) (lines = new DClist));
			else if (keys->next->name != NULL)  // no secthead, add one
				keys->insert(NULL, (long) (lines = new DClist));
			else
				lines = (DClist *) keys->next->id;
			lines->add((long) NewName(val));
		}

		return true;
	}

	// if key is empty, delete non-comments, add val if any to end
	if (*key == '\0') {
		DCnlist *nkeys = remsets(keys);
		sections.changeid((long) nkeys, (long) keys);
		keys = nkeys;

		if (val) {
			//keys->add(NULL, (long) NewName(val));
			if (keys->next == NULL)
				keys->add(NULL, (long) (lines = new DClist));
			else if (keys->next->name != NULL)
				keys->insert(NULL, (long) (lines = new DClist));
			else
				lines = (DClist *) keys->next->id;
			lines->add((long) NewName(val));
		}

		if (histitem)
			insertsecthist(keys);

		return true;
	}

	// key was specified
	// if key not found, add to end of list
	if ((nm = (char *) (keys->getitem(key, &item, false, false))) == NULL) {
		keys->addnew(key, (long) (lines = new DClist));
		if (val) {
			str = NewName("=", strlen(val) + 1);
			strcat(str, val);
		}
		else
			str = NewName("= ");
		lines->add(ni = (long) str);
	}
	else if (val) {	// if key found, replace its val with ptr to copy of new val
	                // but do not free old value
		lines = (DClist *) item->id;
		while ((lines = lines->next) != NULL) {
			line = (char *) lines->id;
			if (*line == '=')
				break;
			line = NULL;
		}
		while (*val == '=')
			val++;
		str = NewName("=", strlen(val) + 1);
		strcat(str, val);
		if (line)  // found, replace it
			lines->id = (long) str;
		else {     // go back to start of list to add it
			lines = (DClist *) item->id; 
			lines->add((long) str);
		}
		ni = (long) str;
	}
	else {	// if new val is NULL, comment out (was delete) key from list
		lines = (DClist *) item->id;
		char *nkey = NewName(";=", strlen(key) + 2);
		strcat(nkey, key);
		keys->replace(nkey, (long) nm);
		// keys->remove((long) nm);
	}

	if (histitem)
		insertsethist(lines, ni);

	return true;
}


// rename a section in the sections list

bool DCpfile::rsect(const char *sect, const char *nsect)
{
	long id = 0;

	if (moverr    // already done
	 && changed)  // successfully
		return true;

	// look for sect and get its id (keylist)
	if (!sect 
	 || !nsect)
		return false;

	if ((id = sections(sect)) == 0)
		return false;

	sections.replace(NewName(nsect), id);

	if (histitem)
		insertsecthist((DCnlist *) id);

	changed = true;
	return true;
}


// add a history line for this setting

void DCpfile::insertsethist(DClist *lines, long ni)
{
	DClist *lp = NULL;

	if (!histitem
	 || !lines)
		return;

	if (!ni) {
		for (lp = lines; lp; lp = lp->next) {
			ni = lp->id;
			if (ni
			 && (*((char *) ni) == '='))
				break;
		}
	}

	lines->insertbefore(ni, (long) histitem);
}


// add a history line for this section

void DCpfile::insertsecthist(DCnlist *keys)
{
	DClist *lines = NULL;
	DClist *lp = NULL;
	long ni = 0;

	if (!histitem)
		return;

	if (keys->next
	 && (keys->next->name == NULL)  // section head
	 && ((lines = (DClist *) keys->next->id) != NULL)) {
		for (lp = lines; lp; lp = lp->next) {
			ni = lp->id;
			if (ni
			 && (*((char *) ni) == ']'))
				break;
		}
		lines->insertbefore(ni, (long) histitem);
	}
}


// processing for merge functions

unl DCpfile::preadall(const char *sect, const char *key, char *val, unl vsize)
{
	DCnlist *sects = NULL;
	DCnlist *keys = NULL;
	DClist *lines = NULL;
	char *line = NULL;
	unl blen = 0;
	long pos = 0;
	char *str = NULL;
	const char *set = NULL;
	unl len = 0;

	if (key == NULL) {  // read all section instance content into one buffer
		sects = &sections;
		while (sects->next) {
			sects = sects->next;
			if (sects->name
			 && !stricmp(sect, sects->name)) {
				keys = (DCnlist *) sects->id;
				keys = keys->next; 
				lines = (DClist *) keys->id;
				lines = lines->next;
				while (lines
				 && ((line = (char *) lines->id) != NULL)) {
					if (*line == ']') {
						len = strlen(line) + strlen(sect);
						if (pos++)  // comment out all heads after the first
							str = NewName(";[", len + 2);
						else        // first one
							str = NewName("[", len + 1);
						strcat(str, sect);
						strcat(str, line);
						blen = putline(str, val, blen, vsize);
						DeleteName(str);
					}
					else
						blen = putline(line, val, blen, vsize);
					lines = lines->next;
				}
				// rest of content is the same for all
				while ((keys = keys->next) != NULL) {
					set = keys->name;
					lines = (DClist *) keys->id;
					lines = lines->next;
					while (lines
					 && ((line = (char *) lines->id) != NULL)) {
						if ((*line == '=')
						 && (set != NULL)) {
							len = strlen(line) + strlen(set);
							str = NewName(set, len);
							strcat(str, line);
							blen = putline(str, val, blen, vsize);
							DeleteName(str);
						}
						else
							blen = putline(line, val, blen, vsize);
						lines = lines->next;
					}
				}
			}
			if (blen >= vsize)
				break;
		}
		return blen;
	}

	// read all setting instance values nul-delimited into buffer
	keys = (DCnlist *) sections(sect);
	keys = keys->next;  // skip null header
	while ((keys = keys->next) != NULL) {
		set = keys->name;
		if (set
		 && !stricmp(set, key)) {
			lines = (DClist *) keys->id;
			lines = lines->next;
			while (lines
			 && ((line = (char *) lines->id) != NULL)) {
				if (*line == '=') {
					len = strlen(line + 1);
					if ((blen + len + 1) >= vsize)
						continue;
					strcpy(val + blen, line + 1);
					blen += len + 1;
				}
				lines = lines->next;
			}
		}
		if (blen >= vsize)
			break;
	}
	val[blen] = '\0';
	return blen;
}


long DCpfile::putline(char *line, char *buf, unl blen, unl bsize)
{
	if (!line
	 || !buf
	 || (blen >= bsize))
		return blen;

	unl len = strlen(line);
	if ((blen + len + 1) > bsize)
		return blen;

	if (len) { // non-blank line
		strcpy(buf + blen, line);
		blen += len;
	}
	buf[blen++] = '\n';
	buf[blen] = '\0';
	return blen;
}


bool DCpfile::delall(const char *sect, const char *key)
{
	DCnlist *sects = NULL;
	DCnlist *keys = NULL;
	long pos = 0;
	char *str = NULL;
	const char *set = NULL;
	char *nkey = NULL;
	char *np = NULL;


	if (key == NULL) {  // delete all section instances after the first
		str = NewName(";", strlen(sect) + 1);
		strcat(str, sect);
		sects = &sections;
		while (sects->next) {
			sects = sects->next;
			if (sects->name
			 && !stricmp(sect, sects->name)
			 && pos++) {
				changed = true;
				sects->name = NewName(str);
				if (histitem)
					insertsecthist((DCnlist *) sects->id);
			}
		}
		DeleteName(str);
		return true;
	}
	
	// delete all setting instances after the first
	np = nkey = NewName(key);  // get rid of const
	while (*nkey == '=')
		nkey++;
	str = NewName(";=", strlen(nkey) + 2);
	strcat(str, nkey);
	keys = (DCnlist *) sections(sect);
	keys = keys->next;  // skip null header
	while ((keys = keys->next) != NULL) {
		set = keys->name;
		if (set
		 && !stricmp(set, key)
		 && pos++) {
			changed = true;
			keys->name = NewName(str);
			if (histitem)
				insertsethist((DClist *) keys->id);
		}
	}
	DeleteName(str);
	DeleteName(np);
	return true;
}


// change the full content of a section in the sections list

bool DCpfile::csect(const char *sect, const char *val)
{
	long id = 0;
	DCnlist *keys = NULL;
	DCnlist *xkeys = NULL;

	if (moverr    // already done
	 && changed)  // successfully
		return true;

	// look for sect and get its id (keylist)
	if (!sect)
		return false;
	changed = true;

	keys = new DCnlist;

	if (((id = sections(sect)) != 0)          // section exists
	 && sections.changeid((long) keys, id)) { // and could change keys
		xkeys = (DCnlist *) id; // delete old keys
		xkeys->empty();
		delete xkeys;
	}
	else  // put in as new section at end
		sections.addnew(sect, (long) keys);

	// keys is the id of the new section, either way
	if (val)
		addsectlines(sect, keys, val);
	if (histitem)
		insertsecthist(keys);

	return true;
}


// parse and add buffer contents to a new section

void DCpfile::addsectlines(const char *sect, DCnlist *keys, const char *val)
{
	DClist *lines = new DClist;
	DClist *lastlines = NULL;
	DCnlist *okeys = keys;
	long nextid = 0;
	char *nl = NULL;
	char *cpt = NULL;
	char *equ = NULL;
	char *sectname = NULL;
	char *sectend = NULL;
	bool sectmatch = false;
	bool addedsect = false;
	long snlen = 0;
	char *setname = NULL;
	char *setend = NULL;
	long bcnt = 0;
	bool inhead = true;
	bool pend = false;
	bool pendadded = false;

	cpt = NewName(val);
	keys->add(NULL, (long) lines); // set up for sect name

	while (*cpt) {
		if (*cpt == '\r')
			cpt++;

		nl = strchr(cpt, '\n');
		if ((nl == NULL)
		 && (strlen(cpt) != 0))   // last line may not have \n
		  nl = cpt + strlen(cpt); // so point nl at null

		// line starts with cpt, ends with nl
		// group blanks and comments with head or a tuple

		if (nl == cpt) { // blank line is delimiter
			bcnt++;
			if (!inhead  // may precede or follow sectname
			 && !pend) { // only first one stays with prev item
				lastlines = lines;
				lines = new DClist;  // to use for next tuple
				pend = true;
			}
			else if (pend
			 && !pendadded) { 
				lastlines->add(lines);
				lines->id = 0;
				lines->next = NULL;
				addblank(lines, lastlines, bcnt);
				pendadded = true;
			}
		}
		else if (*cpt == '[') {  // section name
			inhead = true;
			// skip to end of section name
			sectname = cpt + 1;
			sectend = cpt + 1;
			while ((sectend < nl)
			 && *sectend
			 && (*sectend != ']'))
				sectend++;
			snlen = sectend - sectname;

			if (strnicmp(sect, sectname, snlen)) {
				// different section added before or after edited one
				DCnlist *xkeys = NULL;
				if (!nextid  // identify section after original one
				 && !addedsect  // not already checked
				 && sections.find((long) keys, &xkeys)
				 && xkeys
				 && xkeys->next)
					nextid = xkeys->next->id;
				sections.insertat((sectname = NewName(sectname, snlen)),
				                  (long)  (keys = new DCnlist),
				                  (sectmatch ? nextid : (long) keys));
				if (!pend)
					lines = new DClist;
				keys->add(NULL, (long) lines);
				addedsect = true;
			}
			else {
				sectmatch = true;
				if (addedsect) {  // new sect was added first
					keys = okeys;
					lines = (DClist *) keys->next->id;
					lastlines = NULL;
				}
			}
			if (bcnt)
				addblank(lines, lastlines, bcnt);
			else
				lines->add((long) NewName(""));

			if (*sectend == ']')
				lines->add((long) NewName(sectend, nl - sectend));
			else 
				lines->add((long) NewName("]"));

			inhead = false;
			lastlines = lines;
			lines = new DClist;  // for next sect or tuple
			pend = true;
			pendadded = false;
		}
		else if (istuple(cpt)) {
			if (*cpt == ';') { // deleted setting
				setname = cpt + 1;
				while (*setname == '=')
					setname++;
			}
			else
				setname = cpt;
			equ = strchr(setname, '='); // always present if istuple == true
			for (setend = equ; setend > setname; setend--) {
				if (!isascii(*(setend - 1))
				 || !isspace(*(setend - 1)))
					break;
			}
			snlen = setend - cpt;
			setname = NewName(cpt, snlen);
			if (!pend)  // need new list
				lines = new DClist;
			pend = false;
			addblank(lines, lastlines, bcnt);
			lines->add((long) NewName(equ, nl - equ));
			keys->add(setname, (long) lines);
			lastlines = lines;
			lines = new DClist;  // for next sect or tuple
			pend = true;
			pendadded = false;
		}
		else {  // must be macro or comment line
			addblank(lines, lastlines, bcnt);
			lines->add((long) NewName(cpt, nl - cpt));
		}

		if (*nl == '\0')
			break;  // end of last line
		cpt = nl + 1;
	}
	if (pend) {  // blank lines or comments after last setting
		while ((lines = lines->next) != NULL)
			lastlines->add(lines->id);
	}
}


// see if a line is a setting or not

bool DCpfile::istuple(const char *cpt)
{
	if (*cpt == ';') {
		if ((*(cpt + 1) != '=')
		 || (*(cpt + 2) == '='))  // Carolyns nasty comments
			return false;
		cpt += 2;  // skip deleted setting marker
	}

	const char *equ = strchr(cpt, '=');
	if (!equ)
		return false;

	const char *mac = strchr(cpt, '<');
	if (mac
	 && (mac < equ))  // macro can be on right in tuple
		return false;

	const char *nl = strchr(cpt, '\n');
	if (nl
	 && (nl < equ))  // crossed to next line
		return false;

	return true;
}


// move section within its file by rearranging sections list

bool DCpfile::mvsect(const char *sect, long opos, long npos)
{
	long oid = 0;
	DCnlist *optr = NULL;
	DCnlist *nptr = NULL;
	DCnlist *nsect;
	long cnt = 0;
	long pos = 0;
	bool named = false;

	
	if (moverr    // already done
	 && changed)  // successfully
		return true;

// look for sect and check its id (keylist)
	if (!sect)
		return false;

	if (opos == npos)
		return true;  // nothing to do

	if ((oid = sections.getitem(sect, &optr, false, false)) == 0)
		return false;

	nsect = new DCnlist();
	cnt = sections.count();
	nptr = sections.next;

	if (npos > opos)
		npos++;
	else
		opos++;

	for (pos = 0; pos < cnt; pos++) {
		if (!nptr
		 && (npos != pos))
			return false;

		// handle comments and blank lines at start
		if (!named
		 && (nptr->name == NULL)) { // file heading
			nsect->add(nptr->name, nptr->id);
			named = true;
			nptr = nptr->next;
			if (!nptr)
				return false;  // no sections in file
		}

		if (pos == opos) { // at original location
			if ((nptr->id != oid)
			 || (nptr->name != optr->name))
				return false;
			// skip this item
		}
		else if (pos == npos) {  // at new location
			nsect->add(optr->name, oid);
			continue;  // don't advance nptr
		}
		else
			nsect->add(nptr->name, nptr->id);

		nptr = nptr->next;
	}

	if (histitem)
		insertsecthist((DCnlist *) oid);

	// nsect is now a replacement for sections
	sections = *nsect;

	changed = true;
	return true;
}


// move setting within section by rearranging keys list

bool DCpfile::mvset(const char *sect, const char *set, long opos, long npos)
{
	long oid = 0;
	DCnlist *secptr = NULL;
	DCnlist *optr = NULL;
	DCnlist *nptr = NULL;
	DCnlist *keys = NULL;
	DCnlist *nset = NULL;
	long cnt = 0;
	long pos = 0;
	bool named = false;

	if (moverr    // already done
	 && changed)  // successfully
		return true;

	// look for sect and check its id (keylist)
	if (!sect)
		return false;

	if (opos == npos)
		return true;  // nothing to do

	if ((oid = sections.getitem(sect, &secptr, false, false)) == 0)
		return false;  // no such section

	keys = (DCnlist *) oid;
	nset = new DCnlist();
	cnt = keys->count();
	nptr = keys->next;

	if ((oid = keys->getitem(set, &optr, false, false)) == 0)
		return false;  // no such setting

	if (npos > opos)
		npos++;
	else
		opos++;

	for (pos = 0; pos < cnt; pos++) {
		if (!nptr
		 && (npos != pos))
			return false;  // no content in section

		// handle comments and blank lines
		if (!named
		 && (nptr->name == NULL)) { // section heading
			nset->add(nptr->name, nptr->id);
			named = true;
			nptr = nptr->next;
			if (!nptr)
				return false;  // no settings in section
		}

		if (pos == opos) { // at original location
			if ((nptr->id != oid)
			 || (nptr->name != optr->name))
				return false;  // didn't match, error
			// skip this item
		}
		else if (pos == npos) {  // at new location
			nset->add(optr->name, oid);
			continue;  // don't advance nptr
		}
		else
			nset->add(nptr->name, nptr->id);

		nptr = nptr->next;
	}

	if (histitem)
		insertsethist((DClist *) oid);

	// nset is now a replacement for keys
	secptr->id = (long) nset;

	changed = true;
	return true;
}


// rename setting in keys list

bool DCpfile::rset(const char *sect, const char *set, const char *nset)
{
	long id = 0;

	if (moverr    // already done
	 && changed)  // successfully
		return true;

	// look for sect and get its id (keylist)
	if (!sect 
	 || !set
	 || !nset)
		return false;

	if ((id = sections(sect)) == 0)
		return false;   // no such section

	DCnlist *keys = (DCnlist *) id;
	long kid = keys->getitem(set);
	if (kid == 0)
		return false;  // setting not in section

	keys->replace(NewName(nset), kid);

	if (histitem)
		insertsethist((DClist *) kid);

	changed = true;

	return true;
}


// restore section or setting, name of item to restore starts with ';'

bool DCpfile::restore(const char *sect, const char *set)
{
	DCnlist *keys = NULL;
	DClist *lines = NULL;

	if (moverr    // already done
	 && changed)  // successfully
		return true;

	if (!sect)
		return false;

	if (set
	 && (*set != ';'))
		return false;

	if (!set
	 && (*sect != ';'))
		return false;

	if ((keys = (DCnlist *) sections(sect)) == NULL)
		return false;

	if (set) {  // restore setting
		if ((lines = (DClist *) keys->getitem(set)) == NULL)
			return false;
		keys->replace(NewName(set + 2), (long) lines);
		if (histitem)
			insertsethist(lines);
	}
	else {       // restore section
		sections.replace(NewName(sect + 1), (long) keys);
		if (histitem)
			insertsecthist(keys);
	}

	changed = true;
	return true;
}


// move [End] to end of section list

void DCpfile::fixend(void)
{
	long id = 0;
	char *ename = NULL;
	if ((id = sections("end")) == 0)
		return;
	ename = sections.find(id);
	sections.remove(id);
	sections.add(ename, id);
	changed = true;
}


// remove all settings, macros, and blank lines in keys, keep comments


DCnlist *DCpfile::remsets(DCnlist *keys)
{
	DCnlist *nkeys = new DCnlist;
	DClist *nlines = new DClist;
	DClist *lines = NULL;
	char *line = NULL;

	nkeys->add(NULL, (long) nlines);

	while ((keys = keys->next) != NULL) {
		lines = (DClist *) keys->id;
		while ((lines = lines->next) != NULL) {
			if (*(line = (char *) lines->id) == ';')
				nlines->add((long) line);
		}
	}

	return nkeys;
}


// initial load of file

bool DCpfile::pset(void)
{
	DCnlist *keys = new DCnlist;
	DClist *lines = new DClist;
	DClist *lastlines = NULL;
	char buf[1024];
	char *equ = NULL;
	char *nl = NULL;
	char *sectname = NULL;
	char *sectend = NULL;
	long snlen = 0;
	char *setname = NULL;
	char *setend = NULL;
	bool atstart  = true;
	bool pend = false;
	bool pendadded = false;
	long len = 0;
	long bcnt = 0;


	if (Ferr != fok)
		return false;

	changed = false;
	moverr = false;

	// heading is first section with NULL name
	sections.add(NULL, (long) keys);
	keys->add(NULL, (long) lines);

	// read in entire file to cache
	while (getLine(buf, 1024)) {

		// trim line endings
		nl = buf + strlen(buf) - 1;
		while ((nl >= buf)
		 && ((*nl == '\r')
		  || (*nl == '\n')))
			*nl-- = '\0';
		len = (nl - buf) + 1;

		if (*buf == '\0')  {   // blank lines
			bcnt++;
			if (!atstart
			 && !pend) {
				lastlines = lines;
				lines = new DClist;  // for next sect or tuple
				pend = true;
			}
			else if (pend
			 && !pendadded) { 
				lastlines->add(lines);
				lines->id = 0;
				lines->next = NULL;
				addblank(lines, lastlines, bcnt);
				pendadded = true;
			}
		}
		else if (*buf == '[') { // section start
			sectend = strchr(buf, ']');
			if (!sectend) { // missing ']', whole nine is name
				snlen = len - 1;
				sectend = NewName("]");
			}
			else {
				snlen = sectend - (buf + 1);
				sectend = NewName(sectend, (len - (snlen + 1)));
			}
			sectname = NewName(buf + 1, snlen);
			sections.add(sectname, (long) (keys = new DCnlist));
			if (!pend)  // need new list
				lines = new DClist;
			pend = false;
			addblank(lines, lastlines, bcnt);
			lines->add((long) sectend);
			keys->add(NULL, (long) lines);
			lastlines = lines;
			lines = new DClist;  // for next sect or tuple
			pend = true;
			pendadded = false;
		}
		else if (istuple(buf)) {  // setting
			if (*buf == ';') { // deleted setting
				setname = buf + 1;
				while (*setname == '=')
					setname++;
			}
			else
				setname = buf;
			equ = strchr(setname, '=');
			for (setend = equ; setend > setname; setend--) {
				if (!isascii(*(setend - 1))
				 || !isspace(*(setend - 1)))
					break;
			}
			snlen = setend - buf;  // include the ;= in the key
			setname = NewName(buf, snlen);
			if (!pend)  // need new list
				lines = new DClist;
			pend = false;
			addblank(lines, lastlines, bcnt);
			lines->add((long) NewName(equ));
			keys->add(setname, (long) lines);
			lastlines = lines;
			lines = new DClist;  // for next sect or tuple
			pend = true;
			pendadded = false;
		}
		else if (*buf == ';') { // comment
			addblank(lines, lastlines, bcnt);
			lines->add((long) NewName(buf, len));
			atstart = false;  // so that blank line after comments ends heading
		}
		else {  // must be macro line
			addblank(lines, lastlines, bcnt);
			lines->add((long) NewName(buf, len));
		}
	}

	// blank lines or comments or macros
	// after last setting or section head
	if (pend) { 
		while ((lines = lines->next) != NULL)
			lastlines->add(lines->id);
	}

	//fclose(fptr);  // done with it, but leave name
	//fptr = NULL;
	//return (Ferr == fok);
	CloseFile();
	return true;
}


// add blank lines between items to retain original idea of spacing

void DCpfile::addblank(DClist *lines, DClist *lastlines, long &bcnt)
{
	if (!bcnt)
		return;

	if ((bcnt > 1)
	 && lastlines) {
		lastlines->add((long) NewName(""));
		bcnt--;
	}

	while (bcnt-- > 0)
		lines->add((long) NewName(""));

	bcnt = 0;
}


// write out file to disk

void DCpfile::pflush(void)
{
	// write cache to file and close
	if (!changed)
		return;

	char *tn = NULL;
	char *per = NULL;
	DCwfile *wf = NULL; 
	static char *savetn = NULL;
	static char *savefname = NULL;

	// create new file with ext .new
	if (!moverr) {
		tn = NewName(pfname);

		per = strrchr(tn, '.');
		if (per)
			strcpy(per + 1, "new");
		wf = new DCwfile(tn, ftxt); 

		// write each section, ending with a blank line
		fixend();
		sections.writeall(wf, pwsect);

		// delete original, rename .new to orig ext
		wf->CloseFile();
#if 0
		if (!DeleteFile(pfname))
			fucked = ::GetLastError();
		else
			fucked = 0;
#endif

		histitem = NULL;
	}
	else {
		tn = savetn;
		pfname = savefname;
	}

	if (!MoveFileEx(tn, pfname, MOVEFILE_COPY_ALLOWED
	                    | MOVEFILE_REPLACE_EXISTING
											| MOVEFILE_WRITE_THROUGH)) {
		//LogEvent(logerr, 1, "Failed to replace file ", fname, 
		//                    ", left file ", tn);
		moverr = true;
		savefname = NewName(pfname);
		savetn = NewName(tn);
		MovErrVal = ::GetLastError();
		MovErrFptr = this;
	}
	else {
		moverr = false;
		savefname = NULL;
		savetn = NULL;
		MovErrVal = 0;
		MovErrFptr = NULL;
	}
}


// called on each item (keys list) in the sections list

void DCpfile::pwsect(DCwfile *wf, char *n, long i)
{
	if (i == 0)
		return;

	DCnlist *keys = (DCnlist *) i;

	if (n != NULL)  // section name
		sectname = n;

	keys->writeall(wf, pwkey);
}


// called on each item (lines list) in the keys lists

void DCpfile::pwkey(DCwfile *wf, char *n, long i)
{
	if (i == 0)
		return;

	DClist *lines =  (DClist *) i;

	if (n != NULL)  // setting
		setname = n;

	lines->writeall(wf, pwline);
}


char *DCpfile::sectname = NULL; // from pwsect for pwline
char *DCpfile::setname = NULL;  // from pwkey for pwline


// called on each item (single line) in the lines lists

void DCpfile::pwline(DCwfile *wf, long i)
{
	if (i == 0)
		return;

	char *line = (char *) i;

	if (*line == ']') {        // section name
		wf->putChar('[');
		wf->putStr(sectname);
	}
	else if (*line == '=')    // tuple, prefix with setting name
		wf->putStr(setname);

	if (*line != '\0')         // not a blank line
		wf->putStr(line);     // comment, macro, or section name

	wf->putCr();;
}


// write all files before exit

void DCpfile::pflushx(const char *fname, long ptr)
{
	DCpfile *pf = NULL;

	// called for each list item
	if (!fname || !ptr)
		return;

	pf = (DCpfile *) ptr;
	pf->pflush();

	if (!MovErrFptr)
		delete pf;
}


// public interface functions


// get an item using the PPS protocol

unl DCpfile::getval(const char *sect, const char *key, const char *defval,
	 char *val, unl vsize, const char *fname)
{
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// if in cache, read from cache
		return pf->pread(sect, key, val, defval, vsize);
	}
	else {
		// add to cache and read
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			return pf->pread(sect, key, val, defval, vsize);;
		}
	}

	return false;
	//return GetPrivateProfileString(sect, key, defval, val, vsize, fname);
}


// get multiple duplicate items

unl DCpfile::getvalall(const char *sect, const char *key,
	 char *val, unl vsize, const char *fname)
{
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// if in cache, read from cache
		return pf->preadall(sect, key, val, vsize);
	}
	else {
		// add to cache and read
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			return pf->preadall(sect, key, val,  vsize);
		}
	}

	return false;
}

// delete multiple duplicate items leaving first one

unl DCpfile::deleteall(const char *sect, const char *key, const char *fname)
{
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// if in cache, read from cache
		return pf->delall(sect, key);
	}
	else {
		// add to cache and read
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			return pf->delall(sect, key);
		}
	}

	return false;
}



DCilist *DCpfile::Keylist = NULL;
DCilist *DCpfile::Keyptr = NULL;
char *DCpfile::KeyFile;
char *DCpfile::KeySect;


bool DCpfile::getkeys(const char *sect, const char *fname)
{
	//if (Keylist) {
	//	Keyptr = Keylist->next;
	//	return true;
	//}

	KeyFile = NewName(fname);
	KeySect = NewName(sect);

	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// if in cache, read from cache
		Keylist = pf->pgetkeys(sect);
		if (Keylist) {
			Keyptr = Keylist->next;
			return true;
		}
	}
	else {
		// add to cache and read
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			Keylist = pf->pgetkeys(sect);
			if (Keylist) {
				Keyptr = Keylist->next;
				return true;
			}
		}
	}

	return false;
}


bool DCpfile::getnextkey(char **key, char **val)
{
	if (Keyptr) {
		*key = (char *) Keyptr->id;
		*val = (char *) Keyptr->val;
		Keyptr = Keyptr->next;
		return true;
	}

	return false;
}


void DCpfile::releasekeys(const char *sect, const char *fname)
{
	if (KeyFile
	 && KeySect
	 && !strcmp(fname, KeyFile)
	 && !strcmp(sect, KeySect)) {
		DeleteName(KeyFile);
		DeleteName(KeySect);
		KeyFile = KeySect + NULL;
		Keylist = Keyptr = NULL;
	}

	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL)
		// if in cache, read from cache
		pf->prelkeys(sect);

	// if not in cache, nothing to release
}


// set an item using the PPS protocol

bool DCpfile::putval(const char *sect, const char *key,	const char *val, const char *fname)
{
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// in cache, update in memory
		return pf->pwrite(sect, key, val);
	}
	else {
		// add to cache and update
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			return pf->pwrite(sect, key, val);
		}
		else
			delete pf;
	}
	
	return false;
	//return WritePrivateProfileString(sect, key, val, fname);
}


// move a section within a file

bool DCpfile::movsect(const char *sect, long opos, long npos, const char *fname)
{
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// in cache, update in memory
		return pf->mvsect(sect, opos, npos);
	}
	else {
		// add to cache and update
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			return pf->mvsect(sect, opos, npos);
		}
		else
			delete pf;
	}
	
	return false;
}


// move a setting within a section

bool DCpfile::movset(const char *sect, const char *set, long opos, long npos, const char *fname)
{
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// in cache, update in memory
		return pf->mvset(sect, set, opos, npos);
	}
	else {
		// add to cache and update
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			return pf->mvset(sect, set, opos, npos);
		}
		else
			delete pf;
	}
	
	return false;
}


// rename a section

bool DCpfile::rensect(const char *sect, const char *nsect, const char *fname)
{
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// in cache, update in memory
		return pf->rsect(sect, nsect);
	}
	else {
		// add to cache and update
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			return pf->rsect(sect, nsect);
		}
		else
			delete pf;
	}
	
	return false;
}


// restore a deleted section or setting

bool DCpfile::rest(const char *sect, const char *set, const char *fname)
{
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// in cache, update in memory
		return pf->restore(sect, set);
	}
	else {
		// add to cache and update
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			return pf->restore(sect, set);
		}
		else
			delete pf;
	}
	
	return false;
}


// change the content of a section (usually for a macro section)

bool DCpfile::chsect(const char *sect, const char *val, const char *fname)
{
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// in cache, update in memory
		return pf->csect(sect, val);
	}
	else {
		// add to cache and update
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			return pf->csect(sect, val);
		}
		else
			delete pf;
	}
	
	return false;
}


// rename a setting

bool DCpfile::renset(const char *sect, const char *set, const char *nset, const char *fname)
{
	DCpfile *pf = NULL;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		// in cache, update in memory
		return pf->rset(sect, set, nset);
	}
	else {
		// add to cache and update
		pf = new DCpfile(fname);
		if (pf->pset()) {
			ProfileFiles.addnew(fname, (long) pf);
			return pf->rset(sect, set, nset);
		}
		else
			delete pf;
	}
	
	return false;
}


// add file to cache; not needed for above operations

void DCpfile::psetup(const char *fname)
{
	DCpfile *pf = NULL;
	ending = false;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) == NULL) {
		// force cache for named file (such as primary .ref file)
		pf = new DCpfile(fname);
		if (pf->pset())
			ProfileFiles.addnew(fname, (long) pf);
		else
			delete pf;
	}
}


// write a file out from cache; not needed for above operations

void DCpfile::pupdate(const char *fname)
{
	DCpfile *pf = NULL;
	ending = false;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL)
		pf->pflush();
}


// close one file, write it out
DCpfile *DCpfile::MovErrFptr = NULL;
long DCpfile::MovErrVal = 0;


bool DCpfile::pclose(const char *fname)
{
	DCpfile *pf = NULL;
	ending = true;

	if ((pf = (DCpfile *) ProfileFiles.getfile(fname)) != NULL) {
		pflushx(fname, (long) pf);
		if (!MovErrFptr) {
			ProfileFiles.remove((long) pf);
			return true;
		}
	}
	return false;
}


// close the program, write everything out

void DCpfile::pflushall(void)
{
	// flush all the cached files to disk and close them
	ending = true;
	ProfileFiles.all(pflushx);
	ProfileFiles.empty();
}


// add a history item for the next operation

void DCpfile::addhist(const char *hist)
{
	if (hist)
		histitem = NewName(hist);
	else
		histitem = NULL;
}



// end of dcfilp.cpp

