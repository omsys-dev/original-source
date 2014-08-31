// drxmlrl.cpp is the code for dcl XML related links, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcelem.h"
#include "drxml.h"
#ifdef DITA
#include "expat.h"
#endif
#ifdef UDOC
#include "mxlparser.h"
#include "drxmlud.h"
#endif
#include "drxmlfl.h"


// related topics

// DITA-OT behaviors in XHTML:
// For reltable items, or related-links siblings, list at bottom of
//  topic with bold headings:  Related concepts:, Related tasks:,
//  Related reference:, Related topics:
// For related-links items, if role is not sibling, put link after:
//  Previous topic:, Next topic:  on same line
// When topics have parent/child relationship in map, put children
//  links indented at end with shortdescs, no heading.  Parent is
//  shown after Parent topic: heading, just link, no shortdesc.

bool XMLrd::InRelLinks = false;
bool XMLrd::InLinkPool = false;
bool XMLrd::InLinkList = false;

bool XMLrd::AppendLinksToTopics = true;
bool XMLrd::GenerateALinks = false;

bool XMLrd::AddContentsLink = false;
bool XMLrd::AddLOFLink = false;
bool XMLrd::AddLOTLink = false;
bool XMLrd::AddGlossaryLink = false;
bool XMLrd::AddIndexLink = false;

char *XMLrd::AddedLinksFormat = "RelatedHead";
char *XMLrd::AddedLinksSpacer = " <U+2022/> ";
char *XMLrd::AddedLinksStart = NULL;
char *XMLrd::AddedLinksEnd = NULL;

bool XMLrd::UseAddedDivider = false;
char *XMLrd::AddedDividerFormat = "RelatedDivider";
char *XMLrd::AddedDividerText = " ";

bool XMLrd::GeneratePrevNext = false;
bool XMLrd::GenerateParentChild = false;
bool XMLrd::GenerateSiblings = false;

bool XMLrd::UsePrevNext = true;
bool XMLrd::UseParent = true;
bool XMLrd::UseChildren = true;
bool XMLrd::UseSiblings = true;
bool XMLrd::UseAncestors = false;
bool XMLrd::UseDescendants = false;
bool XMLrd::UseCousins = false;
bool XMLrd::UseFriends = false;
bool XMLrd::UseTopicTypes = true;

bool XMLrd::UseRelDescription = false;
bool XMLrd::UseRelDescAsTitle = false;

char *XMLrd::PrevHead = "Previous Topic:";
char *XMLrd::NextHead = "Next Topic:";
char *XMLrd::ParentHead = "Parent Topic:";
char *XMLrd::ChildHead = "Child Topics:";
char *XMLrd::SiblingHead = "Sibling Topics:";
char *XMLrd::AncestorHead = "Ancestor Topics:";
char *XMLrd::DescendantHead = "Descendant Topics:";
char *XMLrd::CousinHead = "Cousin Topics:";
char *XMLrd::FriendHead = "Friend Topics:";

char *XMLrd::RelatedHead = "Related Topics:";

char *XMLrd::ConceptsHead = "Related Concepts:";
char *XMLrd::TasksHead = "Related Tasks:";
char *XMLrd::ReferencesHead = "Related References:";

char *XMLrd::HeadInlineFormat = "Strong";
char *XMLrd::HeadBlockFormat = "RelatedHead";
char *XMLrd::TopicTitleFormat = "RelatedTopic";
char *XMLrd::ShortdescFormat = "RelatedDesc";

bool XMLrd::UseRelatedDivider = true;
bool XMLrd::SearchRelatedLinks = false;
bool XMLrd::RelDividerSet = true;
char *XMLrd::RelatedDividerFormat = "RelatedDivider";
char *XMLrd::RelatedDividerText = " ";


// Process related topics after all other ParseXML tasks.
// Determine parents, next, prev, siblings, and children based on
// PageSequence and topic level.  Merge other related topics from
// related-links and reltable rows (and maybe siblings), and write.


DCvlist XMLrd::PrevLinks;
DCvlist XMLrd::NextLinks;
DCvlist XMLrd::ParentLinks;
DCvlist XMLrd::ChildLinks;
DCvlist XMLrd::SiblingLinks;
DCvlist XMLrd::AncestorLinks;
DCvlist XMLrd::DescendantLinks;
DCvlist XMLrd::CousinLinks;
DCvlist XMLrd::FriendLinks;
DCvlist XMLrd::ConceptLinks;
DCvlist XMLrd::TaskLinks;
DCvlist XMLrd::ReferenceLinks;
DCvlist XMLrd::RelatedLinks;

DClist *XMLrd::CurrChildLinks = NULL;
DClist *XMLrd::CurrSiblingLinks = NULL;
DClist *XMLrd::CurrAncestorLinks = NULL;
DClist *XMLrd::CurrDescendantLinks = NULL;
DClist *XMLrd::CurrCousinLinks = NULL;
DClist *XMLrd::CurrFriendLinks = NULL;
DClist *XMLrd::CurrConceptLinks = NULL;
DClist *XMLrd::CurrTaskLinks = NULL;
DClist *XMLrd::CurrReferenceLinks = NULL;
DClist *XMLrd::CurrRelatedLinks = NULL;


DCvlist XMLrd::TopicSequence;
DCnlist XMLrd::TopicSeqHrefs;  // seqtopics indexed by href of topic
long XMLrd::TSeqCount = 0;


void
XMLrd::ProcessRelatedTopics(void)
{
	long topic = 0;
	long lasttopic = 0;
	long tmptopic = 0;
	DCtopic *ctopic = NULL;
	DCtopic *otopic = NULL;
	DCtopic *ptopic = NULL;
	DCtopic *stopic = NULL;
	DCtopic *ttopic = NULL;
	long clevel = 0;
	long olevel = 0;
	DClist *nlist = NULL;
	DCvlist cparents;
	rellink *rl = NULL;
	seqtopic *cseq = NULL;
	seqtopic *oseq = NULL;
	seqtopic *pseq = NULL;
	seqtopic *sseq = NULL;
	seqtopic *tseq = NULL;
	seqtopic *tnxt = NULL;
	rellinkinfo *ri = NULL;

	// determine topic sequence
	lasttopic = GetTopicSequence();

	if (GeneratePrevNext
	 || GenerateParentChild
	 || GenerateSiblings) {
		// go through TopicSequence and identify relatives
		for (topic = 1; topic <= lasttopic; topic++) {
			if (ctopic)
				otopic = ctopic;
			oseq = cseq;
			cseq = (seqtopic *) TopicSequence.find(topic);
			if (!cseq)
				continue;
			ctopic = cseq->topic;  // NULL for topicheads
			if (ctopic
			 && !ctopic->mref->userel)
				continue;
			olevel = clevel;
			clevel = cseq->level;
			if (ctopic          // topicheads don't have rellinks
			 && !ctopic->rlinks)
				ctopic->rlinks = new rellinkinfo;
			if (GeneratePrevNext) {
				if (ctopic) {
					if (topic > 1) {
						tmptopic = topic;
						do {
							tseq = (seqtopic *) TopicSequence.find(--tmptopic);
						} while (tseq && !tseq->topic && (tmptopic > 0));
						if (tseq) {
							ctopic->rlinks->lprev = new rellink;
							ctopic->rlinks->lprev->seq = tseq;
							ctopic->rlinks->lprev->ltopic = tseq->topic;
						}
					}
					if (topic < lasttopic) {
						tmptopic = topic;
						do {
							tseq = (seqtopic *) TopicSequence.find(++tmptopic);
						} while (tseq && !tseq->topic && (tmptopic <= lasttopic));
						if (tseq) {
							ctopic->rlinks->lnext = new rellink;
							ctopic->rlinks->lnext->seq = tseq;
							ctopic->rlinks->lnext->ltopic = tseq->topic;
						}
					}
				}
			}
			if (GenerateParentChild
			 && (topic > 1)) {
				if (clevel > olevel)   // prev is parent
					cparents.replace(pseq = oseq, olevel);
				else if (clevel < olevel) { // moved up
					if (clevel > 1)  // get parent
						pseq = (seqtopic *) cparents.find(clevel - 1);
					else
						pseq = NULL;
				}
				if (pseq
				 && ctopic) { // pseq is our parent
					ptopic = pseq->topic;
					ctopic->rlinks->lparent = new rellink;
					ctopic->rlinks->lparent->seq = pseq;
					ctopic->rlinks->lparent->ltopic = ptopic;
					if (ptopic
					 && ptopic->rlinks) {  // topicheads don't have children
						if (!ptopic->rlinks->lchildren)
							ptopic->rlinks->lchildren = new DClist;
						rl = new rellink;
						rl->ltopic = ctopic;
						rl->seq = cseq;
						ptopic->rlinks->lchildren->add((long) rl);
					}
				}
			}
			if (GenerateSiblings
			 && (topic > 1)) {
				if (clevel < olevel)
					sseq = (seqtopic *) cparents.find(clevel);
				else {
					if (clevel == olevel)  // prev is sibling, get its siblings
						sseq = oseq;
					else
						sseq = NULL;
					cparents.replace(cseq, clevel);
				}
				if (sseq) {
					stopic = sseq->topic;
					if (stopic
					 && ctopic
					 && stopic->rlinks) {
						if (!stopic->rlinks->lsiblings)
							stopic->rlinks->lsiblings = new DClist();
						ctopic->rlinks->lsiblings = new DClist(stopic->rlinks->lsiblings);
						// add this topic to all previous siblings lists
						nlist = ctopic->rlinks->lsiblings;
						rl = new rellink;
						rl->ltopic = ctopic;
						rl->seq = cseq;
						while((nlist = nlist->next) != NULL) {
							ttopic = ((rellink *) nlist->id)->ltopic;
							ttopic->rlinks->lsiblings->add((long) rl);
						}
						stopic->rlinks->lsiblings->add((long) rl);
						rl = new rellink;
						rl->ltopic = stopic;
						rl->seq = sseq;
						ctopic->rlinks->lsiblings->add((long) rl);
					}
				}
			}
		}
	}

	// for each topic, combine related-links and reltable items
	for (topic = 1; topic <= lasttopic; topic++) {
		cseq = (seqtopic *) TopicSequence.find(topic);
		if (cseq)
			ctopic = cseq->topic;
		if (ctopic) {  // topics, not topicheads
			if (!ctopic->mref->userel)
				continue;
			if (GenerateALinks) {
				char *alid = ctopic->alinkid;
				if (alid) {
					long aln = RelAlinkNames(alid);
					DClist *al = (DClist *) RelAlinkRefs.find(aln);
					if (al && al->count())
						SetRelTopicAlinks(al, ctopic);
				}
			}

			ri = ctopic->rlinks;
			if (ri == NULL)
				ri = ctopic->rlinks = new rellinkinfo;

			// collect reltable and related-links items and add to
			// rellinks in topic, including the seqs for them
			// if we make a single rellink item for each topic, 
			// we can reuse it in references in other topics

			// if topic is first in its doc, it may be identified in the
			// reltable lists by hrefs both with and without topic id
			if (UsePrevNext) {
				SetSingleRelLink(&ri->lprev, (rellink *) PrevLinks.find((long) ctopic));
				SetSingleRelLink(&ri->lnext, (rellink *) NextLinks.find((long) ctopic));
			}
			if (UseParent)
				SetSingleRelLink(&ri->lparent, (rellink *) ParentLinks.find((long) ctopic));
			if (UseChildren
			 || (AddTopicHeadChildren
			  && ctopic->headchildren))
				SetRelLinkGroup(&ri->lchildren, (DClist *) ChildLinks.find((long) ctopic));
			if (UseSiblings)
				SetRelLinkGroup(&ri->lsiblings, (DClist *) SiblingLinks.find((long) ctopic));
			if (UseAncestors)
				SetRelLinkGroup(&ri->lancestors, (DClist *) AncestorLinks.find((long) ctopic));
			if (UseDescendants)
				SetRelLinkGroup(&ri->ldescendants, (DClist *) DescendantLinks.find((long) ctopic));
			if (UseCousins)
				SetRelLinkGroup(&ri->lcousins, (DClist *) CousinLinks.find((long) ctopic));
			if (UseFriends)
				SetRelLinkGroup(&ri->lfriends, (DClist *) FriendLinks.find((long) ctopic));
			if (UseTopicTypes) {
				SetRelLinkGroup(&ri->lconcepts, (DClist *) ConceptLinks.find((long) ctopic));
				SetRelLinkGroup(&ri->ltasks, (DClist *) TaskLinks.find((long) ctopic));
				SetRelLinkGroup(&ri->lreferences, (DClist *) ReferenceLinks.find((long) ctopic));
			}
			SetRelLinkGroup(&ri->lrelated, (DClist *) RelatedLinks.find((long) ctopic));

			// now get the reltable items
			if (ctopic->mref->href) {
				nlist = (DClist *) RelTableLinkList.getitem(ctopic->mref->href);
				if (nlist) {
					while ((nlist = nlist->next) != NULL) {
						rl = (rellink *) nlist->id;
						tseq = (seqtopic *) TopicSeqHrefs.getitem(rl->lhref);
						rl->seq = tseq;
						if (UseTopicTypes) {
							switch (rl->at->ltoptype) {
								case rlconcept:
									SetRelLinkGroupItem(&ri->lconcepts, rl);
									break;
								case rltask:
									SetRelLinkGroupItem(&ri->ltasks, rl);
									break;
								case rlreference:
									SetRelLinkGroupItem(&ri->lreferences, rl);
									break;
								default:
									SetRelLinkGroupItem(&ri->lrelated, rl);
									break;
							}
						}
						else
							SetRelLinkGroupItem(&ri->lrelated, rl);
					}
				}
			}
		}
	}
	if (GenerateALinks)
		SetReltableAlinks();

	// write desired links at end of topic
	if (AppendLinksToTopics
	 || GenerateALinks) {
		for (topic = 1; topic <= lasttopic; topic++) {
			tseq = (seqtopic *) TopicSequence.find(topic);
			tnxt = (seqtopic *) TopicSequence.find(topic + 1);
			if (tseq->topic
			 && !tseq->topic->mref->userel)
				continue;
			WriteRelatedLinks(tseq->topic, tnxt ? tnxt->topic : NULL); 
		}
	}
}


long
XMLrd::GetTopicSequence(void)
{
	// what if just nested topics with no map?  ****
	// need to generate our own TOC from the topics then...

	ProcMapPageGroup();

	DCpage *cpg = NULL;
	DCvlist *ts = &TopicSequence;
	seqtopic *seq = NULL;
	DCtopic *ctopic = NULL;

	if (!ts->next)
		return 0;

	for (PageSeqNum = UseContents ? 2 : 1;  // start after map page
		   PageSeqNum <= LastPageSeq;
		   PageSeqNum++) {
		cpg = (DCpage *) Pages.find(PageSequence.find(PageSeqNum));
		if (((ctopic = cpg->topic) == NULL)
		 || !ctopic->userel)
			continue;

		if (!ts)
			break;

		while ((ts = ts->next) != NULL) {
			seq = (seqtopic *) ts->vp;
			if (seq->type == sqtopic) {
				seq->topic = ctopic;
				if (ctopic->mref) {
					char *href = ctopic->mref->href;
					TopicSeqHrefs.add(href, (long) seq);
					if (ctopic->topicnum == 1) { // first in doc
						char *hash = strchr(href, '#');
						char *alt = NULL;
						if (hash)
							alt = NewName(href, hash - href);
						else if (ctopic->id) {
							alt = NewName(href, strlen(href) + strlen(ctopic->id) + 1);
							strcat(alt, "#");
							strcat(alt, ctopic->id);
						}
						if (alt)
							TopicSeqHrefs.add(alt, (long) seq);
					}	
				}
				break;
			}
		}
	}

	return TSeqCount;
}


void
XMLrd::ProcMapPageGroup(void)
{
	DCpage *cpg = NULL;  //(DCpage *) Pages.find(1);
	DCitem *item = NULL;  //cpg->pgroup->first;

	while ((cpg = (DCpage *) MapPages.dequeue()) != NULL) {
		item = cpg->pgroup->first;
		if (cpg->pgroup == TOCWrapper)
			continue;
		for (; item != NULL; item = item->next) {  // step through group
			if (item->ctl == cpg->pagetext) { // skip to start of text group
				ProcMapGroup(item->next, NULL);
				break;
			}
		}
	}

	if (TOCWrapper)
		ProcMapGroup(TOCWrapper->first, NULL);
}


DCgroup *
XMLrd::ProcMapGroup(DCitem *item, DCgroup *mgp)
{
	static seqtopic *seq = NULL;
	DCctl *cp = NULL;
	DCctl *start = NULL;

	if (!item)
		return NULL;

	for (; item != NULL; item = item->next) {  // step through group
		if (mgp) {
			if ((item->ctl->dt() == imnem)
			 && (item->ctl->maj() == 2)
			 && (item->ctl->min() == 122)
			 && (item->ctl->siz() == 0))   // map group ctl end
				mgp = NULL;
			else if ((item->ctl->dt() == etext)
			 && (item->ctl->maj() == 2)
			 && (item->ctl->min() == 112)
			 && (item->ctl->dat() == 2)) {
				if (seq && (seq->type == sqhead)) {
					cp = new DCctl(etext, 2, 112, 4);
					cp->ex(item->ctl->ex(), item->ctl->siz());
					mgp->add(*cp);
				}
				continue; // skip location ctls
			}
			else if ((item->ctl->dt() == ilong)
			 && (item->ctl->maj() == 2)
			 && (item->ctl->min() == 112)
			 && (item->ctl->dat() == 2))
				continue; // skip location ctls
			else if ((item->ctl->dt() == group)
			 && (item->ctl->maj() == 2)
			 && (item->ctl->min() == 110)
			 && (item->ctl->dat() == 1))
				continue; // skip index item groups
			else if ((item->ctl->dt() == etext)
			 && (item->ctl->maj() == 2)
			 && (item->ctl->min() == 121))
				return mgp; // skip attribute groups
#if 0
			else if ((item->ctl->dt() == group)
			 && (item->ctl->maj() == 1)
			 && (item->ctl->min() == 30)
			 && (item->ctl->dat() == 1))
				continue;  // skip dcl ignore groups
#endif
			else if ((item->ctl->dt() == group)
			 && (item->ctl->maj() == 2)
			 && (item->ctl->min() == 10)
			 && (item->ctl->dat() == 7)
			 && (((DCgroup *) item->ctl)->first != NULL)
			 && (((DCgroup *) item->ctl)->first != (DCitem *) 0xBAADF00D)) {
			  // proc text obj xref groups
				start = ((DCgroup *) item->ctl)->first->ctl;
				DCgroup *xgp = new DCgroup(DCctl(group, 2, 10, 7));
				mgp->add(*((DCctl*) xgp));
				ProcMapGroup(((DCgroup *) item->ctl)->first, xgp);
			}
			else if ((item->ctl->dt() == group)
			 && (item->ctl->maj() == 0)
			 && (item->ctl->min() == 0)
			 && (((DCgroup *) item->ctl)->first != NULL)
			 && (((DCgroup *) item->ctl)->first != (DCitem *) 0xBAADF00D)) {
				start = ((DCgroup *) item->ctl)->first->ctl;
				if ((start->dt() == etext)
				 && (start->maj() == 2)
				 && (start->min() == 121))
					continue; // skip attribute groups
				mgp = ProcMapGroup(((DCgroup *) item->ctl)->first, mgp);
			}
			else
				mgp->add(*(item->ctl));				
		}
		else if (item->ctl->dt() == group) {
			if ((item->ctl->maj() == 2)
			 && (item->ctl->min() == 110)
			 && (item->ctl->dat() == 1))
				continue;  // skip index item groups
			else if ((item->ctl->dt() == group)
			 && (item->ctl->maj() == 2)
			 && (item->ctl->min() == 10)
			 && (item->ctl->dat() == 7))
				continue; // skip xref groups
#if 0
			else if ((item->ctl->maj() == 1)
			 && (item->ctl->min() == 30)
			 && (item->ctl->dat() == 1))
				continue;  // skip dcl ignore groups
			else if ((item->ctl->maj() == 1)
			 && (item->ctl->min() == 30)
			 && (item->ctl->dat() == 2))
				continue;  // skip dcl ignore groups
#endif
#if 0
			else if ((item->ctl->maj() == 2)
			 && (item->ctl->min() == 75)
			 && (((DCgroup *) item->ctl)->first != NULL)
			 && (((DCgroup *) item->ctl)->first != (DCitem *) 0xBAADF00D)) {
				start = ((DCgroup *) item->ctl)->first->ctl;
				mgp = ProcMapGroup(((DCgroup *) item->ctl)->first, mgp);
			}
			else if ((item->ctl->maj() == 0)
			 && (item->ctl->min() == 0)
			 && (((DCgroup *) item->ctl)->first != NULL)
			 && (((DCgroup *) item->ctl)->first != (DCitem *) 0xBAADF00D)) {
				start = ((DCgroup *) item->ctl)->first->ctl;
				if ((start->dt() == etext)
				 && (start->maj() == 2)
				 && (start->min() == 121))
					continue; // skip attribute groups
				mgp = ProcMapGroup(((DCgroup *) item->ctl)->first, mgp);
			}
#endif
			else if ((((DCgroup *) item->ctl)->first != NULL)
			 && ( (unl) ((DCgroup *) item->ctl)->first > 0x1000)
			 && (((DCgroup *) item->ctl)->first != (DCitem *) 0xBAADF00D)) {
				start = ((DCgroup *) item->ctl)->first->ctl;
				if ((start->dt() == etext)
				 && (start->maj() == 2)
				 && (start->min() == 121))
					continue; // skip attribute groups
				mgp = ProcMapGroup(((DCgroup *) item->ctl)->first, mgp);
			}
		}
		else if ((item->ctl->dt() == imnem)
		 && (item->ctl->maj() == 2)
		 && (item->ctl->min() == 122)
		 && (item->ctl->dat() != 0)) {  // map group ctl start
			switch (item->ctl->dat()) {
				case 1:  // head
					mgp = new DCgroup();
					seq = new seqtopic;
					seq->type = sqhead;
					seq->level = item->ctl->siz();
					seq->title = mgp;
					TopicSequence.add(seq, ++TSeqCount);
					break;

				case 2:  // topic
					mgp = new DCgroup();
					seq = new seqtopic;
					seq->type = sqtopic;
					seq->level = item->ctl->siz();
					seq->title = mgp;
					TopicSequence.add(seq, ++TSeqCount);
					break;

				case 3:  // desc
					if (seq) {
						mgp = new DCgroup();
						seq->desc = mgp;
						seq = NULL;
					}
					break;

				default:
					break;
			}
		}
	}
	return mgp;
}


void 
XMLrd::SetSingleRelLink(rellink **prl, rellink *nl)
{
	if (!nl    // no explicit setting
	 || *prl)  // already set from sequence
		return;

	if (!nl->seq)  // always sclocal, look for it in TopicSequence
		nl->seq = FindSeqTopic(nl); 

	if (nl->seq)
		*prl = nl;
}


void 
XMLrd::SetRelLinkGroup(DClist **prlist, DClist *nlist)
{
	if (!nlist)      // no settings to add
		return;

	if (!*prlist)   // no settings made yet
		*prlist = new DClist;

	rellink *rl = NULL;
	while ((nlist = nlist->next) != NULL) {
		rl = (rellink *) nlist->id;
		if (!rl->seq && !rl->ltopic) {
			if (rl->at->lscope == scpeer) {  // change suffix for peer link
			}
			else if (rl->at->lscope == scexternal) {  // should be a URL
			}
			else {  // sclocal is default, identify correct seq
				if ((rl->seq = FindSeqTopic(rl)) == NULL)
					continue;
				if (SeqInList(rl->seq, *prlist))
					continue;
			}
		}
		(*prlist)->add((long) rl);
	}
}


void 
XMLrd::SetRelLinkGroupItem(DClist **prlist, rellink *rl)
{
	if (!rl)      // nothing to add
		return;

	if (!*prlist)   // no settings made yet
		*prlist = new DClist;

	if (!rl->seq && !rl->ltopic) {
		if (rl->at->lscope == scpeer)  // change suffix for peer link
			rl->lhref = SetPeerLink(rl->lhref);
		else if (rl->at->lscope == scexternal) {  // should be a URL
		}
		else {  // sclocal is default, identify correct seq
			if ((rl->seq = FindSeqTopic(rl)) == NULL)
				return;
			if (SeqInList(rl->seq, *prlist))
				return;
		}
	}
	(*prlist)->add((long) rl);
}


seqtopic *
XMLrd::FindSeqTopic(rellink *rl)
{
	DCvlist *ts = &TopicSequence;
	seqtopic *tseq = NULL;

	if (rl->lhref
	 && ((tseq = (seqtopic *) TopicSeqHrefs.getitem(rl->lhref)) != NULL))
		return tseq;

	while ((ts = ts->next) != NULL) {
		tseq = (seqtopic *) ts->vp;
		if (tseq->topic) {
			if (rl->ltopic == tseq->topic)
				return tseq;
			if (rl->lhref
			 && tseq->topic->mref
			 && tseq->topic->mref->href
			 && !strcmp(rl->lhref, tseq->topic->mref->href))
				return tseq;
		}
	}
	return NULL;  // not found
}


bool
XMLrd::SeqInList(seqtopic *seq, DClist *lst)
{
	rellink *rl = NULL;

	while ((lst = lst->next) != NULL) {
		rl = (rellink *) lst->id;
		if (rl->seq == seq)
			return true;
	}
	return false;
}


void
XMLrd::SetRelTopicAlinks(DClist *al, DCtopic *ctopic)
{
	// go through list of hrefs, find topics, and add alid to each
	char *alid = ctopic->alinkid;
	DCtopic *topic = NULL;

	while (al->next) {
		topic = FindSeqTopicHref((char *) al->id);
		SetOneTopicAlink(alid, topic);
		al = al->next;
	}
}


void
XMLrd::SetReltableAlinks(void)
{
	// go through ReltableAlinkRefs and for each row,
	// set ReltableAlinkNames in topic-alinks for each topicref
	char *alid = NULL;
	DCmapelem *mref = NULL;
	DCtopic *topic = NULL;

	for (long i = 1; i <= ReltableAlinkNameCount; i++) {
		alid = ReltableAlinkNames.find(i);
		RelRowAlinkRefs = (DClist *) ReltableAlinkRefs.find(i);
		while (RelRowAlinkRefs->next) {
			mref = (DCmapelem *) RelRowAlinkRefs->next->id;
			topic = FindSeqTopicHref(mref->href);
			SetOneTopicAlink(alid, topic);
		}
	}
}


void
XMLrd::SetOneTopicAlink(char *alid, DCtopic *topic)
{
	char *al = NULL;

	if (topic) {
		DClist *alids = topic->alinks;
		if (!alids) {
			alids = new DClist;
			topic->alinks = alids;
		}
		else {  // look for duplicate
			DClist *ap = alids;
			while ((ap = ap->next) != NULL) {
				al = (char *) ap->id;
				if ((al == alid)
				 || !stricmp(al, alid))
					return;  // already there
			}
		}
		alids->add((long) alid);
	}
}


DCtopic *
XMLrd::FindSeqTopicHref(char *href)
{
	DCvlist *ts = &TopicSequence;
	seqtopic *tseq = NULL;

	tseq = (seqtopic *) TopicSeqHrefs.getitem(href);
	if (tseq)
		return tseq->topic;

	while ((ts = ts->next) != NULL) {
		tseq = (seqtopic *) ts->vp;
		if (tseq->topic) {
			if (tseq->topic->mref
			 && tseq->topic->mref->href
			 && !strcmp(href, tseq->topic->mref->href))
				return tseq->topic;
		}
	}
	return NULL;  // not found
}



void
XMLrd::WriteRelatedLinks(DCtopic *topic, DCtopic *nexttopic)
{
	if (!topic)
		return;

	static bool merging = false;
	static bool merged = false;
	static rellinkinfo *rlinks = NULL;
	static DCgroup *headchildren = NULL;
	static DClist *topset = NULL;
	static rellink *rltop = NULL;

	TextGroup = topic->page->relend;
	topic->page->setgroup(&TextGroup);
	PageStarted = true;

	if (GenerateALinks) {
		DClist *al = topic->alinks;
		if (al) {
			while (al->next) {
				WriteAlink((char *) al->next->id);
				al = al->next;
			}
		}
	}

	if (AppendLinksToTopics) {
		if (nexttopic
		 && !nexttopic->chunked) {
			if (!merging)  {  // first one, set up statics
				merging = true;
				rlinks = new rellinkinfo;
				topset = new DClist;
				topset->add((long) topic);
				rltop = nexttopic->rlinks->lprev;

				// set prev and parent
				if (UsePrevNext)
					rlinks->lprev = topic->rlinks->lprev;
				if (UseParent)
					rlinks->lparent = topic->rlinks->lparent;

				// set up siblings, children, and rest to be processed
				if (AddTopicHeadChildren)
					headchildren = topic->headchildren;
				else if (UseChildren)
					rlinks->lchildren = topic->rlinks->lchildren;
				if (UseSiblings)
					rlinks->lsiblings = topic->rlinks->lsiblings;
				if (UseAncestors)
					rlinks->lancestors = topic->rlinks->lancestors;
				if (UseDescendants)
					rlinks->ldescendants = topic->rlinks->ldescendants;
				if (UseCousins)
					rlinks->lcousins = topic->rlinks->lcousins;
				if (UseFriends)
					rlinks->lfriends = topic->rlinks->lfriends;
				if (UseTopicTypes) {
					rlinks->lconcepts = topic->rlinks->lconcepts;
					rlinks->ltasks = topic->rlinks->ltasks;
					rlinks->lreferences = topic->rlinks->lreferences;
				}
				rlinks->lrelated = topic->rlinks->lrelated;
			}
			else   // middle topic, remove from siblings and children
				CombineRelLinks(rlinks, topic->rlinks, topset, topic);
		}
		else if (merging) {
			// end of merge, set next, remove from siblings and children
			if (UsePrevNext)
				rlinks->lnext = topic->rlinks->lnext;
			CombineRelLinks(rlinks, topic->rlinks, topset, topic);

			// put out results
			if (UseRelatedDivider
			  || !SearchRelatedLinks)
				RelDividerSet = false;
			if (UsePrevNext) {
				WriteSingleRelLink(PrevHead, rlinks->lprev);
				WriteSingleRelLink(NextHead, rlinks->lnext);
			}
			if (UseParent)
				WriteSingleRelLink(ParentHead, rlinks->lparent);
			if (AddTopicHeadChildren
			 && headchildren)
				WriteTopicHeadChildren(headchildren, rlinks->lchildren);
			else if (UseChildren)
				WriteRelLinkGroup(ChildHead, rlinks->lchildren);
			if (UseSiblings)
				WriteRelLinkGroup(SiblingHead, rlinks->lsiblings);
			if (UseAncestors)
				WriteRelLinkGroup(AncestorHead, rlinks->lancestors);
			if (UseDescendants)
				WriteRelLinkGroup(DescendantHead, rlinks->ldescendants);
			if (UseCousins)
				WriteRelLinkGroup(CousinHead, rlinks->lcousins);
			if (UseFriends)
				WriteRelLinkGroup(FriendHead, rlinks->lfriends);
			if (UseTopicTypes) {
				WriteRelLinkGroup(ConceptsHead, rlinks->lconcepts);
				WriteRelLinkGroup(TasksHead, rlinks->ltasks);
				WriteRelLinkGroup(ReferencesHead, rlinks->lreferences);
			}
			WriteRelLinkGroup(RelatedHead, rlinks->lrelated);
			WriteAddedLinks();

			merging = false;
			rlinks = NULL;
			headchildren = NULL;
			topset = NULL;
			merged = true;
		}
		else {
			if (merged) { // first after merged set
				topic->rlinks->lprev = rltop;
				rltop = NULL;
				merged = false;
			}
			if (UseRelatedDivider
			 || !SearchRelatedLinks)
				RelDividerSet = false;
			if (UsePrevNext) {
				WriteSingleRelLink(PrevHead, topic->rlinks->lprev);
				WriteSingleRelLink(NextHead, topic->rlinks->lnext);
			}
			if (UseParent)
				WriteSingleRelLink(ParentHead, topic->rlinks->lparent);
			if (AddTopicHeadChildren
			 && topic->headchildren)
				WriteTopicHeadChildren(topic->headchildren, topic->rlinks->lchildren);
			else if (UseChildren)
				WriteRelLinkGroup(ChildHead, topic->rlinks->lchildren);
			if (UseSiblings)
				WriteRelLinkGroup(SiblingHead, topic->rlinks->lsiblings);
			if (UseAncestors)
				WriteRelLinkGroup(AncestorHead, topic->rlinks->lancestors);
			if (UseDescendants)
				WriteRelLinkGroup(DescendantHead, topic->rlinks->ldescendants);
			if (UseCousins)
				WriteRelLinkGroup(CousinHead, topic->rlinks->lcousins);
			if (UseFriends)
				WriteRelLinkGroup(FriendHead, topic->rlinks->lfriends);
			if (UseTopicTypes) {
				WriteRelLinkGroup(ConceptsHead, topic->rlinks->lconcepts);
				WriteRelLinkGroup(TasksHead, topic->rlinks->ltasks);
				WriteRelLinkGroup(ReferencesHead, topic->rlinks->lreferences);
			}
			WriteRelLinkGroup(RelatedHead, topic->rlinks->lrelated);
			WriteAddedLinks();
		}
	}

	PageStarted = false;
	TextGroup = NULL;
}


void
XMLrd::CombineRelLinks(rellinkinfo *rlinks, rellinkinfo *tlinks,
	DClist *topset, DCtopic *topic)
{
	// add current topic to set
	topset->add((long) topic);

	// remove current topic from rlinks
	RemoveRelLink(rlinks->lchildren, topic);
	RemoveRelLink(rlinks->lsiblings, topic);

	// add current topic's refs that are not in topset to rlinks
	MergeRelLink(rlinks->lchildren, tlinks->lchildren, topset);
	MergeRelLink(rlinks->lsiblings, tlinks->lsiblings, topset);
}


void
XMLrd::RemoveRelLink(DClist *links, DCtopic *topic)
{
	if (!links || !topic)
		return;

	rellink *rl = NULL;

	while (links->next) {
		rl = (rellink *) links->next->id;
		if (rl->ltopic == topic) {
			links->next = links->next->next;
			break;
		}
		links = links->next;
	}		
}


void
XMLrd::MergeRelLink(DClist *links, DClist *merge, DClist *topset)
{
	if (!links || !merge || !topset)
		return;

	rellink *rl = NULL;

	while (merge->next) {
		rl = (rellink *) merge->next->id;
		if (topset->find((long) (rl->ltopic)) == NULL)
			links->add((long) rl);
		merge = merge->next;
	}
}


void
XMLrd::WriteAlink(char *alname)
{
	DCctl *cp = new DCctl(etext, 2, 110, 3);
	char *txt = NewName("alink:", strlen(alname) + 7);
	strcat(txt, alname);
	cp->ex(txt, strlen(txt));
	TextGroup->add(*cp);
}



bool XMLrd::AddedLinksSet = false;
bool XMLrd::AddedLinksUsed = false;
DCgroup *XMLrd::AddedLinksGroup = NULL;
DCgroup *XMLrd::ContentsLinkGroup = NULL;
DCgroup *XMLrd::LOFLinkGroup = NULL;
DCgroup *XMLrd::LOTLinkGroup = NULL;
DCgroup *XMLrd::GlossaryLinkGroup = NULL;
DCgroup *XMLrd::IndexLinkGroup = NULL;
DCgroup *XMLrd::AddSpacerGroup = NULL;
DCgroup *XMLrd::AddStartGroup = NULL;
DCgroup *XMLrd::AddEndGroup = NULL;


void
XMLrd::WriteAddedLinks(void)
{
	if (!AddedLinksSet)
		SetUpAddedLinks();

	if (!AddedLinksUsed)
		return;

	TextGroup->add(*AddedLinksGroup);
}


void
XMLrd::SetUpAddedLinks(void)
{
	AddedLinksSet = true;

	// make components

	if (AddContentsLink) {
		ContentsLinkGroup = new DCgroup;
		SetOneAddedLink(ContentsLinkGroup, "Rtoc", TOCTitle);
		AddedLinksUsed = true;
	}

	if (AddLOFLink) {
		LOFLinkGroup = new DCgroup;
		SetOneAddedLink(LOFLinkGroup, "Rlof", LOFTitle);
		AddedLinksUsed = true;
	}

	if (AddLOTLink) {
		LOTLinkGroup = new DCgroup;
		SetOneAddedLink(LOTLinkGroup, "Rlot", LOTTitle);
		AddedLinksUsed = true;
	}

	if (AddGlossaryLink) {
		GlossaryLinkGroup = new DCgroup;
		SetOneAddedLink(GlossaryLinkGroup, "Rgls", GLSTitle);
		AddedLinksUsed = true;
	}

	if (AddIndexLink) {
		IndexLinkGroup = new DCgroup;
		SetOneAddedLink(IndexLinkGroup, "Ridx", IDXTitle);
		AddedLinksUsed = true;
	}

	if (!AddedLinksUsed)
		return;

	if (AddedLinksSpacer) {
		AddSpacerGroup = new DCgroup;
		WriteSubformatHead(AddSpacerGroup, NULL,
			 AddedLinksSpacer, NULL, "AddedLinksSpacer");
	}

	if (AddedLinksStart) {
		AddStartGroup = new DCgroup;
		WriteSubformatHead(AddStartGroup, NULL,
			 AddedLinksStart, NULL, "AddedLinksStart");
	}

	if (AddedLinksEnd) {
		AddEndGroup = new DCgroup;
		WriteSubformatHead(AddEndGroup, NULL,
			 AddedLinksEnd, NULL, "AddedLinksEnd");
	}


	// assemble the line

	bool start = true;
	DCgroup *tg = TextGroup;
	AddedLinksGroup = new DCgroup;

	TextGroup = AddedLinksGroup;

	if (UseAddedDivider) {
		StartPara();
		SetFormat(AddedDividerFormat);
		if (AddedDividerText)
			PutTextStrCtl(AddedDividerText);
		EndPara();
	}

	StartPara();
	SetFormat(AddedLinksFormat);

	if (AddStartGroup)
		TextGroup->add(*AddStartGroup);

	if (ContentsLinkGroup) {
		TextGroup->add(*ContentsLinkGroup);
		start = true;
	}

	if (LOFLinkGroup) {
		if (start)
			TextGroup->add(*AddSpacerGroup);
		TextGroup->add(*LOFLinkGroup);
		start = true;
	}

	if (LOTLinkGroup) {
		if (start)
			TextGroup->add(*AddSpacerGroup);
		TextGroup->add(*LOTLinkGroup);
		start = true;
	}

	if (GlossaryLinkGroup) {
		if (start)
			TextGroup->add(*AddSpacerGroup);
		TextGroup->add(*GlossaryLinkGroup);
		start = true;
	}

	if (IndexLinkGroup) {
		if (start)
			TextGroup->add(*AddSpacerGroup);
		TextGroup->add(*IndexLinkGroup);
	}

	if (AddEndGroup)
		TextGroup->add(*AddEndGroup);

	EndPara();
	TextGroup = tg;
}


void
XMLrd::SetOneAddedLink(DCgroup *gp, char *fanchor, char *txt)
{
	DCgroup *tg = TextGroup;
	TextGroup = gp;

	SetFormat(AddedLinksFormat, true);
	DCctl *cp = new DCctl(etext, 2, 112, 3);
	cp->ex(MainDocName, strlen(MainDocName));
	if (IsRtf)
		cp->maj((uns) 0);  // remove it
	TextGroup->add(*cp);
	cp = new DCctl(etext, 2, 112, 4);
	cp->ex(fanchor, strlen(fanchor));
	TextGroup->add(*cp);
	PutTextStrCtl(txt);
	SetFormat(NULL, true);

	TextGroup = tg;
}


void
XMLrd::WriteRelLinkGroup(char *title, DClist *linklist)
{
	if (!linklist
	 || !linklist->next)
		return;

	if (!RelDividerSet)
		WriteRelDivider();

	// write heading line
	if (title
	 && *title) {
		StartPara();
		SetFormat(HeadBlockFormat);
		PutTextStrCtl(title);
		EndPara();
	}
	WriteRelLinkGroupBody(linklist);
}


void 
XMLrd::WriteTopicHeadChildren(DCgroup *txtgrp, DClist *linklist)
{
	DCgroup *lasttext = TextGroup;
	TextGroup = txtgrp;

	if (TopicHeadChildHead
	 && *TopicHeadChildHead) {
		StartPara();
		SetFormat(TopicHeadChildHeadFormat);
		PutTextStrCtl(TopicHeadChildHead);
		EndPara();
	}

	WriteRelLinkGroupBody(linklist);
	TextGroup = lasttext;
}


void
XMLrd::WriteRelLinkGroupBody(DClist *linklist)
{
	if (!linklist)
		return;

	long lnum = 0;
	rellink *link = NULL;

	// write a line for each link
	while (linklist->next) {
		linklist = linklist->next;
		link = (rellink *) linklist->id;
		StartPara();
		SetFormat(TopicTitleFormat);
		WriteRelLinkTitle(link);
		EndPara();

		// write desc, if any, on line following each link
		if (UseRelDescription)
			WriteRelLinkDesc(link);
	}
}


void
XMLrd::WriteSingleRelLink(char *title, rellink *link)
{
	if (!link)
		return;

	if (!RelDividerSet)
		WriteRelDivider();

	// write heading then write link on same line
	StartPara();
	SetFormat(TopicTitleFormat);
	if (title
	 && *title) {
		SetFormat(HeadInlineFormat, true);
		PutTextStrCtl(title);
		PutTextCharCtl(0x2003);
		SetFormat(NULL, true);
	}
	WriteRelLinkTitle(link);
	EndPara();

	// write desc, if any, on following line
	if (UseRelDescription)
		WriteRelLinkDesc(link);
}


void
XMLrd::WriteRelLinkTitle(rellink *link)
{
	DCctl *cp = NULL;
	if (UseRelDescAsTitle) {
		cp = new DCctl(imnem, 2, 122, 4, 1);
		TextGroup->add(*cp);
	}

	if (link->seq
	 && link->seq->title)
		TextGroup->add(*(link->seq->title));
	else if (link->ltopic
	 && link->at) {		
		// write link and title text, format already set
		CurrLinkInfo = new linkinfo;
		CurrLinkInfo->href = link->lhref;
		SetHyperlink(CurrLinkInfo, link->at->lscope, link->ltopic);
		if (link->ltopic->titlechanged)  // title ctl is invalid
			UpdateTopicTitle(link->ltopic);  // may be missing number
		PutTextCtrls(link->ltopic->title);
		delete CurrLinkInfo;
		CurrLinkInfo = NULL;
	}
	else if (link->at) {  // url or peer reference
		CurrLinkInfo = new linkinfo;
		CurrLinkInfo->href = link->lhref;
		SetHyperlink(CurrLinkInfo, link->at->lscope, NULL);
		if (link->ltext
		 && (link->ltext->first))
			PutTextStrCtl((link->ltext)->getText());
			//TextGroup->add(*(link->ltext));
		else
			PutTextStrCtl(link->lhref);
		delete CurrLinkInfo;
		CurrLinkInfo = NULL;
	}
	else if (link->ltext
	 && (link->ltext->first))
		TextGroup->add(*(link->ltext));

	if (UseRelDescAsTitle) {
		cp = new DCctl(imnem, 2, 122, 4, 0);
		TextGroup->add(*cp);
	}
}


void
XMLrd::WriteRelLinkDesc(rellink *link)
{
	DCctl *cp = NULL;
	DCctl *ecp = NULL;
	char *txt = NULL;

	if (UseRelDescAsTitle) {
		cp = new DCctl(imnem, 2, 122, 5, 1);
		ecp = new DCctl(imnem, 2, 122, 5, 0);
	}
	if (link->seq
	 && link->seq->desc
	 && link->seq->desc->first
	 && ((txt = (link->seq->desc)->getText()) != NULL)
	 && (*txt != 0)) {
		StartPara();
		SetFormat(ShortdescFormat);
		if (UseRelDescAsTitle)
			TextGroup->add(*cp);
		TextGroup->add(*(link->seq->desc));
		if (UseRelDescAsTitle)
			TextGroup->add(*ecp);
		EndPara();
	}
	else {		
		// write best desc available in link ****
		DCgroup *desc = NULL;
		if (link->lshort)
			desc = link->lshort;
		//else if (link->ltext)
		//	desc = link->ltext;
		else if (link->ldesc)
			desc = link->ldesc;
		else if (link->ltopic
		 && link->ltopic->desc)
			desc = link->ltopic->desc;
		else if (link->seq
		 && link->seq->topic
		 && link->seq->topic->desc)
			desc = link->seq->topic->desc;

		if (desc) {
			StartPara();
			SetFormat(ShortdescFormat);
			if (UseRelDescAsTitle)
				TextGroup->add(*cp);
			TextGroup->add(*desc);
			if (UseRelDescAsTitle)
				TextGroup->add(*ecp);
			EndPara();
		}
	}
}


void
XMLrd::WriteRelDivider(void)
{
	StartPara();
	SetFormat(RelatedDividerFormat);

	if (!SearchRelatedLinks) {
		DCctl *cp = new DCctl(etext, 2, 110, 3);
		cp->ex("search:no");
		TextGroup->add(*cp);
	}

	if (UseRelatedDivider
	 && RelatedDividerText)
		PutTextStrCtl(RelatedDividerText);
	EndPara();
	RelDividerSet = true;
}


// Process reltables as each map is read.  Referenced topics may
// not be included yet; they may need to be added to DocsToGo,
// if they are not added by another map during MapsToGo processing
// Package any shortdesc in the reltable topicref with the link.

DCgroup *XMLrd::PreRelTableTextGroup = NULL;

DCvlist XMLrd::RelColAttrs;
DCvlist XMLrd::RelCellAttrs;
DCvlist XMLrd::RelRowTopicrefs;
long XMLrd::RelColCount = 0;

DCnlist XMLrd::ReltableAlinkNames;
DCvlist XMLrd::ReltableAlinkRefs;
DClist *XMLrd::RelRowAlinkRefs = NULL;
long XMLrd::ReltableAlinkNameCount = 0;


void
XMLrd::ProcReltable(DCmapelem *elem, const char **atts, int depth)
{
	static long relcol = 0;
	static relattr *rareltable = NULL;
	static relattr *rarelrow = NULL;
	static relattr *rarelcol = NULL;
	static relattr *rarelcell = NULL;
	static DClist *cellrefs = NULL;

	bool mapref = false;
	bool locktitle = false;

	const char *idattr = NULL;
	const char *navattr = NULL;
	const char *typeattr = NULL;
	const char *formattr = NULL;
	const char *lockattr = NULL;
	const char *lockmetattr = NULL;
	const char *chunk = NULL;
	const char *copyto = NULL;
	const char *toc = NULL;
	const char *conrefatt = NULL;
	const char *conkeyrefatt = NULL;
	const char *collection = NULL;
	const char *scope = NULL;
	const char *linking = NULL;
	const char *print = NULL;
	const char *search = NULL;
	attrp alist[] = {
	 {"id", &idattr },
	 {"navtitle", &navattr },
	 {"type", &typeattr },
	 {"format", &formattr },
	 {"locktitle", &lockattr },
	 {"lockmeta", &lockmetattr },
	 {"chunk", &chunk },
	 {"copy-to", &copyto },
	 {"toc", &toc },
	 {"conref", &conrefatt },
	 {"conkeyref", &conkeyrefatt },
	 {"collection-type", &collection },
	 {"scope", &scope },
	 {"linking", &linking },
	 {"print", &print },
	 {"search", &search },
	 {NULL, NULL }
	};

	if (atts && *atts)
		ProcAttrs(atts, alist);

	if (lockattr
	 && !stricmp(lockattr, "yes"))
		locktitle = true;

	if (locktitle)
		elem->usenav = true;

	if (navattr)
		elem->navtitle = NewName(navattr);

	if (toc)
		elem->toc = (!stricmp(toc, "no")) ? false : true;

#if 0
	if (conkeyrefatt)
		SetMapConrefInfo(conkeyrefatt, true);
	else if (conrefatt)
		SetMapConrefInfo(conrefatt, false);
#endif

	if (conrefatt || conkeyrefatt)
		CheckMapConref(elem);

	if (chunk) {
		SetChunkVal(chunk);
		elem->copyto = NewName(copyto);
	}

	if (formattr) {
		elem->format = NewName(formattr);
		if (!strcmp(formattr, "ditamap"))
			mapref = true;
	}

	relattr *ra = new relattr;
	elem->ar = ra;

	if (scope)
		ra->lscope = (scopetyp) LinkScopes.getitem(scope);
	if (typeattr) {
		ra->ltype = NewName(typeattr);
		ra->ltoptype = (roletyp) LinkRoles.getitem(typeattr);
	}
	if (formattr) {
		ra->lform = NewName(formattr);
		ra->lftype = (formtyp) LinkForms.getitem(formattr);
	}
	if (collection) {
		ra->lcoll = NewName(collection);
		ra->lcollect = (colltyp) LinkCollTypes.getitem(collection);
	}
	if (linking) {
		ra->linking = NewName(linking);
		ra->links = (linktyp) LinkingTypes.getitem(linking);
	}


	// attribute inheritance, from least to most specific:
	// reltable, relcolspec, relrow, relcell, topicgroup/head, topicref

	if (elem->checkType((epRel << 8) | epMap)) { // reltable
		relcol = 0;
		RelColAttrs.empty();
		rareltable = ra;
		RelColCount = 0;
		PreRelTableTextGroup = TextGroup;
	}
	else if (elem->checkType((epTabRow << 8) | epTabHead)) { // relheader
		// does not have attributes above, just a container for relcolspecs
	}
	else if (elem->checkType((epTabCol << 8) | epTable)) { // relcolspec
		relcol++;
		AdjustRelAttrs(ra, rareltable);
		RelColAttrs.add(ra, relcol);
		RelColCount = relcol;
	}
	else if (elem->checkType((epTabRow << 8) | epTabBody)) { // relrow
		relcol = 0;
		AdjustRelAttrs(ra, rareltable);
		rarelrow = ra;
		RelRowTopicrefs.empty();
		if (GenerateALinks
		 && idattr
		 && *idattr) {
			long al = 0;
			if ((al = ReltableAlinkNames(idattr)) != 0) {
				RelRowAlinkRefs = (DClist *) ReltableAlinkRefs.find(al);
			}
			else {
				ReltableAlinkNames.addnew(idattr, ++ReltableAlinkNameCount);
				RelRowAlinkRefs = new DClist;
				ReltableAlinkRefs.add(RelRowAlinkRefs, ReltableAlinkNameCount);
			}
		}
	}
	else if (elem->checkType(epTabCell)) { // relcell
		relcol++;
		rarelcol = (relattr *) RelColAttrs.find(relcol);
		AdjustRelAttrs(ra, rarelcol);
		AdjustRelAttrs(ra, rarelrow);
		rarelcell = ra;
		RelCellAttrs.empty();
		cellrefs = new DClist();
		RelRowTopicrefs.add(cellrefs, relcol);
	}
	else if (elem->checkType((epTopic << 8) | epGroup)) { // topicgroup
		AdjustCellAttrs(ra, rarelcell, depth);
	}
	else if (elem->checkType(epTopic, (epMeta << 8) | epRef)) { // topichead
		AdjustCellAttrs(ra, rarelcell, depth);
		CurrRelLinkTopicref = elem;
	}
	else if (elem->checkType((epTopic << 8) | epRef)) { // topicref
		AdjustCellAttrs(ra, rarelcell, depth);
		if (elem->href) {
			cellrefs->add((long) elem);
			if (RelRowAlinkRefs)
				RelRowAlinkRefs->add((long) elem);
		}
		CurrRelLinkTopicref = elem;
	}
	else if (elem->checkType((epTopic << 8) | epMeta)) { // topicmeta
		if (CurrRelLink
		 && lockmetattr
		 && !stricmp(lockmetattr, "yes"))
			CurrRelLinkTopicref->usenav = true;
	}
	else if (elem->checkType((epLink << 8) | epText), epSeq) {   // linktext
		if (CurrRelLinkTopicref
		 && !CurrRelLinkTopicref->ltext)
			CurrRelLinkTopicref->ltext = TextGroup = new DCgroup();
	}
	else if (elem->checkType(epText)) {   // mainly shortdesc
		if (CurrRelLinkTopicref
		 && !CurrRelLinkTopicref->ldesc)
			CurrRelLinkTopicref->ldesc = TextGroup = new DCgroup();
	}

}


void XMLrd::ProcRelRowEnd(void)
{
	RelRowAlinkRefs = NULL;

	// topicrefs that are not target-only link to all in the same row,
	// *except* those in the same cell and those that are source-only.
	// since topics not yet processed, use href as topic identifier.
	// each source topic needs its own DClist of topics it links to.
	// use DCmapelem* as key to list of hrefs and list of linklists.

	DClist *trsrc = NULL;
	DCmapelem *tsrc = NULL;
	DClist *trtarg = NULL;
	DCmapelem *targ = NULL;

	for (long col = 1; col <= RelColCount; col++) {
		trsrc = (DClist *) RelRowTopicrefs.find(col);
		while ((trsrc = trsrc->next) != NULL) {
			tsrc = (DCmapelem *) trsrc->id;
			if (CheckRelLinking(tsrc, true)) {
				for (long tcol = 1; tcol <=RelColCount; tcol++) {
					if (tcol == col) { // same cell, use if collection attr
						if (tsrc->ar->lcollect != colfamily)
							continue;
						trtarg = (DClist *) RelRowTopicrefs.find(tcol);
						while ((trtarg = trtarg->next) != NULL) {
							targ = (DCmapelem *) trtarg->id;
							if ((targ != tsrc)
							 && CheckRelLinking(targ, false))
								MakeRelLink(tsrc, targ);
						}
					}
					else {
						trtarg = (DClist *) RelRowTopicrefs.find(tcol);
						while ((trtarg = trtarg->next) != NULL) {
							targ = (DCmapelem *) trtarg->id;
							if (CheckRelLinking(targ, false))
								MakeRelLink(tsrc, targ);
						}
					}
				}
			}	
		}
	}
}


bool 
XMLrd::CheckRelLinking(DCmapelem *tref, bool source)
{
	switch (tref->ar->links) {
		case linknone:
			return false;
		case linksource:
			return source;
		case linktarget:
			return !source;
		case linknormal:
		default:
			return true;
	}
}


DCnlist XMLrd::RelTableLinkList;  // list of DClists for hrefs

void XMLrd::MakeRelLink(DCmapelem *tsrc, DCmapelem *targ)
{
	rellink *rl = new rellink;
	DClist *trlist = (DClist *) RelTableLinkList.getitem(tsrc->href);
	if (!trlist)
		RelTableLinkList.add(tsrc->href, (long) (trlist = new DClist));
	trlist->add((long) rl);
	rl->lsrc = targ;
	rl->at = targ->ar;
	rl->lhref = targ->href;
	rl->loclass = targ->outclass;
	rl->lrtype = targ->ar->ltoptype;
	rl->ltext = targ->ltext;
	rl->ldesc = targ->ldesc;
	if (!rl->ltext
	 && targ->usenav
	 && targ->navtitle) {
		rl->ltext = new DCgroup();
		DCctl *cp = new DCctl(etext, 2, 10, 1);
		cp->ex(targ->navtitle);
		rl->ltext->add(*cp);
	}
}



// Process related-links as each topic is read.  Package linktext
// and desc with their link.

DCgroup *XMLrd::PreLinkTextGroup = NULL;
rellink *XMLrd::CurrRelLink = NULL;
DCmapelem *XMLrd::CurrRelLinkTopicref = NULL;
DCnlist XMLrd::LinkRoles;
npair XMLrd::LinkRolesInit[] = {
 { rlprev, "previous" },
 { rlnext, "next" },
 { rlparent, "parent" },
 { rlchild, "child" },
 { rlsibling, "sibling" },
 { rlancestor, "ancestor" },
 { rldescendant, "descendant" },
 { rlcousin, "cousin" },
 { rlfriend, "friend" },
 { rlother, "other" },
 { rltopic, "topic" },
 { rlconcept, "concept" },
 { rltask, "task" },
 { rlreference, "reference" },
 NULL
 // sample and external are deprecated
};

DCnlist XMLrd::LinkScopes;
npair XMLrd::LinkScopesInit[] = {
 { sclocal, "local" },
 { scpeer, "peer" },
 { scexternal, "external" },
 NULL
};

DCnlist XMLrd::LinkTopicTypes;
npair XMLrd::LinkTopicTypesInit[] = {
 { rltopic, "topic" },
 { rlconcept, "concept" },
 { rltask, "task" },
 { rlreference, "reference" },
 NULL
};

DCnlist XMLrd::LinkingTypes;
npair XMLrd::LinkingTypesInit[] = {
 { linknone, "none" },
 { linksource, "sourceonly" },
 { linktarget, "targetonly" },
 { linknormal, "normal" },
 NULL
};

DCnlist XMLrd::LinkForms;
npair XMLrd::LinkFormsInit[] = {
 { ftdita, "dita" },
 { ftditamap, "ditamap" },
 { fthtml, "html" },
 { ftpdf, "pdf" },
 { fttxt, "txt" },
 NULL
};

DCnlist XMLrd::LinkCollTypes;
npair XMLrd::LinkCollTypesInit[] = {
 { colunord, "unordered" },
 { colseqence, "sequence" },
 { colchoice, "choice" },
 { colfamily, "family" },
 NULL
};

DCnlist XMLrd::RelAlinkNames;
DCvlist XMLrd::RelAlinkRefs;
DClist *XMLrd::RelTopicAlinkRefs = NULL;
long XMLrd::RelAlinkNameCount = 0;


bool
XMLrd::ProcRelatedLinks(DCtopelem *elem, const char **atts)
{
	static relattr *rarellinks = NULL;
	static relattr *ralinklist = NULL;
	static relattr *ralinkpool = NULL;

	rellink *rl = NULL;
	roletyp ltype = rlnone;

	const char *idattr = NULL;
	const char *role = NULL;
	const char *otherrole = NULL;
	const char *outclass = NULL;
	const char *typeattr = NULL;
	const char *formattr = NULL;
	const char *scope = NULL;
	const char *collection = NULL;

	attrp alist[] = {
	 {"id", &idattr },
	 {"role", &role },
	 {"otherrole", &otherrole },
	 {"outputclass", &outclass },
	 {"type", &typeattr },
	 {"format", &formattr },
	 {"scope", &scope },
	 {"collection-type", &collection },
	 {NULL, NULL }
	};

	ProcAttrs(atts, alist);

	relattr *ra = new relattr;

	if (scope)
		ra->lscope = (scopetyp) LinkScopes.getitem(scope);
	if (role) {
		ra->lrole = NewName(role);
		ra->lorole = NewName(otherrole);
		ra->lrval = (roletyp) LinkRoles.getitem(role);
	}
	if (typeattr) {
		ra->ltype = NewName(typeattr);
		ra->ltoptype = (roletyp) LinkRoles.getitem(typeattr);
	}
	if (formattr) {
		ra->lform = NewName(formattr);
		ra->lftype = (formtyp) LinkForms.getitem(formattr);
		if (((ra->lftype > ftditamap)
		  || (ra->lftype < ftdita))
		 && !scope)
			ra->lscope = scexternal;
	}
	if (collection) {
		ra->lcoll = NewName(collection);
		ra->lcollect = (colltyp) LinkCollTypes.getitem(collection);
	}

	if (InLinkList)
		AdjustRelAttrs(ra, ralinklist);
	if (InLinkPool)
		AdjustRelAttrs(ra, ralinkpool);
	if (!InLinkList && !InLinkPool)
		AdjustRelAttrs(ra, rarellinks);


	// attributes inherit from general to specific:
	// related-links, then linklist or linkpool, then link

	if (elem->checkType(epRel, epMap)) {  // related links
		rarellinks = ra;
		InLinkList = false;
		ralinklist = NULL;
		InLinkPool = false;
		ralinkpool = NULL;
		if (GenerateALinks
		 && idattr
		 && *idattr)
			SetElemRelLinkAlink(elem->topic, idattr);
	}
	else if (elem->checkType((epLink << 8) | epList, epSeq)) {  // linkpool
		InLinkPool = true;
		ralinkpool = ra;
	}
	else if (elem->checkType((epLink << 8) | epList)) {  // linklist
		InLinkList = true;
		ralinklist = ra;
	}
	else if (CurrElemInfo->checkType(epLink, (epText << 8) | epList)) {   // link
		// create rellink struct for each link, and
		// put rellink in var, or in list for its role,
		// in the topic's rlinks struct
		if (RelTopicAlinkRefs)
			RelTopicAlinkRefs->add((long) CurrElemInfo->href);

		CurrRelLink = rl = new rellink;
		rl->lsrc = CurrElemInfo;
		rl->lhref = CurrElemInfo->href;
		rl->loclass = NewName(outclass);
		rl->at = ra;

		rl->lrtype = GetRoleType(ra->lrval, ra->ltoptype);

		switch(rl->lrtype) {
			case rlprev:
				AddSingleRelLink(rl, &PrevLinks);
				break;
			case rlnext:
				AddSingleRelLink(rl, &NextLinks);
				break;
			case rlparent:
				AddSingleRelLink(rl, &ParentLinks);
				break;

			case rlchild:
				AddRelLink(rl, &CurrChildLinks, &ChildLinks);
				break;
			case rlsibling:
				AddRelLink(rl, &CurrSiblingLinks, &SiblingLinks);
				break;
			case rlancestor:
				AddRelLink(rl, &CurrAncestorLinks, &AncestorLinks);
				break;
			case rldescendant:
				AddRelLink(rl, &CurrDescendantLinks, &DescendantLinks);
				break;
			case rlcousin:
				AddRelLink(rl, &CurrCousinLinks, &CousinLinks);
				break;
			case rlfriend:
				AddRelLink(rl, &CurrFriendLinks, &FriendLinks);
				break;

			case rlconcept:
				AddRelLink(rl, &CurrConceptLinks, &ConceptLinks);
				break;
			case rltask:
				AddRelLink(rl, &CurrTaskLinks, &TaskLinks);
				break;
			case rlreference:
				AddRelLink(rl, &CurrReferenceLinks, &ReferenceLinks);
				break;

			case rlrelated:
				AddRelLink(rl, &CurrRelatedLinks, &RelatedLinks);
				break;

			default:
				break;
		}

#if 0
		if (CurrElemInfo->href) {
			PendLink = true;
			LinkHref = CurrElemInfo->href;
			XrefExt = scope ? !strcmp(scope, "external") : false;
		}
#endif
	}
	else if (CurrElemInfo->checkType(epDesc)) {   // desc
		if (CurrRelLink && !CurrRelLink->ldesc)
			CurrRelLink->ldesc = TextGroup = new DCgroup();
	}
	else if (elem->checkType((epLink << 8) | epText), epSeq) {   // linktext
		if (CurrRelLink && !CurrRelLink->ltext)
			CurrRelLink->ltext = TextGroup = new DCgroup();
	}
	else if (elem->checkType((epLink << 8) | epText)) {   // linkinfo
		// used to describe linklist
	}
	else   // title, many more to process normally
		return false;

	return true;  // completed
}


void
XMLrd::SetElemRelLinkAlink(DCtopic *top, const char *idattr)
{
	long al = 0;
	char *alid = NewName(idattr);
	if ((al = RelAlinkNames(alid)) != 0) {
		RelTopicAlinkRefs = (DClist *) RelAlinkRefs.find(al);
	}
	else {
		RelAlinkNames.addnew(alid, ++RelAlinkNameCount);
		RelTopicAlinkRefs = new DClist;
		RelAlinkRefs.add(RelTopicAlinkRefs, RelAlinkNameCount);
	}
	top->alinkid = alid;
	top->alinks = new DClist;
}


void
XMLrd::InitRelatedLinks(void)
{
	CurrChildLinks = NULL;
	CurrSiblingLinks = NULL;
	CurrAncestorLinks = NULL;
	CurrDescendantLinks = NULL;
	CurrCousinLinks = NULL;
	CurrFriendLinks = NULL;

	CurrConceptLinks = NULL;
	CurrTaskLinks = NULL;
	CurrReferenceLinks = NULL;
	CurrRelatedLinks = NULL;
}


void 
XMLrd::AddSingleRelLink(rellink *rl, DCvlist *list)
{
	if (!list->find((long) CurrTopicInfo))
		list->add(rl, (long) CurrTopicInfo);
}


void 
XMLrd::AddRelLink(rellink *rl, DClist **currlist, DCvlist *list)
{
	DClist *clist = (DClist *) list->find((long) CurrTopicInfo);

	if (clist != NULL)  // already started
		*currlist = clist;
	else if (!*currlist) {
		*currlist = new DClist;
		list->add(*currlist, (long) CurrTopicInfo);
	}
	
	(*currlist)->add((long) rl);
}


roletyp
XMLrd::GetRoleType(roletyp ltype, roletyp ttype)
{
	if (!ltype
	 || !CheckRole(ltype)) {
		if (!UseParent
		 && UseAncestors
		 && (ltype == rlparent))
			ltype = rlancestor;
		else if (!UseChildren
		 && UseDescendants
		 && (ltype == rlchild))
			ltype = rldescendant;
		else if (UseTopicTypes
		 && ttype)
			ltype = ttype;
		else
			ltype = rlrelated;
	}
	return ltype;
}


bool 
XMLrd::CheckRole(roletyp role)
{
	switch (role) {
	
		case rlprev:
		case rlnext:
			if (UsePrevNext)
				return true;
			break;

		case rlparent:
			if (UseParent)
				return true;
			break;

		case rlchild:
			if (UseChildren)
				return true;
			break;

		case rlsibling:
			if (UseSiblings)
				return true;
			break;

		case rlancestor:
			if (UseAncestors)
				return true;
			break;

		case rldescendant:
			if (UseDescendants)
				return true;
			break;

		case rlcousin:
			if (UseCousins)
				return true;
			break;

		case rlfriend:
			if (UseFriends)
				return true;
			break;

		case rlconcept:
		case rltask:
		case rlreference:
			if (UseTopicTypes)
				return true;
			break;

		case rlrelated:
			return true;
			break;

		case rlother:
			break;

		case rlnone:
		default:
			break;
	}

	return false;
}


void 
XMLrd::AdjustRelAttrs(relattr *ra, relattr *rx)
{
	if (!rx)
		return;

	if (ra->lscope == 0)
		ra->lscope = rx->lscope;

	if (ra->lrval == 0)
		ra->lrval = rx->lrval;
	if (ra->lrole == NULL)
		ra->lrole = rx->lrole;
	if (ra->lorole == NULL)
		ra->lorole = rx->lorole;

	if (ra->lftype == 0)
		ra->lftype = rx->lftype;
	if (ra->lform == NULL)
		ra->lform = rx->lform;

	if (ra->ltoptype == 0)
		ra->ltoptype = rx->ltoptype;
	if (ra->ltype == NULL)
		ra->ltype = rx->ltype;

	if (ra->lcollect == 0)
		ra->lcollect = rx->lcollect;
	if (ra->lcoll == NULL)
		ra->lcoll = rx->lcoll;

	if (ra->links == 0)
		ra->links = rx->links;
	if (ra->linking == NULL)
		ra->linking = rx->linking;
}


void 
XMLrd::AdjustCellAttrs(relattr *ra, relattr *rcell, int depth)
{
	relattr *rx = NULL;

	if ((rx = (relattr *) RelCellAttrs.find(depth - 1)) != NULL)
		AdjustRelAttrs(ra, rx);
	else
		AdjustRelAttrs(ra, rcell);
	RelCellAttrs.replace(ra, depth);
}



// end of drxmlrl.cpp


