//BTN0 計時/暫停
//BTN1 清除/消除時間到
//BTN2~4 加時間
#include <msp430.h>
#include <stdio.h>

#define RSPIN   (0x04)           //P11.2
#define RWPIN   (0x02)           //P11.1
#define EPIN    (0x01)           //P11.0

unsigned char XPOS, YPOS;

char upLCD[17];
char downLCD[17] = "  00 : 00 : 00  ";
int timerHour = 0, timerMin = 0, timerSec = 0;
int addMode = 0;
int timerStart = 0;
int btn0IsClick = 0, btn1IsClick = 0, btn2IsClick = 0, btn3IsClick = 0, btn4IsClick = 0;

int main(void)
{
    unsigned int i = 0;
    unsigned char j;
    int k = 0;
    WDTCTL = WDTPW + WDTHOLD;

    //button
    P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P2REN |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P2OUT |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    //LED
    P5DIR |= BIT4 + BIT5;                            // P5.4 P5.5 output
    P5OUT &= ~(BIT4 + BIT5);
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

    UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

    __bis_SR_register(SCG0);                  // Disable the FLL control loop
    UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_4;                     // Select DCO range 16MHz operation
    UCSCTL2 = FLLD_1 + 488;                   // Set DCO Multiplier for 8MHz
                                              // (N + 1) * FLLRef = Fdco
                                              // (249 + 1) * 32768 = 8MHz
                                              // Set FLL Div = fDCOCLK/2
    __bic_SR_register(SCG0);                  // Enable the FLL control loop

    // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLKcycles for DCO to settle
    //__delay_cycles(250000);
    do
    {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);

        SFRIFG1 &= ~OFIFG;
    }
    while (SFRIFG1 & OFIFG);

    while (1)
    {
        for (k = 0; k < 8; ++k)
        {
            //BTN0
            if (!(P2IN & BIT0) && !btn0IsClick){
                btn0IsClick = 1;
                timerStart = !timerStart;
            }
            if(P2IN & BIT0)
                btn0IsClick = 0;
            //BTN1
            if (!(P2IN & BIT1) && !timerStart && !btn1IsClick)
            {
                btn1IsClick = 1;
                timerSec = 0;
                timerMin = 0;
                timerHour = 0;
            }
            if(P2IN & BIT1)
                btn1IsClick = 0;
            //BTN2
            if (!(P2IN & BIT2) && !timerStart && !btn2IsClick){
                btn2IsClick = 1;
                if(++timerHour > 24)
                    timerHour = 0;
            }
            if(P2IN & BIT2)
                btn2IsClick = 0;
            //BTN3
            if (!(P2IN & BIT3) && !timerStart && !btn3IsClick){
                btn3IsClick = 1;
                if(++timerMin > 60)
                    timerMin = 0;
            }
            if(P2IN & BIT3)
                btn3IsClick = 0;
            //BTN4
            if (!(P2IN & BIT4) && !timerStart && !btn4IsClick){
                btn4IsClick = 1;
                if(++timerSec > 60)
                    timerSec = 0;
            }
            if(P2IN & BIT4)
                btn4IsClick = 0;

            display();
            __delay_cycles(100000);
        }
        //__delay_cycles(8000000);


        if (timerStart)
        {
            subTime();
            display();
        }

    }
    return 0;
}

void subTime(){
    if(--timerSec < 0){
        if(timerMin | timerHour){
            timerSec = 59;
            if(--timerMin < 0){
                timerMin = 59;
                --timerHour;
            }
        }
        // timeOut
        else{
            timerSec = 0;
            P5OUT |= BIT4;
            P5OUT &= !BIT5;
            while(P2IN & BIT1){
                P5OUT ^= BIT4 + BIT5;
                __delay_cycles(200000);
            }
            if(!(P2IN & BIT1)){
                P5OUT &= !(BIT4 + BIT5);
                timerStart = 0;
            }
        }
    }
}

void display()
{
    sprintf(downLCD, "  %d%d : %d%d : %d%d  ", timerHour / 10,timerHour % 10, timerMin / 10, timerMin % 10, timerSec / 10,timerSec % 10);
    lcdreset();
    //lcdwda(0, 0, upLCD);
    lcdwda(0, 1, downLCD);
}

void delay(unsigned int t)
{
    unsigned int i, j;
    for (i = 0; i < t; i++)
        for (j = 0; j < 10; j++)
            ;

}

void lcdwaitidle(void)
{
    P10DIR = 0x00;
    P11OUT &= ~RSPIN;
    P11OUT |= RWPIN;
    P11OUT |= EPIN;
    while ((P10IN & 0x80) == 0x80)
        ;
    P11OUT &= ~EPIN;
    P10DIR |= 0Xff;
}

void lcdwcn(unsigned char c)
{
    P11OUT &= ~RSPIN;
    P11OUT &= ~RWPIN;
    P10OUT = c;
    P11OUT |= EPIN;
    _NOP();
    P11OUT &= ~EPIN;
}

void lcdwc(unsigned char c)
{
    lcdwaitidle();
    lcdwcn(c);
}

void lcdwd(unsigned char d)
{
    lcdwaitidle();
    P11OUT |= RSPIN;
    P11OUT &= ~RWPIN;
    P10OUT = d;
    P11OUT |= EPIN;
    _NOP();
    P11OUT &= ~EPIN;
}

void lcdpos(void)
{
    XPOS &= 0x0f;
    YPOS &= 0x03;
    if (YPOS == 0x00)
        lcdwc(XPOS | 0x80);
    else if (YPOS == 0x01)
        lcdwc((XPOS + 0x40) | 0x80);
}

void lcdreset()
{
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

void lcdfill(unsigned char n)
{
    for (YPOS = 0; YPOS < 2; YPOS++)
        for (XPOS = 0; XPOS < 16; XPOS++)
        {
            lcdpos();
            lcdwd(n);
        }
}

void lcdwda(unsigned char x, unsigned char y, unsigned char *s)
{
    YPOS = y;
    for (XPOS = x; XPOS < 16; XPOS++)
    {
        lcdpos();
        lcdwd(*s);
        s++;
        delay(400);
    }
}