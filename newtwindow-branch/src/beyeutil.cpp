#include "config.h"
#include "libbeye/libbeye.h"
using namespace	usr;
/**
 * @namespace	usr
 * @file        beyeutil.c
 * @brief       This file contains useful primitives of BEYE project.
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
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdarg.h>

#include "beyeutil.h"
#include "bconsole.h"
#include "tstrings.h"
#include "libbeye/libbeye.h"

namespace	usr {
bool DumpMode = false;
bool EditMode = false;

char __FASTCALL__ Gebool(bool _bool) { return _bool ? TWC_CHECK_CHAR : TWC_DEF_FILLER; }

unsigned __FASTCALL__ Summ(unsigned char *str,unsigned size)
{
  unsigned res,i;
  res = 0;
  for(i = 0;i < size;i++) res += str[i];
  return res;
}

std::string __FASTCALL__ GetBinary(char val)
{
    char bstr[9];
    bstr[8] = 0;
    for(size_t i = 0;i < 8;i++) bstr[7-i] = ((val >> i) & 1) + '0';
    return bstr;
}

inline void __PURE_FUNC__ GET2DIGIT(char* s,const char* legs,unsigned char val)
{
    s[0] = legs[(((unsigned char)val) >> 4) & 0x0F];
    s[1] = legs[((unsigned char)val) & 0x0F];
}

std::string __FASTCALL__ Get2Digit(uint8_t val)
{
  char str[3] = "  ";
  const char *legs = &legalchars[2];
  GET2DIGIT(str,legs,val);
  return str;
}

std::string __FASTCALL__ Get2SignDig(int8_t val)
{
  char str[4] = "   ";
  const char *legs = &legalchars[2];
  str[0] = val >= 0 ? '+' : '-';
  if(val < 0) val = abs(val);
  GET2DIGIT(&str[1],legs,val);
  return str;
}

std::string __FASTCALL__ Get4Digit(uint16_t val)
{
  char rstr[5] = "    ";
  const char *legs = &legalchars[2];
  unsigned char v;
  v = val>>8;
  GET2DIGIT(rstr,legs,v);
  GET2DIGIT(&rstr[2],legs,val);
  return rstr;
}

std::string __FASTCALL__ Get4SignDig(int16_t val)
{
  char rstr[6] = "     ";
  const char *legs = &legalchars[2];
  unsigned char v;
  rstr[0] = val >= 0 ? '+' : '-';
  if(val < 0) val = abs(val);
  v = val>>8;
  GET2DIGIT(&rstr[1],legs,v);
  GET2DIGIT(&rstr[3],legs,val);
  return rstr;
}

std::string __FASTCALL__ Get8Digit(uint32_t val)
{
  char rstr[9] = "        ";
  const char *legs = &legalchars[2];
  unsigned char v;
  v = val>>24;
  GET2DIGIT(rstr,legs,v);
  v = val>>16;
  GET2DIGIT(&rstr[2],legs,v);
  v = val>>8;
  GET2DIGIT(&rstr[4],legs,v);
  GET2DIGIT(&rstr[6],legs,val);
  return rstr;
}

std::string __FASTCALL__ Get8SignDig(int32_t val)
{
  char rstr[10] = "         ";
  const char *legs = &legalchars[2];
  unsigned char v;
  rstr[0] = val >= 0 ? '+' : '-';
  if(val < 0) val = labs(val);
  v = val>>24;
  GET2DIGIT(&rstr[1],legs,v);
  v = val>>16;
  GET2DIGIT(&rstr[3],legs,v);
  v = val>>8;
  GET2DIGIT(&rstr[5],legs,v);
  GET2DIGIT(&rstr[7],legs,val);
  return rstr;
}

std::string __FASTCALL__ Get16Digit(uint64_t val)
{
  static char rstr[17] = "                ";
  const char *legs = &legalchars[2];
  unsigned char v;
  v = val>>56;
  GET2DIGIT(rstr,legs,v);
  v = val>>48;
  GET2DIGIT(&rstr[2],legs,v);
  v = val>>40;
  GET2DIGIT(&rstr[4],legs,v);
  v = val>>32;
  GET2DIGIT(&rstr[6],legs,v);
  v = val>>24;
  GET2DIGIT(&rstr[8],legs,v);
  v = val>>16;
  GET2DIGIT(&rstr[10],legs,v);
  v = val>>8;
  GET2DIGIT(&rstr[12],legs,v);
  GET2DIGIT(&rstr[14],legs,val);
  return rstr;
}

std::string __FASTCALL__ Get16SignDig(int64_t val)
{
  static char rstr[18] = "                 ";
  const char *legs = &legalchars[2];
  unsigned char v;
  rstr[0] = val >= 0 ? '+' : '-';
  if(val < 0) val = -val;
  v = val>>56;
  GET2DIGIT(&rstr[1],legs,v);
  v = val>>48;
  GET2DIGIT(&rstr[3],legs,v);
  v = val>>40;
  GET2DIGIT(&rstr[5],legs,v);
  v = val>>32;
  GET2DIGIT(&rstr[7],legs,v);
  v = val>>24;
  GET2DIGIT(&rstr[9],legs,v);
  v = val>>16;
  GET2DIGIT(&rstr[11],legs,v);
  v = val>>8;
  GET2DIGIT(&rstr[13],legs,v);
  GET2DIGIT(&rstr[15],legs,val);
  return rstr;
}

static char  __FASTCALL__ __PURE_FUNC__ GetHexAnalog(char val)
{
  return val >= '0' && val <= '9' ? val-'0' : ((toupper(val)-'A'+10)) & 0x0F;
}

void __FASTCALL__ CompressHex(unsigned char * dest,const char * src,unsigned sizedest,bool usespace)
{
  unsigned i,j;
  for(i = j = 0;j < sizedest;j++)
  {
      dest[j] = (GetHexAnalog(src[i]) << 4) | GetHexAnalog(src[i + 1]);
      i += 2;	/* [dBorca] avoid ambiguous side-effects */
      if(usespace) i++;
  }
}

int __FASTCALL__ ExpandHex(char * dest,const unsigned char * src,int size,char hard)
{
    int i,k;
    dest[0] = '\0';
    k = 0;
    for(i = 0;i < size;i++)
    {
	const char * cptr;
	cptr = Get2Digit(src[i]).c_str();
	strcat(dest,cptr); k += 2;
	if(hard == 1) { strcat(dest,!((i + 1)%4) ? ( !((i+1)%16) ? " " : "-" ) : " "); k++; }
	else
	  if(hard == 2) { strcat(dest,!((i + 1)%4) ? "  " : " ");  k += !((i + 1)%4) ? 2 : 1; }
	  else { strcat(dest," "); k++; }
    }
  return k;
}
} // namespace	usr
