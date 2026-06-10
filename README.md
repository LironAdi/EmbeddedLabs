# EmbeddedLabs

EmbeddedLabs is a collection of embedded systems laboratory assignments and a final integrated project developed mainly in C using Code Composer Studio (CCS). The repository focuses on practical bare-metal embedded development, low-level hardware control, peripheral configuration, interrupt-driven programming, finite state machines, and communication between an embedded board and a PC-side application.

The repository is organized as a learning flow: each lab introduces or strengthens a specific embedded-systems topic, and the final project combines the main concepts into a larger complete system.

## Repository Structure

```text
EmbeddedLabs/
├── Lab1/
│   ├── header/
│   └── source/
├── Lab2/
│   ├── header/
│   └── source/
├── Lab3/
│   ├── header/
│   ├── source/
│   └── final_lab3.docx
├── Lab4/
│   ├── header/
│   ├── source/
│   └── final_lab4.pdf
├── FinalProject/
│   ├── CCS/
│   │   ├── header/
│   │   └── source/
│   ├── PC_side/
│   │   ├── GUI_README.txt
│   │   └── Gui.py
│   ├── project_report.pdf
│   └── systemReadMe.txt
└── LICENSE
```

## General Purpose of the Labs

The labs are designed to build practical experience in embedded systems development step by step. Instead of focusing only on theoretical concepts, each lab requires direct interaction with hardware peripherals and real-time embedded behavior.

The main purpose of the labs is to understand how an embedded system is structured, how software communicates with hardware, and how different peripherals can be controlled using modular C code. The labs begin with basic GPIO and LED control, then progress to timers, LCD output, ADC sampling, DMA, UART communication, PWM, interrupts, and finally full system integration.

Across the labs, the code follows a layered architecture that separates the application logic from the hardware-specific implementation. This structure makes the system easier to understand, debug, and extend.

## Software Architecture

Most of the labs use a similar layered software structure:

- **Application Layer**  
  Contains the main program flow, system initialization, and finite state machine logic.

- **API Layer**  
  Contains higher-level functions that implement the behavior of the current lab or project.

- **HAL Layer**  
  Handles hardware abstraction, interrupt service routines, and peripheral-level behavior.

- **BSP Layer**  
  Contains low-level board support configuration such as GPIO pins, timers, ADC, UART, LCD connections, and hardware constants.

This separation helps keep the code organized and demonstrates a common approach used in embedded software projects.

## Lab 1 - GPIO, LEDs, PWM, RGB LED, and FSM Basics

Lab 1 introduces the basic foundations of embedded programming. The main focus of this lab is controlling digital outputs and using a finite state machine to switch between different behaviors.

In this lab, the embedded board controls LEDs using GPIO pins. Different states activate different LED patterns, such as increasing and decreasing LED values, shifting LEDs, generating a simple PWM-like output, and blinking RGB colors. The lab also demonstrates the use of push buttons and interrupts to change the current system state.

The purpose of Lab 1 is to understand how the microcontroller interacts with digital hardware and how a program can be organized around states. It builds the foundation for later labs by introducing GPIO configuration, delay-based timing, LED control, and the basic relationship between hardware events and software behavior.

Main topics covered:

- GPIO configuration
- LED output control
- Push-button input
- Interrupt-based state changes
- Finite state machine design
- Simple PWM behavior
- RGB LED control
- Modular embedded C structure

## Lab 2 - Timers, LCD, ADC, Frequency Measurement, and Buzzer Control

Lab 2 expands the system by adding more advanced peripherals. The main focus of this lab is using timers, displaying information on an LCD, sampling analog signals with the ADC, and generating output signals such as buzzer control.

The lab demonstrates how timers can be used both for measuring signals and for producing controlled timing behavior. The LCD is used to show information to the user, such as counters or measured values. The ADC allows the system to read analog input and use it as part of the program logic.

The purpose of Lab 2 is to connect several common embedded peripherals into one working application. It shows how an embedded system can read input from the physical world, process the information in software, and present the result through an output device.

Main topics covered:

- Timer configuration
- LCD initialization and display
- ADC sampling
- Frequency measurement
- Counter display
- PWM-based buzzer behavior
- Numeric value conversion for LCD output
- Peripheral coordination using FSM logic

## Lab 3 - DMA, String Processing, LCD Display, and Memory Transfer

Lab 3 focuses on DMA and memory-oriented operations. The main idea of this lab is to demonstrate how data can be moved or processed efficiently without relying only on direct CPU copying.

The lab works with stored strings and displays information on the LCD. A selected string can be processed, reversed, and shown to the user. DMA is used as part of the memory transfer process, demonstrating how embedded systems can offload repetitive memory operations from the CPU.

The purpose of Lab 3 is to introduce a more advanced embedded-system mechanism: hardware-assisted memory transfer. This lab also strengthens the use of pointers, buffers, string handling, and LCD display management in embedded C.

Main topics covered:

- DMA configuration
- Memory transfer
- String storage and processing
- String reversal
- Buffer handling
- LCD output
- User-controlled operation flow
- FSM-based embedded behavior

## Lab 4 - UART, ADC, Timers, LCD, PWM, and Integrated Peripheral Control

Lab 4 combines several peripherals into a more complete embedded application. It serves as a transition from small independent exercises toward a larger embedded system.

The lab includes serial communication using UART, analog input using ADC, timing behavior using timers, LCD output, and PWM-based control. By combining these components, the lab demonstrates how an embedded application manages multiple hardware resources at the same time.

The purpose of Lab 4 is to practice system integration. Instead of focusing on a single peripheral, the lab requires coordinating several peripherals through interrupts and state-machine logic. This prepares the software structure for the final project, where PC communication, embedded control, and user feedback are combined into one system.

Main topics covered:

- UART serial communication
- ADC sampling
- Timer-based execution
- LCD display
- PWM output
- Buzzer or output-device control
- Interrupt-driven behavior
- Multi-peripheral embedded integration

## Final Project - Integrated Embedded System with PC Communication

The final project combines the concepts from all previous labs into a larger embedded application. It includes embedded firmware written in C and a PC-side Python GUI used to communicate with the board.

The embedded side is organized using the same layered approach used in the labs. The system includes an application layer, API logic, hardware abstraction, and board support configuration. The project uses UART communication to transfer commands or scripts between the PC and the embedded board. It also includes LCD display handling, timer-based behavior, ADC sensing, flash memory operations, and interrupt-driven control.

The purpose of the final project is to demonstrate how separate embedded topics can be connected into a complete working system. The project shows how the board can receive commands from a PC, store or execute data, interact with peripherals, and display information to the user.

Main topics covered:

- Embedded firmware development in C
- Python-based PC GUI
- UART communication between PC and board
- Command parsing
- Script execution
- Flash memory read/write operations
- LCD display and scrolling
- Timer-based execution
- ADC sensing
- Interrupt-driven embedded control
- Full system integration

## Main Technical Skills Demonstrated

This repository demonstrates hands-on experience with:

- Bare-metal embedded C programming
- Code Composer Studio project development
- GPIO control
- Timers and PWM
- ADC sampling
- UART communication
- LCD control
- DMA transfers
- Flash memory operations
- Interrupt service routines
- Finite state machines
- Modular embedded software design
- PC-to-microcontroller communication

## Development Environment

- **Main language:** C
- **PC-side language:** Python
- **IDE:** Code Composer Studio
- **Target:** MSP430-based embedded platform
- **Communication:** UART serial communication

## How to Use

1. Clone the repository:

```bash
git clone https://github.com/LironAdi/EmbeddedLabs.git
```

2. Open the relevant lab folder in Code Composer Studio.

3. Build the selected project.

4. Flash the program to the embedded board.

5. For the final project, open the PC-side folder and run the Python GUI:

```bash
cd FinalProject/PC_side
python Gui.py
```

Make sure the correct serial port is configured before communicating with the board.

## Summary

EmbeddedLabs documents a complete embedded-systems learning process. The labs start with basic hardware control and gradually move toward complete system integration. Each lab adds another important embedded concept, and the final project combines these concepts into a larger system that communicates with a PC and controls multiple hardware peripherals.

| Part | Main Focus |
|---|---|
| Lab 1 | GPIO, LEDs, PWM, RGB LED, and FSM basics |
| Lab 2 | Timers, LCD, ADC, frequency measurement, and buzzer control |
| Lab 3 | DMA, string processing, LCD output, and memory transfer |
| Lab 4 | UART, ADC, timers, LCD, PWM, and peripheral integration |
| Final Project | Full embedded system with PC GUI, UART, flash, scripts, LCD, and interrupts |

## License

This repository is licensed under the MIT License.
