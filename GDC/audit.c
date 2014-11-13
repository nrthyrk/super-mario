/**********************************文件说明***************************************************
*背景音乐文件                                                                                *
*全局变量： Counter，CounterInt8, fpI8，Port， Found=0，Cnt1, Cnt2，*fp，num，smp1           *
*重要函数                                                                                    *
* void Background_Music(char *filename)开始播放背景音乐                                      *
* void Stop_Music()结束播放背景音乐                                                          *
*********************************************************************************************/

#include "audit.h"
#include "emsfun.c"

extern short int snd;
extern long int t;

unsigned logical_page=0;
unsigned long Counter[6];
unsigned CounterInt8[6],fpI8;
unsigned Port=0x210;

long num[6];
int smp1[6];
int page[6];

char far *fc;

struct wavhead wfhead;

void Testsb()
{
	char Found=0;
	unsigned Cnt1,Cnt2;
	Cnt1=NumberOfTimes1;
	while((Port<=0x260)&&!Found)
	{
		outportb(Port+0x6,1);
		outportb(Port+0x6,0);
		Cnt2=NumberOfTimes2;
		while((Cnt2>0)&&(inportb(Port+0xE)<128))
			--Cnt2;
		if((Cnt2==0)||(inportb(Port+0xA)!=0xAA))
		{
			--Cnt1;
			if(Cnt1==0)
			{
				Cnt1=NumberOfTimes1;
				Port=Port+0x10;
			}
		}
		else
			Found=1;
	}
	if(!Found)
	{
		printf("Nobase port found!\n");
		printf("\nDSP not reseted!\n");
		exit(0);
	}
}

void ems_read(int n)
{

	FILE *fp1;
	int page,i,j;
	char name[8];
	sprintf(name,"%d.wav",n);
	if((fp1=fopen(name,"r+b"))==NULL)
	{
		printf("cannot open the file\n");
		exit(0);
	}
	fseek(fp1,22L,0);
	fread(&wfhead.nchan,sizeof(int),1,fp1);
	fseek(fp1,24L,0);
	fread(&wfhead.nsamps,sizeof(long),1,fp1);
	fseek(fp1,34L,0);
	fread(&wfhead.wbpsam,sizeof(int),1,fp1);
	fseek(fp1,40L,0);
	fread(&num[n],sizeof(long),1,fp1);


	smp1[n]=44100/wfhead.nsamps;
	Counter[n]=1;
	fseek(fp1,44L,0);
	if(wfhead.wbpsam==16)
		num[n]=num[n]/2;
	page=num[n]/16000+1;

	for(i=0;i<page;i++){
		emap(logical_page,0);
		logical_page++;
		for(j=0;j<16000;j++){
			fread(&fc[j],sizeof(char),1,fp1);
		}
	}
	fclose(fp1);
}

void SetTimer(void interrupt(*Rout)(__CPPARGS),unsigned freq)
{
	int ICnt;
	unsigned char intreg21;
	fpI8=(freq+9)/18;
	intreg21=inportb(0x21);
	outportb(0x21,255);
	ICnt=1193180/freq;
	outportb(0x43,0x36);
	outportb(0x40,ICnt&255);
	outportb(0x40,ICnt>>8);
	setvect(OldTimerInt,getvect(8));
	setvect(8,Rout);
	outportb(0x21,intreg21);
}

void RestoreTimer(){
	unsigned char intreg21;
	intreg21=inportb(0x21);
	outportb(0x21,255);
	outportb(0x43,0x36);
	outportb(0x40,0);
	outportb(0x40,0);
	setvect(8,getvect(OldTimerInt));
	outportb(0x21,intreg21);
}


void interrupt NewTimer()
{
	struct REGPACK R;
	static long ii=0,jj=0;
	static long iii=0,jjj=0;
	static long tt;
	if(tt++>441){ t++;tt=0;}
	Counter[snd]++;
	if(snd==1)
	{
		if( ( Counter[1]>=smp1[1]) || iii==0 )
		{

			outportb(Port+0xC,0x10);
			while(inportb(Port+0xC)&0x80);
			emap(page[1]+jjj,0);
			outportb(Port+0xC,fc[iii]);

			Counter[1]=0;
			iii++;
			if(num[1]==jjj*16000+iii) {iii=0;jjj=0;}
			if(iii>=16000) {iii=0;jjj++;}

		}
		ii=0;jj=0;
	}
	else if( ( Counter[snd]>=smp1[snd]) || ii==0 )
	{

		outportb(Port+0xC,0x10);
		while(inportb(Port+0xC)&0x80);
		emap(page[snd]+jj,0);
		outportb(Port+0xC,fc[ii]);

		Counter[snd]=0;
		ii++;
		if(num[snd]==jj*16000+ii) {ii=0;jj=0;snd=1;}
		if(ii>=16000) {ii=0;jj++;}

	}

	outportb(0x20,0x20);
}


void Bg_Music()
{
	int i;
	unsigned segment;
	Testsb();
	logical_page=0;
	if(!ememavail()) {printf("ememavail not");exit(0);}
	if(!ealloc(250,&segment)) {printf("ealloc error!");exit(0);}
	fc=MK_FP(segment,0);	
	for(i=0;i<6;i++){
		page[i]=logical_page;
		ems_read(i);
	}

	SetTimer(NewTimer,44100);
}
void Stop_Music()
{
	RestoreTimer();
}
