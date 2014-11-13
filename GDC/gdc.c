#include "gdc.h"
#include "audit.h"
#include "act.h"

#include "key.c"
#include "gdcfunc.c"
#include "lb.c"

void main()
{
	int move;
	int finish=0;
	int up;
	int down=0;
	int jumploop=0;
	int i;

	init();

	act();

	getmario();

	getinputs();

	snd=1;

	while(!finish)
	{
		marioy=140;

		if (screenx>=data.startpos[0]*16)
			screenx=data.startpos[0]*16;
		else
			screenx=0;
		if (screenx>0)
			marioy=data.startpos[1];

		mariox=40;
		up=0;
		move=0;
		down=0;
		data.sprite=6;

		fillscreen(BACKGROUND);
		while(!finish)
		{

			drawscreen(screenx, mariox, marioy);
			draw_cw();
			draw_bt();

			animate_mario(move);
			move_mario(&move);
			finish=move_cw(move,&up,&jumploop);
			if(finish==3) break;
			finish=move_bt(move);

			if (marioy<192)
				check_keybuf(&move,&finish,&up,&down,&jumploop);

			check_jump(&up,&down,&jumploop);
			check_blocks();
			check_finish(&finish);
		}

		switch (finish)
		{
			case 1:
				finish=0;
				snd=5;
				time_delay(500);
				break;

			case 2:
				screenx=0;
				finish=0;
				snd=0;
				BMP_Load_Screen("act2_1.bmp");
				time_delay(5000);
				break;

			case 3:
				marioy-=20;
				data.sprite=0;
				snd=5;
				for(i=0;marioy<192;i++){
					time_delay(5);
					marioy+=i/3;
					drawscreen(screenx,mariox,marioy);
					draw_cw();
					draw_bt();

					finish=0;
				}
				time_delay(100);
				break;
		}
		free_cw();
		free_bt();
		getinputs();
	}

	Stop_Music();
	exitgame();
}
