
// PIC16F876A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define _XTAL_FREQ 4000000
#define red RC5
#define orange RC4
#define trackRed RC3
#define trackGreen RC1
#define buzzer RC0
#define servo RC2

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include "SerialHeader.h"
#include "analogHeader.h"

short access = 0;
char temp;
char UID[20] = "0004274482";
char INID[20] = {'\0'};
char adcStr[20];
short reading1, reading2;

void gpioInit();
void riskCalling();
void riskAlarm();
void threatCalling();
void safeCalling();
void servoRotate90();
void servoRotate0();

int main()
{
    gpioInit();
    serialInit();
    analogInit();
    __delay_ms(100);
    while(1)
    {
        if(!access)
        {
           serialPrint("Approximate the Tag\r\n");
           serialRead(INID);
           if(!strcmp(INID, UID))
           {
               access = 1;
           }
           else
           {
               serialPrint("Access Denied!\r\n");
               temp = RCREG;
               __delay_ms(1000);
           }
        }
        else
        {
            serialPrint("\r\nAccess Granted!\r\n");
            while(1)
            {
                reading1 = analogRead(1);
                reading2 = analogRead(2);
                //sprintf(adcStr, "0: %d\r1: %d\r\r", reading1, reading2);
                //serialPrint(adcStr);
                if(reading1 < 100 && reading2 < 100)
                {
                    riskCalling();
                }
                else if((reading1 < 100 && reading2 >= 100) || (reading2 < 100 && reading1 >= 100))
                {
                    threatCalling();
                }
                else
                {
                    safeCalling();
                }
            }
        }
        __delay_ms(20);
    }
}

void gpioInit()
{
    TRISC = 0x00;
    PORTC = 0x00;
}

void riskCalling()
{
    red = 1;
    orange = 0;
    trackRed = 1;
    trackGreen = 0;
    serialPrint("Train stopped as the track is not clear!\r\n");
    servoRotate90();
    riskAlarm();
}

void riskAlarm()
{
    buzzer = 1;
    __delay_ms(300);
    buzzer = 0;
    __delay_ms(100);
}

void threatCalling()
{
    red = 1;
    orange = 0;
    trackRed = 0;
    trackGreen = 1;
    serialPrint("Train crossing, be alert..\r\n");
    servoRotate90();
}

void safeCalling()
{
    orange = 1;
    red = 0;
    trackGreen = 0;
    trackRed = 1;
    serialPrint("No threat detected..\r\n");
    servoRotate0();
}

void servoRotate90()
{
    servo = 1;
    __delay_ms(2);
    servo = 0;
    __delay_ms(18);
}
void servoRotate0()
{
    servo = 1;
    __delay_ms(1);
    servo = 0;
    __delay_ms(19);
}