/**
 * @namespace	usr
 * @file        tstrings.h
 * @brief       This file contains start of work for NLS support by BEYE.
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
#ifndef __TSTRINGS__H
#define __TSTRINGS__H

#include "version.h"

namespace	usr {
/** @def BEYE_VERSION
    Ver.SubVer.ExtraVer-ReleaseLevel.Test_FixPak_Level
  */
extern const char* BEYE_VERSION;

extern const char   msgUndef[];
extern const char   msgFatalError[];
extern const char * msgCritExitCause[];
extern const char * msgCritExit[];
extern const char * msgAsmErrList[];
extern const char * msgTypeComments[];
extern const char * msgTypeBitness[];
extern const char * msgComputerErrors[];
extern const char * msgHardPlace[];
extern const char   msgAboutText[];
extern const char * BeyeLogo[];
extern const char * CompPicture[];
extern const char * MBoardPicture[];
extern const char * BitStreamPicture[];
extern const char * ConnectorPicture[];
extern const char * BeyePicture[];
extern const char * BeyeScreenPicture[];
extern const char * CompScreenPicture[];
extern const char * msgWinCompat[];
extern const char * msgHeadDesc[];
extern const char * msgFindOpt[];
extern const char * msgFindOpt2[];

extern const char* BEYE_VER_MSG;

extern const char UNDEFINE[];
extern const char FATAL_ERROR[];

extern const char ISR_JUMP[];
extern const char INT_NUMBER[];
extern const char TYPE_SHIFT[];
extern const char DIG_EVALUTOR[];
extern const char DIG_OPERATORS[];
extern const char EXPRESSION[];
extern const char RESULT[];
extern const char TYPE_HEX_FORM[];
extern const char FILE_PRMT[];
extern const char XLAT_PRMT[];
extern const char START_PRMT[];
extern const char LENGTH_PRMT[];
extern const char INIT_MASK[];
extern const char INPUT_MASK[];
extern const char ERROR_MSG[];
extern const char WARN_MSG[];
extern const char NOTE_MSG[];

extern const char HOW_SEE[];

extern const char NOT_ENTRY[];
extern const char BAD_ENTRY[];
extern const char NO_ENTRY[];
extern const char UNK_SIGNATURE[];
extern const char UNK_HEADER[];
extern const char MOD_REFER[];
extern const char EXT_REFER[];
extern const char EXP_TABLE[];
extern const char RES_NAMES[];
extern const char NORES_NAMES[];
extern const char IMPPROC_TABLE[];
extern const char CORRUPT_BIN_MSG[];
extern const char BUILD_REFS[];
extern const char SYSTEM_BUSY[];

extern const char BACKWARD[];
extern const char FORWARD[];
extern const char TYPE_STR[];
extern const char FIND_STR[];
extern const char PLEASE_WAIT[];
extern const char SEARCHING[];
extern const char STR_NOT_FOUND[];
extern const char SEARCH_MSG[];
extern const char SYS_INFO[];
extern const char CPU_INFO[];

extern const char PAGEBOX_SUB[];
extern const char SPAGEBOX_SUB[];

extern const char NAME_OF_EXP_FILE[];
extern const char NAME_MSG[];
extern const char ACCESS_DENIED[];

extern const char READ_FAIL[];
extern const char WRITE_FAIL[];
extern const char OPEN_FAIL[];
extern const char DUP_FAIL[];
extern const char RESIZE_FAIL[];
extern const char EXPAND_FAIL[];
extern const char TRUNC_FAIL[];

extern const char NOTHING_EDIT[];
} // namespace	usr
#endif
