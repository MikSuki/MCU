#include <msp430.h> 

int flag = 0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    //var
    int t = 1;
    //button
    P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P2REN |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P2OUT |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);

    P5DIR |= (BIT4 + BIT5);
    P5OUT &= ~(BIT4 + BIT5);


    while(1){

        if((P2IN & BIT0) == 0){
            switch(t){
                case 1:
                    P5OUT |= (BIT4);
                    break;
                case 2:
                    P5OUT |= (BIT5);
                    break;
                case 3:
                    P5OUT |= (BIT4 + BIT5);
                    break;
                default:
                    P5OUT |= BIT4;
                }

            flag = 1;
            while(flag != 0){
                if(t > 3){
                    P5OUT ^= (BIT4 + BIT5);
                    _delay_cycles(100000);
                }


                if((P2IN & BIT0) == 1){
                    flag = 0;
                    ++t;
                }
            }
        }
        else
            P5OUT &= ~(BIT4 + BIT5);
    }

    return 0;
}

