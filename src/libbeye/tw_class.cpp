#include "config.h"
#include "libbeye/libbeye.h"
using namespace beye;
/**
 * @namespace   libbeye
 * @file        libbeye/tw_class.c
 * @brief       This file contains implementation of classes for Text Window manager.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Nickols_K
 * @since       2000
 * @note        Development, fixes and improvements
**/
#include <string.h>
#include <stdlib.h>
#include "libbeye/twin.h"

static linearArray *class_set = NULL;

static tCompare __FASTCALL__ comp_class(const void __HUGE__ *e1,const void __HUGE__ *e2)
{
  const TwClass __HUGE__ *t1, __HUGE__ *t2;
  t1 = (const TwClass __HUGE__ *)e1;
  t2 = (const TwClass __HUGE__ *)e2;
  return stricmp(t1->name, t2->name);
}

bool __FASTCALL__ twcRegisterClass(const char *name, unsigned flags, twClassFunc method)
{
  TwClass newest;
  TwClass *exists = twcFindClass(name);
  if(!exists)
  {
     if(!class_set) class_set = la_Build(0,sizeof(TwClass),NULL);
     if(class_set)
     {
	newest.name = new char [strlen(name)+1];
	if(newest.name)
	{
	  strcpy(newest.name,name);
	  newest.flags = flags;
	  newest.method= method;
	  if(!la_AddData(class_set,&newest, NULL))
	  {
	    delete newest.name;
	    return false;
	  }
	  la_Sort(class_set, comp_class);
	  return true;
	}
     }
  }
  return false;
}

static void __FASTCALL__ del_class(void __HUGE__ *it)
{
  const TwClass __HUGE__ *t1;
  t1 = (const TwClass __HUGE__ *)it;
  delete t1->name;
}

void __FASTCALL__ twcDestroyClassSet(void)
{
  if(class_set) la_IterDestroy(class_set, del_class);
}

TwClass * __FASTCALL__ twcFindClass(const char *name)
{
 TwClass key;
 key.name = const_cast<char*>(name);
 return (TwClass *)la_Find(class_set,&key, comp_class);
}