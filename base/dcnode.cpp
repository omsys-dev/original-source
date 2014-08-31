
// dcnode.cc contains code for the dcl node classes, JHG, 9/16/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dcnode.h"

#pragma warning( disable : 4244 4514 )


DCnode *DCnode::TopNode = NULL;
char *DCnode::NodeFile = "stddef.dcl";

unc DCnode::Fields[DCnode::FieldMax];
unc DCnode::FNest = 0;
unc DCnode::CurrField = 1;

uns DCnode::Scopes[DCnode::ScopeMax];
unc DCnode::SNest = 0;
uns DCnode::CurrScope = 1;
uns DCnode::MajorScope = 1;
uns DCnode::MinorScope = 0;
uns DCnode::DataScope = 0;
uns DCnode::SizeScope = 0;
uns DCnode::DataAttr = 0;
uns DCnode::SizeAttr = 0;



// init from the lists in dcnodt.cc (made by tnode)
void
DCnode::ListInit(DCnode *Top)
{
  nstack *stack = new nstack[StackMax];
  short depth = 0;
  ndata *nd = &InitNodeList[1];
  DCnode *np = new DCnode;
  short n_id = 1;
  short saved_sib;

  Top->child = np;

  while (nd->c) {
    np->set(nd->v, strlen(nd->c) + 1, nd->c, NULL, NULL);
    if (nd->child_id) {   // do any child next
      if (nd->sib_id) {   // put any sib on stack
        if (depth < StackMax) {
          stack[depth].sibling = nd->sib_id;
          stack[depth].node = np;
        }
        else {
          fprintf(stderr, "Overflow in InitNodeList, item %hd.\n", n_id);
          exit(dc_err_node);
        }
        depth++;
      }
      np = np->setChild(new DCnode);
    }
    else {               // no child, sib is next
      if (!nd->sib_id) { // no sib, get from stack
        if (depth == 0)
          break;         // nothing there, finished
        depth--;
        if (depth < StackMax) {
          if (n_id != (saved_sib = stack[depth].sibling - 1)) {
            fprintf(stderr, "Error in InitNodeList, item %hd, got %hd.\n",
                                                         n_id, saved_sib);
            exit(dc_err_node);
          }
          np = stack[depth].node;
        }
        else {   
          fprintf(stderr, "Underflow in InitNodeList, item %hd.\n", n_id);
          exit(dc_err_node);
        }
      }
      np = np->setSib(new DCnode);
    }
    n_id++;
    nd++;
  }

  delete [] stack;
}



// called while reading data files for dynamic renaming
// ignores anything that isn't relevant to tree operations

DCctl&
DCnode::ProcNodeCtl(DCctl& c)
{
  if (c.maj() != 1)
    return c;

  switch (c.min()) {
    case 1:  // definition
      switch (CurrField) {
        case 1: // major
          SetMajor(c.dat(), (short) c.siz(), (char *) c.ex());
					MajorScope = c.dat();
          break;
        case 2: // minor
          SetMinor(c.dat(), (short) c.siz(), (char *) c.ex(),
                   MajorScope);
					MinorScope = c.dat();
          break;
        case 3: // data
          SetData(c.dat(), (short) c.siz(), (char *) c.ex(),
                  MinorScope, MajorScope);
					DataScope = c.dat();
          break;
        case 4: // size
          SetSize(c.dat(), (short) c.siz(), (char *) c.ex(),
                  DataScope, MinorScope, MajorScope);
					SizeScope = c.dat();
          break;
        default: // wrong
          break;
      }
      break;
    case 2:  // field spec
      if (c.dt() == group) {
        if (FNest < FieldMax)
          Fields[FNest] = CurrField;
        FNest++;
        CurrField = (unc) c.dat();
				if (CurrField < 4)
					DataAttr = 0;
      }
      else if (c.dt() == endgroup) {
        FNest--;
        if (FNest < FieldMax)
          CurrField = Fields[FNest];
      }
      else CurrField = (unc) c.dat();
      break;
    case 3:  // scope spec
      if (c.dt() == group) {
        if (SNest < ScopeMax)
          Scopes[SNest] = CurrScope;
        SNest++;
        CurrScope = c.dat();
      }
      else if (c.dt() == endgroup) {
        SNest--;
        if (SNest < ScopeMax)
          CurrScope = Scopes[SNest];
      }
      else CurrScope = c.dat();
      switch (CurrField) {
        case 1: // major
          MajorScope = CurrScope;
					MinorScope = DataScope = SizeScope = 0;
          break;
        case 2: // minor
          MinorScope = CurrScope;
					DataScope = SizeScope = 0;
          break;
        case 3: // data
          DataScope = CurrScope;
					SizeScope = 0;
          break;
        case 4: // size
          SizeScope = CurrScope;
          break;
        default: // wrong
          break;
      }
      break;
    case 4:  // attr spec
			switch (CurrField) {
				case 3:
					DataAttr = c.dat();
					SizeAttr = 0;
					break;
				case 4:
					SizeAttr = c.dat();
					break;
				default:
					DataAttr = SizeAttr = 0;
					break;
			}
      SetAttr(DataAttr, SizeAttr, (short) c.siz(), (char *) c.ex(),
              SizeScope, DataScope, MinorScope, MajorScope);
      break;
    default: // ignore
      break;
  }
  return c;
}


// walk the tree, calling user-provided function at each node
void
DCnode::Walk(NDFunc UFunc, DCnode *Top)
{
  short n_id = 0;
  ndata *ndlist = new ndata[DListMax];
  ndata *nd = ndlist;
  nstack *stack = new nstack[StackMax];
  short depth = 0;
  short ns = 0;
  DCnode *np;

  if (!UFunc)
    UFunc = &WalkFunc;
  if (!Top)
    Top = TopNode;

  // make preorder (depth-first) traversal
  stack[depth].sibling = 0;
  stack[depth++].node = Top;

  while (depth) {
    np = stack[--depth].node;
    assert(np);
    if ((ns = stack[depth].sibling) != 0)
      ndlist[ns].sib_id = n_id;
    nd = &ndlist[n_id];
    assert(nd);
    nd->v = np->value;
    nd->c = np->name;
    if (np->sib) {
      stack[depth].node = np->sib;
      stack[depth++].sibling = n_id;
      assert(depth < StackMax);
    }
    nd->sib_id = 0;
    if (np->child) {
      nd->child_id = n_id + 1;
      stack[depth].node = np->child;
      stack[depth++].sibling = 0;
      assert(depth < StackMax);
    }
    else nd->child_id = 0;
    n_id++;
    assert(n_id < DListMax);
  }

  // put out list
  for (nd = ndlist; n_id-- && ((*UFunc)(nd) == true); nd++);

  // clean up
  delete [] ndlist;
  delete [] stack;
}

// print out node data, if no user func provided; return false to quit
bool
DCnode::WalkFunc(ndata *nd)
{
  static short node_id = 0;

  if (!nd)
    return false;

  printf("Node %hd: \"%s\" is %lu, child %hd, sib %hd.\n",
    node_id++, nd->c, nd->v, nd->child_id, nd->sib_id);
  return true;
}



// node-adding functions


bool
DCnode::SetMajor(uns v, short len, char *c)
{
  DCnode *n = TopNode;
  return SetNodeName(v, len, c, n);
}

bool
DCnode::SetMinor(uns v, short len, char *c, uns mj)
{
  DCnode *n;

  if ((n = n->FindMajor(mj)) == NULL) {
    fprintf(stderr,
      "Define failed: minortype %hu to \"%*s\" of majortype %hu.\n",
                                  v, len, c, mj);
    return false;
  }
  return SetNodeName(v, len, c, n);
}

bool
DCnode::SetData(uns v, short len, char *c, uns mn, uns mv)
{
  DCnode *n;

  if ((n = n->FindMinor(mn, mv)) == NULL) {
    fprintf(stderr,
      "Define failed: datafield %hu to \"%*s\" of %hu:%hu.\n",
                                  v, len, c, mv, mn);
    return false;
  }
  return SetNodeName(v, len, c, n);
}

bool
DCnode::SetSize(uns v, short len, char *c, uns d, uns mn, uns mv)
{
  DCnode *n;

  if ((n = n->FindData(d, mn, mv)) == NULL) {
    fprintf(stderr,
      "Define failed: sizefield %hu to \"%*s\" of %hu:%hu:%hu.\n",
                                  v, len, c, mv, mn, d);
    return false;
  }
  return SetNodeName(v, len, c, n);
}

bool
DCnode::SetNodeName(uns v, short len, char *c, DCnode *n)
{
  if (!n->child)
    n->child = new DCnode(v, len, c, NULL, NULL);
  else {
    n = n->child;
    while (n->value != v) {
      if (!n->sib) {
        n->sib = new DCnode(v, len, c, NULL, NULL);
        return true;
      }
      n = n->sib;
    }
    n->renam(len, c);
  }
  return true;
}


// attribute handing

attrs **DCnode::AttrList = NULL;
uns DCnode::AttrCount = 0;

#pragma warning( disable : 4100 )

void
DCnode::SetAttr(uns da, uns sa, short len, char *c, uns s, uns d, uns mn, uns mj)
{
	attrs *att = new attrs;
	att->maj = mj;
	att->min = mn;
	att->dat = d;
	att->siz = s;
	att->dattr = da;
	att->sattr = sa;
	// use len and c later for making elongs into attrs

	if (!AttrList)
		AttrList = new attrs*[AListMax];
	else {
		attrs *last = AttrList[AttrCount - 1];
		if ((last->maj == att->maj)
		 && (last->min == att->min)
		 && (last->dat == att->dat)
		 && (last->siz == att->siz))
			AttrCount--;
	}

	AttrList[AttrCount++] = att;
}

#pragma warning( default : 4100 )


void 
DCnode::MakeAttrList(NAFunc UserAttrFunc)
{
	if (!UserAttrFunc)
		return;

	for (uns i = 0; i < AttrCount; i++)
		(*UserAttrFunc)(AttrList[i]);
}

attrs *
DCnode::GetAttrs(uns mj, uns mn, uns d, uns s)
{
	if (InitAttrList[0].maj == 0)
		return NULL;

	attrs *ap = NULL;
	attrs *def = NULL;

	for (uns pos = 0; InitAttrList[pos].maj != 0; pos++) {
		ap = &InitAttrList[pos];
		if ((ap->maj == mj)
		 && (ap->min == mn)) {      // matched major and minor
			if (ap->dat == 0)         // default applies to any dat unless better found
				def = ap;                 // siz is always 0 when dat is
			else if (ap->dat == d) {  // matched dat (always after any zero item)
				if (ap->siz == 0)         // default applies to any siz
					def = ap;
				else if (ap->siz == s) {  // matched siz, exact match
					def = ap;
					break;
				}
			}
		}
		else if ((ap->maj > mj)     // past any possible matches
		 || ((ap->maj == mj)
		  && (ap->min > mn)))
			break;
	}
	return def;
}


// node-finding functions


DCnode *
DCnode::FindMajor(uns v)
{
  DCnode *n = TopNode;

  n = FindNode(v, n);
  return n;
}

DCnode *
DCnode::FindMajor(char *c)
{
  DCnode *n = TopNode;

  n = FindNode(c, n);
  return n;
}


DCnode *
DCnode::FindMinor(uns v, uns mj)
{
  DCnode *n;

  if ((n = FindMajor(mj)) != NULL)
    n = FindNode(v, n);
  return n;
}

DCnode *
DCnode::FindMinor(char *c, uns mj)
{
  DCnode *n;

  if ((n = FindMajor(mj)) != NULL)
    n = FindNode(c, n);
  return n;
}


DCnode *
DCnode::FindData(uns v, uns mn, uns mj)
{
  DCnode *n;

  if ((n = FindMinor(mn, mj)) != NULL)
    n = FindNode(v, n);
  return n;
}

DCnode *
DCnode::FindData(char *c, uns mn, uns mj)
{
  DCnode *n;

  if ((n = FindMinor(mn, mj)) != NULL)
    n = FindNode(c, n);
  return n;
}


DCnode *
DCnode::FindSize(uns v, uns d, uns mn, uns mj)
{
  DCnode *n;

  if ((n = FindData(d, mn, mj)) != NULL)
    n = FindNode(v, n);
  return n;
}

DCnode *
DCnode::FindSize(char *c, uns d, uns mn, uns mj)
{
  DCnode *n;

  if ((n = FindData(d, mn, mj)) != NULL)
    n = FindNode(c, n);
  return n;
}


DCnode *
DCnode::FindNode(uns v, DCnode *n)
{
  if ((n != NULL)
	 && ((n = n->child) != NULL)) {
    while ((n->value != v)
     && ((n = n->sib) != NULL))
      ;
  }
  return n;
}

DCnode *
DCnode::FindNode(char *c, DCnode *n)
{
  if ((n != NULL)
	 && ((n = n->child) != NULL)) {
    while (strcmp(n->name, c)
     && ((n = n->sib) != NULL))
      ;
  }
  return n;
}


// end of dcnode.cc

