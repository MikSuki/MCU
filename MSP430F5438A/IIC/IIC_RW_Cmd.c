#include "msp430x54x.h"
unsigned char data[64] =
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
unsigned char data2[128] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned char data3;
unsigned char str1[] = "Success!\0";
unsigned char str2[] = "Format Error!\0";
char buffer[1];
int i = 1, rxflag, j;
unsigned char rxbuff[32];
unsigned char *rxptr;
int rxcount, RW_count;
char message[16];

unsigned char IIC_ReadByte(void) {
    while ((UCB2IFG & UCRXIFG) == 0)
        ;            // Wait for USART0 RX buffer to receive a character
    return (UCB2RXBUF);
}

void IIC_writeByte(unsigned char data) {
    while ((UCB2IFG & UCTXIFG) == 0)
        ;            // Wait for USART0 TX buffer to become ready
    UCB2TXBUF = data;                            // Send the data out
}

void BYTE_WRITE(unsigned int address, unsigned char data) {
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

void PAGE_WRITE(unsigned int address, int length) {
    unsigned int j;
    while (UCB2CTL1 & UCTXSTP)
        ;             // Ensure stop condition got sent
    UCB2CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    IIC_writeByte(address >> 8);            //sent address
    IIC_writeByte(address);

    length += RW_count;
    for (j = RW_count; j < length; j++)
        IIC_writeByte(rxbuff[j]);

    while ((UCB2IFG & UCTXIFG) == 0)
        ;        //wait for the last byte has been sent
    UCB2CTL1 |= UCTXSTP;                     // I2C stop condition
    UCB2IFG &= ~UCTXIFG;
    __delay_cycles(5000);
}

void Read_Data(unsigned int address, unsigned int num) {
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

void Current_Read_Data(void) {
    while (UCB2CTL1 & UCTXSTP)
        ;               // Ensure stop condition got sent
    UCB2CTL1 &= ~UCTR;                        // I2C RX
    UCB2CTL1 |= UCTXSTT;                     // I2C TX, start condition

    while (UCB2CTL1 & UCTXSTT)
        ;               // Start condition sent?
    UCB2CTL1 |= UCTXSTP;

    data3 = IIC_ReadByte();                   // Move final RX data to PRxData
}

void sendMessage(unsigned char str[], int length) {
    int k = 0;
    while (k < length) {
        while (!(UCA1IFG & UCTXIFG))
            ;
        UCA1TXBUF = str[k];
        ++k;
    }
    wrap();
}

void freeChar(void) {
    while (rxcount > 0) {
        rxbuff[rxcount - 1] = '\0';
        rxcount--;
    }
}
void wrap(void) {
    while (!(UCA1IFG & UCTXIFG))
        ;
    UCA1TXBUF = '\r';
    while (!(UCA1IFG & UCTXIFG))
        ;
    UCA1TXBUF = '\n';
}

void get_result(){
    unsigned int ads;
    int len;

    if(rxbuff[0] == 'W' || rxbuff[0] == 'R')
    if(rxbuff[1] == ' ' && rxbuff[2] == '0' && rxbuff[3] == 'x'){
        ads = get_address();
        len = get_length();
        // error
        if(ads < 0 || len < 0) {
            sendMessage(str2, 14);
            return;
        }
        // Wirte
        else if(rxbuff[0] == 'W'){
            PAGE_WRITE(ads, len);
            sendMessage(str1, 9);
            return;
        }
        // Read
        else if(rxbuff[0] == 'R'){
            Read_Data(ads, len);
            sendMessage(data2, len);
            return;
        }
    }
    sendMessage(str2, 14);
}

unsigned int get_address(){
    // address postiotn is always start at 4
    int i = 4;
    int ads_len = 0;
    unsigned int ads = 0;
    while(rxbuff[i] >= '0'  && rxbuff[i] <= '9'){
        if(ads_len)
            ads = ads * 16 + (rxbuff[i] - '0');
        else
            ads = rxbuff[i] - '0';
        ++ads_len;
        ++i;
    }

    RW_count = i + 1;
    if(rxbuff[i] != ' ' || ads_len == 0)
        return -1;
    else
        return ads;
}

int get_length(){
    int i = RW_count;
    int len_len = 0;
    int len = 0;
    while(rxbuff[i] >= '0'  && rxbuff[i] <= '9'){
        if(len_len)
            len = len * 10 + (rxbuff[i] - '0');
        else
            len = rxbuff[i] - '0';
        ++len_len;
        ++i;
    }

    RW_count = i + 1;
    if(len == 0)
        return -1;
    else
        return len;
}


// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {
    switch (__even_in_range(UCA1IV, 4)) {
    case 0:
        break;                             // Vector 0 - no interrupt
    case 2:                                // Vector 2 - RXIFG
        while (!(UCA1IFG & UCTXIFG))
            ;    // USCI_A0 TX buffer ready?
        UCA1TXBUF = UCA1RXBUF;            // TX -> RXed character

        if (UCA1RXBUF == '\r') {
            while (!(UCA1IFG & UCTXIFG))
                ;
            UCA1TXBUF = '\n';
        }

        ;
        if (UCA1RXBUF != 0x0D) {
            rxbuff[rxcount] = UCA1RXBUF;
            rxcount++;
        } else {
            rxflag = 1;
        }
        break;
    case 4:
        break;                             // Vector 4 - TXIFG
    default:
        break;
    }
}


void main(void) {
    WDTCTL = WDTPW + WDTHOLD;
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

    while (1) {
        if (rxflag) {

            rxflag = 0;
            get_result();
            freeChar();
        }
    }
}
