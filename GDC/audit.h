/**********************************文件说明***************************************************
*背景音乐文件                                                                                *
*全局变量： Counter，CounterInt8, fpI8，Port， Found=0，Cnt1, Cnt2，*fp，num，smp1           *
*重要函数                                                                                    *
* void Background_Music(char *filename)开始播放背景音乐                                      *
* void Stop_Music()结束播放背景音乐                                                          *
*********************************************************************************************/

#ifndef _AUDIT_H_
#define _AUDIT_H_

#include <alloc.h>
#include <io.h>
#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <bios.h>
#include <mem.h>
#include <fcntl.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define NumberOfTimes1 50
#define NumberOfTimes2 100
#define OldTimerInt 0x60

#ifdef __cplusplus
#define __CPPARGS ...
#else
#define __CPPARGS
#endif

struct wavhead
{
	char riffid[4];
	long cksize;
	char fcctype[4];
	char fmtid[4];
	long ck16;
	unsigned fortag;
	unsigned nchan;
	long nsamps;
	long navgbgs;
	unsigned nblocka;
	unsigned wbpsam;
	char dataid[4];
	long datalen;
};

void PlayWavBg();

void RestoreTimer();

void interrupt NewTimer(__CPPARGS);

void Bg_Music();

void Stop_Music();

void Testsb();

void ems_read(int n);

void SetTimer(void interrupt(*Rout)(__CPPARGS),unsigned freq);

void RestoreTimer();

#endif
