// udxlist.cpp contains the list classes for udx.exe
// Copyright 2013 by Jeremy H. Griffith, under Apache license

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mxlparser.h"
#include "udx.h"



// UDlist functions

void UDlist::init(lpair *np)
{
	for ( ; np->id; np++)
		add(np->name, np->id);
}


void UDlist::add(const char *n, long i)
{
	if ((!n || !*n) && !i)
		return;

	UDlist *last = this;
	while (last->next)
		last = last->next;
	last->next = new UDlist(n, i);
}


void UDlist::replacename(const char *n, long i)
{
	if (!n || !*n)
		return;

  for (UDlist *that = this; that; that = that->next) {
		if (i == that->id) {
			that->name = (char *) n;
			break;
		}
		if (!that->next)
			that->next = new UDlist(n, i);
	}
}


void UDlist::replaceval(const char *n, long i)
{
	if (!n || !*n)
		return;

  for (UDlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && !_stricmp(that->name, n)) {
			that->id = id;
			break;
		}
		if (!that->next)
			that->next = new UDlist(n, i);
	}
}


char *UDlist::find(long i)
{
	if (!i)
		return NULL;

  for (UDlist *that = this; that; that = that->next) {
		if (i == that->id)
			return (char *) that->name;
	}

	return NULL;
}


long UDlist::getitem(const char *n)
{
	if (!n || !*n)
		return NULL;

  for (UDlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && !_stricmp(that->name, n))
			return that->id;
	}

	return NULL;
}


void UDlist::remove(const char *n)
{
	if (!n || !*n)
		return;

	UDlist *last = this;

  for (UDlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && !_stricmp(that->name, n)) {
			last->next = that->next;
			that->next = NULL;
			that->name = NULL;
			that->id = 0;
			delete that;
			break;
		}
		last = that;
	}
}


void UDlist::remove(long i)
{
	if (!i)
		return;

	UDlist *last = this;

  for (UDlist *that = this->next; that; that = that->next) {
		if (that->id == i) {
			last->next = that->next;
			that->next = NULL;
			that->name = NULL;
			that->id = 0;
			delete that;
			break;
		}
		last = that;
	}
}


void UDlist::empty(void)
{
	next = NULL;
	id = 0;
	name = NULL;
}



// UDvlist functions

void UDvlist::init(vpair *np)
{
	for ( ; np->id; np++)
		add(np->name, np->id);
}


void UDvlist::add(const char *n, void *i)
{
	if ((!n || !*n) && !i)
		return;

	UDvlist *last = this;
	while (last->next)
		last = last->next;
	last->next = new UDvlist(n, i);
}


void UDvlist::replacename(const char *n, void *i)
{
	if (!n || !*n)
		return;

  for (UDvlist *that = this; that; that = that->next) {
		if (i == that->id) {
			that->name = (char *) n;
			break;
		}
		if (!that->next)
			that->next = new UDvlist(n, i);
	}
}


void UDvlist::replaceval(const char *n, void *i)
{
	if (!n || !*n)
		return;

  for (UDvlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && !_stricmp(that->name, n)) {
			that->id = id;
			break;
		}
		if (!that->next)
			that->next = new UDvlist(n, i);
	}
}


char *UDvlist::find(void *i)
{
	if (!i)
		return NULL;

  for (UDvlist *that = this; that; that = that->next) {
		if (i == that->id)
			return (char *) that->name;
	}

	return NULL;
}


void *UDvlist::getitem(const char *n)
{
	if (!n || !*n)
		return NULL;

  for (UDvlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && !_stricmp(that->name, n))
			return that->id;
	}

	return NULL;
}


void UDvlist::remove(const char *n)
{
	if (!n || !*n)
		return;

	UDvlist *last = this;

  for (UDvlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && !_stricmp(that->name, n)) {
			last->next = that->next;
			that->next = NULL;
			that->name = NULL;
			that->id = NULL;
			delete that;
			break;
		}
		last = that;
	}
}


void UDvlist::remove(void *i)
{
	if (!i)
		return;

	UDvlist *last = this;

  for (UDvlist *that = this->next; that; that = that->next) {
		if (that->id == i) {
			last->next = that->next;
			that->next = NULL;
			that->name = NULL;
			that->id = NULL;
			delete that;
			break;
		}
		last = that;
	}
}


void UDvlist::empty(void)
{
	next = NULL;
	id = NULL;
	name = NULL;
}

// UDnlist functions

void UDnlist::init(npair *np)
{
	for ( ; np->id; np++)
		add(np->name, np->id);
}


void UDnlist::add(const char *n, char *i)
{
	if ((!n || !*n) && !i)
		return;

	UDnlist *last = this;
	while (last->next)
		last = last->next;
	last->next = new UDnlist(n, i);
}


void UDnlist::replacename(const char *n, char *i)
{
	if (!n || !*n)
		return;

  for (UDnlist *that = this; that; that = that->next) {
		if (i == that->id) {
			that->name = (char *) n;
			break;
		}
		if (!that->next)
			that->next = new UDnlist(n, i);
	}
}


void UDnlist::replaceval(const char *n, char *i)
{
	if (!n || !*n)
		return;

  for (UDnlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && !_stricmp(that->name, n)) {
			that->id = id;
			break;
		}
		if (!that->next)
			that->next = new UDnlist(n, i);
	}
}


char *UDnlist::find(char *i)
{
	if (!i)
		return NULL;

  for (UDnlist *that = this; that; that = that->next) {
		if (i == that->id)
			return (char *) that->name;
	}

	return NULL;
}


char *UDnlist::getitem(const char *n)
{
	if (!n || !*n)
		return NULL;

  for (UDnlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && !_stricmp(that->name, n))
			return that->id;
	}

	return NULL;
}


void UDnlist::remove(const char *n)
{
	if (!n || !*n)
		return;

	UDnlist *last = this;

  for (UDnlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && !_stricmp(that->name, n)) {
			last->next = that->next;
			that->next = NULL;
			that->name = NULL;
			that->id = NULL;
			delete that;
			break;
		}
		last = that;
	}
}


void UDnlist::remove(char *i)
{
	if (!i)
		return;

	UDnlist *last = this;

  for (UDnlist *that = this->next; that; that = that->next) {
		if ((that->id != NULL)
		 && !_stricmp(that->id, i)) {
			last->next = that->next;
			that->next = NULL;
			that->name = NULL;
			that->id = NULL;
			delete that;
			break;
		}
		last = that;
	}
}


void UDnlist::empty(void)
{
	next = NULL;
	id = NULL;
	name = NULL;
}


// lists of two longs

void UDilist::add(long n, long i)
{
	if (!i)
		return;

	UDilist *last = this;
	while (last->next)
		last = last->next;
	last->next = new UDilist(n, i);
}

long UDilist::find(long i)
{
	if (!i)
		return 0;

  for (UDilist *that = this; that; that = that->next) {
		if (i == that->id)
			return that->val;
	}

	return 0;
}

void UDilist::remove(long i)
{
	if (!i)
		return;

	UDilist *last = this;

  for (UDilist *that = this->next; that; that = that->next) {
		if (that->id == i) {
			last->next = that->next;
			that->next = NULL;
			delete that;
			break;
		}
		last = that;
	}
}



