#include <xc.h>
#include "SPI_COMM_BOARD_DRIVER.h"
#include "COMM_BOARD_INTERFACE.h"
#include "SerialCommunication.h"
#include <spi.h>
#include "SPI.h"
#include "CommBoardMessageConsts.h"
#include <stdlib.h>

void initSPICommBoard(){

   INTCON2 = 0;
   INTCON1 = 0;
   U1MODE = 0;

    TRISFbits.TRISF6 = 1;   //SCK2 as Input on G6
    TRISFbits.TRISF2 = 1;   //SDI2 as Input on G7
    TRISFbits.TRISF3 = 0;   //SDO2 as output on G8

    TRISBbits.TRISB14 = 1;   //CS as Input on G9

    SPI1CON1=0;
    SPI1STAT=0;
    SPI1STATbits.SPIROV = 0;
    SPI1STATbits.SPIEN = 0;
    SPI1CON2=0;

    SPI1CON1bits.DISSCK=0;
    SPI1CON1bits.DISSDO = 0;
    SPI1CON1bits.MODE16=0;
    SPI1CON1bits.SMP=0;
    SPI1CON1bits.CKE=0;
    SPI1CON1bits.CKP=0;
    SPI1CON1bits.SSEN = 1;
    SPI1CON1bits.PPRE=0;
    SPI1CON1bits.SPRE = 0;
    SPI1CON1bits.MSTEN=0;

    IFS0bits.SPI1IF = 0;
    IEC0bits.SPI1IE = 1;
    IPC2bits.SPI1IP = 2;    //Interrupt priority for SPI1

    SPI1STATbits.SPIEN=1;
}

static uint8_t actual_message = 0;
static uint8_t send_state = 0;

void initate_send(uint8_t message) {
    actual_message = message;
    send_state = 0;
}

void putByte(uint8_t i) {
    uint8_t trahsbit = SPI1BUF;
    SPI1BUF = i;
}

void __attribute__((__interrupt__,auto_psv)) _SPI1Interrupt(void){
    /*if (SPI1STATbits.SPIROV) {
            // error
            SPI1STATbits.SPIROV	= 0;			// clear overflow
            int trash = SPI1BUF;
    } else if (!SPI1STATbits.SPIRBF) {
            // error
    } else {*/
        // SPI1STATbits.SPIROV = 0;

        uint8_t buffer = SPI1BUF;
        while (SPI1STATbits.SPITBF);
        /*if (buffer == LOAD_READ_AGAIN) {
            if (0 <= send_state && send_state < 20) {
                putByte(69);
                ++send_state;
            } else if (20 <= send_state && send_state < 30 ) {
                if (actual_message + 1 == 69) {
                    putByte(70);
                } else {
                    putByte(actual_message+1);
                }
                ++send_state;
            } else if (30 <= send_state) {
                putByte(actual_message);
            }
        } else {
            // receiveMessageComm(buffer);
            int message = buffer;
            initate_send(message);
            putByte(69);
        }*/
        putByte(buffer);
        IFS0bits.SPI1IF = 0;
    //}
}
