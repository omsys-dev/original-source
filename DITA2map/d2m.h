// DITA2map.cpp derives ditamaps from dita topic files, JHG
// Copyright (c) 2009 by Jeremy H. Griffith.  All rights reserved.

#ifndef D2M_H
#define D2M_H



class D2M;
class XMLrfile;
class HTMwfile;


enum RxmlConstants {
  LevelMax = 16,
  LineMax = 4096, AnsiTabMax = 128,
  ChunkTokMax = 7, FormNameMax = 128,
  ElemPathLen = 1024, NumLenMax = 10,
  RefBufLen = 1024, MaxIniLen = 65500
};


struct topref {
	char *href;
	char *navtitle;
	char *shdesc;
	char *type;
	long depth;
};


struct attrp {
	const char *name;
	const char **var;
};


struct strln {
	char *str;
	long len;
};


class D2M {

	// in dtmapel.cpp

	static long TopicDepth;
	static char *TopicRootElem;
	static char *TopicTitleElem;
	static char *TopicShortdescElem;
	static DCnlist TopicRoots;
	static bool InTopicTitle;
	static bool InTopicShortdesc;

	static void StartElement(const char *name, const char **atts,
		long depth);
	static void ProcAttrs(const char **attp, attrp *alist);

	static void EndElement(const char *name, long depth);

	static void ProcPI(const char *target, const char *data);


	// in dtmaptx.cpp

	static char *PendingText;
	static long PendingLen;

	static void ProcChar(const char *str, int len, long depth);
	static strln *ProcStr(const char *str, int len, long depth);


	// in dtmapmp.cpp

	static DClist TopicRefList;
	static topref *CurrTopicRef;

	static long MapDepth;
	static char *MapRootElem;
	static char *MapTitleElem;
	static char *MapLanguage;
	static bool InTopicrefStart;
	static char *PublicID;
	static char *SystemID;

	static void WriteMapHeader(void);
	static void StartTopicref(const char *type, const char *id);
	static void EndTopicrefStart(bool block);
	static void EndTopicrefBlock(void);
	static void WriteShortdesc(void);
	static void WriteMapEnd(void);


	// in dtmapin.cpp

	static secinfo D2MIniSects[];

	static setinfo ISecOptions[];
	static void CaselessMatchSet(char *);
	static void SpacelessMatchSet(char *);
	static void WildcardMatchSet(char *);

	static setinfo ISecSetup[];
	static setinfo ILogOptions[];

	static setinfo ISecMapOptions[];


 public:

	// in DITA2map.cpp

	static char *D2MVer;
	static swlist SwitchList[];
  static short Verbose;
	static char *TopicFileName;
	static char *MapFileName;
	static char *IniFileName;
	static char *MapTitle;
	static char *DTDPath;
	static bool MapTitleSet;
	static bool DTDPathSet;
	static bool UseTopicShortdesc;

	static XMLrfile *TopicFile;
	static HTMwfile *MapFile;
	static DCirfile *IniFile;

  static bool FileExists(char *fname);
  static void SetMapTitle(char *name);
  static void SetDTDPath(char *name);
  static void SetIniName(char *name);


	static bool CaselessMatch;
	static bool SpacelessMatch;
	static bool WildcardMatch;
	static char *ConfigTemplate;

	static bool UseLog;
	static char *LogFileName;
	static DClog *Log;
	static unc LogLevels[logmax];

	static void InitCurrProps(void);
	static void LogEvent(logtype lt, unc sev, const char *desc,
	 const char *d2 = NULL, const char *d3 = NULL,
	 const char *d4 = NULL, const char *d5 = NULL);


	static void ProcessTopic(void);

};


#endif
