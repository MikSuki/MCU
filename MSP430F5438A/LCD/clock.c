#include <msp430.h>
#include <stdio.h>

#define RSPIN   (0x04)           //P11.2
#define RWPIN   (0x02)           //P11.1
#define EPIN    (0x01)           //P11.0

unsigned char XPOS, YPOS;

char upLCD[2][17] =   {" 2018/10/07 Sun ", "      Timer     "};
char downLCD[2][17] = {"  21 : 06 : 00  ", "  00 : 00 : 00  "};
//0 clock , 1 timer,
int mode = 0;
int main(void)
{
    //clock var
    int monthDay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    char day[7][4]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    int year = 2018, month = 10, date = 7;
    int hour = 21, min = 6, sec = 0;
    //timer var
    int timerHour = 0, timerMin = 0, timerSec = 0;
    int addMode = 0;
    int timerStart = 0;
    int btn2IsClick = 0, btn3IsClick = 0;

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
    P5OUT &= ~BIT4;
    P5OUT |= BIT5;
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
    UCSCTL2 = FLLD_1 + 244;                   // Set DCO Multiplier for 8MHz
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
        display();
        P5OUT ^= (BIT4 + BIT5);          // Toggle P5.4 P5.5

        for(k = 0; k < 8; ++k){
            int needDisplay = 1;
            //BTN0
            if(!(P2IN & BIT0))mode = 0;
            //BTN1
            else if(!(P2IN & BIT1) && mode != 1){
                mode = 1;
                addMode = 0;
            }
            else if(!(P2IN & BIT1) && mode == 1
                    && (timerSec | timerMin | timerHour)){
                timerStart = !timerStart;
            }
            else needDisplay = 0;
            //BTN2
            if(!(P2IN & BIT2) && mode == 1 && !timerStart && !btn2IsClick){
                btn2IsClick = 1;
                if(++addMode > 2)addMode = 0;
            }
            else if(P2IN & BIT2)btn2IsClick = 0;
            //BTN3
            if(!(P2IN & BIT3) && mode == 1 && !timerStart && !btn3IsClick){
                switch(addMode){
                    case 0:
                        if(++timerSec >= 60)timerSec = 0;
                        break;
                    case 1:
                        if(++timerMin >= 60)timerMin = 0;
                        break;
                    case 2:
                        if(++timerHour >= 24)timerHour = 0;
                        break;
                }
                sprintf(downLCD[1], "  %d%d : %d%d : %d%d  ", timerHour/10, timerHour%10, timerMin/10, timerMin%10, timerSec/10, timerSec%10);
                needDisplay = 1;
            }

            if(needDisplay)display();

            __delay_cycles(1000000);
        }
        //__delay_cycles(8000000);


        if(++sec >= 60){
            sec = 0;
            if(++min >= 60){
                min = 0;
                if(hour >= 24){
                    hour = 0;
                    if(++date >= monthDay[month - 1]){
                        date = 1;
                        if(++month >= 12)
                            ++year;
                    }
                    sprintf(upLCD[0], "   %d%d%d%d/%d%d/%d%d   ", year/1000, year/100, year/10, year%10, month/10, month%10, date/10, date%10);
                }
            }
        }
        sprintf(downLCD[0], "  %d%d : %d%d : %d%d  ", hour/10, hour%10, min/10, min%10, sec/10, sec%10);

        if(timerStart){
            if(--timerSec < 0 &&(timerMin | timerHour)){
                timerSec = 59;
                if(--timerMin < 0){
                    timerMin = 59;
                    if(--timerHour < 0){
                        timerHour = 0;
                    }
                }
            }
            else if(!(timerSec | timerMin | timerHour)){
                timerStart = 0;
                timerSec = 0;
            }
            sprintf(downLCD[1], "  %d%d : %d%d : %d%d  ", timerHour/10, timerHour%10, timerMin/10, timerMin%10, timerSec/10, timerSec%10);
        }

    }
    return 0;
}

void display(){
    lcdreset();
    lcdwda(0, 0, upLCD[mode]);
    lcdwda(0, 1, downLCD[mode]);
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

