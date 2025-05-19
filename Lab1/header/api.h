#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern void shifter(char led);
extern void riseAndFallLEDs(char led, int flag);
extern void PWM(void);
extern void BlinkColors(char colors[],int size ,int delay_ms);
#endif







