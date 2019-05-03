//BTN0 計時/暫停
//BTN1 清除/消除時間到
//BTN2~4 加時間
#include <msp430.h>
#include <stdio.h>

#define RSPIN   (0x04)           //P11.2
#define RWPIN   (0x02)           //P11.1
#define EPIN    (0x01)           //P11.0

unsigned char XPOS, YPOS;

char LCD[2][17];
int timerHour = 0, timerMin = 0, timerSec = 0;
int timerStart = 0;
int btnFlag = 5;
int flag = 0;

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    switch (_even_in_range(P2IV, 16))
    {
    case 2:
        btnFlag = 0;
        flag ^= 1;
        break;
    case 4:
        btnFlag = 1;
        break;
    case 6:
        btnFlag = 2;
        break;
    case 8:
        btnFlag = 3;
        break;
    case 10:
        btnFlag = 4;
        break;
    default:
        btnFlag = 5;
    }

    if (btnFlag != 5)
        isClick();
}

int main(void)
{
    unsigned int i = 0;
    unsigned char j;
    WDTCTL = WDTPW + WDTHOLD;

    //button
    P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P2REN |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P2OUT |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P2IE = BIT0 + BIT1 + BIT2 + BIT3 + BIT4;  //interrupt
    P2IES = BIT0 + BIT1 + BIT2 + BIT3 + BIT4;
    __bis_SR_register(GIE);
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
    UCSCTL1 = DCORSEL_5;                     // Select DCO range 16MHz operation
    UCSCTL2 = FLLD_1 + 549;                   // Set DCO Multiplier for 8MHz
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
        __delay_cycles(800000);

        if (timerStart)
        {
            subTime();
        }

    }
    return 0;
}

void isClick()
{
    //BTN0
    if (btnFlag == 0)
    {
        timerStart = !timerStart;
    }
    if (!timerStart)
        switch (btnFlag)
        {
        //BTN1
        case 1:
            timerSec = 0;
            timerMin = 0;
            timerHour = 0;
            break;
            //BTN2
        case 2:
            if (++timerHour > 23)
                timerHour = 0;
            break;
            //BTN3
        case 3:
            if (++timerMin > 60)
                timerMin = 0;
            break;
            //BTN4
        case 4:
            if (++timerSec > 60)
                timerSec = 0;
            break;
        }
    display();
}

void subTime()
{
    if (--timerSec < 0)
    {
        if (timerMin | timerHour)
        {
            timerSec = 59;
            if (--timerMin < 0)
            {
                timerMin = 59;
                --timerHour;
            }
        }
        // timeOut
        else
        {
            timerSec = 0;
            //flag = 1;
            P5OUT |= BIT4;
            P5OUT &= !BIT5;
            while (flag)
            {
                P5OUT ^= BIT4 + BIT5;
                __delay_cycles(300000);
            }
            if (!flag){
                P5OUT &= !(BIT4 + BIT5);
                timerStart = 0;
            }
        }
    }
}

void display()
{
    sprintf(LCD[1], "  %d%d : %d%d : %d%d  ", timerHour / 10, timerHour % 10,
            timerMin / 10, timerMin % 10, timerSec / 10, timerSec % 10);
    lcdreset();
    lcdwda(0, 1, LCD[1]);
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
