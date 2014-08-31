//dcmxl.h is the header for MicroXML and uDoc support, JHG
// Copyright (c) 2013 by Omni Systems, Inc.  All rights reserved.

#ifndef DCMXL_H
#define DCMXL_H



class MXLwr {  // for uDoc input
  friend class HTMwr;
  friend class RTFwr;
	friend class HUDwr;

	// in dcmxl.cpp
	static bool WriteKeydefLib;
	static bool UseKeydefAliases;
	static DCnlist OutputKeydefs;
	static long OutputKeydefCount;
	static DCnlist SourceKeydefs;
	static long SourceKeydefCount;
	static DCnlist KeydefAliases;
	static char *OutputKeydefLibPath;
	static char *OutputKeydefLibID;
	static char *OutputKeydefLibSrc;
	static char *KeylibOutput;
	static char *OutputKey;
	static char *OutputKeySrc;

	static void SetOutputKeydef(char *keys, char *src);
	static void SetOutputKeydefLibProps(char *libsrc);

 public:
	static void WriteOutputKeydefLib(void);

};


#endif
