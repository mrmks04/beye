#include "config.h"
#include "libbeye/libbeye.h"
using namespace beye;
/**
 * @namespace   beye
 * @file        tstrings.c
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
#include "tstrings.h"
/**   English release for all messages */
namespace beye {
const char* BEYE_VERSION=VERSION;
#if defined(__WIN32__) && defined(_MSC_VER)
const char* BEYE_VER_MSG=" Binary EYE v "VERSION"-i386.Win32 Build: " __DATE__ " ";
#else
const char* BEYE_VER_MSG=" Binary EYE v "VERSION"-"HOST" Build: "__DATE__" ";
#endif

const char msgAboutText[] =
"Multiplatform portable Binary EYE (aka viewer)\n"
"with built-in x86/i386/Amd64/AVX2/FMA/XOP,Java\n"
"AVR,ARM/XScale,PPC-64 disassemblers and editor\n"
"for bin-hex-disas modes. Highlights opcodes\n"
"and supports multiple executable structures.\n";
const char * BeyeLogo[] = {
"011100111101100011011110",
"010010100100100100010010",
"011100110000011000011000",
"010010100100010000010010",
"011100111100110000011110"
};

const char * CompPicture[] = {
" ���������������",
" �             �",
" �             �",
" �             �",
" �             �",
" ���������������",
"�����������������"
};

const char *MBoardPicture[] =
{
"��Ŀ������������",
"�             ��",
"��           ���",
"��           ���",
"��           ���",
"�             ��",
"���ٳ���������Ŀ"
};

const char *ConnectorPicture[] =
{
"���Ŀ",
"�   �",
"�   �",
"�   �",
"�   �",
"�   �",
"�����"
};

const char *BitStreamPicture[] =
{
"01010101",
"10011100",
"00001110",
"01010000",
"11001111",
"11001100",
"10010000"
};

const char *BeyePicture[] =
{
"���������������",
"�             �",
"�             �",
"�             �",
"�             �",
"�             �",
"���������������"
};

const char *BeyeScreenPicture[] =
{
"pushf        ",
"push    cs   ",
"push    ax   ",
"iret         ",
"int3         ",
};

const char * CompScreenPicture[] = {
"0001001001100",
"1001000101011",
"0110001110000",
"1010010011111",
};

const char * msgTypeComments[] =
{
 " Mode :",
 " [ ] - Put structures    ",
 "  Type of comments:      ",
 " ( ) - None              ",
 " ( ) - NASM (*.asm)      "
};

const char *msgTypeBitness[] =
{
 " Select bitness:         ",
 " ( )  8-bit (nothing)    ",
 " ( ) 16-bit (word swap)  ",
 " ( ) 32-bit (dword swap) ",
 " ( ) 64-bit (qword swap) "
};

const char * msgFindOpt[] =
{
 " [ ] - Match Case                 ",
 " [ ] - Match Whole Word Only      ",
 " [ ] - Reverse search             "
};

const char * msgFindOpt2[] =
{
 " [ ] - Search for hex             ",
 " [ ] - Use wildcards ( *? )       ",
 " [ ] - Use plugin's output        "
};

const char UNDEFINE[]=         "Undefined";
const char FATAL_ERROR[]=      " *** FATAL ERROR *** : ";

const char ISR_JUMP[]=         " Jump to ISR ";
const char INT_NUMBER[]=       " Interrupt number : ";
const char TYPE_SHIFT[]=       "Enter new offsets : ";
const char DIG_EVALUTOR[]=     " Digital evaluator (hexadecimal) ";
const char DIG_OPERATORS[]=    " Known Operators - ()+-*/% ~|&^ ";
const char EXPRESSION[]=       "Expression:";
const char RESULT[]=           "Result:";
const char TYPE_HEX_FORM[]=    "Enter parameters in hexadecimal form:";
const char FILE_PRMT[]=        "File :";
const char XLAT_PRMT[]=        "Xlat (eXtend Looking At Table) file:";
const char START_PRMT[]=       "Start:";
const char LENGTH_PRMT[]=      "Length:";
const char INIT_MASK[]=        " Initialize mask ";
const char INPUT_MASK[]=       "Enter new value of XX:";
const char ERROR_MSG[]=        " Error ";
const char WARN_MSG[]=         " Warning ";
const char NOTE_MSG[]=         " Note ";

const char HOW_SEE[]=          " How to look ";

const char NOT_ENTRY[]=        "Entry not found";
const char BAD_ENTRY[]=        " Bad entry ";
const char NO_ENTRY[]=         "Entry does not refer to physical page of this file";
const char UNK_SIGNATURE[]=    "Unknown type of signature : ";
const char UNK_HEADER[]=       " Unknown header ";
const char MOD_REFER[]=        " Detected Module References : ";
const char EXT_REFER[]=        " External References : ";
const char EXP_TABLE[]=        " Export Table : ";
const char RES_NAMES[]=        " Resident Names : ";
const char NORES_NAMES[]=      " Non-Resident Names : ";
const char IMPPROC_TABLE[]=    " Import Procedures Table : ";
const char CORRUPT_BIN_MSG[]=  "??? *** Binary format is corrupt or internal error *** ???";
const char BUILD_REFS[]=       "Building reference chains";
const char SYSTEM_BUSY[]=      " System is busy ";

const char BACKWARD[]=         "Backward";
const char FORWARD[]=          "Forward ";
const char FIND_STR[]=         " Find string ";
const char TYPE_STR[]=         "Enter string to search (for control letters hold ALT down + NumPads):";
const char PLEASE_WAIT[]=      "Please wait ... ";
const char SEARCHING[]=        " Searching ";
const char STR_NOT_FOUND[]=    "String not found";
const char SEARCH_MSG[]=       " Search ";
const char SYS_INFO[]=         " System information";
const char CPU_INFO[]=         " CPU information ";

const char PAGEBOX_SUB[]=      " [PgUp]/[PgDn] - Move  [ENTER] - Go Entry ";
const char SPAGEBOX_SUB[]=     " [PgUp]/[PgDn] - Move ";

const char NAME_OF_EXP_FILE[]= " Name of export file ";
const char NAME_MSG[]=         " Name : ";
const char ACCESS_DENIED[]=    " Access denied ";

const char READ_FAIL[]=        " Can't read from file ";
const char WRITE_FAIL[]=       " Can't write into file ";
const char OPEN_FAIL[]=        " Can't open file ";
const char DUP_FAIL[]=         " Can't dup file ";
const char RESIZE_FAIL[]=      " Can't change size of file ";
const char EXPAND_FAIL[]=      " Can't expand file ";
const char TRUNC_FAIL[]=       " Can't truncate file ";
const char NOTHING_EDIT[]=     " Can't edit zero file ";
} // namespace beye
