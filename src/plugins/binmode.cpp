#include "config.h"
#include "libbeye/libbeye.h"
using namespace beye;
/**
 * @namespace   beye_plugins_I
 * @file        plugins/binmode.c
 * @brief       This file contains implementation of binary mode viewer.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Nickols_K
 * @since       1995
 * @note        Development, fixes and improvements
**/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "colorset.h"
#include "bconsole.h"
#include "beyeutil.h"
#include "beyehelp.h"
#include "bin_util.h"
#include "bmfile.h"
#include "reg_form.h"
#include "editor.h"
#include "tstrings.h"
#include "libbeye/file_ini.h"
#include "libbeye/kbd_code.h"
#include "libbeye/libbeye.h"

namespace beye {
static unsigned virtWidthCorr=0;

enum {
    MOD_PLAIN    =0,
    MOD_BINARY   =1,
    MOD_REVERSE  =2,
    MOD_MAXMODE  =2
};

static const char * mod_names[] =
{
   "~Plain text",
   "~Video dump",
   "~Reversed video dump"
};
static unsigned bin_mode = MOD_PLAIN; /**< points to currently selected mode text mode */

static bool __FASTCALL__ binSelectMode( void )
{
  unsigned nModes;
  int i;
  nModes = sizeof(mod_names)/sizeof(char *);
  i = SelBoxA(const_cast<char**>(mod_names),nModes," Select binary mode: ",bin_mode);
  if(i != -1)
  {
    bin_mode = i;
    return true;
  }
  return false;
}

static unsigned __FASTCALL__ drawBinary( unsigned keycode,unsigned tshift )
{
 static __filesize_t bmocpos = 0L;
 __filesize_t _index;
 __filesize_t limit,flen,cfp;
 int len;
 unsigned BWidth,_b_width,count;
 size_t j;
 HLInfo hli;
 tvioBuff it;
 char buffer[__TVIO_MAXSCREENWIDTH*2];
 t_vchar chars[__TVIO_MAXSCREENWIDTH];
 t_vchar oem_pg[__TVIO_MAXSCREENWIDTH];
 ColorAttr attrs[__TVIO_MAXSCREENWIDTH];
 tAbsCoord width,height;

 it.chars=chars;
 it.oem_pg=oem_pg;
 it.attrs=attrs;
 cfp  = BMGetCurrFilePos();
 width = twGetClientWidth(MainWnd);
 BWidth = twGetClientWidth(MainWnd)-virtWidthCorr;
 height = twGetClientHeight(MainWnd);
 if(bin_mode==MOD_PLAIN) _b_width=1;
 else _b_width=2;
 if(cfp != bmocpos || keycode == KE_SUPERKEY || keycode == KE_JUSTFIND)
 {
   bmocpos = cfp;
   flen = BMGetFLength();
   limit = flen - BWidth;
   if(flen < (__filesize_t)BWidth) BWidth = (int)(limit = flen);
   twFreezeWin(MainWnd);
   for(j = 0;j < height;j++)
   {
     count=BWidth*_b_width;
     _index = cfp + j*count;
     len = _index < limit ? (int)count : _index < flen ? (int)(flen - _index) : 0;
     if(len) { lastbyte = _index + len; BMReadBufferEx((any_t*)buffer,len,_index,BM_SEEK_SET); }
     if(bin_mode!=MOD_PLAIN)
     {
	unsigned i,ii;
	for(i=ii=0;i<BWidth;i++)
	{
	    chars[i]=buffer[ii++];
	    attrs[i]=buffer[ii++];
	}
	memset(oem_pg,0,tvioWidth);
	if(bin_mode==MOD_REVERSE)
	{
	    t_vchar *t;
	    t=it.chars;
	    it.chars=it.attrs;
	    it.attrs=(ColorAttr*)t;
	}
	count=len/2;
	memset(&it.chars[count],TWC_DEF_FILLER,tvioWidth-count);
	memset(&it.attrs[count],browser_cset.main,tvioWidth-count);
     }
     else
       memset(&buffer[len],TWC_DEF_FILLER,tvioWidth-len);
     if(isHOnLine(_index,width))
     {
	hli.text = buffer;
	HiLightSearch(MainWnd,_index,0,BWidth,j,&hli,HLS_NORMAL);
     }
     else
     {
	if(bin_mode==MOD_PLAIN)
	    twDirectWrite(1,j + 1,buffer,width);
	else
	    twWriteBuffer(MainWnd,1,j + 1,&it,width);
     }
   }
   twRefreshWin(MainWnd);
 }
 return tshift;
}

static void __FASTCALL__ HelpBin( void )
{
   hlpDisplay(1000);
}

static unsigned long __FASTCALL__ binPrevPageSize( void ) { return (twGetClientWidth(MainWnd)-virtWidthCorr)*twGetClientHeight(MainWnd)*(bin_mode==MOD_PLAIN?1:2); }
static unsigned long __FASTCALL__ binCurrPageSize( void ) { return (twGetClientWidth(MainWnd)-virtWidthCorr)*twGetClientHeight(MainWnd)*(bin_mode==MOD_PLAIN?1:2); }
static unsigned long __FASTCALL__ binPrevLineWidth( void ) { return (twGetClientWidth(MainWnd)-virtWidthCorr)*(bin_mode==MOD_PLAIN?1:2); }
static unsigned long __FASTCALL__ binCurrLineWidth( void ) { return (twGetClientWidth(MainWnd)-virtWidthCorr)*(bin_mode==MOD_PLAIN?1:2); }
static const char *  __FASTCALL__ binMiscKeyName( void ) { return "Modify"; }

static bool __FASTCALL__ binDetect( void ) { return true; }

static void save_video(unsigned char *buff,unsigned size)
{
  BFile* bHandle;
  const char *fname;
  unsigned i;
  fname = BMName();
  bHandle = beyeOpenRW(fname,BBIO_SMALL_CACHE_SIZE);
  if(bHandle == &bNull)
  {
      err:
      errnoMessageBox(WRITE_FAIL,NULL,errno);
      return;
  }
  bHandle->seek(BMGetCurrFilePos(),BFile::Seek_Set);
  if(bin_mode==MOD_REVERSE) bHandle->seek(1,BFile::Seek_Cur);
  for(i=0;i<size;i++)
  {
    if(!bHandle->write_byte(buff[i])) goto err;
    bHandle->seek(1,BFile::Seek_Cur);
  }
  delete bHandle;
  BMReRead();
}

static void __FASTCALL__ EditBin( void )
{
 TWindow *ewin;
 bool inited;
 if(!BMGetFLength()) { ErrMessageBox(NOTHING_EDIT,NULL); return; }
 ewin = WindowOpen(1,2,tvioWidth-virtWidthCorr,tvioHeight-1,TWS_CURSORABLE);
 twSetColorAttr(browser_cset.edit.main); twClearWin();
 drawEditPrompt();
 twUseWin(ewin);
 edit_x = edit_y = 0;
 if(bin_mode==MOD_PLAIN)
    inited=editInitBuffs(tvioWidth-virtWidthCorr,NULL,0);
 else
 {
    unsigned long flen,cfp;
    unsigned i,size,msize = tvioWidth*tvioHeight;
    unsigned char *buff = new unsigned char [msize*2];
    if(buff)
    {
	flen = BMGetFLength();
	cfp = BMGetCurrFilePos();
	size = (unsigned)((unsigned long)msize > (flen-cfp) ? (flen-cfp) : msize);
	BMReadBufferEx(buff,size*2,cfp,BM_SEEK_SET);
	BMSeek(cfp,BM_SEEK_SET);
	for(i=0;i<size;i++) buff[i]=bin_mode==MOD_BINARY?buff[i*2]:buff[i*2+1];
	inited=editInitBuffs(tvioWidth-virtWidthCorr,buff,size);
	delete buff;
    }
    else
    {
	MemOutBox("Editor initialization");
	inited=false;
    }
 }
 if(inited)
 {
   FullEdit(NULL,bin_mode==MOD_PLAIN?NULL:save_video);
   editDestroyBuffs();
 }
 CloseWnd(ewin);
 PaintTitle();
}

static void __FASTCALL__ binReadIni( hIniProfile *ini )
{
  char tmps[10];
  if(isValidIniArgs())
  {
    beyeReadProfileString(ini,"Beye","Browser","LastSubMode","0",tmps,sizeof(tmps));
    bin_mode = (unsigned)strtoul(tmps,NULL,10);
    if(bin_mode > MOD_MAXMODE) bin_mode = MOD_MAXMODE;
    beyeReadProfileString(ini,"Beye","Browser","VirtWidthCorr","0",tmps,sizeof(tmps));
    virtWidthCorr = (unsigned)strtoul(tmps,NULL,10);
    if(virtWidthCorr>tvioWidth-1) virtWidthCorr=tvioWidth-1;
  }
}

static void __FASTCALL__ binSaveIni( hIniProfile *ini )
{
  char tmps[10];
  /** Nullify LastSubMode */
  sprintf(tmps,"%i",bin_mode);
  beyeWriteProfileString(ini,"Beye","Browser","LastSubMode",tmps);
  sprintf(tmps,"%u",virtWidthCorr);
  beyeWriteProfileString(ini,"Beye","Browser","VirtWidthCorr",tmps);
}

static unsigned __FASTCALL__ binCharSize( void ) { return bin_mode==MOD_PLAIN?1:2; }

static bool __FASTCALL__ binIncVirtWidth( void )
{
  if(virtWidthCorr) { virtWidthCorr--; return true; }
  return false;
}

static bool __FASTCALL__ binDecVirtWidth( void )
{
  if(virtWidthCorr < tvioWidth-1) { virtWidthCorr++; return true; }
  return false;
}

REGISTRY_MODE binMode =
{
  "~Binary mode",
  { NULL, "BinMod", NULL, NULL, NULL, NULL, "<<<   ", "   >>>", NULL, "UsrNam" },
  { NULL, binSelectMode, NULL, NULL, NULL, NULL, binDecVirtWidth, binIncVirtWidth, NULL, udnUserNames },
  binDetect,
  __MF_NONE,
  drawBinary,
  NULL,
  binCharSize,
  binMiscKeyName,
  EditBin,
  binPrevPageSize,
  binCurrPageSize,
  binPrevLineWidth,
  binCurrLineWidth,
  HelpBin,
  binReadIni,
  binSaveIni,
  NULL,
  NULL,
  NULL
};
} // namespace beye