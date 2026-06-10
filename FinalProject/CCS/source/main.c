#include  "../header/api.h"         // private library - API layer
#include  "../header/app.h"         // private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;




void main(void){
  state = state0;       // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  erase_all_file_segments();

  lcd_init();          // config the LCD
  lcd_clear();         // CLR the LCD
  lcd_cursor(0);
  //cursor_off();
  TIMER_A0_config();
  TIMER_A1_config();

  ADCconfig();
  cursor_off;


  UART_config();
  TA0CCR1 = 420 + 9*90;




  while(1){
    switch(state){
      case state0://sleep mode
          enterLPM(lpm_mode);
        break;

      case state1://objects proximity + with rotate 0-180
          break;

      case state2://objects proximity + without rotate
          //TA0CCR1 = 420 + 9*90;//90 degrees only for test
          angle_num = array_to_num(angle_array);
          TA0CCR1 =  420 + 9 * angle_num;
        break;

      case state3://calibration mode
        break;

      case state4:
          Timer_A1_intterupt_disable();
          max_num = array_to_num(max_dist);
          break;
      case state5:

          break;
      case state6:
          if (run_script_request == 1) {
              lcd_clear();
              run_script_request = 0;
              run_script_mode(struct_addresses[top_index]);

          }
          if (file_upload_finished == 1){
              write_struct_to_flash(&my_script,struct_pointer);
              struct_addresses[num_files++] = struct_pointer;
              struct_pointer += sizeof(FileEntry);
              display_scrolling_view();
              size_from_flash = find_size(struct_addresses[0]);
              file_upload_finished = 0;
          }
          break;

      case state7:

        break;


    }
  }
}





