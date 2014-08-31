 
// dwrtfrf.cc is the code for dcl Microsoft RTF raster writing, JHG, 2/6/97
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwrtf.h"
#include "dwrtffl.h"



// processing switch


bool
RTFwr::ProcRf(RTFctl *cp)
{
  switch((cp->rp)->scan_op) {
    case nothing:   // just put with its group for later
      return true;

    case pass:      // ignore, don't save
      break;

    case rasprop:
			if (cp->dat() == 0)
				ProcGrMetafile(cp);
			else
				ProcGrRaster(cp);
      break;
    case rasbmap:
			if (cp->dat() == 9)
				ProcGrMetaRecs(cp);
			else
				ProcGrBitmap(cp);
      break;
    case rascmap:
      ProcGrColormap(cp);
      break;

		case epsprop:
			ProcGrEPSProp(cp);
			break;
		case epshead:
			ProcGrEPSHead(cp);
			break;

    default:
      break;
  }
  return false;  // default is to discard ctl after scan
}


// metafile processing

wmeta *RTFwr::Metafile;

void
RTFwr::ProcGrMetafile(RTFctl *cp)
{
	assert(cp->dat() == 0);
  long *lptr = (long *) cp->ex();

	Metafile = new wmeta;
  Graphic->gtype = 0x1;
	Graphic->gparm = (short *) Metafile;
	Graphic->gsize = 1;
	Metafile->mtype = 1; // component
  Metafile->ixpos = GetTwips(lptr[0], pg_unit);
	Metafile->iypos = GetTwips(lptr[1], pg_unit);
	Metafile->iwide = GetTwips(lptr[2], pg_unit);
	Metafile->ihigh = GetTwips(lptr[3], pg_unit);
}


void
RTFwr::ProcGrMetaRecs(RTFctl *cp)
{
	assert(cp->dat() == 9);
	Metafile->mrecs = new DCblock();
	*(Metafile->mrecs) = *cp;
}


void
RTFwr::WriteGrMetafile(RTFwfile *wf, wmeta *mf, frame *fr)
{
	// write out all WMF records
	uns *chp = ((uns *) mf->mrecs->p()) + 20;
	uns objcount = *(((uns *) mf->mrecs->p()) + 16);
	long rsize;
	uns rcode;
	long i;
	bool RawInclMeta = false;

	if (objcount == 0)
		return;

	if (mf->mtype == 1)
		fr = RFrame;

	// remapping of objs
	bool RemapObjs = true;
	DCvlist MetaObjDefList;
	unc *objtypes;	// 0 = pen, 1 = brush, 2 = font, 0xFF = none
	unc *objstates;	// 0 = unused, 1 = defined
	uns objsel[3];	// orig handle of selected obj of each type

	if (RemapObjs) {
		RawInclMeta = false;
		objtypes = new unc[objcount];
		memset(objtypes, 0xFF, objcount);
		objstates = new unc[objcount];
		memset(objstates, 0, objcount);
		objsel[0] = objsel[1] = objsel[2] = 0xFFFF;
	}

  wf->putMEnd();
  wf->putML(3);
	UpdateWMFHead(3);
  wf->putMS(0x01E);  // save DC

	bool orgset = false;
	bool extset = false;
	bool orgput = false;
	short mxorg = mf->ixpos;
	short myorg = mf->iypos;
	short mxext = mf->iwide;
	short myext = mf->ihigh;
	short xorg = 0;
	short yorg = 0;
	short xext = 0;
	short yext = 0;
	uns mmode = 0;
	bool worg = false;
	bool wext = false;
	short select = -1;

	if (RawInclMeta) {
		// delete our existing objects
		for (select = WMFobjmax; select >= 0; select--)
		  WriteWMFDel(wf, select);
		// allow for more new ones
		if (objcount > WMFobjmax)
			WMFobjmax = objcount;
	}

	for (;;) {
		rsize = *((long *) chp);
		chp += 2;

		if ((rcode = *chp++) == 0) {
			assert(rsize == 3);
			break;
		}
		else if (rcode == 0x103) { // mapping mode
			mmode = *chp++;
			assert ((mmode == 7) || (mmode == 8));
			continue;
		}
		else if (rcode == 0x20D) { // viewport org
			worg = true;
			chp += 2;
			continue;
		}
		else if (rcode == 0x20E) { // viewport ext
			wext = true;
			chp += 2;
			continue;
		}
		else if (rcode == 0x20B) { // window org
			if (orgset == true) {
				chp += 2;
				continue;
			}
			yorg = (short) *chp++;
			xorg = (short) *chp++;
			orgset = true;
			if (orgput == true)  // already out
				orgput = false;    // change it
			else
				continue;
		}
		else if (rcode == 0x20C) { // window ext
			if ((extset == true)
			 && (yext > 1)    // dummy values in OLE objects
			 && (xext > 1)) {
				chp += 2;
				continue;
			}
			yext = (short) *chp++;
			xext = (short) *chp++;
			extset = true;
			if (orgput == true)  // already out
				orgput = false;    // change it
			else
				continue;
		}
		else if (rcode == 0x626) { // comment
			chp += (rsize - 3);
			continue;
		}

		if ((orgput == false)   // put at start of WMF
		 && ((orgset && extset)     // as long as values seen
		  || IsWMFObject(rcode))) { // or real object is next
			if (!myext || !mxext)
				continue;
			if (mf->mtype == 1) {
			  wf->putMEnd();
				wf->putML(5);
				UpdateWMFHead(5);
				wf->putMS(0x20B);
				wf->putMS(yorg - (short) (((long) yext * myorg) / myext));
				wf->putMS(xorg - (short) (((long) xext * mxorg) / mxext));
			}
			orgset = true;

			if (mf->mtype == 1) {
			  wf->putMEnd();
				wf->putML(5);
				UpdateWMFHead(5);
				wf->putMS(0x20C);
				wf->putMS((short) (((long) yext * fr->high) / myext));
				wf->putMS((short) (((long) xext * fr->wide) / mxext));
			}
			extset = true;

			orgput = true;
		}

		if (RemapObjs) {
			unc objtype = 0xFF;
			long wsize = rsize;
			
			if (rcode == 0x1F0) {	// delete object
				uns delobj = *chp++;
				// remove stored definition from DCvlist, zero in objstates
				// put FFFF in objsel for type if selected, FF in objtypes
				MetaObjDefList.remove(delobj + 1);
				if (delobj < objcount) {
					objstates[delobj] = 0;
					objtype = objtypes[delobj];
					if (objsel[objtype] == delobj)
						objsel[objtype] = 0xFFFF;
					objtypes[delobj] = 0xFF;
				}
				// let deletion of type wait until new selection of type
				continue;
			}

			else if ((rcode == 0x12D) // select object
			 || (rcode == 0x234)) {   // select palette
				uns selobj = (short) *chp++;
				if (selobj < objcount) {
					// determine type from objtypes
					objtype = objtypes[selobj];
					// if already selected in objsel, ignore; else:
					if (objsel[objtype] != selobj) {
						// define new item from stored def in DCvlist
						uns *sp = (uns *) MetaObjDefList.find(selobj + 1);
						if (sp == NULL)
							continue;  // not defined, skip it
						long osize = *( (long *) sp);

						// delete prev item of same type
					  WriteWMFDel(wf, objtype);

						// write definition
						wf->putMEnd();
						wf->putML(osize);
						UpdateWMFHead(osize);
						for (i = 2; i < osize; i++)
							wf->putMS(sp[i]);

						// select object by its type
					  WriteWMFSel(wf, objtype);

						// put orig handle in objsel for type
						objsel[objtype] = selobj;
					}
				}
				continue;
			}

			else if (rcode == 0x2FA) {   // create pen
				objtype = 0;
				wsize = 8;
			}
			else if ((rcode == 0x2FC)   // create brush
			 || (rcode == 0x1F9)
			 || (rcode == 0x142))
 				objtype = 1;
			else if (rcode == 0x2FB)    // create font
				objtype = 2;
			else if (rcode == 0x0F7)    // create palette
				objtype = 3;

			if (objtype != 0xFF) {  // creating obj
				// orig obj handle is lowest free slot in objstates
				for (i = 0; i < objcount; i++) {
					if (objstates[i] == 0)
						break;
				}
				if (i < objcount) {
					// store definition, including rsize and rcode, in DCvlist
					// index by original obj handle (as used for selobj/delobj)
					uns *ip = new uns[wsize];
					*((long *) ip) = wsize; // **** nonportable
					memcpy(ip + 2, chp - 1, (wsize - 2) * sizeof(short));
					MetaObjDefList.add(ip, i + 1);
					objstates[i] = 1;
					objtypes[i] = objtype;
				}
				chp += (rsize - 3);
				rsize = wsize;
				continue;
			}
		}

		else if (!RawInclMeta) {
			// handle pen, brush, and font selection consistently
			if (rcode == 0x1F0) {	// delete object
				chp++;
				continue;
			}
			else if (rcode == 0x12D) {  // select object
				chp++;
				continue;
			}
			else if (rcode == 0x2FA)    // create pen
				select = 0;
			else if ((rcode == 0x2FC)   // create brush
			 || (rcode == 0x1F9)
			 || (rcode == 0x142))
 				select = 1;
			else if (rcode == 0x2FB)    // create font
				select = 2;

			if (select >= 0) {  // deselect former object
			  WriteWMFDel(wf, select);
			}
		}


	  wf->putMEnd();
		wf->putML(rsize);
		UpdateWMFHead(rsize);
		wf->putMS(rcode);
		if ((rcode == 0xF43) && (mf->mtype == 2)) {
			for (i = 3; i < 10; i++)
				wf->putMS(*chp++);
			wf->putMS(myext);
			wf->putMS(mxext);
			wf->putMS(myorg);
			wf->putMS(mxorg);
			chp += 4;
			for (i = 14; i < rsize; i++)
				wf->putMS(*chp++);
		}
		else if ((rcode == 0xB41) && (mf->mtype == 2)) {
			for (i = 3; i < 9; i++)
				wf->putMS(*chp++);
			wf->putMS(myext);
			wf->putMS(mxext);
			wf->putMS(myorg);
			wf->putMS(mxorg);
			chp += 4;
			for (i = 13; i < rsize; i++)
				wf->putMS(*chp++);
		}
		else {
			for (i = 3; i < rsize; i++)
				wf->putMS(*chp++);
		}
		if ((!RemapObjs)
		 && (!RawInclMeta)) {
			if (select >= 0) {  // select new object
			  WriteWMFSel(wf, select);
				select = -1;
			}
		}
	}

	wf->putMEnd();
	wf->putML(4);
	UpdateWMFHead(4);
	wf->putMS(0x127);  // restore DC
	wf->putMS(-1);

	if (RemapObjs) {
		delete [] objtypes;
		delete [] objstates;
		MetaObjDefList.clear(true);
	}
}


uns RTFwr::WMFObjCodes[WMFObjCodesMax] = {
 0x213, 0x214, 0x324, 0x325,
 0x418, 0x41B, 0x521, 0x61C,
 0x817, 0x81A, 0x830, 0xA32, 0xB41, 0xF43
};

bool
RTFwr::IsWMFObject(uns code)
{
	for (unc i = 0; i < WMFObjCodesMax; i++) {
		if (code == WMFObjCodes[i])
			return true;
	}
	return false;
}


// raster processing


rast *RTFwr::Raster;


void
RTFwr::ProcGrRaster(RTFctl *cp)
{
  if (Verbose)
    printf("raster %d ", cp->dat());

  Raster = new rast;
  Graphic->gtype = 0xF43;
  Graphic->gparm = (short *) Raster;
  Graphic->gsize = 34;

  long *lptr = (long *) cp->ex();
  Raster->pdeep = cp->dat();
  Raster->ixpos = GetTwips(lptr[0], pg_unit);
  Raster->iypos = GetTwips(lptr[1], pg_unit);
  Raster->iwide = GetTwips(lptr[2], pg_unit);
  Raster->ihigh = GetTwips(lptr[3], pg_unit);
	Raster->dpi = BitmapDPI;

  Raster->pwide = lptr[4];
  Raster->phigh = lptr[5];
	Raster->pcolor = NULL;

	GrStretch = 3;
	Graphic->gypos = Raster->iypos;

  switch (Raster->pdeep) {
    case 1:
			GrStretch = 1;
      //Raster->pbpl = ((Raster->pwide + 31) / 32) * 4;
      Raster->pbpl = ((Raster->pwide + 15) / 16) * 2;
      Raster->cmap = 2;
      break;
    case 4:
      //Raster->pbpl = ((Raster->pwide + 7) / 8) * 4;
      Raster->pbpl = ((Raster->pwide + 7) / 8) * 4;
      Raster->cmap = 16;
      break;
    case 8:
      //Raster->pbpl = ((Raster->pwide + 3) / 4) * 4;
      Raster->pbpl = ((Raster->pwide + 1) / 2) * 2;
      Raster->cmap = 256;
      break;
    case 24:
      //Raster->pbpl = Raster->pwide * 4;
      //Raster->pbpl = (((Raster->pwide * 3) + 1) / 2) * 2;
      Raster->pbpl = (((Raster->pwide * 3) + 3) / 4) * 4;
      Raster->cmap = 0;
      break;
		case 32:
			Raster->pbpl = Raster->pwide * 4;
			Raster->cmap = 0;
			break;
    default:
      Raster->pbpl = 0;
      Raster->cmap = 0;
      break;
  }
}


void
RTFwr::ProcGrColormap(RTFctl *cp)
{
  uns maptype = cp->dat();

  Raster->pcolor = new unc[Raster->cmap * 4];
  Graphic->gsize += Raster->cmap * 2;

  if (maptype == 1) {  // SunRaster
    unc *bptr = (unc *) cp->ex();
    long msize = cp->siz() / 3;
    uns cmax = (Raster->cmap < (short) msize) ? Raster->cmap : (uns) msize;
    for (uns i = 0; i < cmax; i++) { // fill map
      Raster->pcolor[i * 4] = bptr[(msize * 2) + i];    // blue
      Raster->pcolor[(i * 4) + 1] = bptr[msize + i];    // green
      Raster->pcolor[(i * 4) + 2] = bptr[i];            // red
    }
  }
  else if (maptype == 2)  // MS DIB
     memcpy(Raster->pcolor, cp->ex(), cp->siz());
}


void
RTFwr::ProcGrBitmap(RTFctl *cp)
{
  Raster->comp = 0;  // begin with uncompressed form
  bool compressed = false;
	bool flip = BitmapFlip;
	short line = 0;

  switch (cp->dat()) {
    case 1: // uncompressed
    case 6: // DIB uncompressed
			line = (short) (cp->siz() / Raster->phigh);
			if ((Raster->pbpl == line - 2) // rounded up more
			 || (Raster->pbpl == line + 2)) // rounded up less
				Raster->pbpl = line;
		  Raster->pbits = new DCblock(Raster->phigh * Raster->pbpl);
      *(Raster->pbits) = *cp;
      break;
    case 4: // rf
			CheckGrSunRaster(cp->siz(), (unc *) cp->ex()); // sets pbpl
			if (Graphic->gsize == 0)
				return;
		  Raster->pbits = new DCblock(Raster->phigh * Raster->pbpl);
			ProcGrSunRaster(cp->siz(), (unc *) cp->ex()); // unpacks it
			if (Raster->pdeep == 1) {   // switch black and white for rf
				if (Raster->pcolor == NULL) {
					Raster->pcolor = new unc[8];
					Graphic->gsize += 4;
				}
				((long *) Raster->pcolor)[0] = 0x00FFFFFF;
				((long *) Raster->pcolor)[1] = 0;
			}
			else if (Raster->pdeep == 8)
				RepackRaster(((Raster->pbpl + 3) / 4) * 4);
			flip = flip ? false : true;  // reverse meaning
      break;
    case 7: // DIB, 4-bit compressed
    case 8: // DIB, 8-bit compressed
      compressed = true;
		  Raster->pbits = new DCblock(Raster->phigh * Raster->pbpl);
      *(Raster->pbits) = *cp;
      break;
    case 2: // pbm
    case 3: // pcx
    case 5: // TIFF packbits
    default:
			Graphic->gsize = 0;
			return;
  }

	if (flip) {
	  Raster->ihigh = -(Raster->ihigh); // flip
		Raster->iypos -= Raster->ihigh;	  // move start to bottom
	}

	if ((Raster->pdeep == 24)
	 && (Ras24Mode > 0))
		Convert24BitRaster();

	if (Raster->pdeep == 24)
		Repack24BitRaster();
	else if (Raster->pdeep == 1)
		RepackMonoRaster();
  else if (CompRasters
	 && (!compressed)  // compress here if depth is 4 or 8
   && ((Raster->pdeep == 4) || (Raster->pdeep == 8)))
		ProcGrBits();
  Graphic->gsize += Raster->pbits->l() / 2;
}


void
RTFwr::Repack24BitRaster(void)
{
	// pack to next highest 4-byte boundary
	long new_pbpl = (((Raster->pwide * 3) + 3) / 4) * 4;

	RepackRaster(new_pbpl);
}


void
RTFwr::RepackMonoRaster(void)
{
	// pack to next highest 4-byte boundary
	long new_pbpl = ((Raster->pwide + 31) / 32) * 4;

	RepackRaster(new_pbpl);
}


void
RTFwr::RepackRaster(long new_pbpl)
{
	if ((new_pbpl == Raster->pbpl) // already at correct 2-byte boundary
	 && (Raster->pbits->l() == (size_t)(new_pbpl * Raster->phigh)))
		return;  // and length is not excessive from garbage at end

	DCblock *nb = new DCblock(Raster->phigh * new_pbpl);
	unc *nptr = (unc *) nb->p();
	unc *optr = (unc *) Raster->pbits->p();

	// use shorter of two for copy
	long line = (Raster->pbpl > new_pbpl) ? new_pbpl : Raster->pbpl;

	for (long i = 0; i < Raster->phigh; i++) {
		memcpy(nptr, optr, line);
		nptr += new_pbpl;
		optr += Raster->pbpl;
	}

  delete Raster->pbits;
  Raster->pbits = nb;
	Raster->pbpl = (short) new_pbpl;
}


void
RTFwr::Convert24BitRaster(void)
{
	assert(Raster->pdeep == 24);
	assert(Raster->cmap == 0);

	short obpl = Raster->pbpl;
	switch (Ras24Mode) {
		case 1:  // mono
			GrStretch = 1;
      Raster->pbpl = ((Raster->pwide + 31) / 32) * 4;
      Raster->cmap = 2;
      break;
		case 4:  // 16 color
			return;  // **** not implemented
			//GrStretch = 3;
      //Raster->pbpl = ((Raster->pwide + 7) / 8) * 4;
      //Raster->cmap = 16;
      //break;
		case 8:  // 256 color
			return;  // **** not implemented
			//GrStretch = 3;
      //Raster->pbpl = ((Raster->pwide + 3) / 4) * 4;
      //Raster->cmap = 256;
			//break;
		default: // wrong
			assert(0);
			break;
	}
	Raster->pdeep = Ras24Mode;  // new depth
  DCblock *nb = new DCblock(Raster->phigh * Raster->pbpl);

	Raster->pcolor = (Ras24Mode > 1) ? new unc[Raster->cmap * 4] : NULL;
	Graphic->gsize += Raster->cmap * 2;

	unc *olp = (unc *) Raster->pbits->p();
	unc *op = olp;
	unc *nlp = (unc *) nb->p();
	unc *np = nlp;
	unc pb = 0;
	short line = 0;
	short px = 0;
	unc blue, green, red;

	if (Ras24Mode == 1) {
		Raster->comp = 0;
		for (line = 0; line < Raster->phigh; line++) {
			for (px = 0; px < Raster->pwide; px++) {
				blue = *op++;
				green = *op++;
				red = *op++;
				if ((blue | green | red) != 0)  // anything not black is white
					pb |= 1;
				if ((px & 7) == 7) { // 8 bits packed
					*np++ = pb;
					pb = 0;
				}
				else pb <<= 1;
			}
			if ((px & 7) != 0) {  // pack the leftover bits
				for (;; px++) {
					pb |= 1;  // fill with white
					if ((px & 7) == 7) { // 8 bits packed
						*np++ = pb;
						pb = 0;
						break;
					}
					pb <<= 1;
				}
			}
			olp += obpl;
			op = olp;
			nlp += Raster->pbpl;
			while (np < nlp)
				*np++ = 0xFF;
		}
	}
	delete Raster->pbits;
	Raster->pbits = nb;
}


void
RTFwr::ProcGrBits(void)
{
  uns ppb;
	uns bwide;
	uns bhigh = 0;
	uns lastp;
	uns difflim;
	uns samelim;

	if (!CompRasters)
		return;

  if (Raster->pdeep == 8) {
    Raster->comp = ppb = 1;
    bwide = (uns) Raster->pwide;
    lastp = 0;
    difflim = 254;
    samelim = 255;
  }
  else {
    Raster->comp = ppb = 2;
    bwide = (Raster->pwide + 1) / 2;
    lastp = Raster->pwide & 1;
    difflim = 127;
    samelim = 127;
  }

	// allow for pathological rasters where comp increases size
  DCblock *nb = new DCblock((Raster->pbits->l() / 8) * 10);
  unc *sptr = (unc *) Raster->pbits->p();
  unc *fptr = sptr;
  unc *nptr = sptr + 1;
  unc *lptr = sptr + bwide;
  unc *eptr = sptr + Raster->pbits->l();
  unc *dptr = (unc *) nb->p();
  uns same = 0;
  uns diff = 0;
  unc pad = Raster->pbpl - bwide;

  while (nptr < eptr) {
    while (nptr < lptr) {
      if (*nptr == *sptr) {	// two are the same
        if (diff) {					// in a diff run
          if (((diff * ppb) < 3) 	 // diff run too short
           || (nptr == (lptr - 1))) {  // no room for same run
            diff++;					// add pixel to diff run anyway
          }
					else {
						*dptr++ = 0;			// 0 starts diff run
						*dptr++ = diff * ppb;	 // count of pixels
						memcpy(dptr, fptr, diff);	// run itself
						dptr += diff;
						fptr += diff;
						if (diff & 1)			// pad to even byte
							*dptr++ = 0;
						diff = 0;
						same++;
					}
        }
        else if (++same == samelim) {	// max in a same run
          *dptr++ = samelim * ppb; // count of pixels
          *dptr++ = *sptr;				// value of pixel
          same = 0;
          fptr = nptr; // move after run just recorded
        }
      }
      else {		 					 // two pixels are different
        if (same) {	       // must end any same run
          *dptr++ = (same + 1) * ppb;
          *dptr++ = *sptr;
          same = 0;
          fptr = nptr;
        }
        else if (++diff == difflim) {	 // max in a diff run
          *dptr++ = 0;			// 0 starts diff run
          *dptr++ = difflim * ppb; // count of pixels
          memcpy(dptr, fptr, difflim); //  the run itself
          dptr += difflim;
          fptr = nptr;
          if (diff & 1)			// pad to even byte
            *dptr++ = 0;
          diff = 0;
        }
        sptr = nptr;
      }
      nptr++;
    }
		// at end of line
    if (same) {        // can always write out same run
      *dptr++ = ((same + 1) * ppb) - lastp;
      *dptr++ = *sptr;
    }
    else if ((((diff + 1) * ppb) - lastp) >= 3) {	// diff run
			diff++;
      *dptr++ = 0;
      *dptr++ = (diff * ppb) - lastp;
      memcpy(dptr, fptr, diff);
      dptr += diff;
      fptr += diff;
      if (diff & 1)
        *dptr++ = 0;
    }
    else if (diff == 1) {  // two-byte diff run
      *dptr++ = (unc) ppb;
      *dptr++ = *fptr++;
      *dptr++ = ppb - lastp;
      *dptr++ = *fptr;
    }
    else {	         // single diff byte at end
      *dptr++ = ppb - lastp;
      *dptr++ = *fptr;
    }
    sptr = fptr = nptr + pad;
    nptr = sptr + 1;
    lptr += Raster->pbpl;
    *dptr++ = 0;
    *dptr++ = 0;
    same = diff = 0;
		// bail if over 12.5% size increase (buffer allows 25%)
		if ((dptr - (unc *) nb->p()) > (((++bhigh * bwide) / 8) * 9)) {
			Raster->comp = 0;
			delete nb;
			return;
		}
  }

  *dptr++ = 0;  // end of bitmap
  *dptr++ = 1;
  nb->set(dptr - (unc *) nb->p());
  delete Raster->pbits;
  Raster->pbits = nb;
}


void
RTFwr::CheckGrSunRaster(long rsize, unc *bptr)
{
  unc *eptr = bptr + rsize;
	long usize = 0L;

  while (bptr < eptr) {
    if (*bptr++ == 0x80) {
      if (*bptr != 0)
        usize += *bptr++;
      bptr++;
    }
		usize++;
  }

	if ((usize % Raster->phigh) != 0) {  // error in dimensions
		if ((Raster->pbpl != 0)  // computed bytes per line
		 && ((usize % Raster->pbpl) == 0))  // gives even line count
			Raster->phigh = usize / Raster->pbpl; // fixup height
		else {  // unrecoverable mess, skip it
			assert(0);
			Graphic->gsize = 0;
			return;
		}
	}
	Raster->pbpl = (short) (usize / Raster->phigh);
}


void
RTFwr::ProcGrSunRaster(long rsize, unc *bptr)
{
  unc *eptr = bptr + rsize;
  unc *pptr = (unc *) Raster->pbits->p();
	long dsize = Raster->pbits->l();
  long pos = 0L;
  uns i, lim;

  while (bptr < eptr) {
    if (*bptr == 0x80) {
      if (*++bptr == 0)
        pptr[pos++] = 0x80;
      else {
        lim = *bptr++;
        for (i = 0; i <= lim; i++)
          pptr[pos++] = *bptr;
      }
      bptr++;
    }
    else pptr[pos++] = *bptr++;
		if (pos >= dsize)  // limit to allocated space
			break;
  }
	assert(pos == dsize);

	if (Raster->pdeep == 24) {
		unc red = 0;
		pos = 0;
		for (uns line = 0; line < Raster->phigh; line++) {
			pos = Raster->pbpl * line;
			for (i = 0; i < (Raster->pbpl - 2); i += 3) {
				red = pptr[pos + i];
				pptr[pos + i] = pptr[pos + i + 2];
				pptr[pos + i + 2] = red;
			}
		}
	}
	else if ((Raster->pdeep == 1)
	 && (Raster->pwide < (Raster->pbpl * 8))) { // zero any fill bits
		unc fillbits = (unc) ((Raster->pbpl * 8) - Raster->pwide);
		unc bmask = 0xFF << (fillbits & 7);  // mask for last data
		uns bfill = fillbits / 8;  // bytes of all fill at end
		pos = 0;
		for (uns line = 0; line < Raster->phigh; line++) {
			pos += Raster->pbpl - bfill;
			if (bmask != 0xFF) {
				pos--;
				pptr[pos] &= bmask;
				pos++;
			}
			for (i = 0; i < bfill; i++)
				pptr[pos++] = 0;
		}
	}
}



// EPS header processing


void
RTFwr::ProcGrEPSProp(RTFctl *cp)
{
  if (Verbose)
    printf("raster %d ", cp->dat());

  Raster = new rast;
  Graphic->gtype = 0xF43;
  Graphic->gparm = (short *) Raster;
  Graphic->gsize = 34;

  long *lptr = (long *) cp->ex();
  Raster->ixpos = GetTwips(lptr[0], pg_unit);
  Raster->iypos = GetTwips(lptr[1], pg_unit);
  Raster->iwide = GetTwips(lptr[2], pg_unit);
  Raster->ihigh = -(GetTwips(lptr[3], pg_unit)); // flip
	Raster->iypos -= Raster->ihigh;
}


void
RTFwr::ProcGrEPSHead(RTFctl *cp)
{
	long raslen = cp->siz();
	unc *rasptr = (unc *) cp->ex();
  long *lptr = (long *) cp->ex();

	if ((cp->dat() != 4)  // SunRaster only
	 || (*lptr != 0x956AA659)) { // backwards
		delete Raster;
		Raster = NULL;
		delete Graphic;
		Graphic = NULL;
		return;
	}

  Raster->pwide = revL(lptr[1]);
  Raster->phigh = revL(lptr[2]);
  Raster->pdeep = (short) revL(lptr[3]);

	GrStretch = 3;

  switch (Raster->pdeep) {	// padded to 2-byte boundaries
    case 1:
			GrStretch = 1;
      // Raster->pbpl = ((Raster->pwide + 31) / 32) * 4;
      Raster->pbpl = ((Raster->pwide + 15) / 16) * 2;
      Raster->cmap = 2;
      break;
    case 4:
      // Raster->pbpl = ((Raster->pwide + 7) / 8) * 4;
      Raster->pbpl = ((Raster->pwide + 3) / 4) * 2;
      Raster->cmap = 16;
      break;
    case 8:
      // Raster->pbpl = ((Raster->pwide + 3) / 4) * 4;
      Raster->pbpl = ((Raster->pwide + 1) / 2) * 2;
      Raster->cmap = 256;
      break;
    case 24:
      //Raster->pbpl = Raster->pwide * 4;
      Raster->pbpl = (((Raster->pwide * 3) + 3) / 4) * 4;
      Raster->cmap = 0;
      break;
    default:
      Raster->pbpl = 0;
      Raster->cmap = 0;
      break;
  }
	raslen -= 32;  // skip over header
	rasptr += 32;

	if ((Raster->pdeep == 4) || (Raster->pdeep == 8)) {
	  Raster->pcolor = new unc[Raster->cmap * 4];
		Graphic->gsize += Raster->cmap * 2;
    uns cmax = Raster->cmap;
    for (uns i = 0; i < cmax; i++) {
      Raster->pcolor[i * 4] = rasptr[(Raster->cmap * 2) + i];  // blue
      Raster->pcolor[(i * 4) + 1] = rasptr[Raster->cmap + i];    // green
      Raster->pcolor[(i * 4) + 2] = rasptr[i];                   // red
    }
  }
	else if (Raster->pdeep == 1) { // switch black and white
	  Raster->pcolor = new unc[8];
		Graphic->gsize += 4;
		((long *) Raster->pcolor)[0] = 0x00FFFFFF;
		((long *) Raster->pcolor)[1] = 0;
	}
	raslen -= Raster->cmap * 3;	 // skip over colormap
	rasptr += Raster->cmap * 3;

  Raster->comp = 0;  // begin with uncompressed form
  Raster->pbits = new DCblock(Raster->phigh * Raster->pbpl);
	ProcGrSunRaster(raslen, rasptr);
  if ((Raster->pdeep == 4) || (Raster->pdeep == 8))
		ProcGrBits();
	else if (Raster->pdeep == 1)
		RepackMonoRaster();
	else if (Raster->pdeep == 24)
		Repack24BitRaster();
  Graphic->gsize += Raster->pbits->l() / 2;
}


long
RTFwr::revL(long val)
{
	return ((val & 0xFF) << 24) +
         ((val & 0xFF00) << 8) +
         ((val >> 8) & 0xFF00) +
         ((val >> 24) & 0xFF);
}



// import object conversion


void
RTFwr::ConvertImportObjectFrame(frame* fr, frame* pfr, graph* gr)
{
	char* chp = GraphFileName(fr->ngrfile ? fr->ngrfile : fr->grfile);
	LogEvent(loginfo, 1, "Embedding graphic file \"", chp, "\" in WMF");

	// if format is BMP, convert to DIB object
	char* suf = strrchr(chp, '.');
	if (suf != NULL)
		suf++;

	Graphic = NULL;

	if (suf && !stricmp(suf, "bmp")) 	// open file and create Raster
		ConvertImportedBMP(fr, pfr, chp, gr);
	else if (suf && !stricmp(suf, "wmf"))	// repackage as Metafile
		ConvertImportedWMF(fr, chp, gr);

	if (Graphic) {
		if (!fr->grlist)
			fr->grlist = new DCvlist;
		fr->grcount++;
		(fr->grlist)->add(Graphic, fr->grcount);
	}

	// clean up
	delete [] chp;
}


void
RTFwr::ConvertImportedBMP(frame* fr, frame* pfr, char* chp, graph* gr)
{
	DCrfile* bmf = new DCrfile(fr->ngrfile ? fr->ngrfile : chp);
	char *slash = NULL;
	char *nname = NULL;

	if ((bmf->Ferr != fok)
	 && ((slash = strrchr(chp, '/')) == NULL)    // new name has no path
	 && ((slash = strrchr(fr->grfile, '/')) != NULL)) {  // but old one did
		nname = NewName(strlen(chp) + (slash - fr->grfile) + 1);
		strncpy(nname, fr->grfile, (slash - fr->grfile) + 1);
		strcat(nname, chp);
		bmf = new DCrfile(nname);
	}

	if (bmf->Ferr != fok) {
		bmf = new DCrfile(fr->grfile);
		if (bmf->Ferr != fok) {
			LogEvent(logerr, 1, "Cannot read \"", chp, "\" or \"", fr->grfile, "\"");
			return;  // bail out
		}
		else {
			LogEvent(loginfo, 1, "Reading graphic file \"", fr->grfile, "\" instead");
		}
	}

	if ((bmf->getChar() != 'B')
	 || (bmf->getChar() != 'M')) {
		bmf->CloseFile();
		LogEvent(logerr, 1, "Graphic file \"", chp, "\" is not BMP format");
		return;  // not a BMP, bail out
	}
	long flen = bmf->getILong();

	bmf->seek(10L);  // offset of image
	long fstart = bmf->getILong();

	// at start of header now
	long fhead = bmf->getILong();

	if ((fhead != 40L)     // BITMAPINFOHEADER
	 && (fhead != 12L)) {  // BITMAPCOREINFO (OS/2)
		bmf->CloseFile();
		LogEvent(logerr, 1, "Graphic file \"", chp, "\" has wrong header size");
		return;  // wrong header size
	}
	bool OS2 = (fhead == 12L) ? true : false;

	Graphic = new graph;
	Graphic->gtype = 0xF43;
	Graphic->gpen = gr->gpen;
	Graphic->gbrush = gr->gbrush;
	Raster = new rast;
	Graphic->gparm = (short *) Raster;
	Graphic->gsize = 34;

	Raster->pwide = OS2 ? (long) bmf->getIShort() : bmf->getILong();
	Raster->phigh =  OS2 ? (long) bmf->getIShort() :bmf->getILong();
	short planes = bmf->getIShort();
	assert(planes == 1);
	Raster->pdeep = bmf->getIShort();
	Raster->dpi = fr->dpi;

	if ((fr->eqtype != 2)
	 && (Raster->dpi != 0)	// rescale bitmap
	 && (fr->grcount == 0)
	 && (pfr->grcount == 1)) {
		pfr->high = fr->high = (Raster->phigh * 1440L) / Raster->dpi; 
		pfr->wide = fr->wide = (Raster->pwide * 1440L) / Raster->dpi;
		WMFyoff = fr->ypos = 0;  // reorg to top left
		WMFxoff = fr->xpos = 0;
	}

	Raster->ixpos = 0;
	Raster->iypos = 0;
	Raster->iwide = fr->wide;
	Raster->ihigh = fr->high;

	Raster->plcrop = 0;
	Raster->prcrop = 0;
	Raster->ptcrop = 0;
	Raster->pbcrop = 0;

	GrStretch = 3;

	switch (Raster->pdeep) {
		case 1:
			GrStretch = 1;
			Raster->pbpl = ((Raster->pwide + 15) / 16) * 2;
			Raster->cmap = 2;
			break;
		case 4:
			Raster->pbpl = ((Raster->pwide + 7) / 8) * 4;
			Raster->cmap = 16;
			break;
		case 8:
			Raster->pbpl = ((Raster->pwide + 3) / 4) * 4;
			Raster->cmap = 256;
			break;
		case 24:
	    //Raster->pbpl = Raster->pwide * 4;
		  Raster->pbpl = (((Raster->pwide * 3) + 3) / 4) * 4;
			Raster->cmap = 0;
			break;
		default:
			Raster->pbpl = 0;
			Raster->cmap = 0;
			break;
	}

	Raster->comp = OS2 ? 0 : (short) bmf->getILong();
	long ilen = OS2 ? 0 : bmf->getILong();
	if (Raster->comp == 0)
		ilen = 0;  // remove any trash values

	if (!OS2)
		bmf->seek(bmf->tell() + 8);	// skip resolutions

	long clen = OS2 ? 0 : bmf->getILong();
	if (clen == 0)
		clen = Raster->cmap;
	long cilen = OS2 ? 0 : bmf->getILong();
	assert(cilen <= clen);

	// start of color map
	if (Raster->cmap) {  
		Raster->pcolor = new unc[Raster->cmap * 4];
		Graphic->gsize += Raster->cmap * 2;
		if (OS2) {
			unc *tblock = new unc[clen * 3];
			bmf->getBlock(tblock, clen * 3);
			for (uns icol = 0; icol < clen; icol++) {
				Raster->pcolor[(icol * 4)] = tblock[(icol * 3)];
				Raster->pcolor[(icol * 4) + 1] = tblock[(icol * 3) + 1];
				Raster->pcolor[(icol * 4) + 2] = tblock[(icol * 3) + 2];
				Raster->pcolor[(icol * 4) + 3] = 0;
			}
			delete [] tblock;
		}
		else
			bmf->getBlock(Raster->pcolor, clen * 4);
	}
	assert(bmf->tell() == fstart);

	if (ilen == 0)
		ilen = flen - fstart;
	//else
	//	assert(ilen == flen - fstart);

	if (Raster->comp == 0) {  // uncompressed
		assert(Raster->pbpl * Raster->phigh <= ilen);
		Raster->pbpl = (short) (ilen / Raster->phigh);
		ilen = ((long) Raster->pbpl) * ((long) Raster->phigh);
	}

	Raster->pbits = new DCblock(ilen);
	*(Raster->pbits) = *bmf;
	bmf->CloseFile();

	// compress here if depth is 4 or 8
	if (CompRasters
	 && (!Raster->comp)
	 && ((Raster->pdeep == 4) || (Raster->pdeep == 8)))
		ProcGrBits();
	Graphic->gsize += Raster->pbits->l() / 2;
}


char RTFwr::ImpWMFType = 'C';


void
RTFwr::ConvertImportedWMF(frame* fr, char* chp, graph* gr)
{
	bool placed = false;
	DCrfile* bmf = new DCrfile(fr->ngrfile ? fr->ngrfile : chp);

	char *slash = NULL;
	char *nname = NULL;

	if ((bmf->Ferr != fok)
	 && ((slash = strrchr(chp, '/')) == NULL)    // new name has no path
	 && ((slash = strrchr(fr->grfile, '/')) != NULL)) {  // but old one did
		nname = NewName(strlen(chp) + (slash - fr->grfile) + 1);
		strncpy(nname, fr->grfile, (slash - fr->grfile) + 1);
		strcat(nname, chp);
		bmf = new DCrfile(nname);
	}

	if (bmf->Ferr != fok) {
		bmf = new DCrfile(fr->grfile);
		if (bmf->Ferr != fok) {
			LogEvent(logerr, 1, "Cannot read \"", chp, "\" or \"", fr->grfile, "\"");
			return;  // bail out
		}
		else {
			LogEvent(loginfo, 1, "Reading graphic file \"", fr->grfile, "\" instead");
		}
	}


	long fsize = bmf->getSize();
	long bsize = fsize;
	long id = bmf->getILong();
	const long PlaceHeadSize = 22;
	
	if (id == 0x9AC6CDD7) { // placeable metafile
		placed = true;
		bmf->seek(PlaceHeadSize); // skip header
		bsize -= PlaceHeadSize;   // reduce to base metafile size
		id = bmf->getILong();
	}

	short winver = bmf->getIShort();
	long hsize = bmf->getILong();

	if (((id != 0x90001)  // plain metafile
	  && (id != 0x90002)) // disk metafile
	 || (winver != 0x300)
	 || (hsize > (bsize / 2))) { // may be zero padded at end
		bmf->CloseFile();
		LogEvent(logerr, 1, "Graphic file \"", chp, "\" is not WMF format");
		return;  // not a WMF, bail out
	}

	Graphic = new graph;
	Metafile = new wmeta;
  Graphic->gtype = 0x1;
	Graphic->gpen = gr->gpen;
	Graphic->gbrush = gr->gbrush;
	Graphic->gparm = (short *) Metafile;
	Graphic->gsize = 1;
	Metafile->mtype = (ImpWMFType == 'F') ? 2 : 1; // imported
  Metafile->ixpos = fr->xpos;
	Metafile->iypos = fr->ypos;
	Metafile->iwide = fr->wide;
	Metafile->ihigh = fr->high;

	DCblock *bp = new DCblock(fsize);
	bmf->seek(0);
	*bp = *bmf;  // read entire file

	if (placed)
		Metafile->mrecs = bp;
	else {
		Metafile->mrecs = new DCblock(fsize + PlaceHeadSize);
		// no need to write actual header, won't be read anyway
		char *fcp = ((char *) (Metafile->mrecs->p())) + PlaceHeadSize;
		memmove(fcp, bp->p(), bp->l());
		delete bp;
	}
}



// raster writing


void
RTFwr::WriteGrRaster(RTFwfile *wf, rast *rs)
{
  wf->putML(0x00CC0020);
  wf->putMS(0);

  wf->putMS(rs->phigh - (rs->ptcrop + rs->pbcrop)); // source
  wf->putMS(rs->pwide - (rs->plcrop + rs->prcrop));
  wf->putMS(rs->ptcrop);
  wf->putMS(rs->plcrop);

  wf->putMS(rs->ihigh); // destination
  wf->putMS(rs->iwide);
  wf->putMS(rs->iypos + WMFyoff);
  wf->putMS(rs->ixpos + WMFxoff);
  wf->putMEnd();

  wf->putMChar(' ');
  wf->putML(40);  // BITMAPINFO header length
  wf->putML(rs->pwide);
  wf->putML(rs->phigh);
  wf->putMS(1);
  wf->putMS(rs->pdeep);
  wf->putML(rs->comp);
  wf->putML(rs->comp ? rs->pbits->l() : 0);
  wf->putMEnd();

  wf->putMChar(' ');
  wf->putML(0);
  wf->putML(0);
  wf->putML(0);
  wf->putML(0);

  // color map
  if (rs->pcolor) {
    uns cmax = rs->cmap * 4;
    for (uns i = 0; i < cmax; i++) {
      if ((i & 0x3F) == 0)
        wf->putMEnd();
      if ((i & 3) == 0)
        wf->putMChar(' ');
      wf->putMB(rs->pcolor[i]);
    }
    wf->putMEnd();
  }
  else if (rs->cmap == 2) {
    wf->putMEnd();
    wf->putMChar(' ');
    wf->putML(0);         // black
    wf->putML(0xFFFFFF);  // white
    wf->putMEnd();
  }

  size_t i;
  wf->putMChar(' ');
  if ((rs->comp == 0)  // uncompressed image, break lines at row ends
	 && (rs->pbits->l() == ((size_t) rs->phigh * (size_t) rs->pbpl))) {
    unc *rowp;
    for (uns rrow = 0; rrow < rs->phigh; rrow++) {
      rowp = &(((unc *) rs->pbits->p())[rrow * rs->pbpl]);
      for (i = 0; i < (size_t) rs->pbpl; i++)
        wf->putMB(rowp[i]);
      wf->putMEnd();
    }
  }
  else { // compressed, or other unknown line break
    unc *rptr = (unc *) rs->pbits->p();
    for (i = 0; i < rs->pbits->l(); i++)
      wf->putMB(rptr[i]);
  }
}



// end of dwrtfrf.cpp


