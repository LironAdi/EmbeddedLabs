File: main.c
----------------------------------------------------------------------
Purpose: Main application file. Contains the FSM implementation and system initialization.

Functions:
  - main(): Entry point of the program. Initializes hardware and enters FSM loop.

File: bsp.c
----------------------------------------------------------------------
Purpose: Board Support Package. Responsible for low-level hardware configurations (GPIO, Timers, ADC, UART).

Functions:
  - GPIOconfig(): Configures GPIO pins for buttons, LCD, sensors, and PWM.
  - TIMER_A0_config(): Initializes Timer A0 for PWM generation.
  - TIMER_A1_config(): Initializes Timer A1 for trigger and echo timing.
  - ADCconfig(): Configures the ADC10 for LDR light sensing.
  - UART_config(): Initializes UART interface for serial communication.

File: halGPIO.c
----------------------------------------------------------------------
Purpose: Hardware Abstraction Layer. Handles interrupts and peripheral control logic.

Functions:
  - PORT1_VECTOR(): ISR for PB1 - used to scroll or execute script.
  - PORT2_VECTOR(): ISR for PB0 - handles calibration or scrolling.
  - TIMER0_A0_VECTOR(): ISR for LCD script timing.
  - TIMER1_A1_VECTOR(): ISR for servo scanning and measuring loop.
  - ISR_UART_RX(): Handles UART RX interrupt. Parses incoming serial commands and triggers FSM updates.


File: api.c
----------------------------------------------------------------------
Purpose: API Layer. Contains command parsing, script execution, flash operations, and display functions.

Functions:
  - read_struct_from_flash(): Reads a FileEntry structure from flash memory.
  - init_count_up(): Initializes LCD counter for counting up.
  - inc_lcd(): Increments the LCD display counter.
  - init_count_down(): Initializes LCD counter for countdown.
  - dec_lcd(): Decrements the LCD display counter.
  - print_operand_char(): Converts hex string to character value.
  - rra_lcd(): Displays script data on LCD with scrolling.
  - set_delay(): Sets delay using Timer A0 for scripts.
  - write_byte_to_flash(): Writes a byte to flash memory.
  - get_flash_segment_start(): Gets flash segment address for a given byte address.
  - erase_flash_segment_auto(): Erases flash segment if needed before writing.
  - erase_flash_segment(): Erases a specific flash segment.
  - erase_all_file_segments(): Clears all memory segments used for script storage.
  - write_struct_to_flash(): Writes FileEntry structure to flash.
  - scroll_view_on_button_press(): Handles LCD scrolling.
  - display_scrolling_view(): Displays file names from flash.
  - parse_command(): Splits command into opcode and operand.
  - execute_command(): Executes command based on parsed opcode.
  - run_script_mode(): Executes script from flash memory.
  - find_type(): Returns the file type of a script.
  - lcd_show_page(): Displays a script page on LCD.
  - show_text(): Scrolls script content.

File: api.h
----------------------------------------------------------------------
Purpose: Header file for API layer. Declares API functions and shared variables.

Functions: (header file – no implementations)

File: halGPIO.h
----------------------------------------------------------------------
Purpose: Header file for HAL layer. Declares peripheral control functions and global variables.

Functions: (header file – no implementations)

File: app.h
----------------------------------------------------------------------
Purpose: Application layer definitions. Includes state enums and file struct.

Functions: (header file – no implementations)

File: bsp.h
----------------------------------------------------------------------
Purpose: Header for BSP layer. Contains pin definitions and hardware constants.

Functions: (header file – no implementations)
