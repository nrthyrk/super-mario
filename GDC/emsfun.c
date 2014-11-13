unsigned char emm_name[]="EMMXXXX0";
unsigned int emm_handle;
unsigned char emm_alloc=0;

int ememavail(void)
/*测试EMS是否可用(EMM386.exe已加载)*/
{
	union REGS in,out;
	unsigned char far *emm;
	emm=(unsigned char far *)(0x67*4+2);
	emm=MK_FP(*((unsigned int *)emm),0xA);
	if(memcmp(emm_name,emm,8)) return 0;

	in.h.ah=0x42;
	int86(0x67,&in,&out);
	if(out.h.ah) return 0;

	if(out.x.dx) return(out.x.dx);
	else return 0;
}


char ealloc(unsigned int pages,unsigned *segment)
/*申请EMS页面数*/
{
	union REGS in,out;
	if(emm_alloc) return(0);

	in.h.ah=0x43;
	in.x.bx=pages;
	int86(0x67,&in,&out);
	if(out.h.ah) return(0);
	emm_alloc=1;
	emm_handle=out.x.dx;

	in.h.ah=0x41;
	int86(0x67,&in,&out);
	if(out.h.ah) return(0);
	*segment=out.x.bx;
	return 1;
}

int efree(void)
/*释放EMS*/
{
	union REGS in,out;
	if(!emm_alloc) return 0;

	in.h.ah=0x45;
	in.x.dx=emm_handle;
	int86(0x67,&in,&out);
	if(out.h.ah) return 0;
	emm_alloc=0;
	return(1);
}

int emap(unsigned int pageno,unsigned char n)
/*映射16kEMS页到物理页，n为物理页偏移(取0-3，共4页)*/
{
	union REGS in,out;
	if(!emm_alloc) return 0;

	if(pageno==65535)return(0);
	in.h.ah=0x44;
	in.h.al=n;
	in.x.bx=pageno;
	in.x.dx=emm_handle;
	int86(0x67,&in,&out);
	if(out.h.ah) return 0;
	return 1;
}
