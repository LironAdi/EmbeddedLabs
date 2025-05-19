#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer

//-------------------------------------------------------------
//            Rise And fall LEDs
//-------------------------------------------------------------
 void riseAndFallLEDs(char led, int flag){
     setLEDs(led);
      int i = 0;
      if (flag == 0){
          for (i; i<20; i++){
              incLEDs(1);
              delay(LEDs_SHOW_RATE);
          }
          //flag = 1; //flag = 1 - decLEDs
      }
      else {
          for (i; i<20; i++){
              incLEDs(-1);
              delay(LEDs_SHOW_RATE);
          }
          //flag = 0; //flag = 0 - incLEDs
      }
 }



//-------------------------------------------------------------
//           Shift LEDs left
//-------------------------------------------------------------
void shifter(char led){
    setLEDs(led);
    delay(Shifter_LEDs_SHOW_RATE);
    int j = 0;
    for (j; j<14; j++){
        if (LEDsArrPort == 0x80){
            clrLEDs();
            char led = 0x001;
            setLEDs(led);
            delay(Shifter_LEDs_SHOW_RATE);
        }
        shiftLEDs(2);
        delay(Shifter_LEDs_SHOW_RATE);
      }

}

void PWM(){
    while (state == state3){
        if (PWM_flag == 0){
            PWMArr = 0x80;
            PWM_UP_delay(17);
            PWMArr = 0x00;
            PWM_DOWN_delay(2);
        }
        else{
            PWMArr = 0x80;
            PWM_UP_delay(10);
            PWMArr = 0x00;
            PWM_DOWN_delay(37);
        }
    }
}

void BlinkColors(char colors[],int size ,int delay_ms){
    int r = 0;
    while(r < 20){
        int m = 0;
        for (m; m<size; m++){
            delay(delay_ms*RGB_SHOW_RATE);
            char a = colors[m];
            PrintRGB(a);
            r++;
        }
    }
}







 
  

