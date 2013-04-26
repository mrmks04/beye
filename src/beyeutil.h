/**
 * @namespace   beye
 * @file        beyeutil.h
 * @brief       This file contains prototypes of BEYE utilities.
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
#ifndef __BEYEUTIL__H
#define __BEYEUTIL__H

#include <limits>

#ifndef __TWIN_H
#include "libbeye/twin.h"
#endif

#ifndef __BBIO_H
#include "libbeye/bbio.h"
#endif

#ifndef __FILE_INI_RUNTIME_SUPPORT_SYSTEM__
#include "libbeye/file_ini.h"
#endif

namespace beye {
    extern char legalchars[];
    extern bool DumpMode;
    extern bool EditMode;

    extern __filesize_t lastbyte;
    extern char * ini_name;

    int   __FASTCALL__ Gebool(bool _bool);
    any_t**            FAllocPtrPtr(unsigned num);
    any_t*             FAllocPtr(unsigned size);
    void               FFreeArr(any_t**arr,unsigned n);
    void               CriticalExit(int code);

    void               init_addons(void);
    void               term_addons(void);
    void               SelectTool( void );
    void               init_sysinfo( void );
    void               term_sysinfo( void );
    void               SelectSysInfo( void );

    bool               FileUtils( void );

    char * __FASTCALL__ Get2Digit(uint8_t);
    char * __FASTCALL__ Get2SignDig(int8_t);
    char * __FASTCALL__ Get4Digit(uint16_t);
    char * __FASTCALL__ Get4SignDig(int16_t);
    char * __FASTCALL__ Get8Digit(uint32_t);
    char * __FASTCALL__ Get8SignDig(int32_t);
#if __WORDSIZE >= 32
    char * __FASTCALL__ Get16Digit(uint64_t);
    char * __FASTCALL__ Get16SignDig(int64_t);
#else
    char * __FASTCALL__ Get16Digit(uint32_t low,uint32_t high);
    char * __FASTCALL__ Get16SignDig(int32_t low,int32_t high);
#endif
    char * __FASTCALL__ GetBinary(char val);

    void     __FASTCALL__ CompressHex(unsigned char * dest,const char * src,unsigned sizedest,bool usespace);
    unsigned __FASTCALL__ Summ(unsigned char *array,unsigned size);

    void   ExtHelp(void);
    void   drawEditPrompt( void );
    void   drawEmptyPrompt( void );
    void   drawEmptyListPrompt( void );
    void   drawAsmEdPrompt( void );
    int    EditAsmActionFromMenu( void );
    void   drawListPrompt( void );
    void   drawOrdListPrompt( void );
    void   drawSearchListPrompt( void );
    void   drawHelpPrompt( void );
    int    HelpActionFromMenu( void );
    void   drawHelpListPrompt( void );
    void   drawPrompt( void );
    int    MainActionFromMenu(void);
    void   About( void );

    __filesize_t __FASTCALL__ WhereAMI(__filesize_t ctrl_pos);
#define RAPREF_NONE    0  /**< means reference is not appended */
#define RAPREF_DONE    std::numeric_limits<int32_t>::max() /**< means reference is appended */
		   /** Appends disassembler reference to string.
		     * @param str          string buffer for append to
		     * @param ulShift      physical address of field, that required of binding
		     * @param mode         see reg_form.h for detail
		     * @param codelen      length of field, that required binding
		     * @param r_shift      used only if APPREF_TRY_LABEL mode is set, contains real value of field, that required binding
		     * @return             one of RAPREF_* constants or physical
					   offset of target which is applied to
					   fixing field.
		    **/
    unsigned long __FASTCALL__ AppendAsmRef(char *str,__filesize_t ulShift,
					       int mode,char codelen,
					       __filesize_t r_shift);

    void  ShowSysInfo( void );
    void  MainLoop( void );

    int  __FASTCALL__ isHOnLine(__filesize_t cp,int width);
    enum {
	HLS_NORMAL               =0x0000,
	HLS_USE_DOUBLE_WIDTH     =0x0001,
	HLS_USE_BUFFER_AS_VIDEO  =0x0002
    };

typedef union tag_HLInfo
{
  const char     *text;
  tvioBuff        buff;
}HLInfo;

    void __FASTCALL__ HiLightSearch(TWindow *out,__filesize_t cfp,tRelCoord minx,
			  tRelCoord maxx,tRelCoord y,HLInfo *buff,unsigned flags);

/** Class memory array */

typedef struct tag_memArray
{
  any_t**  data;
  unsigned nItems;
  unsigned nSize;
}memArray;

    memArray *__FASTCALL__ ma_Build( int maxitems, bool interact );
    bool     __FASTCALL__ ma_AddString(memArray *obj,const char *data,bool interact);
    bool     __FASTCALL__ ma_AddData(memArray *obj,const any_t*data,unsigned size,bool interact);
    void      __FASTCALL__ ma_Destroy(memArray *obj);
    int       __FASTCALL__ ma_Display(memArray *obj,const char *title,int flg,unsigned defsel);

    extern __filesize_t headshift;

    int      __FASTCALL__ ExpandHex(char * dest,const unsigned char * src,int size,char hard);

    void		PaintTitle( void );
    bool               NewSource( void );
    __filesize_t       IsNewExe(void);
			  /** return true if LastOpenFile == Current open file */
    bool              isValidIniArgs( void );

    unsigned __FASTCALL__ beyeReadProfileString(hIniProfile *ini,
				      const char *section,
				      const char *subsection,
				      const char *_item,
				      const char *def_value,
				      char *buffer,
				      unsigned cbBuffer);

    bool __FASTCALL__ beyeWriteProfileString(hIniProfile *ini,
						 const char *section,
						 const char *subsection,
						 const char *item,
						 const char *value);
} // namespace beye
#endif
