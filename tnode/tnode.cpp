
// tnode.cc generates the dcl mnemonic name tree table, dcnodt.cc, JHG
// Copyright (c) 2007 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"

bool NodeInit(ndata *);
void AttrInit(attrs *);

ndata InitNodeList[1] = {{ 0, NULL, 0, 0}};  // placeholders
attrs InitAttrList[1] = {{ 0, 0, 0, 0, 0, 0}};

// start of code

int
main(int argc, char *argv[])
{
  DCnode::InitNodes("stddef.dcl");

  printf("\n// dcnodt.cc is the initial node table for dcl translators\n");
  printf("// Copyright (c) 2007 by Omni Systems, Inc.  All rights reserved.\n");
  printf("\n#include \"dcglob.h\"\n");
  printf("#include \"dcbase.h\"\n");
  printf("#include \"dcnode.h\"\n");
  printf("\nndata InitNodeList[] = {\n");
  DCnode::Walk(&NodeInit);
  printf(" { 0, NULL, 0, 0}\n};\n");
  printf("\nattrs InitAttrList[] = {\n");
  DCnode::MakeAttrList(&AttrInit);
  printf(" { 0, 0, 0, 0, 0, 0}\n};\n");
  printf("\n// end of dcnodt.cc\n\n");
  return 0;
}

// user function for building dcnodt.cc table 

bool
NodeInit(ndata *nd)
{
  if (!nd)
    return false;
 
  printf(" { %hd, \"%s\", %hd, %hd},\n",
         nd->v, nd->c, nd->child_id, nd->sib_id);
  return true;
}
 

// the InitAttrList is used for the production of XDCL,
// to specify the attr names to be used for ctl values.

void
AttrInit(attrs *ap)
{
  if (!ap)
    return;
 
  printf(" { %hd, %hd, %hd, %hd, %hd, %hd},\n",
      ap->maj, ap->min, ap->dat, ap->siz, ap->dattr, ap->sattr);
}


// end of tnode.cc

