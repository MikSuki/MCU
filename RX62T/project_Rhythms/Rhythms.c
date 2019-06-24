/***********************************************************************/
/*                                                                     */
/*  FILE        :Rhythms.c                                           */
/*  DATE        :Fri, May 24, 2019                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :RX62T                                                 */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.51).    */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/

//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

void main(void);
int pow();
void delay(int);
void chsBlock(int);
void chsX(int);
void chsY(int);
void clearGame();
void clearX();
void clearY();
void draw();
void drawChsMenu();
void chsBigNum(int);
void chsNum(int);
void chsText(int);
void drawText(int, int);
void drawRecip();
void drawGame();
void drawStop();
void drawEnd();
void drawAll();
void btnClick(int, int *);
void timeUp();
void selSong();

#ifdef __cplusplus
extern "C"
{
	void abort(void);
}
#endif

// note + duration
int NOTE[25] = {-1, 20000000 / 311, 20000000 / 330, 20000000 / 349, 20000000 / 392, 20000000 / 440, 20000000 / 466, 20000000 / 523, 20000000 / 587, 20000000 / 622, 20000000 / 698, 20000000 / 740, 20000000 / 784, 20000000 / 880, 20000000 / 932, 20000000 / 1047, 20000000 / 1175, 20000000 / 1245, 20000000 / 1397, 20000000 / 495, 20000000 / 554, 20000000 / 661, 20000000 / 990, 20000000 / 310, 20000000 / 370};

unsigned char melody[] =
	{
		0, 0, 0, 191, 201, 84, 201, 80, 100, 200, 200, 200, 31, 31,
		194, 51, 190, 80, 50, 50, 50, 30, 44, 31, 40, 80,
		30, 30, 1, 81, 81, 81, 204, 41, 40, 200, 200, 200, 0, 191, 201,
		84, 201, 80, 100, 200, 200, 200, 32, 32, 194, 51, 60, 80,
		50, 50, 50, 21, 31, 40, 81, 201, 201, 81, 81, 211, 211, 101, 81, 82, 80,
		81, 201, 191, 191, 200, 50, 190, 190, 190, 81, 211, 104, 211, 100, 220,
		210, 210, 210, 51, 51, 84, 201, 80, 100, 100, 100, 100, 100,
		192, 201, 80, 200, 211, 211, 84, 51, 51, 50, 120, 100, 210, 80,
		100, 100, 100, 100, 220, 220, 220, 220, 101, 211, 80, 80, 1, 81,
		210, 81, 211, 210, 220, 100, 100, 100, 100, 220, 220, 220, 220,
		101, 211, 80, 80, 01, 81, 210, 81, 212, 211, 200, 190, 190, 190, 191, 201};

unsigned char melody1[] = {
	0, 120,
	130, 140,
	130, 100,
	130, 120,
	0, 120,
	130, 140,
	150, 140,

	130, 120, //8
	0, 120,
	130, 140,
	130, 100,
	130, 120,
	160, 1, 151,
	0, 140,

	130, 141, 151, //15
	181, 1, 0,
	122, 82, 162, 82, 152, 82, 142, 82,
	132, 82, 142, 82, 132, 82, 122, 82,
	132, 82, 142, 82, 152, 82, 142, 82,

	132, 82, 102, 82, 132, 82, 122, 82, //20
	122, 82, 162, 82, 152, 82, 142, 82,
	132, 82, 142, 82, 132, 82, 142, 82,
	132, 82, 142, 82, 152, 82, 142, 82,
	132, 82, 102, 82, 132, 82, 122, 82,

	122, 82, 162, 82, 152, 82, 142, 82, //25
	132, 82, 142, 82, 132, 82, 122, 82,
	132, 82, 142, 82, 152, 82, 142, 82,
	132, 82, 102, 82, 132, 82, 122, 82,
	142, 82, 162, 82, 152, 82, 142, 82,

	132, 82, 142, 82, 132, 82, 122, 82,
	132, 82, 142, 82, 152, 82, 142, 82,
	132, 82, 102, 82, 132, 82, 122, 82,
	152, 152, 182, 161, 2, 1,
	0, 152, 142,

	155, 185, 160, //35
	151, 141,
	151, 182, 161, 2, 1,
	0, 151, 161,
	175, 185,

	165, 2, 171, 1, //40
	151, 182, 161, 2, 1,
	0, 151, 141,
	155, 185, 160,
	151, 141,

	151, 182, 161, 2, 1, //45
	0, 151, 161,
	175, 185,
	81, 111, 101, 131,
	135, 120,

	135, 120, //50
	135, 120,
	141, 131, 121, 101,
	135, 125, 81,
	135, 125, 81,
	135, 125, 81,

	140, 150, 130, 140,
	122, 82, 162, 82, 152, 82, 142, 82, //56 //r
	132, 82, 142, 82, 132, 82, 122, 82,
	132, 82, 142, 82, 152, 82, 142, 82,
	132, 82, 102, 82, 132, 82, 122, 82,

	122, 82, 162, 82, 152, 82, 142, 82, //61
	132, 82, 142, 82, 132, 82, 142, 82,
	132, 82, 142, 82, 152, 82, 142, 82,
	132, 82, 102, 82, 132, 82, 122, 82,
	122, 82, 162, 82, 152, 82, 142, 82,

	132, 82, 142, 82, 132, 82, 122, 82, //66
	132, 82, 142, 82, 152, 82, 142, 82,
	132, 82, 102, 82, 132, 82, 122, 82,
	142, 82, 162, 82, 152, 82, 142, 82,
	132, 82, 142, 82, 132, 82, 122, 82,

	132, 82, 142, 82, 152, 82, 142, 82, //71 //
	132, 82, 102, 82, 131, 123, 133, 143, 153,
	162, 122, 142, 122, 152, 122, 162, 122,
	152, 122, 132, 122, 182, 122, 162, 92,
	160, 0
	/* 
	71, 1, 56, 66, 72, //76
	162, 42, 62, 42, 72, 42, 162, 42,
	152, 32, 52, 32, 102, 32, 162, 12,
	162, 1, 22, 32*/
};
unsigned char melody2[] = {
	8, 81, 190, 81, //1
	200, 81, 200, 58,
	1, 51, 111, 210, 81,
	200, 81, 200, 58,
	0, 81, 190, 81,
	200, 81, 200, 58,

	01, 191, 191, 40, 191, //7
	50, 191, 50, 238,
	0, 81, 190, 81,
	200, 80, 200, 58,
	01, 51, 111, 210, 81,
	200, 81, 200, 58,

	0, 81, 190, 81, //13
	200, 81, 200, 58,
	1, 191, 191, 40, 191,
	50, 191, 50, 231, 231, 241,
	29,
	01, 231, 21, 249,

	01, 231, 231, 241, 39, //20
	01, 231, 31, 29,	   //end 1

	//repeats from 1

	8, 81, 190, 81, //1
	200, 81, 200, 58,
	1, 51, 111, 210, 81,
	200, 81, 200, 5, 8,
	0, 81, 190, 81,
	200, 81, 200, 58,

	1, 191, 191, 40, 191, //7
	50, 191, 50, 238,
	0, 81, 190, 81,
	200, 81, 200, 58,
	1, 51, 111, 210, 81,
	200, 81, 200, 58,

	0, 81, 190, 81, //13
	200, 81, 200, 58,
	1, 191, 191, 40, 191,
	50, 191, 50, 231, 231, 241,
	29,
	1, 231, 21, 249,

	1, 231, 231, 241, 39, //20
	1, 231, 31, 21,		  //end 2
	28, 51, 21,
	111, 210, 81, 134};

int song_len[3] = {156, 388, 176};
char *song = 0;
int time_div[3] = {18, 25, 25};
int bpm = 140;
int wholenote = (60000 * 4) / 140;

int divider = 0, noteDuration = 0;

int music_cnt = 0;
int beat_cnt = 0;
int cur_song_len = 0;

int draw_arr[24][3] = {
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
};
int LED[6][3] = {
	{-1, -1, -1},
	{-1, -1, -1},
	{-1, -1, -1},
	{-1, -1, -1},
	{-1, -1, -1},
	{-1, -1, -1}};

int draw_t = 0;
int *text = 0;
int text_big_one[3] = {813070, 816170, 717121};
int text_big_two[5] = {816070, 816120, 816170, 712151, 1217081};
int text_big_thre[4] = {816070, 816120, 816170, 717151};
int text_big_four[3] = {816120, 717151, 712081};
int text_small_one[3] = {2000, 4040, 4021};
int text_small_two[5] = {4000, 4020, 4040, 304010, 1030};
int text_small_thre[4] = {4000, 4020, 4040, 4031};
int text_small_four[3] = {4020, 3001, 50311};
int text_small_six[5] = {4000, 4020, 4040, 5001, 205031};
int text_small_seven[4] = {4000, 305011, 304010, 203020};
int text_small_eight[5] = {4000, 4020, 4040, 5001, 5031};
int text_small_nine[5] = {4000, 4020, 4040, 3001, 5031};

int text_C[3] = {4000, 4040, 104001};
int text_A[4] = {103000, 103020, 5001, 5031};
int text_S[5] = {4000, 4020, 4040, 1010, 304031};
int text_T[2] = {5000, 105021};
int text_L[2] = {4040, 4001};
int text_E[4] = {104000, 104020, 104040, 5001};
int text_I[1] = {5011};
int text_N[5] = {5001, 5041, 102010, 203020, 304030};
int text_H[3] = {5001, 5031, 103010};
int text_K[6] = {5001, 102020, 203010, 304000, 203030, 304040};
int text_Y[5] = {1000, 405000, 102010, 304010, 205021};
int text_R[6] = {5001, 104000, 104020, 304010, 203030, 304040};
int text_B[7] = {5001, 103000, 104020, 104040, 203010, 304030, 304040};
int text_O[4] = {5001, 104000, 104040, 5031};
int text_D[6] = {5001, 103000, 103040, 304020, 304010, 304030};
int text_P[4] = {5001, 104000, 104020, 304010};
int text_G[5] = {5001, 104000, 204020, 104040, 304030};
int text_M[5] = {5001, 5041, 204021, 102010, 304010};
int text_V[3] = {4001, 4031, 103040};

// Castle in the Sky
// RANK
int text_title0[17] = {0, 1, 2, 3, 4, 5, -1, 6, 7, -1, 3, 8, 5, -1, 2, 9, 10};
// Bloddy tear
int text_title1[11] = {12, 4, 13, 14, 14, 10, -1, 3, 5, 1, 11};
// green hill
int text_title2[10] = {16, 6, 5, 5, 7, -1, 8, 6, 4, 4};
int song_title_len[3] = {17, 11, 10};

int score[3] = {0, 0, 0};
int mode = 0;
int menu_mode = 0;
int chs_song = 0;
int chs_degree = 2;
//       new_block_max / game_offset / drop_max
int degree_arg[4] = {10019, 3014, 1311, 822};
int intr_btn0 = 0;
int intr_btn1 = 0;
int intr_btn2 = 0;
int intr_btn3 = 0;
int intr_btn0_cnt = 0;
int intr_btn1_cnt = 0;
int intr_btn2_cnt = 0;
int intr_btn3_cnt = 0;
int intr_cmt0 = 0;
int cmt_cnt = 0;
int drop_cnt = 0;
int drop_cnt_max = 4;
int new_block_max = 12;
int text_offset_max = 4;
int new_block_flag = 0;
int drop_interv_min = 0;
int drop_interv_max = 0;
int feedback = -1;
int game_offset = 1;
int text_offset = 0;
int text_index = 0;

int top_i = 0;
int song_num = 3;
int ori_offsetX[3] = {8, 5, 2};
int ori_offsetY[3] = {-3, 9, 21};
int offsetX[3] = {8, 5, 2};
int offsetY[3] = {-3, 9, 21};
int up_flag = 0;
int down_flag = 0;
int move_cnt = 0;

void main(void)
{
	R_PG_Timer_Start_CMT_U0_C0();

	R_PG_IO_PORT_Set_P2();
	R_PG_IO_PORT_Set_P7();
	R_PG_IO_PORT_Set_P9();
	R_PG_IO_PORT_Set_PA();
	R_PG_IO_PORT_Set_PB();

	R_PG_ExtInterrupt_Set_IRQ0();
	R_PG_ExtInterrupt_Set_IRQ1();
	R_PG_ExtInterrupt_Set_IRQ3();
	R_PG_ExtInterrupt_Set_IRQ5();

	R_PG_IO_PORT_Write_P2(0xff);
	R_PG_IO_PORT_Write_P7(0xff);
	R_PG_IO_PORT_Write_P9(0xff);
	R_PG_IO_PORT_Write_PA(0xff);
	R_PG_IO_PORT_Write_PB(0xff);

	while (1)
	{
		// display mode
		switch (mode)
		{
		// ¿ïÃö
		case 0:
			drawChsMenu();
			break;
		// ­Ë¼Æ
		case 1:
			drawRecip();
			break;
		// Game
		case 2:
			drawGame();
			break;
		// stop
		case 3:
			drawStop();
			break;
		case 4:
			drawEnd();
		}
		draw();
		// button click
		if (intr_btn0)
		{
			btnClick(0, &intr_btn0_cnt);
			intr_btn0 = 0;
		}
		if (intr_btn1)
		{
			btnClick(1, &intr_btn1_cnt);
			intr_btn1 = 0;
		}
		if (intr_btn2)
		{
			btnClick(2, &intr_btn2_cnt);
			intr_btn2 = 0;
		}
		if (intr_btn3)
		{
			btnClick(3, &intr_btn3_cnt);
			intr_btn3 = 0;
		}
		// timer
		if (intr_cmt0)
		{
			timeUp();
			intr_cmt0 = 0;
		}
	}
}

// intr
void Irq0IntFunc()
{
	intr_btn0 = 1;
}

void Irq1IntFunc()
{
	intr_btn1 = 1;
}

void Irq3IntFunc()
{
	intr_btn2 = 1;
}

void Irq5IntFunc()
{
	intr_btn3 = 1;
}

void Cmt0IntFunc()
{
	intr_cmt0 = 1;
}

void btnClick(int index, int *intr_cnt)
{
	int i = 5;
	if (++*intr_cnt < 2)
	{
		feedback = index;

		switch (mode)
		{
		case 0:
			switch (index)
			{
			case 0:
				if (menu_mode)
				{
					if (--chs_degree <= 0)
						chs_degree = 1;
				}
				else
				{
					if (--chs_song < 0)
						chs_song = 0;
					text_offset = 0;
					text_index = 0;
				}
				break;
			case 1:
				if (menu_mode)
				{
					selSong();
					clearGame();
				}
				else
				{
					menu_mode = 1;
					chs_degree = 2;
				}
				break;
			case 2:
				if (menu_mode)
				{
					if (++chs_degree > 4)
						chs_degree = 4;
				}
				else
				{
					if (++chs_song > 2)
						chs_song = 2;
					text_offset = 0;
					text_index = 0;
				}
				break;
			case 3:
				if (menu_mode)
				{
					text_index = 0;
					text_offset = 0;
					menu_mode = 0;
				}
			}
			break;
		case 1:
			break;
		case 2:
			if (index != 3)
			{
				while (i >= 0)
				{
					if (LED[i][index] >= 18)
					{
						if (LED[i][index] >= 20 && LED[i][index] <= 22)
							++score[0];
						else
							++score[1];
						LED[i][index] = -1;

						break;
					}
					else if (LED[i][index] != -1)
						break;
					--i;
				}
			}
			else
			{
				mode = 3;
				chs_degree = 2;
				R_PG_Timer_StopModule_MTU_U0();
				//R_PG_Timer_Set_MTU_U0_C2();
				//R_PG_Timer_HaltCount_MTU_U0_C2();
			}
			break;
		case 3:
			switch (index)
			{
			case 0:
				if (--chs_degree < 1)
					chs_degree = 3;
				break;
			case 1:
				switch (chs_degree - 1)
				{
				// replay
				case 0:
					clearGame();
					break;
				// continue
				case 1:
					cmt_cnt = 60;
					mode = 1;
					break;
				// leave
				case 2:
					mode = 0;
					menu_mode = 0;
					text_offset = 0;
					text_index = 0;
				}
				break;
			case 2:
				if (++chs_degree > 3)
					chs_degree = 1;
			}
			break;
		case 4:
			switch (index)
			{
			case 0:
				if (--chs_degree < 0)
					chs_degree = 0;
				break;
			case 1:
				mode = 0;
				menu_mode = 0;
				text_offset = 0;
				text_index = 0;
				break;
			case 2:
				if (++chs_degree > 2)
					chs_degree = 2;
				break;
			}
		}
	}
	else
		*intr_cnt = 0;

	delay(20000);
}

void timeUp()
{
	int i, j, k, m;
	int *cur_song;

	switch (mode)
	{
	case 0:
		if (++cmt_cnt >= text_offset_max)
		{
			cmt_cnt = 0;
			if (--text_offset < -5)
			{
				text_offset = 0;
				if (++text_index > song_title_len[chs_song])
				{
					text_index = 0;
					text_offset = 24;
				}
			}
			break;
		}
		break;
	case 1:
		if (--cmt_cnt < 0)
		{
			mode = 2;
			R_PG_Timer_Set_MTU_U0_C2();
			R_PG_Timer_StartCount_MTU_U0_C2();
		}

		break;
	case 2:
		// new block flag
		if (++cmt_cnt >= new_block_max && music_cnt < cur_song_len * 0.95)
		{
			cmt_cnt = 0;
			new_block_flag = 1;
		}
		// LED drop
		if (++drop_cnt > drop_cnt_max)
		{
			drop_cnt = 0;
			for (i = 5; i >= 0; --i)
			{
				for (j = 0; j < 3; ++j)
				{
					if (LED[i][j] != -1)
						LED[i][j] += game_offset;
					if (LED[i][j] >= 24)
					{
						LED[i][j] = -1;
						++score[2];
					}
				}
			}
		}
		// create new block
		if (new_block_flag)
		{
			k = 0;
			m = music_cnt + 1;

			cur_song = song;
			while (k < drop_interv_max)
			{
				switch (*cur_song % 10)
				{
				case 0:
					k += 8;
					break;
				case 1:
					k += 4;
					break;
				case 2:
					k += 2;
					break;
				case 3:
					k += 1;
					break;
				case 4:
					k += 12;
					break;
				case 5:
					k += 6;
					break;
				case 6:
					k += 3;
					break;
				case 7:
					k += 1;
					break;
				case 8:
					k += 16;
					break;
				case 9:
					k += 2;
				}

				++cur_song;
				if (k >= drop_interv_min && k <= drop_interv_max)
				{
					k = drop_interv_max;
					j = rand() % 3;

					if ((LED[0][j] >= 6 || LED[0][j] == -1) && LED[5][j] == -1)
					{
						// clear
						new_block_flag = 0;
						i = 4;
						while (i-- >= 0)
						{
							if (LED[i][j] != -1)
								LED[i + 1][j] = LED[i][j];
						}
						LED[0][j] = 2;
					}
				}
			}
		}
		// music
		if (++beat_cnt > noteDuration)
		{
			beat_cnt = 0;
			if (++music_cnt > cur_song_len)
			{
				R_PG_Timer_StopModule_MTU_U0();
				//R_PG_Timer_Set_MTU_U0_C2();
				//R_PG_Timer_HaltCount_MTU_U0_C2();
				chs_degree = 0;
				mode = 4;
			}
			++song;
			divider = *song % 10;
			switch (divider)
			{
			case 0:
				divider = 4;
				break;
			case 1:
				divider = 8;
				break;
			case 2:
				divider = 16;
				break;
			case 3:
				divider = 32;
				break;
			case 4:
				divider = -4;
				break;
			case 5:
				divider = -8;
				break;
			case 6:
				divider = -16;
				break;
			case 7:
				divider = -32;
				break;
			case 8:
				divider = 2;
				break;
			case 9:
				divider = -1;
				break;
			}

			if (divider > 0)
			{
				noteDuration = wholenote / divider;
			}
			else
			{
				noteDuration = (wholenote) / abs(divider);
				noteDuration *= 1.5;
			}

			noteDuration /= time_div[chs_song];

			R_PG_Timer_SetTGR_A_MTU_U0_C2(NOTE[*song / 10]);
			R_PG_Timer_SetTGR_B_MTU_U0_C2(NOTE[*song / 10] / 2);
		}
		break;
	}
}

void draw()
{
	int i, j, k;
	for (i = 0; i < 24; ++i)
	{
		clearX();
		clearY();
		chsY(i);
		for (j = 2; j >= 0; --j)
		{
			clearX();
			chsBlock(j);
			if (j == 1 && i < 8)
			{
				for (k = 7; k >= 0; --k)
				{
					clearX();
					if (draw_arr[i][j] / pow(k))
					{
						chsX(k);
						draw_arr[i][j] %= pow(k);
					}
					delay(750);
				}
				draw_arr[i][j] = 0;
			}
			else
			{
				for (k = 7; k >= 0; --k)
				{
					if (draw_arr[i][j] / pow(k))
					{
						chsX(k);
						draw_arr[i][j] %= pow(k);
					}
				}
				draw_arr[i][j] = 0;

/* 
				if (j == 0)
					delay(200);
				else
					delay(1000);*/
				delay(500);
			}
		}
	}
}

void drawChsMenu()
{
	int i, j;

	switch (menu_mode)
	{
	case 0:
		i = 0;
		for (j = text_index; j < text_index + 5; ++j)
		{
			if (j >= song_title_len[chs_song] || text_offset + i * 5 > 23)
				break;
			switch (chs_song)
			{
			case 0:
				chsText(text_title0[j]);
				draw_arr[23][1] = 100;
				draw_arr[22][1] = 1110;
				break;
			case 1:
				chsText(text_title1[j]);
				draw_arr[0][1] = 100;
				draw_arr[1][1] = 1110;
				draw_arr[23][1] = 100;
				draw_arr[22][1] = 1110;
				break;
			case 2:
				chsText(text_title2[j]);
				draw_arr[0][1] = 100;
				draw_arr[1][1] = 1110;
				break;
			}
			drawText(text_offset + i++ * 5, 10);
		}
		break;
	case 1:
		chsText(11);
		drawText(2, 2);
		chsText(1);
		drawText(7, 2);
		chsText(7);
		drawText(12, 2);
		chsText(9);
		drawText(18, 2);

		chsBigNum(chs_degree);
		drawText(0, 5);
		break;
	}
}

void chsBigNum(int i)
{
	switch (i)
	{
	case 1:
		draw_t = 3;
		text = text_big_one;
		break;
	case 2:
		draw_t = 5;
		text = text_big_two;
		break;
	case 3:
		draw_t = 4;
		text = text_big_thre;
		break;
	case 4:
		draw_t = 3;
		text = text_big_four;
		break;
	}
}

void chsNum(int i)
{
	switch (i)
	{
	case 0:
		chsText(13);
		break;
	case 1:
		draw_t = 3;
		text = text_small_one;
		break;
	case 2:
		draw_t = 5;
		text = text_small_two;
		break;
	case 3:
		draw_t = 4;
		text = text_small_thre;
		break;
	case 4:
		draw_t = 3;
		text = text_small_four;
		break;
	case 5:
		chsText(2);
		break;
	case 6:
		draw_t = 5;
		text = text_small_six;
		break;
	case 7:
		draw_t = 4;
		text = text_small_seven;
		break;
	case 8:
		draw_t = 5;
		text = text_small_eight;
		break;
	case 9:
		draw_t = 5;
		text = text_small_nine;
		break;
	}
}

void chsText(int i)
{
	switch (i)
	{
	case 0:
		draw_t = 3;
		text = text_C;
		break;
	case 1:
		draw_t = 4;
		text = text_A;
		break;
	case 2:
		draw_t = 5;
		text = text_S;
		break;
	case 3:
		draw_t = 2;
		text = text_T;
		break;
	case 4:
		draw_t = 2;
		text = text_L;
		break;
	case 5:
		draw_t = 4;
		text = text_E;
		break;
	case 6:
		draw_t = 1;
		text = text_I;
		break;
	case 7:
		draw_t = 5;
		text = text_N;
		break;
	case 8:
		draw_t = 3;
		text = text_H;
		break;
	case 9:
		draw_t = 6;
		text = text_K;
		break;
	case 10:
		draw_t = 5;
		text = text_Y;
		break;
	case 11:
		draw_t = 6;
		text = text_R;
		break;
	case 12:
		draw_t = 7;
		text = text_B;
		break;
	case 13:
		draw_t = 4;
		text = text_O;
		break;
	case 14:
		draw_t = 6;
		text = text_D;
		break;
	case 15:
		draw_t = 4;
		text = text_P;
		break;
	case 16:
		draw_t = 5;
		text = text_G;
		break;
	case 17:
		draw_t = 5;
		text = text_M;
		break;
	case 18:
		draw_t = 3;
		text = text_V;
		break;
	}
}

void drawText(int offsetX, int offsetY)
{
	int a0, a1, a2, a3;
	int j, k;

	while (draw_t-- > 0)
	{
		a0 = *text / 1000000 * 10 + *text % 1000000 / 100000;
		a1 = (*text % 100000 / 10000) * 10 + *text % 10000 / 1000;
		a2 = (*text % 1000 / 100) * 10 + (*text % 100 / 10);
		a3 = *text % 10;
		text++;

		// x
		if (!a3)
		{
			a0 += offsetX;
			if (a0 > 23)
				continue;
			a1 += offsetX;
			if (a1 < 0)
				continue;
			a2 += offsetY;

			k = a0;
			for (j = 0; j < 3; ++j)
			{
				if (k >= 8 * j + 8)
					continue;
				while (k < 8 * j + 8 && k < a1)
					draw_arr[a2][j] += pow(k++ % 8);
			}
		}
		// y
		else
		{
			a0 += offsetY;
			if (a0 > 23)
				continue;
			a1 += offsetY;
			if (a1 < 0)
				continue;
			a2 += offsetX;

			for (j = a0; j < a1; ++j)
				draw_arr[j][a2 / 8] += pow(a2 % 8);
		}
	}
}

void drawRecip()
{
	chsBigNum(cmt_cnt / 20 + 1);
	drawText(0, 0);
}

void drawGame()
{
	int i, j, m, k, cntY;
	// draw game block
	for (i = 0; i < 6; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			if (LED[i][j] != -1)
			{
				for (cntY = LED[i][j]; cntY < LED[i][j] + 3; ++cntY)
				{
					if (cntY >= 24)
						break;
					k = 1;
					m = 0;
					while (m++ < 6)
						draw_arr[cntY][j] += pow(k++);
				}
			}
		}
	}
	// draw click feedback
	if (feedback != -1)
	{
		for (i = 0; i < 24; ++i)
		{
			draw_arr[i][feedback] += pow(0);
			draw_arr[i][feedback] += pow(7);
		}
		feedback = -1;
	}
	// draw progress bar
	m = music_cnt / (cur_song_len / 24 + 1);
	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			if (j > m)
			{
				i = 3;
				break;
			}
			draw_arr[0][i] += pow(j);
		}
		m -= 7;
	}
}

void clearGame()
{
	int i = 0;
	for (; i < 6; ++i)
	{
		LED[i][0] = -1;
		LED[i][1] = -1;
		LED[i][2] = -1;
	}

	score[0] = 0;
	score[1] = 0;
	score[2] = 0;

	music_cnt = 0;
	cmt_cnt = 60;
	beat_cnt = 0;
	mode = 1;
}

void drawStop()
{
	int i;
	chsText(11);
	drawText(10, 1);

	chsText(0);
	drawText(10, 9);

	chsText(4);
	drawText(10, 17);

	for (i = 0; i < 3; ++i)
	{
		draw_arr[0 + (chs_degree - 1) * 8][i] = 11111111;
		draw_arr[6 + (chs_degree - 1) * 8][i] = 11111111;
	}
}

void drawEnd()
{
	switch (chs_degree)
	{
	case 0:
		chsText(15);
		drawText(10, 3);
		break;
	case 1:
		chsText(16);
		drawText(10, 3);
		break;
	case 2:
		chsText(17);
		drawText(10, 3);
	}

	if (score[chs_degree] > 100)
	{
		chsNum(score[chs_degree] / 100);
		drawText(5, 12);
		chsNum(score[chs_degree] / 10 % 10);
		drawText(10, 12);
		chsNum(score[chs_degree] % 10);
		drawText(15, 12);
	}
	else if (score[chs_degree] > 10)
	{
		chsNum(score[chs_degree] / 10);
		drawText(8, 12);
		chsNum(score[chs_degree] % 10);
		drawText(13, 12);
	}
	else
	{
		chsNum(score[chs_degree] % 10);
		drawText(10, 12);
	}
}

void drawAll()
{
	int i, j;
	for (i = 0; i < 24; ++i)
	{
		for (j = 0; j < 3; ++j)
			draw_arr[i][j] = 11111111;
	}
}

void selSong()
{
	switch (chs_song)
	{
	case 0:
		song = melody;
		break;
	case 1:
		song = melody1;
		break;
	case 2:
		song = melody2;
	}

	cur_song_len = song_len[chs_song];
	srand(cur_song_len * game_offset);
	drop_interv_max = drop_cnt_max * time_div[chs_song] * 22 * bpm * 16 / game_offset / 60 / 1000;
	drop_interv_min = drop_interv_max - 10;

	new_block_max = degree_arg[chs_degree - 1] / 100;
	game_offset = degree_arg[chs_degree - 1] / 10 % 10;
	drop_cnt_max = degree_arg[chs_degree - 1] % 10;
}

void delay(int i)
{
	int cnt = 0;
	while (++cnt < i)
		;
}

void chsBlock(int i)
{

	R_PG_IO_PORT_Write_PA(0);

	switch (i)
	{
	case 0:
		R_PG_IO_PORT_Write_PA5(1);
		break;
	case 1:
		R_PG_IO_PORT_Write_PA4(1);
		break;
	case 2:
		R_PG_IO_PORT_Write_PA2(1);
	}
}

void chsX(int i)
{
	switch (i)
	{
	case 0:
		R_PG_IO_PORT_Write_P76(0);
		break;
	case 1:
		R_PG_IO_PORT_Write_P75(0);
		break;
	case 2:
		R_PG_IO_PORT_Write_P74(0);
		break;
	case 3:
		R_PG_IO_PORT_Write_P73(0);
		break;
	case 4:
		R_PG_IO_PORT_Write_P72(0);
		break;
	case 5:
		R_PG_IO_PORT_Write_P24(0);
		break;
	case 6:
		R_PG_IO_PORT_Write_P23(0);
		break;
	case 7:
		R_PG_IO_PORT_Write_P22(0);
	}
}

void chsY(int i)
{
	if (i <= 7)
	{
		if (i >= 4)
		{
			R_PG_IO_PORT_Write_P93(0);
			i -= 4;
		}
		if (i >= 2)
		{
			R_PG_IO_PORT_Write_P92(0);
			i -= 2;
		}
		if (i)
		{
			R_PG_IO_PORT_Write_P91(0);
		}
	}
	else
	{
		i -= 8;
		if (i >= 8)
		{
			R_PG_IO_PORT_Write_PB5(0);
			i -= 8;
		}
		if (i >= 4)
		{
			R_PG_IO_PORT_Write_PB2(0);
			i -= 4;
		}
		if (i >= 2)
		{
			R_PG_IO_PORT_Write_PB1(0);
			i -= 2;
		}
		if (i)
		{
			R_PG_IO_PORT_Write_PB0(0);
		}
		R_PG_IO_PORT_Write_P94(0);
		R_PG_IO_PORT_Write_PB6(0);
	}
}

void clearX()
{
	R_PG_IO_PORT_Write_P2(0xff);
	R_PG_IO_PORT_Write_P7(0xff);
}

void clearY()
{
	R_PG_IO_PORT_Write_P9(0xff);
	R_PG_IO_PORT_Write_PB(0xff);
}

int pow(int m)
{
	int i = 0, x = 1;
	if (!m)
		return 1;

	for (; i < m; ++i)
		x *= 10;
	return x;
}

#ifdef __cplusplus
void abort(void)
{
}
#endif
