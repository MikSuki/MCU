#include <msp430.h> 

int main(void)
{
    //var
    int i = 0;
    int isClick = 0;
    int lightTime = 0;
    int sub = 0;

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    //button
    P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P2REN |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P2OUT |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    //led
    P5DIR |= (BIT4 + BIT5);
    P5OUT &= ~(BIT4 + BIT5);

    while (1)
    {
        if ((P2IN & BIT0) == 0)
        {
            isClick = 1;
            lightTime = 100;
            sub = -20;
        }
        else if ((P2IN & BIT1) == 0)
        {
            isClick = 1;
            lightTime = 900;
            sub = 20;
        }

        while (isClick)
        {
            for (i = 0; i < lightTime; ++i)
            {
                P5OUT |= (BIT5);
                P5OUT &= ~(BIT4);
            }
            for (i = 0; i < 1000 - lightTime; ++i)
            {
                P5OUT |= (BIT4);
                P5OUT &= ~(BIT5);
            }

            if (lightTime > 1000 || lightTime < 0)
                isClick = 0;
            else
                lightTime -= sub;
        }
    }
    return 0;
}

