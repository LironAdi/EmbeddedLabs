#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer


//extern void printSWs2LEDs(void);
//extern void printArr2SWs(char Arr[], int size, unsigned int rate);
extern void LCD_Counter();
extern void LCD_Freq();
extern void PWM_Buzzer();
extern void intToStr(int num,char* str);
#endif







