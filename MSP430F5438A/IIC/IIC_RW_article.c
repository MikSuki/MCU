#include "msp430x54x.h"

#define RSPIN   (0x04)           //P11.2
#define RWPIN   (0x02)           //P11.1
#define EPIN    (0x01)           //P11.0

unsigned char XPOS, YPOS;
unsigned char data[64];
unsigned char data2[200] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned char data3;
unsigned char str1[] = "Write Success!  ";
unsigned char str2[] = "Read Success!   ";
char buffer[1];
int i = 1, rxflag, j;
// check is chi or eng
int textSize = 0;
unsigned char rxbuff[200];
unsigned char *rxptr;
int rxcount, W_count = 0;
char message[16];

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

void display(char str[])
{
    lcdreset();
    lcdwda(0, 1, str);
}

unsigned char IIC_ReadByte(void)
{
    while ((UCB2IFG & UCRXIFG) == 0)
        ;            // Wait for USART0 RX buffer to receive a character
    return (UCB2RXBUF);
}

void IIC_writeByte(unsigned char data)
{
    while ((UCB2IFG & UCTXIFG) == 0)
        ;            // Wait for USART0 TX buffer to become ready
    UCB2TXBUF = data;                            // Send the data out
}

void BYTE_WRITE(unsigned int address, unsigned char data)
{
    while (UCB2CTL1 & UCTXSTP)
        ;             // Ensure stop condition got sent
    UCB2CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    IIC_writeByte(address >> 8);           //sent address
    IIC_writeByte(address);
    IIC_writeByte(data);
    while ((UCB2IFG & UCTXIFG) == 0)
        ;
    UCB2CTL1 |= UCTXSTP;                    // I2C stop condition
    UCB2IFG &= ~UCTXIFG;                     // Clear USCI_B0 TX int flag
}

void PAGE_WRITE(unsigned int address, int length)
{
    unsigned int j;
    while (UCB2CTL1 & UCTXSTP)
        ;             // Ensure stop condition got sent
    UCB2CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __delay_cycles(10000);
    IIC_writeByte(address >> 8);            //sent address
    IIC_writeByte(address);
    for (j = 0; j < length; j++)
    {
        IIC_writeByte(rxbuff[j]);

        if (!(++address % 64))
        {
            // IIC stop
            while ((UCB2IFG & UCTXIFG) == 0)
                ;
            UCB2CTL1 |= UCTXSTP;
            UCB2IFG &= ~UCTXIFG;
            __delay_cycles(10000);

            // IIC start
            while (UCB2CTL1 & UCTXSTP)
                ;
            UCB2CTL1 |= UCTR + UCTXSTT;

            IIC_writeByte(address >> 8);            //sent address
            IIC_writeByte(address);
        }
    }
    while ((UCB2IFG & UCTXIFG) == 0)
        ;        //wait for the last byte has been sent
    UCB2CTL1 |= UCTXSTP;                     // I2C stop condition
    UCB2IFG &= ~UCTXIFG;
    __delay_cycles(10000);
}

void Read_Data(unsigned int address, unsigned int num)
{
    unsigned int j = 0;
    while (UCB2CTL1 & UCTXSTP)
        ;               // Ensure stop condition got sent
    UCB2CTL1 |= UCTR + UCTXSTT;               // I2C TX, start condition
    IIC_writeByte(address >> 8);              //sent address
    IIC_writeByte(address);

    while ((UCB2IFG & UCTXIFG) == 0)
        ;         //make sure the add has been sent
    UCB2CTL1 &= ~UCTR;                        // I2C RX
    UCB2CTL1 |= UCTXSTT;                      // I2C start condition

    while (--num)                              // Decrement RX byte counter
    {
        data2[j] = IIC_ReadByte();            // Move RX data to address PRxData
        j++;
        if (num == 1)                           // Only one byte left?
            UCB2CTL1 |= UCTXSTP;                  // Generate I2C stop condition
    }

    data2[j] = IIC_ReadByte();              // Move final RX data to PRxData

}

void Current_Read_Data(void)
{
    while (UCB2CTL1 & UCTXSTP)
        ;               // Ensure stop condition got sent
    UCB2CTL1 &= ~UCTR;                        // I2C RX
    UCB2CTL1 |= UCTXSTT;                     // I2C TX, start condition

    while (UCB2CTL1 & UCTXSTT)
        ;               // Start condition sent?
    UCB2CTL1 |= UCTXSTP;

    data3 = IIC_ReadByte();                   // Move final RX data to PRxData
}

void sendMessage(unsigned char str[], int length)
{
    int k = 0;
    while (k < length)
    {
        while (!(UCA1IFG & UCTXIFG))
            ;
        UCA1TXBUF = str[k];
        ++k;
    }
    wrap();
}

void freeChar(void)
{
    while (rxcount > 0)
    {
        rxbuff[rxcount - 1] = '\0';
        rxcount--;
    }
    rxcount = 0;
}
void wrap(void)
{
    while (!(UCA1IFG & UCTXIFG))
        ;
    UCA1TXBUF = '\r';
    while (!(UCA1IFG & UCTXIFG))
        ;
    UCA1TXBUF = '\n';
}

void get_result()
{
    if (rxbuff[0] == 'R' && rxbuff[1] == '_')
    {
        Read_Data(0x00, W_count);
        __delay_cycles(5000);
        sendMessage(data2, W_count);
        display(str2);
    }
    else
    {
        PAGE_WRITE(0x00, rxcount);
        W_count = rxcount;
        sendMessage(str1, 14);
        display(str1);
    }

}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    switch (__even_in_range(UCA1IV, 4))
    {
    case 0:
        break;                             // Vector 0 - no interrupt
    case 2:                                // Vector 2 - RXIFG
        while (!(UCA1IFG & UCTXIFG))
            ;    // USCI_A0 TX buffer ready?
        UCA1TXBUF = UCA1RXBUF;            // TX -> RXed character

        if (UCA1RXBUF == '\r')
        {
            while (!(UCA1IFG & UCTXIFG))
                ;
            UCA1TXBUF = '\n';
        }

        if (UCA1RXBUF != 0x0D)
        {

            if (UCA1RXBUF == '\b')
            {
                rxbuff[rxcount - 1] = '\0';
                rxcount--;
            }
            else
            {
                rxbuff[rxcount] = UCA1RXBUF;
                rxcount++;
            }
        }
        else
        {
            if (rxcount)
                rxflag = 1;
        }
        break;
    case 4:
        break;                             // Vector 4 - TXIFG
    default:
        break;
    }
}

void main(void)
{
    //WDTCTL = WDTPW + WDTHOLD;
    WDTCTL = WDT_ARST_1000; // 1000 ms ­«±Ò¤@¦¸
    // LCD
    P9DIR |= BIT6 + BIT7;
    P9DS |= BIT6 + BIT7;
    P9OUT &= ~BIT7; //K
    P9OUT |= BIT6; //A
    P10OUT = 0;
    P11OUT = 0;
    P10DIR |= 0XFF;
    P11DIR |= 0XFF;
    P11OUT &= ~EPIN;
    // UART
    P5SEL = BIT6 + BIT7;                        // P5.6,7 = USCI_A0 TXD/RXD
    UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA1CTL1 |= UCSSEL_1;                     // CLK = ACLK
    UCA1BR0 = 0x03;                        // 32kHz/9600=3.41 (see User's Guide)
    UCA1BR1 = 0x00;                           //
    UCA1MCTL = UCBRS_3 + UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
    UCA1CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
    UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
    // IIC
    P8DIR |= BIT7; //WP
    P8OUT &= ~BIT7; // DISABLE WP
    P9SEL |= BIT1 + BIT2;                       // Assign I2C pins to USCI_B2
    UCB2CTL1 |= UCSWRST;                      // Enable SW reset
    UCB2CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
    UCB2CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
    UCB2BR0 = 10;                             // fSCL = SMCLK/10= ~100kHz
    UCB2BR1 = 0;
    UCB2I2CSA = 0x50;          // Slave Address is 050h     (SA bit6 is the MSB)
    UCB2CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation

    __bis_SR_register(/*LPM3_bits + */GIE);    // Enter LPM3, interrupts enabled
    __no_operation();                        // For debugger

    lcdreset();

    while (1)
    {
        if (rxflag)
        {
            rxflag = 0;
            get_result();
            freeChar();
        }
        // feed dog
        WDTCTL = WDT_ARST_1000;
    }
}
