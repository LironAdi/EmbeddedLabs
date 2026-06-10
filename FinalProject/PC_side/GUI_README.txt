
========================
DCS GUI Application - README
========================

Description:
------------
This Python application is a graphical interface designed for interacting with a custom embedded system based on the MSP430 microcontroller. The system supports object detection, telemeter measurements, light source detection, and file management using UART communication. The GUI was built using CustomTkinter.

Main Features:
--------------
1. **Objects Detector Tab**
   - Initiates a scan of nearby objects (180° rotation).
   - Displays angle and distance in real time.
   - Polar plot for object visualization.

2. **Telemeter Tab**
   - Sends a fixed angle to the MCU and receives distance.
   - Continuous single-point measurement.

3. **Light Sources Tab**
   - Initiates scan for detecting light sources using calibrated LDR sensors.
   - Supports live display and graphical plotting.
   - Calibration (automatic or upload previous).

4. **Max Distance Tab**
   - Allows user to set a distance threshold slider to filter valid data.

5. **File Mode Tab**
   - Upload and send text/script files to MCU.
   - Parse and encode script files into hex protocol.
   - Display received scan data or script execution results.

Communication Protocol:
------------------------
- Serial UART at 9600 baud (COM6).
- Commands begin with '@' followed by an opcode (e.g. A, B, M, L, C...).
- Script file upload supports command translation to hex.
- Special commands used to toggle modes and read sensor data.

Dependencies:
-------------
- customtkinter
- pyserial
- matplotlib
- numpy

Execution:
----------
Simply run the file using Python 3.
Make sure your MCU is connected to COM6 or change `ser = serial.Serial(...)` accordingly.

Authors:
--------
Liron & Dvir (DCS Final Project)
