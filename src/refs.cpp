#include "config.h"
#include "libbeye/libbeye.h"
using namespace beye;
/**
 * @namespace   beye
 * @file        refs.c
 * @brief       This file contains basic level routines for resolving references.
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "beye.h"
#include "reg_form.h"
#include "beyeutil.h"
#include "bconsole.h"

namespace beye {
extern REGISTRY_BIN binTable;
unsigned long __FASTCALL__ AppendAsmRef(char *str,__filesize_t ulShift,int mode,char codelen,__filesize_t r_sh)
{
  static bool warn_displayed = false;
  unsigned long ret = RAPREF_NONE;
  if(beye_context().active_format()->bind) ret = beye_context().active_format()->bind(str,ulShift,mode,codelen,r_sh);
  else
  {
    if(beye_context().active_format() != &binTable && !warn_displayed)
    {
      WarnMessageBox("Sorry! Reference resolving for this format is still not supported",NULL);
      warn_displayed = true;
    }
  }
  return ret;
}
} // namespace beye
