#ifndef _GDC_H_
#define _GDC_H_

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

#define VGA256 0x13
#define TEXT_MODE 0x03
#define TEXTCOLOR  35
#define BACKGROUND 91

typedef struct as
{
	char red;
	char green;
	char blue;
} colorpal;

typedef struct
{
	char mario[7][16][16];
	char far *block[52];
	char world[12][217];
	char far *coin[3];
	short int bumped[2];
	short int bumploop;
	short int sprite;
	short int direction;
	short int maxlength;
	short int startpos[2];
	colorpal palette[256];
	colorpal marioc[2];
} graphics;

typedef struct wt/*刺猬的结构*/
{
  short int action;/*标记是否被激活*/
  short int x;
  short int y;/*相对于屏幕的位置*/
  short int dx;
  short int dy;
  /*落下或者移动的速度*/
  struct wt *next;
} cwtype;

typedef struct bt/*刺猬的结构*/
{
  short int action;/*标记是否被激活*/
  short int x;
  short int y;/*相对于屏幕的位置*/
  short int dx;
  short int dy;
  /*落下或者移动的速度*/
  struct bt *next;
} btype;

typedef struct ct
{
	short int coinx;
	short int coiny;
	short int coinloop;
	short int dy;
	struct ct *next;
} cointype;

short int screenx;
short int screen_move;
short int snd;

graphics data;
unsigned int times[3];
int mariox;
int marioy;
short int landed;
cointype *hcoin;
cwtype *hcw=NULL,*tcw=NULL;
btype *hbt=NULL,*tbt=NULL;
long int t;

unsigned char far *charset=(unsigned char far *)0xF000FA6EL;
unsigned char far *buffer=(char far *)0xA0000000L;

void init();
void fillscreen(int color);
void getinputs();
void getmario();
char *getpcx(FILE *file, char *temp, int length);
void setpalette(int index, colorpal color);
void drawscreen(int x, int mx, int my);

int startscreen();
void check_keybuf(int *movep, int *finishp, int *upp, int *downp, int *jumploop);
void animate_mario(int move);
void move_mario(int *movep);
void removecoin();

void exitgame();
void check_jump(int *upp, int *downp, int *jumploopp);
void check_blocks();
void check_finish(int *finishp);
void set_mode(int mode);
void put_pix(int x,int y,int color);
void bm_to_pbm(char far *tmp,char far *blck);
void put_mask_pbm(int x,int y,char far *blck);

void add_cw(short int x,short int y);
void free_cw();
void draw_cw();
int move_cw(int move,int *upp,int *jumploopp);
void addcoin(int x, int y);
void removecoin();
void drawcoin(int x, int mx, int my);
void add_bt(short int x,short int y);
void free_bt();
void draw_bt();
int move_bt(int move);

#endif