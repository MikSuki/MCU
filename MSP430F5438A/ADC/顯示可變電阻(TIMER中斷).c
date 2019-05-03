#include <msp430.h>

#define RSPIN   (0x04)           //P11.2
#define RWPIN   (0x02)           //P11.1
#define EPIN    (0x01)           //P11.0

unsigned char XPOS, YPOS;
volatile unsigned int results;
float v = 0;
char upLCD[16] = "results =       ";
char downLCD[16] = "    v   =  .    ";

int main(void) {
	unsigned int i = 0;
	unsigned char j;
	int k = 0;
	WDTCTL = WDTPW + WDTHOLD;
	//button
	P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
	P2REN |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
	P2OUT |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
	// LCM Backlit enable
	P9DIR |= BIT6 + BIT7;
	P9DS |= BIT6 + BIT7;
	P9OUT &= ~BIT7; //K
	P9OUT |= BIT6; //A
	//
	P10OUT = 0;
	P11OUT = 0;
	P10DIR |= 0XFF;
	P11DIR |= 0XFF;
	P11OUT &= ~EPIN;
	//Timer0
	TA0CCTL0 = CCIE;
	TA0CCR0 = 1000000 / 8 / 2;
	TA0CTL = TASSEL_2+ ID_3 + MC_2;
	//ADC設定
	ADC12CTL0 = ADC12ON + ADC12SHT02 + ADC12REFON + ADC12REF2_5V; // 16 ADC12CLK cycles, Reference on, Internal ref = 2.5V
	ADC12CTL1 = ADC12SHP; // enable sample timer
	ADC12CTL2 = ADC12RES_2 + ADC12REFOUT; //12BIT,REF OUT continuous
	ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_15; // VeREF+, ADC i/p ch A15 (AD15連到可變電阻)
	REFCTL0 &= ~REFMSTR;
	ADC12IE = 0x001; // ADC_IFG upon conv result-ADCMEMO
	__delay_cycles(35); // 35us delay to allow Ref to settle
	// based on default DCO frequency.
	// See Datasheet for typical settle time.
	P5SEL |= BIT0; // REF out
	ADC12CTL0 |= ADC12ENC; // ADC12_A enabled
	__bis_SR_register(GIE);

	return 0;
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
	ADC12CTL0 |= ADC12SC;
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void) {
	switch (__even_in_range(ADC12IV, 34)) {
	case 6: // Vector 6: ADC12IFG0
		results = ADC12MEM0;
		v = 2.5 * results / 4096;
		display();
		break;
	}
}

void display() {
	upLCD[10] = results / 1000 + '0';
	upLCD[11] = results / 100 % 10 + '0';
	upLCD[12] = results / 10 % 10 + '0';
	upLCD[13] = results % 10 + '0';
	downLCD[10] = (int) v % 10 + '0';
	downLCD[12] = (int) (v * 10) % 10 + '0';
	downLCD[13] = (int) (v * 100) % 10 + '0';
	downLCD[14] = (int) (v * 1000) % 10 + '0';
	lcdreset();
	//lcdwda(0, 0, upLCD);
	lcdwda(0, 1, downLCD);
}

void delay(unsigned int t) {
	unsigned int i, j;
	for (i = 0; i < t; i++)
		for (j = 0; j < 10; j++)
			;

}

void lcdwaitidle(void) {
	P10DIR = 0x00;
	P11OUT &= ~RSPIN;
	P11OUT |= RWPIN;
	P11OUT |= EPIN;
	while ((P10IN & 0x80) == 0x80)
		;
	P11OUT &= ~EPIN;
	P10DIR |= 0Xff;
}

void lcdwcn(unsigned char c) {
	P11OUT &= ~RSPIN;
	P11OUT &= ~RWPIN;
	P10OUT = c;
	P11OUT |= EPIN;
	_NOP();
	P11OUT &= ~EPIN;
}

void lcdwc(unsigned char c) {
	lcdwaitidle();
	lcdwcn(c);
}

void lcdwd(unsigned char d) {
	lcdwaitidle();
	P11OUT |= RSPIN;
	P11OUT &= ~RWPIN;
	P10OUT = d;
	P11OUT |= EPIN;
	_NOP();
	P11OUT &= ~EPIN;
}

void lcdpos(void) {
	XPOS &= 0x0f;
	YPOS &= 0x03;
	if (YPOS == 0x00)
		lcdwc(XPOS | 0x80);
	else if (YPOS == 0x01)
		lcdwc((XPOS + 0x40) | 0x80);
}

void lcdreset() {
	delay(150);
	lcdwcn(0x38);
	delay(50);
	lcdwcn(0x38);
	delay(50);
	lcdwcn(0x38);
	lcdwc(0x38);
	lcdwc(0x08);
	lcdwc(0x01);
	lcdwc(0x06);
	lcdwc(0x0c);
}

void lcdfill(unsigned char n) {
	for (YPOS = 0; YPOS < 2; YPOS++)
		for (XPOS = 0; XPOS < 16; XPOS++) {
			lcdpos();
			lcdwd(n);
		}
}

void lcdwda(unsigned char x, unsigned char y, unsigned char *s) {
	YPOS = y;
	for (XPOS = x; XPOS < 16; XPOS++) {
		lcdpos();
		lcdwd(*s);
		s++;
		delay(400);
	}
}