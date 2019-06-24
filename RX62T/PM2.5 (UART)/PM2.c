
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void get_value();

int i = 0;
int show_time = 0;
int start = 1;
int timeout = 0;
int blink = 0;

int pos = 0;
char mode[] = {0xf, ~0x8, ~0x4, ~0x2};
int min = 0;
int sec = 0;
int time[] = {0, 0, 0, 0};
char number[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};



int rec_flag = 1;
unsigned char data[24];
int value = 0;

void main(void)
{
	
	R_PG_Clock_Set();
	R_PG_IO_PORT_Set_P2();
	R_PG_IO_PORT_Set_P7();
	R_PG_IO_PORT_Set_P9();
	R_PG_SCI_Set_C0();
	R_PG_Timer_Start_CMT_U0_C0();
	
	
	while(1){
		R_PG_SCI_ReceiveAllData_C0(&data, 24);
		if(rec_flag){
			rec_flag = 0;
			value = (data[12] << 8) + data[13];
			get_value(value);				
		}
	}
}

void get_value(v){
	time[0] = v / 1000;
	time[1] = (v / 100) % 10;
	time[2] = (v / 10) % 10;
	time[3] = v % 10;
}

void Sci0ReFunc(){
	if(data[0] == 0x42 && data[1] == 0x4d)
		rec_flag = 1;
}

void Sci0EeFunc(){
	
}

void Cmt0IntFunc(){
	if(++show_time < 1) return;
	else show_time = 0;	

	if(++i > 4)
		i = 0;
	R_PG_IO_PORT_Write_P9(mode[i]);
	R_PG_IO_PORT_Write_P7(~number[time[i]]);	
}

#ifdef __cplusplus
void abort(void)
{

}
#endif