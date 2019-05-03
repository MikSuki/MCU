// Btn 0 -> 小時
// Btn 1 -> 分鐘
// Btn 2 -> 秒
// Btn 3 -> 選擇調時鐘、鬧鐘或不調
// Btn 4 -> 開始 / 暫停 (alarm)
// 任意鍵停止鬧鐘
#include "msp430x54x.h"
#include <stdio.h>
#define RSPIN   (0x04)           //P11.2
#define RWPIN   (0x02)           //P11.1
#define EPIN    (0x01)           //P11.0
unsigned char XPOS, YPOS;
int isAlarm = 0;
int timeUp = 0;
int mode = 0;
int pos = 0;
int ah = 0, am = 0;
char LCD[2][17];
void display() {
	sprintf(LCD[0], "  %d%d : %d%d : %d%d  ", RTCHOUR / 16, RTCHOUR % 16,
	RTCMIN / 16, RTCMIN % 16, RTCSEC / 16, RTCSEC % 16);
	if (isAlarm)
		sprintf(LCD[1], "  %d%d : %d%d : 00 %c", (RTCAHOUR - 0x80) / 16,
		RTCAHOUR % 16, (RTCAMIN - 0x80) / 16, RTCAMIN % 16, 'A');
	else
		sprintf(LCD[1], "  %d%d : %d%d : 00  ", RTCAHOUR / 16, RTCAHOUR % 16,
		RTCAMIN / 16, RTCAMIN % 16);
	lcdreset();
	lcdwda(0, 0, LCD[0]);
	lcdwda(0, 1, LCD[1]);
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
void blink() {
	XPOS = pos;
	YPOS = mode - 1;
	lcdpos();
	lcdwc(0xf);
}
void alarm() {
	isAlarm = !isAlarm;
	// enable / disable  AE
	RTCAHOUR ^= RTCAE;
	RTCAMIN ^= RTCAE;
}
void main(void) {
	WDTCTL = WDTPW + WDTHOLD;             // Stop Watchdog Timer
	// LCD
	// LCM Backlit enable
	P9DIR |= BIT6 + BIT7;
	P9DS |= BIT6 + BIT7;
	P9OUT &= ~BIT7; //K
	P9OUT |= BIT6; //A
	P10OUT = 0;
	P11OUT = 0;
	P10DIR |= 0XFF;
	P11DIR |= 0XFF;
	P11OUT &= ~EPIN;
	// button
	P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
	P2REN |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
	P2OUT |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
	P2IE = BIT0 + BIT1 + BIT2 + BIT3 + BIT4;  //interrupt
	P2IES = BIT0 + BIT1 + BIT2 + BIT3 + BIT4;
	// timer
	P8DIR |= 0x60;
	P8SEL |= 0x60;
	TA1CCTL0 &= !CCIE;
	TA1CCR0 = 0;
	TA1CCTL1 = OUTMOD_7;
	TA1CCR1 = 0;
	TA1CTL = TASSEL_2 + MC_1;
	// RTC
	P7SEL |= 0x03;                            // XT1
	// Loop until 32kHz crystal stabilizes
	while ((SFRIFG1 & OFIFG))              // Test oscillator fault flag
	{
		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
		SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}
	RTCCTL01 =
	RTCBCD + RTCHOLD + RTCMODE + RTCTEVIE + RTCRDYIE + RTCAIE;
	RTCPS1CTL = RT1PSIE + RT1IP_3;
	// RTC enable, BCD mode,
	// alarm every Minute,
	// enable RTC interrupt
	// Init time
	RTCSEC = 0x56;                       // Set Seconds
	RTCMIN = 0x32;                       // Set Minutes
	RTCHOUR = 0x10;                       // Set Hours
	RTCAHOUR = 0x10;                          // RTC Hour Alarm
	RTCAMIN = 0x33;                           // RTC Minute Alarm
	ah = 10;
	am = 33;
	RTCCTL01 &= ~RTCHOLD;                 // Enable RTC
	__bis_SR_register(LPM3_bits + GIE);   // Enter LPM3 w/ interrupt
}
// Basic Timer interrupt service routine
#pragma vector=RTC_VECTOR
__interrupt void RTC(void) {
	switch (__even_in_range(RTCIV, 16)) {
	case 2:
		RTCCTL01 &= ~RTCRDYIFG;
		display();
		if (mode)
			blink();
		break;
	case 6:
		timeUp = 1;
		TA1CCTL0 = CCIE;
		TA1CCR0 = 2047;
		TA1CCR1 = 1024;
		break;
	}
}
// BUTTON INTERRUPT
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
	if (timeUp) {
		timeUp = 0;
		// stop music
		TA1CCTL0 &= ~CCIE;
		TA1CCR0 = 0;
		TA1CCR1 = 0;
		alarm();
		RTCAHOUR |= ah / 10 * 16 + ah % 10;
		RTCAMIN |= am / 10 * 16 + am % 10;
		return;
	}
	int t, maxT = 0;
	volatile unsigned char *p;
	int *pA = 0;
	switch (__even_in_range(P2IV, 16)) {
	// hour
	case 2:
		if (!mode)
			return;
		maxT = 24;
		if (mode == 1)
			p = &RTCHOUR;
		else{
			p = &RTCAHOUR;
			pA = &ah;
		}

		pos = 0x3;
		break;
		// min
	case 4:
		if (!mode)
			return;
		maxT = 60;
		if (mode == 1)
			p = &RTCMIN;
		else{
			p = &RTCAMIN;
			pA = &am;
		}

		pos = 0x8;
		break;
		// sec
	case 6:
		if (!mode)
			return;
		if (mode == 1) {
			maxT = 60;
			p = &RTCSEC;
		}
		pos = 0xd;
		break;
		// mode
	case 8:
		if (++mode > 2)
			mode = 0;
		else if (mode == 1)
			pos = 0x0;
		else if (mode == 2)
			pos = 0x0;
		break;
		// start / pause
	case 10:
		alarm();
		break;
	}
	// if maxT != 0 , a time is need to clear
	if (maxT) {
		t = *p / 16 * 10 + *p % 16;
		if (++t >= maxT)
			t = 0;
		*p = t / 10 * 16 + t % 10;
		if(pA) *pA = t;
	}

	if (mode)
		display();
}
// timer interrupt
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A(void) {
	--TA1CCR0;
	TA1CCR1 = TA1CCR0 / 2;
	if (TA1CCR0 <= 10)
		TA1CCR0 = 2047;
}
