
// dwinffl.h is the header for inf file writing classes, JHG
// Copyright (c) 1998 by Omni Systems, Inc.  All rights reserved.

#ifndef DWINFFL_H
#define DWINFFL_H


class INFwfile : public DCwfile {

 public:
  INFwfile(char *fname);
  virtual void CloseFile(void);

  void putShort(short n);
  void putEnd(void)
    { putChar('\n'); }
  
	void putSection(char *str);
	void putComment(char *str);
	void putKey(char *str);

};

#endif

