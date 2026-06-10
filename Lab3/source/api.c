#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer

unsigned int sentence_len[11]= {36,37,33,36,34,32,31,42,39,46,61} ;// the length
char print_lcd[70];
char line1[17];
unsigned int num_printes=0;
unsigned int i = 0;
unsigned int flag = 1;
//-------------------------------------------------------------
//            Print SWs value onto LEDs
//-------------------------------------------------------------
void printSWs2LEDs(void){
	unsigned char ch;
	
	ch = readSWs();
	print2LEDs(ch);
}
//--------------------------------------------------------------------
//            Print array to LEDs array with rate of LEDs_SHOW_RATE
//--------------------------------------------------------------------            
void printArr2SWs(char Arr[], int size, unsigned int rate){
	unsigned int i;
	
	for(i=0; i<size; i++){
		print2LEDs(Arr[i]);
		delay(rate);
	}
}

/*void reverse_string(char** data_base){
    DMA1SA = (void*)data_base[num_string];
    DMA1DA =&print_lcd;
    DMA1SZ = sentence_len[num_string];
    lcd_puts(print_lcd);
}*/

    void reverse_string(char** data_base){
        //lcd_clear();
        lcd_home();
        if (num_string>0 && num_string<10){//checks if entered a num
            DMACTL1 = DMAONFETCH; // DMA transfer occurs automatically
            DMA1SA = (void*)&data_base[num_string][strlen(data_base[num_string]) - 1];
            DMA1DA = &print_lcd;
            DMA1SZ = sentence_len[num_string];
            DMA1CTL &= ~DMAIFG; // reset interrupts before starting
            DMA1CTL |= DMAEN;
            DMA1CTL |= DMAREQ;
            //enterLPM(mode0);
            //while ((DMA1CTL & DMAIFG) == 0) {
            //}
            //DMA1CTL &= ~DMAEN;

        lcd_clear();
        DMA1CTL &= ~DMAEN;
        DMA1CTL &= ~DMAREQ;
        delay(2000);
        while (i <= sentence_len[num_string]-1){
            if (i < 16){
            lcd_data(print_lcd[i]);
            }
            else if(i == 16){
                lcd_new_line;
                lcd_data(print_lcd[i]);
                line1[i - 16] = print_lcd[i];
            }
            else if (i>16 && i<=31){
                lcd_data(print_lcd[i]);
                line1[i -16] = print_lcd[i];

            }
            else if(i == 32 && flag == 1){
                delay(2000);
                lcd_clear();
                lcd_cursor(0);
                lcd_puts(line1);
                lcd_new_line;
                lcd_data(print_lcd[i]);
            }
            else if (i > 32 && flag == 1){
                lcd_data(print_lcd[i]);
            }
            i++;

        }

        }
        if (i>sentence_len[num_string]-1){
            i = 0;
            num_string = 11;
            state = state0;
        }
    }


 
  

