//dctxfx.h is the header for text effects (borders, shading), JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#ifndef DCTXFX_H
#define DCTXFX_H


// borders

enum brdrtyp {
	brdrnone = 0, brdrs, brdrdb, brdrdot, brdrdash,           // all
	brdrth, brdrsh, brdrhair,                                 // rtf
  brdrridge, brdroutset, brdrgroove, brdrinset, brdrhidden  // css
};


struct brdrinfo {
	brdrtyp type;
	long thick;
	long color;
	long space;
};

class DCborder {
 public:
	brdrtyp type;
	long thick;
	long color;
	long space;

	DCborder() : type(brdrnone) { }
	DCborder(brdrinfo *br) : type(br->type), thick(br->thick),
	 color(br->color), space(br->space) { }
	virtual ~DCborder() {}

	void setRule(DCctl *cp);
	void setColor(unl color);
};


// shading

enum shdtyp {
	shdnone = 0, shdhoriz, shdvert, shdfdiag, shdbdiag, shd2cross, shddcross,
	shddkhoriz, shddkvert, shddkfdiag, shddkbdiag, shddkcross, shddkdcross,
};

struct shdinfo {
	shdtyp type;
	long color;
	long bcolor;
	long tint;
};

class DCshade {
 public:
	shdtyp type;
	long color;
	long bcolor;
	long tint;

	DCshade() : type(shdnone) { }

	void setFill(DCctl *cp);
	void setColor(unl color);
};


// numbers

enum nlevtyp {
 ntNone = 0, ntHide, ntNum,
 ntLCRom, ntUCRom,
 ntLCAlpha, ntUCAlpha,
 ntSym
};

enum resettyp {
 rtNone = 0, rtTopic = 1, rtTable = 3
};

struct numstrinfo {
	char *name;        // stream name
	long streamid;     // internal ID number, index for lists
	char *seqid;       // ID to use for RTF SEQ fields
	long levels;       // number of counters
	bool rep;          // true to repeat sequence, false for last only
	long tcount;       // count of defined types
	char **syms;       // string array of symbols for type ntSym
	long scount;       // count of defined symbols
	resettyp reset;    // reset condition for stream
	DCilist* levtyps;  // curr level types, indexed by level
	DCilist* levseqs;  // curr level seq values, seq indexed by level
	DCilist* levstrts; // curr level start values, indexed by level
};

struct numforminfo {
	char *name;       // format name
	char *sname;      // stream name
	char *based;      // name of format based on
	char *lname;      // lang file name to use in format, if any
	long formid;      // internam ID number, index for lists
	long streamid;    // from [NumberProperties] StreamN
	long level;       // from [NumberProperties] LevelN
	long start;       // from [NumberProperties] StartN ( -1 for no reset)
	nlevtyp typ;      // from [NumberProperties] type name
	bool xrefpx;      // from [NumberProperties] XrefPrefix
	bool xreftab;     // from [NumberProperties] XrefTabs
	bool xrefsx;      // from [NumberProperties] XrefSuffix
	bool atend;       // from [NumberProperties] AtEnd
	DClist keep;      // from [NumberProperties] Keep
	char *form;       // from [NumberFormats], text with number tokens
};



// headers and footers

struct hfinfo {
	char *name;
	char *fmtname;
	char *cont;
};


#endif


