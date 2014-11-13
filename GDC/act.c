#include "act.h"

void Set_BMP_Palette_Register(int index,RGB_BMP_ptr color)
{
	outp(0x3c6,0xff);
	outp(0x3c8,index);
	outp(0x3c9,color->red);
	outp(0x3c9,color->green);
	outp(0x3c9,color->blue);
}

void Check_Bmp(bmp_picture_ptr bmp_ptr)
{
	if(bmp_ptr->file.bfType!=0x4d42)
	{
		printf("Not a BMP file!\n");
		exit(1);
	}
	if(bmp_ptr->info.biCompression!=0)
	{
		printf("Can not display a compressed BMP file!\n");
		exit(1);
	}
	if(bmp_ptr->info.biBitCount!=8)
	{
		printf("Not a index 16color BMP file!\n");

		exit(1);
	}
}


void BMP_Load_Screen(char *bmp)
{
	int i,fp;
	bmp_picture bmp256;
	char *file_name;
	if ((fp=open(bmp,O_RDONLY))==1)
		return;

	read(fp,&bmp256.file,sizeof(bitmapfile));
	read(fp,&bmp256.info,sizeof(bitmapinfo));

	Check_Bmp((bmp_picture_ptr)&bmp256);

	for (i=0;i<256;i++)
	{
		read(fp,&bmp256.palette[i].blue,1);
		read(fp,&bmp256.palette[i].green,1);
		read(fp,&bmp256.palette[i].red,1);
		read(fp,&bmp256.palette[i].reserved,1);
		bmp256.palette[i].blue=bmp256.palette[i].blue>>2;
		bmp256.palette[i].green=bmp256.palette[i].green>>2;
		bmp256.palette[i].red=bmp256.palette[i].red>>2;
	}
	for (i=0;i<256;i++)
		Set_BMP_Palette_Register(i,(RGB_BMP_ptr)&bmp256.palette[i]);

	for(i=SCREEN_HEIGHT-1;i>=0;i--)
	{
		lseek(fp,1078+(long)(SCREEN_HEIGHT-i-1)*SCREEN_WIDTH,0);
		read(fp,&buffer[i*SCREEN_WIDTH],SCREEN_WIDTH);
	}
	close(fp);
}


void act()
{
	snd=0;
	BMP_Load_Screen("act1.bmp");
	Bg_Music();
	time_delay(500);
	BMP_Load_Screen("act2_1.bmp");
	time_delay(500);
	BMP_Load_Screen("act2_2.bmp");
	time_delay(500);
	BMP_Load_Screen("act3.bmp");
	time_delay(500);
	BMP_Load_Screen("act4.bmp");
	time_delay(500);
	BMP_Load_Screen("act5.bmp");
	time_delay(500);
	BMP_Load_Screen("act6.bmp");
	time_delay(500);
	BMP_Load_Screen("act7.bmp");
	time_delay(500);
	BMP_Load_Screen("act8.bmp");
	time_delay(500);
}
