#include <msp430.h>

#define PERIOD 5000



void LED_dutyControl(int period, int duty_cnt) {
    int i;

    /* P5.4 LED on, P5.5 LED close */
    P5OUT |= BIT4;
    P5OUT &= ~BIT5;
    for(i=0 ; i<duty_cnt ; i++);    // 持續時間: 0 ~ duty_cnt

    /* P5.5 LED on, P5.4 LED close */
    P5OUT |= BIT5;
    P5OUT &= ~BIT4;
    for(; i<period ; i++);        // 持續時間: duty_cnt ~ period
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    /* Port 5 */
    P5SEL &= ~(BIT4+BIT5);   // 設0作為IO使用
    P5DIR |= (BIT4+BIT5);    // 設1輸出

    /* Port 2 */
    P2SEL &= ~(BIT0+BIT1);  // 設0作為IO使用
    P2DIR &= ~(BIT0+BIT1);  // 設0輸入
    P2REN |= (BIT0+BIT1);   // 設1致能Pullup or Pulldown
    P2OUT |= (BIT0+BIT1);   // 設1做Pull up

    int cnt = 0;    // LED duty cnt

    while(1) {
    	
        /* P2.0 button is pressed */
        if(!(P2IN&BIT0)) {        	
            if(cnt < PERIOD)
                cnt += PERIOD/100;  // 100段duty_cnt
            //while(!(P2IN&BIT0));	//防彈跳 
        }

        /* P2.1 button is pressed */
        if(!(P2IN&BIT1)) {
            if(cnt > 0)
                cnt -= PERIOD/100;  // 100段duty_cnt
            //while(!(P2IN&BIT0));	//防彈跳 
        }

        LED_dutyControl(PERIOD, cnt);
    }

    //return 0;
}
