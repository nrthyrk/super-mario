#ifndef _ACT_H_
#define _ACT_H_

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

#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320

extern short int snd;
extern unsigned char far *buffer;

typedef struct BMP_file
{
	unsigned int bfType;
	unsigned long bfSize;
	unsigned int Reserved1;
	unsigned int reserved2;
	unsigned long bfOffset;
} bitmapfile;

typedef struct BMP_info
{
	unsigned long biSize;
	unsigned long biWidth;
	unsigned long biHeight;
	unsigned int biPlanes;
	unsigned int biBitCount;
	unsigned long biCompression;
	unsigned long biSizeImage;
	unsigned long biXpolsPerMeter;
	unsigned long biYpelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
} bitmapinfo;


typedef struct RGB_BMP_typ
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;
} RGB_BMP, *RGB_BMP_ptr;

typedef struct bmp_picture_typ
{
	bitmapfile file;
	bitmapinfo info;
	RGB_BMP palette[256];
	char far *buffer;
} bmp_picture, *bmp_picture_ptr;

void Set_BMP_Palette_Register(int index,RGB_BMP_ptr color);

void Check_Bmp(bmp_picture_ptr bmp_ptr);

void BMP_Load_Screen(char *bmp);

void act();

#endif
