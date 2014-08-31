// dclog.h is the header for dcl logging classes, JHG
// Copyright (c) 1993-2008 by Omni Systems, Inc.  All rights reserved.

#ifndef DCLOG_H
#define DCLOG_H

enum logtype {
	logerr = 1, logwarn, logquery, loginfo, logdebug,
	logmax, logcont = 0
};

class DClog : public DCwfile {
 protected:
	static char lType[logmax];
	static char *lStart;

	time_t tStamp;
	char *tStr;
	unc *lLevels;

	void logTime(bool nl = true);

 public:
	DClog(const char *lname, const char *mname,
   const char *ver, unc *llev, bool spc = false);
  virtual ~DClog();

	static bool ErrWarn;

	void logEvent(logtype lt, unc sev, const char *desc, const char *d2 = NULL,
   const char *d3 = NULL, const char *d4 = NULL, const char *d5 = NULL);

};

#endif



