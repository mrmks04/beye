#include "config.h"
#include "libbeye/libbeye.h"
using namespace	usr;
/**
 * @namespace   libbeye
 * @file        libbeye/osdep/qnx/os_dep.c
 * @brief       This file contains implementation of OS depended part for QNX4.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Andrew Golovnia
 * @since       2001
 * @note        Development, fixes and improvements
 *
 * @author      Mauro Giachero
 * @since       11.2007
 * @note        Added __get_home_dir() and some optimizations
**/
#include <stdexcept>
#include <iostream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/sched.h>
#include <sys/qnxterm.h>
#include <sys/kernel.h>

#define LIBDIR "/usr/lib"

static char _ini_name[FILENAME_MAX+1];
static char _rc_dir_name[FILENAME_MAX+1];
static char _home_dir_name[FILENAME_MAX + 1];
static bool break_status;
static pid_t proxy=0;
static timer_t t=0;
static struct sigevent evp;
static struct itimerspec it;

/*
The home directory is a good place for configuration
and temporary files.
At least (strlen(progname) + 9) characters should be
available before the buffer end.
The trailing '/' is included in the returned string.
*/
char* __FASTCALL__ __get_home_dir(const char *progname)
{
	char *p;

	if (_home_dir_name[0]) return _home_dir_name; //Already computed

	if((p=getenv("HOME"))==NULL||strlen(p)>FILENAME_MAX-10)
		strcpy(_home_dir_name,"/tmp");
	else
		strcpy(_home_dir_name,p);

	strcat(_home_dir_name,"/");

	return p;
}

char* __FASTCALL__ __get_ini_name(const char *progname)
{
	char *p;

	if (_ini_name[0]) return _ini_name; //Already computed

	p=__get_home_dir(progname);
	strcpy(_ini_name,progname);
	strcat(_ini_name,".");
	strcat(_ini_name,progname);
	strcat(_ini_name,"rc");

	return _ini_name;
}

char* __FASTCALL__ __get_rc_dir(const char *progname)
{
	if (_rc_dir_name[0]) return _rc_dir_name; //Already computed

	strcpy(_rc_dir_name,LIBDIR"/");
	strcat(_rc_dir_name,progname);
	strcat(_rc_dir_name,"/");
	return _rc_dir_name;
}

void __FASTCALL__ __OsYield()
{
	it.it_value.tv_sec=0;
	it.it_value.tv_nsec=100000;
	it.it_interval.tv_sec=0;
	it.it_interval.tv_nsec=0;
	timer_settime(t,TIMER_ADDREL,&it,NULL);
	term_flush();
	Receive(proxy,NULL,0);
}

bool __FASTCALL__ __OsGetCBreak()
{
	return break_status;
}

void __FASTCALL__ __OsSetCBreak(bool state)
{
	break_status=state;
}

static void cleanup(int sig)
{
    __term_sys();
    std::ostringstream os;
    os<<sig;
    throw std::runtime_error(std::string("Terminated by signal")+os.str();
}

/* static struct sigaction sa; */

void __FASTCALL__ __init_sys()
{
	int i=0;
	struct sched_param sp;
	if(term_load()<0) throw std::runtime_error("Can't init terminal");

	umask(0077);
	signal(SIGTERM,cleanup);
	signal(SIGINT,cleanup);
	signal(SIGQUIT,cleanup);
	signal(SIGILL,cleanup);
	sp.sched_priority=PRIO_USER_DFLT;
	sched_setscheduler(0,SCHED_OTHER,&sp);
	if(proxy==0) proxy=qnx_proxy_attach(0,NULL,0,-1);
	if(proxy==-1) throw std::runtime_error("Can't attach proxy");
	evp.sigev_signo=-(proxy);
	if(t==0) t=timer_create(CLOCK_REALTIME,&evp);
	if(t==-1) throw std::runtime_error("Can't create timer");
	_ini_name[0] = '\0';
	_rc_dir_name[0] = '\0';
	_home_dir_name[0] = '\0';
}

void __FASTCALL__ __term_sys()
{
	if(t!=-1) timer_delete(t);
	qnx_proxy_detach(proxy);
}

static timer_callback *user_func = NULL;
static struct itimerval otimer;
static void (*old_alrm)(int) = SIG_DFL;

static void my_alarm_handler( int signo )
{
  if(user_func) (*user_func)();
  UNUSED(signo);
}

unsigned  __FASTCALL__ __OsSetTimerCallBack(unsigned ms,timer_callback func)
{
   unsigned ret;
   struct itimerval itimer;
   user_func = func;
   getitimer(ITIMER_REAL,&otimer);
   old_alrm = signal(SIGALRM,my_alarm_handler);
   signal(SIGALRM,my_alarm_handler);
   itimer.it_interval.tv_sec = 0;
   itimer.it_interval.tv_usec = ms*1000;
   itimer.it_value.tv_sec = 0;
   itimer.it_value.tv_usec = ms*1000;
   setitimer(ITIMER_REAL,&itimer,NULL);
   getitimer(ITIMER_REAL,&itimer);
   ret = itimer.it_interval.tv_sec*1000 + itimer.it_interval.tv_usec/1000;
   if(!ret) __OsRestoreTimer();
   return ret;
}

			     /* Restore time callback function to original
				state */
void  __FASTCALL__ __OsRestoreTimer()
{
  signal(SIGALRM,old_alrm);
  setitimer(ITIMER_REAL,&otimer,NULL);
}

void __FASTCALL__ __nls_OemToOsdep(unsigned char *buff,unsigned int len)
{
}

void __FASTCALL__ __nls_OemToFs(unsigned char *buff,unsigned len)
{
}

void __FASTCALL__ __nls_CmdlineToOem(unsigned char *buff,unsigned len)
{
}

