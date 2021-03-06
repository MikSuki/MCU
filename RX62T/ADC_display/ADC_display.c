
/***********************************************************************/
/*                                                                     */
/*  FILE        :adc_display.c                                         */
/*  DATE        :Thu, Mar 28, 2019                                     */
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
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void get_value();

int i = 0;
int start = 1;
int timeout = 0;
int blink = 0;

int pos = 0;
char mode[] = {0xf, ~0x8, ~0x4, ~0x2};
int min = 0;
int sec = 0;
int time[] = {0, 0, 0, 0};
char number[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};

void main(void)
{
	// enable	
	R_PG_Clock_Set();
	R_PG_IO_PORT_Set_P7();
	R_PG_IO_PORT_Set_PB2();
	R_PG_IO_PORT_Set_P9();
	R_PG_Timer_Set_GPT_U0();
	R_PG_Timer_Set_GPT_U0_C0();
	R_PG_ADC_12_Set();
	R_PG_ADC_12_Set_S12ADA0();
	R_PG_Timer_StartCount_GPT_U0_C0();
	R_PG_ExtInterrupt_Set_IRQ0();
	// init status
	R_PG_IO_PORT_Write_P7(0xff);
	R_PG_IO_PORT_Write_P9(0xff);
	
	
	while(1){
		if(!timeout){
			R_PG_IO_PORT_Write_P9(mode[pos]);
			R_PG_IO_PORT_Write_P7(~number[time[pos]]);
			
			if(++i > 1000){
				i = 0;	
				if(++pos >= 4){
					pos = 0;
					R_PG_IO_PORT_Write_PB2(0);
				}
				else
					R_PG_IO_PORT_Write_PB2(1);
			}
		}
	}
}

void Irq0IntFunc(){
	start = !start;
}


void S12ad0IntFunc(){
	unsigned int v = 0;
	if(!start) return;
	R_PG_ADC_12_GetResult_S12ADA0(&v);
	v = v  * 5 *1000/ 4096 ;
	get_value(v);
}

void get_value(v){
	time[0] = v / 1000;
	time[1] = (v / 100) % 10;
	time[2] = (v / 10) % 10;
	time[3] = v % 10;
}


#ifdef __cplusplus
void abort(void)
{
	
}
#endif


