void add_cw(short int x,short int y)
{
	cwtype *temp;

	temp=(cwtype *)malloc(sizeof(cwtype));
	temp->next=NULL;
	temp->x=x;
	temp->y=y;
	temp->action=1;
	temp->dx=1;
	temp->dy=0;
	if(hcw==NULL)
		hcw=temp;
	else
		tcw->next=temp;
	tcw=temp;

}
void free_cw()
{
	cwtype *p;
	while(hcw!=NULL){p=hcw;hcw=hcw->next; free(p);}
	hcw=NULL;
	tcw=NULL;
}

void draw_cw()
{
	int i,j;
	cwtype *p;
	p=hcw;
	while(p!=NULL){
		if(p->action==1){
			put_masked_pbmcw(p->x,p->y,data.block[37]);
		}
		p=p->next;
	}

}

int move_cw(int move,int *upp,int *jumploopp)
{
	int up=*upp;
	int jumploop=*jumploopp;
	cwtype *p,*temp;
	int loop1,loop2;
	p=hcw;

	while(p!=NULL){
		if(p->action==-1) {p=p->next;continue;}
		/*action==1*/
		if(marioy>p->y && marioy<p->y+14 &&abs(mariox+32-p->x)<10) {p->action=-1;up=-10;jumploop++;snd=3;continue;}
		if(abs(p->x+8-(mariox+32+8))<10 && abs(p->y+8-(marioy-8))<10)return 3;
		if(p->y<160) p->dy++;

		for (loop1=0;loop1<=p->dy;loop1++) {
			if (data.world[(p->y+loop1)/16+1][(p->x+screenx+13)/16-2]>26)
			{
				p->dy=loop1;
				p->dy=0;
			}
		}
		p->y+=p->dy;
		p->x=p->x-p->dx;
		if(screen_move) 
		{
			p->x=p->x-move/4;
		}

		if(p->y>=200 || p->x<=32 ||p->x>=320-16) 
		{
			p->action=-1;
			p=p->next;
			continue;
		}
		p=p->next;
	}
	*upp=up;
	*jumploopp=jumploop;
	return 0;
}






void addcoin(int x, int y)
{
	cointype *temp,*c;
	c=hcoin;
	while ((c->next)!=NULL)
		c=c->next;
	temp=(cointype *)malloc(sizeof(cointype));
	temp->next=NULL;
	temp->coinx=x;
	temp->coiny=y;
	temp->coinloop=-10;
	temp->dy=0;
	c->next=temp;
}

void removecoin()
{
	cointype *temp;
	cointype *c=hcoin;
	temp=c->next;
	c->next=c->next->next;
	free(temp);
}

void drawcoin(int x, int mx, int my)
{
	cointype *c=hcoin;
	int loop,num=0;
	while ((c->next)!=NULL)
	{
		num++;
		c=c->next;
	}
	c=hcoin;
	for (loop=0;loop<num;loop++)
	{
		c=c->next;
		if (((c->coinx)*16+32-x)>16)
			put_masked_pbm((c->coinx)*16+32-x,(c->coiny)*16+c->dy,data.coin[abs((c->coinloop)%3)], mx, my);
		c->coinloop++;
		c->dy+=c->coinloop;
		if (c->coinloop>=10)
			removecoin();
	}
}


void add_bt(short int x,short int y)
{
	btype *temp;

	temp=(btype *)malloc(sizeof(btype));
	temp->next=NULL;
	temp->x=x;
	temp->y=y;
	temp->action=1;
	temp->dx=-1;
	temp->dy=-1;
	if(hbt==NULL) hbt=temp;
	else
		tbt->next=temp;
	tbt=temp;

}
void free_bt()
{
	btype *p;
	while(hbt!=NULL){p=hbt;hbt=hbt->next; free(p);}
	hbt=NULL;
	tbt=NULL;
}

void draw_bt()/*0代表画背景，1代表画子画面*/
{
	int i,j;
	btype *p;
	p=hbt;
	while(p!=NULL){
		if(p->action==1){
			put_masked_pbmcw(p->x,p->y,data.block[38]);
		}
		p=p->next;
	}

}
int move_bt(int move)
{
	btype *p,*temp;
	int loop1,loop2;
	p=hbt;

	while(p!=NULL){
		if(p->action==-1) {p=p->next;continue;}
		/*action==1*/
		if(abs(p->x+8-(mariox+32+8))<14 && abs(p->y+8-(marioy-8))<14)return 3;

		if(p->x<mariox+32-50) p->dx=prand();
		else if(p->x-50>mariox+32) p->dx=-prand();
		if(p->y<marioy-50) p->dy=prand();
		else if(p->y-50>marioy) p->dy=-prand();

		p->y+=p->dy;
		p->x+=p->dx;
		for (loop1=0;loop1<=p->dy;loop1++) {
			if (data.world[(p->y+loop1)/16+1][(p->x+screenx+13)/16-2]>26)
			{
				p->dy=-p->dy;
			}
		}
		if(screen_move)
		{
			p->x=p->x-move/4;
			screen_move=0;
		}
		if(p->y>=200 || p->x<=32 ||p->x>=320-16 ||p->y<16) 
		{
			p->action=-1;
			p=p->next;
			continue;
		}
		p=p->next;
	}
	return 0;
}

int prand()
{
	srand(time(NULL));
	return rand()%2;
}

