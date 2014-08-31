 
// dwhtmrf.cc is the code for dcl Microsoft HTM raster writing, JHG, 1/4/97
// Copyright (c) 1996 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dwhtm.h"
#include "dwhtmfl.h"


#ifdef UNUSED

// processing switch


bool
HTMwr::ProcRf(HTMctl *cp)
{
  switch((cp->rp)->scan_op) {
    case nothing:   // just put with its group for later
      return true;

    case pass:      // ignore, don't save
      break;

    case rasprop:
      ProcGrRaster(cp);
      break;
    case rasbmap:
      ProcGrBitmap(cp);
      break;
    case rascmap:
      ProcGrColormap(cp);
      break;

    default:
      break;
  }
  return false;  // default is to discard ctl after scan
}



// raster processing


rast *HTMwr::Raster;


void
HTMwr::ProcGrRaster(HTMctl *cp)
{
  if (Verbose)
    printf("raster %d ", cp->dat());

  Raster = new rast;
  Graphic->gtype = 0xF43;
  Graphic->gparm = (uns *) Raster;
  Graphic->gsize = 34;

  long *lptr = (long *) cp->ex();
  Raster->pdeep = cp->dat();
  Raster->ixpos = GetTwips(lptr[0], pg_unit);
  Raster->iypos = GetTwips(lptr[1], pg_unit);
  Raster->iwide = GetTwips(lptr[2], pg_unit);
  Raster->ihigh = GetTwips(lptr[3], pg_unit);
  Raster->pwide = lptr[4];
  Raster->phigh = lptr[5];

  switch (Raster->pdeep) {
    case 1:
      Raster->pbpl = ((Raster->pwide + 31) / 32) * 4;
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
HTMwr::ProcGrColormap(HTMctl *cp)
{
  uns maptype = cp->dat();

  Raster->pcolor = new unc[Raster->cmap * 4];
  Graphic->gsize += Raster->cmap * 2;

  if (maptype == 1) {  // SunRaster
    unc *bptr = (unc *) cp->ex();
    long msize = cp->siz() / 3;
    uns cmax = Raster->cmap * 4;
    for (uns i = 0; i < cmax; i += 4) {
      Raster->pcolor[i] = bptr[(msize * 2) + i];  // blue
      Raster->pcolor[i + 1] = bptr[msize + i];    // green
      Raster->pcolor[i + 2] = bptr[i];            // red
    }
  }
  else if (maptype == 2)  // MS DIB
     memcpy(Raster->pcolor, cp->ex(), cp->siz());
}


void
HTMwr::ProcGrBitmap(HTMctl *cp)
{
  Raster->comp = 0;  // begin with uncompressed form
  Raster->pbits = new DCblock(Raster->phigh * Raster->pbpl);
  bool compressed = false;

  switch (cp->dat()) {
    case 1: // uncompressed
      break;
    case 2: // pbm
      break;
    case 3: // pcx
      break;
    case 4: // rf
      ProcGrSunRaster(cp);
      break;
    case 5: // packbits
      break;
    case 7: // DIB, 4-bit compressed
    case 8: // DIB, 8-bit compressed
      compressed = true;
    case 6: // DIB uncompressed
      *(Raster->pbits) = *cp;
      break;
    default:
      break;
  }
  // compress here if depth is 4 or 8
  if (!compressed
   && ((Raster->pdeep == 4) || (Raster->pdeep == 8))) {
    uns ppb, bwide, lastp, difflim, samelim;
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
    DCblock *nb = new DCblock(Raster->pbits->l());
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
        if (*nptr == *sptr) {
          if (diff) {
            if ((diff * ppb) < 3) {
              diff++;
              sptr = nptr++;
              continue;
            }
            *dptr++ = 0;
            *dptr++ = diff * ppb;
            memcpy(dptr, fptr, diff);
            dptr += diff;
            fptr += diff;
            if (diff & 1)
              *dptr++ = 0;
            diff = 0;
          }
          if (++same == samelim) {
            *dptr++ = samelim * ppb;
            *dptr++ = *sptr;
            same = 0;
            sptr = nptr;
          }
        }
        else {
          if (same) {
            *dptr++ = (same + 1) * ppb;
            *dptr++ = *sptr;
            same = 0;
            fptr = nptr;
          }
          else if (++diff == difflim) {
            *dptr++ = 0;
            *dptr++ = difflim * ppb;
            memcpy(dptr, fptr, difflim);
            dptr += difflim;
            fptr = nptr;
            diff = 0;
          }
          sptr = nptr;
        }
        nptr++;
      }
      if (diff == 1) {  // at end of line
        *dptr++ = (unc) ppb;
        *dptr++ = *fptr++;
        *dptr++ = ppb - lastp;
        *dptr++ = *fptr;
      }
      else if (diff) {
        *dptr++ = 0;
        *dptr++ = (diff * ppb) - lastp;
        memcpy(dptr, fptr, diff);
        dptr += diff;
        fptr += diff;
        if (diff & 1)
          *dptr++ = 0;
      }
      else if (same) {
        *dptr++ = ((same + 1) * ppb) - lastp;
        *dptr++ = *sptr;
      }
      else {
        *dptr++ = ppb - lastp;
        *dptr++ = *fptr;
      }
      sptr = fptr = nptr + pad;
      nptr = sptr + 1;
      lptr += Raster->pbpl;
      *dptr++ = 0;
      *dptr++ = 0;
      same = diff = 0;
    }
    *dptr++ = 0;  // end of bitmap
    *dptr++ = 1;
    nb->set(dptr - (unc *) nb->p());
    delete Raster->pbits;
    Raster->pbits = nb;
  }
  Graphic->gsize += Raster->pbits->l() / 2;
}


void
HTMwr::ProcGrSunRaster(HTMctl *cp)
{
  long rsize = cp->siz();
  unc *bptr = (unc *) cp->ex();
  unc *eptr = bptr + rsize;
  uns lsize = 0;
  unc *pptr = (unc *) Raster->pbits->p();
  switch (Raster->pdeep) {
    case 1:
      lsize = ((Raster->pwide + 15) / 16) * 2;
      break;
    case 8:
      lsize = ((Raster->pwide + 1) / 2) * 2;
      break;
    default:
      break;
  }
  assert(lsize);
  uns pos = 0;
  uns rrow = 0;
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
    if (pos >= lsize) {
      pptr = &(((unc *) Raster->pbits->p())[++rrow * Raster->pbpl]);
      pos = 0;
    }
  }
}



// raster writing


void
HTMwr::WriteGrRaster(HTMwfile *wf, rast *rs)
{
  wf->putML(0x00CC0020);
  wf->putMS(0);

  wf->putMS(rs->phigh - (rs->ptcrop + rs->pbcrop)); // source
  wf->putMS(rs->pwide - (rs->plcrop + rs->prcrop));
  wf->putMS(rs->ptcrop);
  wf->putMS(rs->plcrop);

  wf->putMS(rs->ihigh); // destination
  wf->putMS(rs->iwide);
  wf->putMS(rs->iypos);
  wf->putMS(rs->ixpos);
  wf->putEnd();

  wf->putChar(' ');
  wf->putML(40);  // BITMAPINFO header length
  wf->putML(rs->pwide);
  wf->putML(rs->phigh);
  wf->putMS(1);
  wf->putMS(rs->pdeep);
  wf->putML(rs->comp);
  wf->putML(rs->pbits->l());
  wf->putEnd();

  wf->putChar(' ');
  wf->putML(0);
  wf->putML(0);
  wf->putML(0);
  wf->putML(0);

  // color map
  if (rs->pcolor) {
    uns cmax = rs->cmap * 4;
    for (uns i = 0; i < cmax; i++) {
      if ((i & 0x3F) == 0)
        wf->putEnd();
      if ((i & 3) == 0)
        wf->putChar(' ');
      wf->putMB(rs->pcolor[i]);
    }
    wf->putEnd();
  }
  else if (rs->cmap == 2) {
    wf->putEnd();
    wf->putChar(' ');
    wf->putML(0);         // black
    wf->putML(0xFFFFFF);  // white
    wf->putEnd();
  }

  size_t i;
  wf->putChar(' ');
  if (rs->comp == 0) {  // uncompressed image, break lines at row ends
    unc *rowp;
    for (uns rrow = 0; rrow < rs->phigh; rrow++) {
      rowp = &(((unc *) rs->pbits->p())[rrow * rs->pbpl]);
      for (i = 0; i < (size_t) rs->pbpl; i++)
        wf->putMB(rowp[i]);
      wf->putEnd();
    }
  }
  else { // compressed
    unc *rptr = (unc *) rs->pbits->p();
    for (i = 0; i < rs->pbits->l(); i++)
      wf->putMB(rptr[i]);
  }
}

#endif


// end of dwhtmrf.cc


