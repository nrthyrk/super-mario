void set_mode(int mode)
{
	union REGS inregs,outregs;
	inregs.h.ah=0;
	inregs.h.al=(unsigned char)mode;
	int86(0x10,&inregs,&outregs);
}

void put_pix(int x,int y,int color)
{
	buffer[(y<<8)+(y<<6)+x]=color;
}

void bm_to_pbm(char far *tmp,char far *blck)
{
	int i;
	for(i=0;i<258;i++)
		blck[i]=tmp[i];

}

void graphchar(int xc, int yc, char c,int color)
{
	int x,y;
	unsigned char data;
	unsigned char far *work;
	unsigned char bitmask=0x80;
	work=charset+(c<<3);
	for(y=0;y<8;y++)
	{
		bitmask=0x80;
		for(x=0;x<8;x++)
		{
			if((*work & bitmask))
				put_pix(x+xc,y+yc,color);
			bitmask=(bitmask>>1);
		}
		work++;
	}
}


void graphstring(int x,int y,char *string,int color)
{
	int loop;
	for(loop=0;loop<strlen(string);loop++)
		graphchar(x+loop*8,y,string[loop],color);
}

void put_masked_pbm(int x,int y,char far *blck, int mx, int my){
	int i,j;
	for(i=0;i<16;i++) {
		for(j=0;j<16;j++) {
			if (i+x >= mx+32 && i+x < mx+16+32 && j+y >= my-15 && j+y < my+16-15)
			{
				if (data.direction==0 && data.mario[abs(data.sprite/2)][j+y - (my-15)][i+x - (mx+32)] != -1)
					continue;
				if (data.direction!=0 && data.mario[abs(data.sprite/2)][j+y - (my-15)][(mx+32 + 15) - (i+x)] != -1)
					continue;
			}
			buffer[x+y*320+i+j*320]=blck[j*16+i+2]==0?BACKGROUND:blck[j*16+i+2];
		}
	}
}

void put_masked_pbmcw(int x,int y,char far *blck){
	int i,j;
	for(i=0;i<16;i++) {
		for(j=0;j<16;j++) {
			if(blck[j*16+i+2]!=0)buffer[x+y*320+i+j*320]=blck[j*16+i+2];
		}
	}
}

void draw_block(int x,int y,int mx, int my){
	int i,j;
	cwtype *pcw;
	btype *p;
	int incwbt;

	for(i=0;i<16;i++) {
		for(j=0;j<16;j++) {
			if (i+x >= mx+32 && i+x < mx+16+32 && j+y >= my-15 && j+y < my+16-15)
			{
				if (data.direction==0 && data.mario[abs(data.sprite/2)][j+y - (my-15)][i+x - (mx+32)] != -1)
					continue;
				if (data.direction!=0 && data.mario[abs(data.sprite/2)][j+y - (my-15)][(mx+32 + 15) - (i+x)] != -1)
					continue;
			}
			/*incwbt = 0;
			pcw=hcw;
			while(pcw!=NULL){
				if(pcw->action==1){
					if (i+x >= pcw->x && i+x < pcw->x+16 && j+y >= pcw->y && j+y < pcw->y+16 
						&& data.block[37][(j+y-pcw->y) * 16 + i+x - pcw->x + 2] != 0)
						incwbt = 1;
				}
				pcw=pcw->next;
			}
			
			p=hbt;
			while(p!=NULL){
				if(p->action==1){
					if (i+x >= p->x && i+x < p->x+16 && j+y >= p->y && j+y < p->y+16 
						&& data.block[37][(j+y-p->y) * 16 + i+x - p->x + 2] != 0)
						incwbt = 1;
				}
				p=p->next;
			}

			if (incwbt)
				continue;*/

			buffer[i+x+(j+y)*320]=BACKGROUND;
		}
	}

}

void init()
{
	int loop;
	set_mode(VGA256);
	install_new_key_handler();
	for (loop=0;loop<52;loop++) {
		if ((data.block[loop]=(char far *)malloc(258))==NULL)
			exitgame();
	}
	for (loop=0;loop<3;loop++) {
		if ((data.coin[loop]=(char far *)malloc(258))==NULL)
			exitgame();
	}
	if ((hcoin=(cointype *)malloc(sizeof(cointype)))==NULL)
		exitgame();
	hcoin->next=NULL;

	screenx=0;
	data.maxlength=216;
	marioy=140;
	data.startpos[0]=85;
	data.startpos[1]=140;
	snd=0;
}


void fillscreen(int color)
{
	unsigned int i;
	for(i=0;i<320*200L;i++) buffer[i]=color;
}


void getinputs()
{
	FILE *in;
	int count,loop1,loop2,loop3;
	char *temp,ch;
	char far *tempbm;

	if ((in=fopen("level001.dat","r"))==NULL)
	{
		exitgame();
		printf("level001.dat could not be found!\n");
		exit(1);
	}
	fseek(in,0,SEEK_SET);
	for (loop1=0;loop1<12;loop1++) {
		for (loop2=0;loop2<=data.maxlength;loop2++)
		{
			ch=getc(in);
			if(ch<='0')
				data.world[loop1][loop2]=0;
			if (ch>='a' && ch<='z')
				data.world[loop1][loop2]=ch-70;
			if (ch>='A' && ch<='Z')
				data.world[loop1][loop2]=ch-64;
			if (loop1==0)
				data.world[loop1][loop2]=0;
		}
	}
	fclose(in);

	if ((in=fopen("BLOCK001.PCX","rb"))==NULL)
	{
		exitgame();
		printf("block001.pcx could not be found!\n");

		exit(1);
	}
	temp=getpcx(in,temp,6000);
	tempbm=(char far *)farmalloc(258);

	for (loop1=0;loop1<26;loop1++)
	{
		count=2;
		for (loop2=0;loop2<16;loop2++) {
			for (loop3=0;loop3<16;loop3++)
				tempbm[count++]=temp[loop1*17+(loop2+1)*320+loop3+1];
		}
		bm_to_pbm(tempbm,data.block[loop1+26]);
	}
	free(temp);
	fseek(in,-768,SEEK_END);
	for (loop1=0;loop1<256;loop1++)
	{
		data.palette[loop1].red=(getc(in)>>2);
		data.palette[loop1].green=(getc(in)>>2);
		data.palette[loop1].blue=(getc(in)>>2);
	}
	fclose(in);

	if ((in=fopen("BGRND001.PCX","rb"))==NULL)
	{
		exitgame();
		printf("bgrnd0001.pcx could not be found!\n");

		exit(1);
	}
	temp=getpcx(in,temp,6000);
	for (loop1=0;loop1<26;loop1++)
	{
		count=2;
		for (loop2=0;loop2<16;loop2++) {
			for (loop3=0;loop3<16;loop3++)
				tempbm[count++]=temp[loop1*17+(loop2+1)*320+loop3+1];
		}
		bm_to_pbm(tempbm,data.block[loop1]);
	}
	free(temp);
	fclose(in);

	if ((in=fopen("COIN.PCX","rb"))==NULL)
	{
		exitgame();
		printf("coin.pcx could not be found!\n");
		exit(1);
	}
	temp=getpcx(in,temp,6000);
	for (loop1=0;loop1<3;loop1++)
	{
		count=2;
		for (loop2=0;loop2<16;loop2++) {
			for (loop3=0;loop3<16;loop3++)
				tempbm[count++]=temp[loop1*17+(loop2+1)*320+loop3];
		}
		bm_to_pbm(tempbm,data.coin[loop1]);
	}
	free(temp);
	fclose(in);
	farfree(tempbm);


	for (loop1=0;loop1<256;loop1++)
		setpalette(loop1,data.palette[loop1]);

}

void getmario()
{
	FILE *marioin;
	int loop1,loop2,loop3;
	char *temp;
	if ((marioin=fopen("MARIO.PCX","rb"))==NULL)
	{
		exitgame();
		printf("MARIO.PCX could not be found!\n");
		exit(1);
	}
	fseek(marioin,128,SEEK_SET);
	temp=(char *)malloc(6000);
	temp=getpcx(marioin,temp,6000);
	for (loop1=0;loop1<7;loop1++) {
		for (loop2=0;loop2<=15;loop2++) {
			for (loop3=0;loop3<=15;loop3++)
				data.mario[loop1][loop2][loop3]=temp[loop1*17+(loop2+1)*320+loop3+1];
		}
	}
	fseek(marioin,-765,SEEK_END);
	for (loop1=0;loop1<2;loop1++)
	{
		data.marioc[loop1].red=(getc(marioin)>>2);
		data.marioc[loop1].green=(getc(marioin)>>2);
		data.marioc[loop1].blue=(getc(marioin)>>2);
	}

	fclose(marioin);
	free(temp);
}

char *getpcx(FILE *file, char *temp, int length)
{
	long int count=0;
	int numbytes,loop;
	unsigned char ch;
	fseek(file,128,SEEK_SET);
	temp=(char *)malloc(length);
	while(count<length)
	{
		ch=fgetc(file);
		if ((ch>=192 && ch<=255))
		{
			numbytes=ch-192;
			ch=fgetc(file);
			for(loop=0;loop<numbytes;loop++)
				temp[count++]=ch;
		}
		else
			temp[count++]=ch;
	}
	return(temp);
}


void setpalette(int index, colorpal color)
{
	outp(0x3c6,0xff);
	outp(0x3c8,index);
	outp(0x3c9,color.red);
	outp(0x3c9,color.green);
	outp(0x3c9,color.blue);
}


void drawscreen(int x, int mx, int my)
{
	int loop1,loop2,back,x2;
	static int dy;
	x2=x;
	back=0;
	while(x2>=16)
	{
		x2-=16;
		back++;
	}
	for (loop1=0;loop1<12;loop1++) {
		for (loop2=back;loop2<=back+16;loop2++) {
			if (data.world[loop1][loop2]>0)
			{
				if (data.bumped[0]==loop2 && data.bumped[1]==loop1)
				{
					dy+=data.bumploop;
					put_masked_pbm((loop2<<4)-x+32,(loop1<<4)+dy,data.block[data.world[loop1][loop2]-1],mx,my);
					data.bumploop++;
					if(data.bumploop==3)
					{
						data.bumploop=0;
						data.bumped[0]=0;
						data.bumped[1]=0;
						dy=0;
					}
				}
				else
				{
					if(data.world[loop1][loop2]==38) 
					{
						add_cw((loop2<<4)-x,(loop1<<4));
						data.world[loop1][loop2]=0;
					}
					else if(data.world[loop1][loop2]==39)
					{
						add_bt((loop2<<4)-x,(loop1<<4));
						data.world[loop1][loop2]=0;
					}
					else
						put_masked_pbm((loop2<<4)-x+32,(loop1<<4),data.block[data.world[loop1][loop2]-1],mx,my);
				}
			}
			else
			{
				draw_block((loop2<<4)-x+32,(loop1<<4),mx,my);
			}
		}
	}
	drawcoin(x, mx, my);

	/*int loop1,loop2;*/
	for (loop1=0;loop1<=15;loop1++) {
		for (loop2=0;loop2<=15;loop2++) {
			if (data.mario[abs(data.sprite/2)][loop1][loop2]>=0) {
				if (my-15+loop1>=0 && my-15+loop1<192)
				{
					if (data.direction==0)/*向前*/
						put_pix(mx+loop2+32,my-15+loop1,data.mario[data.sprite/2][loop1][loop2]);
					else/*向后，反向画出*/
						put_pix(mx+15-loop2+32,my-15+loop1,data.mario[data.sprite/2][loop1][loop2]);
				}
			}
		}
	}

}

void check_keybuf(int *movep, int *finishp, int *upp, int *downp, int *jumpp)
{
	int move=*movep;
	int finish=*finishp;
	int up=*upp;
	int down=*downp;
	int jumploop=*jumpp;
	if (keybuf[KEY_RIGHT] && !keybuf[KEY_LEFT])
	{
		if (keybuf[KEY_CTRL])
		{
			if (move<16)
				move++;
		}
		else
		{
			if (move>8)
				move--;
			if (move<8)
				move++;
		}
		if (landed)
			data.direction=0;
	}
	if (keybuf[KEY_LEFT] && !keybuf[KEY_RIGHT])
	{
		if (keybuf[KEY_CTRL])
		{
			if (move>-16)
				move--;
		}
		else
		{
			if (move<-8)
				move++;
			if (move>-8)
				move--;
		}
		if (landed) 
			data.direction=1;
	}
	if (keybuf[KEY_UP] && !down)
	{
		up=-17;
		data.sprite=12;
		jumploop++;
		if (jumploop>10)
		{
			down=1;
			jumploop=0;
		}
	}

	if ((!keybuf[KEY_RIGHT] && !keybuf[KEY_LEFT]) || (keybuf[KEY_RIGHT] && keybuf[KEY_LEFT]))
	{
		if (move>0)
			move--;
		if (move<0)
			move++;
	}
	if(keybuf[KEY_Q])
	{
		graphstring(130,100,"Quit(Y/N)",TEXTCOLOR);
		clear_key_buffer();
		while(!keybuf[KEY_Y] && !keybuf[KEY_N]);
		if (keybuf[KEY_Y] && !keybuf[KEY_N])
			exitgame();
		clear_key_buffer();
		graphstring(130,100,"Quit(Y/N)",BACKGROUND);
	}
	*movep=move;
	*finishp=finish;
	*upp=up;
	*downp=down;
	*jumpp=jumploop;
}

void animate_mario(int move)
{
	if (landed)
	{
		if (data.sprite==10 && ((move>0 && data.direction==0) || (move<0 && data.direction==1)))
			data.sprite=1;
		if (move==0)
			data.sprite=0;
		if ((move>0 && data.direction==0) || (move<0 && data.direction==1))
		{
			if(abs(move)<16)
				data.sprite++;
			else
				data.sprite+=2;
			if (data.sprite>=10)
				data.sprite=2;
		}
		if ((move>0 && data.direction==1) || (move<0 && data.direction==0))
			data.sprite=10;
	}
}

void move_mario(int *movep)
{
	int move=*movep;
	int loop1,loop2;
	if (marioy>16)
	{
		if (move>0) {
			for (loop1=0;loop1<16;loop1++) {
				for (loop2=0;loop2<=move;loop2++) {
					if (data.world[(marioy-loop1)/16][(mariox+screenx+13+loop2/4)/16]>26)
						move=loop2;
				}
			}
		}
		if (move<0) {
			for (loop1=0;loop1<16;loop1++) {
				for (loop2=0;loop2>=move;loop2--) {
					if (data.world[(marioy-loop1)/16][(mariox+screenx+2+loop2/4)/16]>26 || (mariox+loop1/2)<=0)
						move=loop2;
				}
			}
		}
	}
	if(mariox>100 && move>0) {
		screenx+=move/4;
		screen_move=1;
	}
	else {
		if (mariox+move/4>0) {
			mariox+=move/4;screen_move=0;
		}
	}
	*movep=move;
}



void exitgame()
{
	int loop;
	for (loop=0;loop<52;loop++)
		farfree(data.block[loop]);
	for (loop=0;loop<3;loop++)
		farfree(data.coin[loop]);
	free(hcoin);
	free_cw();
	free_bt(); 
	uninstall_new_key_handler();
	RestoreTimer();
	set_mode(TEXT_MODE);
}

void check_jump(int *upp, int *downp, int *jumploopp)
{
	int loop1,loop2;
	int up=*upp;
	int down=*downp;
	int jumploop=*jumploopp;
	loop1=3;
	landed=0;
	while (loop1<13 && landed==0)
	{
		if (data.world[(marioy+1)/16][(mariox+screenx+loop1)/16]>26)
			landed=1;
		else
			landed=0;
		loop1++;
	}
	if (!landed || marioy<16) {
		if (up<17)
			up+=1;
	}
	if (up>0) 
	{
		down=1;
		loop1=marioy;
		if (marioy<207)
		{
			while(loop1++<marioy+up/4)
			{
				for (loop2=3;loop2<14;loop2+=9) {
					if (data.world[loop1/16][(mariox+screenx+loop2)/16]>26)
					{
						marioy=loop1-1;
						loop1=marioy+up/4;
						loop2=13;
						up=0;
						down=0;
						data.sprite=0;
					}
				}
			}
		}
		marioy+=up/4;

	}
	if (up<0) 
	{
		if (!keybuf[KEY_UP])
		{
			down=1;
			jumploop=0;
		}
		loop1=marioy;
		marioy+=up/4;
		if(!up) snd=2;
		if (marioy>24)
		{
			while(loop1-->=marioy+up/4)
			{
				if(data.world[loop1/16-1][(mariox+screenx+8)/16]>26 ||data.world[loop1/16-1][(mariox+screenx+8)/16]==18)
				{
					marioy=loop1;
					loop1=marioy+up/4-1;
					up=-up;
					down=1;
					jumploop=0;
					if (data.bumped[0]==0 && data.bumped[1]==0)
					{
						data.bumped[0]=(mariox+screenx+8)/16;
						data.bumped[1]=marioy/16-1;
						data.bumploop=-3;
					}
				}
				if (data.world[loop1/16-1][(mariox+screenx+12)/16]>26 && data.world[loop1/16-1][(mariox+screenx+8)/16]<26)
					mariox-=1;
				if (data.world[loop1/16-1][(mariox+screenx+3)/16]>26 && data.world[loop1/16-1][(mariox+screenx+8)/16]<26)
					mariox+=1;
			}
		}
	}
	*upp=up;
	*downp=down;
	*jumploopp=jumploop;
}

void check_blocks()
{
	int bumpedblock=0;
	if (data.world[(marioy-8)/16][(mariox+screenx+8)/16]==10)
	{
		data.world[(marioy-8)/16][(mariox+screenx+8)/16]=0;

		snd=4;
	}
	if (data.bumploop==-3)
	{
		bumpedblock=data.world[marioy/16-1][(mariox+screenx+8)/16];
		switch (bumpedblock)
		{
			case 18:
			case 28:
			data.world[marioy/16-1][(mariox+screenx+8)/16]=27;

			snd=4;
			addcoin(data.bumped[0],data.bumped[1]-1);break;
			case 13:
			data.world[marioy/16-1][(mariox+screenx+8)/16]=27;
			add_cw(mariox,40);
			add_cw(mariox+16,20);
			add_cw(mariox-16,20);
			snd=4;
			break;
			case 29:
			if (data.world[data.bumped[1]-1][data.bumped[0]]==10)
			{
				data.world[data.bumped[1]-1][data.bumped[0]]=0;

				addcoin(data.bumped[0],data.bumped[1]-1);
				snd=4;
			}
			break;
			default:
			data.bumped[0]=data.bumped[1]=0;
			data.bumploop=0;
			break;
		}
	}

}

void check_finish(int *finishp)
{
	int finish=*finishp;
	if (marioy>=184)
		finish=1;
	if ((mariox+screenx+16)/16==data.maxlength-13)
		finish=2;
	*finishp=finish;
}

time_delay(int ti)
{
	long int tt;
	tt=t;
	while(t-tt<ti);
} 

