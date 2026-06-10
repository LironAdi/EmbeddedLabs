import customtkinter as ctk
import serial
from tkinter import filedialog
import math
import os
import matplotlib.pyplot as plt
import numpy as np
import threading, queue
import time

# UART Setup
ser = serial.Serial("COM6", 9600,parity=serial.PARITY_NONE, stopbits=1, timeout=0.1)
uart_q = queue.Queue()
stop_event = threading.Event()
reader_thread = None

# GUI Setup
ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")

root = ctk.CTk()
root.geometry("600x400")
root.title("DCS Project GUI")

# Tabs
tabview = ctk.CTkTabview(root)
tabview.pack(expand=True, fill="both")

tabview.add("Objects Detector")
tabview.add("Telemeter")
tabview.add("Light Sources")
tabview.add("max distance")
tabview.add("File Mode")

# Tab 1: Objects Detector
frame1 = tabview.tab("Objects Detector")
title1 = ctk.CTkLabel(frame1, text="Objects Detector System", font=("Arial", 16))
title1.pack(pady=10)

output1 = ctk.CTkTextbox(frame1, height=200, width=500)
output1.pack()
Running = False
max_dist = 200
measurements = []
measurements_light = []  # dvir add here
left_calib = []  # dvir add here
right_calib = []  # dvir add here
source = []  # dvir add here
# --- Custom calibration override ---
custom_calib = [36, 40, 44, 48, 52, 56, 58, 60, 62, 64, 69, 72, 75, 78, 81, 87, 91, 95, 99, 103, 107, 109, 111, 113,
                115, 119, 123, 127, 131, 135, 140, 140, 140, 140, 140, 140, 145, 150, 155, 160, 168, 169, 170, 171, 172,
                173, 174, 175, 176, 177]
use_custom_calib = False


def task1_start_scanning():
    global Running
    ser.reset_input_buffer()  # clean the buffer before the start
    ser.write('@'.encode())
    ser.write('A'.encode())
    Running = True
    output1.insert("end", "Started Object Scan...\n")
    objects_scan()


def objects_scan():
    global Running
    if Running:
        result = ser.read(3)
        if len(result) == 3:
            angle = result[0]
            distance = result[1] | (result[2] << 8)
            measurements.append([angle, distance])
            ser.reset_input_buffer()  # clean the buffer before the start
            if distance < max_dist:
                output1.insert("end", f"Angle: {angle}, Distance: {distance}\n")

            output1.see("end")
        root.after(50, objects_scan)  # the UART is delivering every 50ms 48 Byte


def stop_scan():
    global Running
    Running = False
    output1.insert("end", "stopped scan.\n")
    ser.write('@'.encode())
    ser.write('S'.encode())
    objects = identify_objects(measurements)
    # objects = [[2,3,4],[3,6,7]]
    i = 0
    plot_scan_polar(measurements)
    distances = [obj[1] for obj in objects]
    first_elements = [obj[0] for obj in objects]
    final = list(zip(first_elements, distances))
    plot_scan_polar(final)
    for i in range(len(objects)):
        output1.insert("end",
                       f"obj{i}: start_angle:{objects[i][0]} ,distance:{objects[i][1]} ,length:{objects[i][2]}\n")
        output1.see("end")
    measurements.clear()
    objects.clear()


def identify_objects(my_list):
    object_dist = []
    length = len(my_list)
    k = 0

    while k < length - 1:
        start_angle = my_list[k][0]
        distances = [my_list[k][1]]
        flag_obj = True
        penalty = 0

        while flag_obj and k < length - 2:  # in object
            k += 1
            current_dist = my_list[k][1]
            ref = sum(distances) / len(distances)
            temp_max = ref * 1.12
            temp_min = ref * 0.88

            if temp_min <= current_dist <= temp_max and current_dist < max_dist:

                distances.append(current_dist)
                penalty = 0

            else:
                penalty += 1

            if penalty > 2:
                flag_obj = False

        final_angle = my_list[k][0]
        avg_distance = sum(distances) / len(distances)
        len_obj = avg_distance * math.sin(math.radians(final_angle - start_angle) / 2)

        if len_obj >= 3 and len(distances) >= 3:
            center_angle = start_angle + (final_angle - start_angle) / 2
            object_dist.append([center_angle, avg_distance, round(len_obj, 1)])

        if len(object_dist) == 5:
            break

        k += 1

    return object_dist


def plot_scan_polar(measurements, title="measurements "):
    angles_deg = [m[0] for m in measurements if m[1] <= max_dist]
    distances = [m[1] for m in measurements if m[1] <= max_dist]
    angles_rad = np.radians(angles_deg)

    plt.figure(figsize=(8, 8))
    ax = plt.subplot(111, polar=True)
    ax.scatter(angles_rad, distances, marker="o", linestyle="-")

    ax.set_theta_zero_location("E")
    ax.set_theta_direction(1)
    ax.set_thetamin(0)
    ax.set_thetamax(180)

    ax.set_title(title, va="bottom")
    plt.show()


btn1_start = ctk.CTkButton(frame1, text="Start Scan", command=task1_start_scanning)
btn1_start.pack(pady=5)  # geometry
btn1_stop = ctk.CTkButton(frame1, text="Stop", command=stop_scan)
btn1_stop.pack(pady=5)

# Tab 2: Telemeter
frame2 = tabview.tab("Telemeter")
title2 = ctk.CTkLabel(frame2, text="Telemeter", font=("Arial", 16))
title2.pack(pady=10)

angle_entry = ctk.CTkEntry(frame2, placeholder_text="Enter angle (0-180)")
angle_entry.pack(pady=5)

output2 = ctk.CTkLabel(frame2, text="Distance: ---")
output2.pack(pady=10)


def task2_scanning():
    global Running, max_dist
    # Running = True
    ser.reset_input_buffer()  # clean the buffer before the start
    ser.write('@'.encode())
    ser.write('M'.encode())
    if Running:
        result = ser.read(2)
        if len(result) == 2:
            distance = result[0] | (result[1] << 8)
            if distance <= max_dist:
                output2.configure(text=f"Distance:{distance}")
        root.after(50, task2_scanning)


def task2_stop_scanning():
    global Running
    Running = False
    # output2.insert("end", "stopped scan.\n")
    ser.reset_input_buffer()
    ser.write('@'.encode())
    ser.write('S'.encode())
    output2.configure(text="stop")


def send_angle():
    global Running
    Running = True
    angle = angle_entry.get()
    if angle.isdigit():  # checks for num
        ser.reset_input_buffer()  # clean the buffer before the start
        ser.write('@'.encode())
        ser.write('B'.encode())
        ser.write(angle.encode())  # sends angle to the pyhton
        output2.configure(text=f"Sent angle {angle}, waiting for distance...")
        root.after(50, task2_scanning)
    else:
        output2.configure(text="Invalid angle")


btn2 = ctk.CTkButton(frame2, text="Go", command=send_angle)
btn2.pack(pady=5)

# btn2_scanning = ctk.CTkButton(frame2, text="scanning", command=task2_scanning)
# btn2_scanning.pack(pady=5)

btn2_stop = ctk.CTkButton(frame2, text="Stop", command=task2_stop_scanning)
btn2_stop.pack(pady=5)

# Tab 3: Light Sources
frame3 = tabview.tab("Light Sources")
title3 = ctk.CTkLabel(frame3, text="Light Sources Detector", font=("Arial", 16))
title3.pack(pady=10)


def use_custom_calibration():
    """Override left/right calibration with the chosen array (50 values)."""
    global left_calib, right_calib, use_custom_calib
    left_calib = [bytes([v]) for v in custom_calib]
    right_calib = [bytes([v]) for v in custom_calib]
    use_custom_calib = True
    output3.insert("end", "Using custom calibration array (override ON).\n")
    output3.see("end")


def calibration():
    output3.delete("1.0", "end")
    ser.reset_input_buffer()  # clean the buffer before the start
    ser.write('@'.encode())
    ser.write('C'.encode())
    check_if_finish()


def check_if_finish():
    finish = ser.read(1)
    if finish:
        # show_light_scan_controls()
        #  ser.reset_input_buffer()  # clean the buffer before the start
        get_calibration()
        # for ch in left_calib:
        # print(ord(left_calib))
        # print(right_calib)


    else:
        root.after(100, check_if_finish)


# def show_light_scan_controls():
#     output3.pack()
#     btn3.pack(pady=5)


btn3_calib = ctk.CTkButton(frame3, text="calibration", command=calibration, fg_color="red", hover_color="#aa0000")
btn3_calib.pack(pady=5)

btn3_use_custom = ctk.CTkButton(frame3, text="Upload old Calibration", command=use_custom_calibration)
btn3_use_custom.pack(pady=5)


output3 = ctk.CTkTextbox(frame3, height=200, width=500)
output3.pack()

def start_light_scan():
    global Running
    Running = True
    ser.reset_input_buffer()  # clean the buffer before the start
    ser.write('@'.encode())
    ser.write('L'.encode())  ##dvir add here
    output3.insert("end", "Started Light Sources Scan...\n")
    light_scan()


btn3 = ctk.CTkButton(frame3, text="Detect Lights", command=start_light_scan)
btn3.pack(pady=5)

def light_scan():  # dvir add this
    global Running
    global left_calib
    global right_calib
    if Running:
        result = ser.read(2)
        if len(result) == 2:
            angle = result[0]
            light_power = result[1]
            measurements_light.append([angle, light_power])
            output3.insert("end", f"Angle: {angle}, LightPower: {light_power}\n")
            output3.see("end")
            # check if reached 180°
            if angle == 180 or angle == b'\xB4':
                Running = False  # stop scanning
                # sample_to_num()
                left_calib_num = [ord(x) for x in left_calib]
                right_calib_num = [ord(x) for x in right_calib]
                source = light_analysis(left_calib_num, right_calib_num, measurements_light, 60)  # dvir add here
                plot_scan_polar(source, title="light beam ")
                plot_scan_polar(measurements_light, title="measurements_light ")
                source.clear()
                measurements_light.clear()
                output3.insert("end", left_calib_num[1])
                # output3.insert("end", source[0][0])

            # dvir add here
        if Running:
            root.after(50, light_scan)  # dvir add here   #dvir add this


def light_analysis(left_calib, right_calib, measurements_light, threshold=60, hysteresis=8, rise_limit=10):
    if len(left_calib) < 50 or len(right_calib) < 50:
        output3.insert("end", "Missing calibration (run calibration first).\n")
        return []

    sources = []
    in_source = False
    peak_angle, peak_val = None, 255
    source_count = 0
    rise_count = 0
    prev_val = None

    base_left = max(left_calib)
    base_right = max(right_calib)

    for angle, value in measurements_light:
        if angle < 90:
            diff = base_left - value
        elif angle > 90:
            diff = base_right - value
        else:
            diff = max(base_left, base_right) - value

        if not in_source and diff > threshold:
            in_source = True
            peak_angle, peak_val = angle, value
            rise_count = 0

        elif in_source:
            if value < peak_val:
                peak_val = value
                peak_angle = angle

            if prev_val is not None and value > prev_val:
                rise_count += 1
            else:
                rise_count = 0

            if rise_count >= rise_limit:
                in_source = False
                source_count += 1
                peak_cm = find_nearest_distance(
                    left_calib if peak_angle <= 90 else right_calib,
                    peak_val
                )
                sources.append((peak_angle, peak_cm))
                output3.insert("end", f"Light Source #{source_count}: Distance = {peak_cm} cm, Angle = {peak_angle}°\n")
                output3.see("end")
                rise_count = 0

        prev_val = value

    if in_source:
        source_count += 1
        peak_cm = find_nearest_distance(
            left_calib if peak_angle <= 90 else right_calib,
            peak_val
        )
        sources.append((peak_angle, peak_cm))
        output3.insert("end",
                       f"Light Source #{source_count}: Distance = {peak_cm} cm, Angle = {peak_angle}°\n")
        output3.see("end")

    return sources


def find_nearest_distance(calib_values, measured_value):  # dvir add this
    diffs = [abs(v - measured_value) for v in calib_values]
    idx = diffs.index(min(diffs))
    return idx


def get_calibration():  # dvir add this
    for i in range(100):
        result = ser.read(1)
        if i < 50:
            left_calib.append(result)
        else:
            right_calib.append(result)


frame4 = tabview.tab("max distance")


def on_slide(value):
    label.configure(text=f"value: {int(value)}")


def save_value():
    global max_dist
    max_dist = int(slider.get())
    saved_label.configure(text=f"max_dist saved: {max_dist}")


slider = ctk.CTkSlider(frame4, from_=0, to=400, number_of_steps=400, command=on_slide)
slider.pack(pady=20)
print(slider)
label = ctk.CTkLabel(frame4, text="value: 0", font=("Arial", 16))
label.pack()
saved_label = ctk.CTkLabel(frame4, text="max_dist saved: 0", font=("Arial", 14))
saved_label.pack()
save_button = ctk.CTkButton(frame4, text="save value", command=save_value)
save_button.pack(pady=10)

# Tab 5: File Mode
frame5 = tabview.tab("File Mode")
title5 = ctk.CTkLabel(frame5, text="File Mode", font=("Arial", 16))
title5.pack(pady=10)

output5 = ctk.CTkTextbox(frame5, height=200, width=500)
output5.pack()



def text_file():
    file_type = 'T'
    filepath = filedialog.askopenfilename()
    if filepath:
        filename = os.path.basename(filepath)
        filename_no_ext = os.path.splitext(filename)[0]
        with open(filepath, "r") as f:
            data = f.read()
            data_size = len(data)
            size_str = str(data_size).zfill(4)
            filename_without = filename_no_ext.ljust(12, ' ')
            ser.write('@'.encode())
            ser.write('D'.encode())
            ser.write(file_type.encode())
            ser.write(size_str.encode())
            ser.write(filename_without.encode())
            ser.write(data.encode())
            print(f"{filename_without}X")
            output5.insert("end", "text send\n")
            output5.insert("end", "text stored successfully\n")
            output5.see("end")
            # output5.insert("end", f"fill_content: {data}\n")
            # output5.insert("end", f"Data size: {filename_without}\n")
            # output5.insert("end", f"File type: {file_type}\n")
            # output5.insert("end", f"Full path: {filepath}\n")


def script_file():
    global Running
    file_type = 'S'
    filepath = filedialog.askopenfilename()
    if filepath:
        filename = os.path.basename(filepath)
        filename_no_ext = os.path.splitext(filename)[0]
        with open(filepath, "r") as f:
            lines = [line.strip() for line in f]
            translated_lines = translate_script_file(lines)
            data_size = len(translated_lines)
            size_str = str(data_size).zfill(4)
            filename_without = filename_no_ext.ljust(12, ' ')
            ser.write('@'.encode())
            ser.write('D'.encode())
            ser.write(file_type.encode())
            ser.write(size_str.encode())
            ser.write(filename_without.encode())
            ser.write(translated_lines.encode())
            output5.insert("end", "script send\n")
            output5.insert("end", "script stored successfully\n")
            output5.see("end")
            # output5.insert("end", f"translated_lines: {translated_lines}\n")
            # output5.insert("end", f"Data size: {filename_without}\n")
            # output5.insert("end", f"File type: {file_type}\n")
            # output5.insert("end", f"Full path: {filepath}\n")
            # start_servo_deg()
            # start_servo_scan()
            # start_uart_handler()
            start_uart_thread()


def translate_script_file(lines):
    command_map = {
        "inc_lcd": "01",
        "dec_lcd": "02",
        "rra_lcd": "03",
        "set_delay": "04",
        "clear_lcd": "05",
        "servo_deg": "06",
        "servo_scan": "07",
        "sleep": "08"
    }

    translated_lines = []  # List to accumulate the translated lines
    result = ''
    for line in lines:
        parts = line.strip().split()
        command = parts[0]
        if command not in command_map:
            raise ValueError("Unknown command: {command}")

        hex_value = command_map[command]

        if len(parts) > 1:
            if command == "servo_scan":
                servo_args = parts[1].split(',')
                if len(servo_args) != 2:
                    raise ValueError("Invalid number of arguments for servo_scan")
                servo_value = format(int(servo_args[0]), '02X') + format(int(servo_args[1]), '02X')
                hex_value += servo_value
            else:
                value = int(parts[1])
                hex_value += format(value, '02X')
        result = result + hex_value + "\n"
        # translated_lines.append(hex_value + "\n")
    return result


def start_uart_handler():
    global Running, measurements, count_data, pending_response

    # Reset state for both modes (scan or distance)
    Running = True
    count_data = 0
    pending_response = False
    measurements = []

    ser.reset_input_buffer()
    root.after(50, wait_for_uart_response)


def wait_for_uart_response():
    global Running, measurements, count_data, pending_response

    while ser.in_waiting >= 1:
        opc = ser.read(1)

        if opc == b'D':  # Distance mode (servo_deg)
            if ser.in_waiting >= 2:
                data = ser.read(2)
                if len(data) == 2:
                    distance = data[0] | (data[1] << 8)
                    output2.configure(text=f"Distance: {distance}")
                    count_data += 1
                    pending_response = True

        elif opc == b'S':  # Scan mode (angle + distance)
            if ser.in_waiting >= 3:
                data = ser.read(3)
                if len(data) == 3:
                    angle = data[0]
                    distance = data[1] | (data[2] << 8)

                    if distance < max_dist:
                        output5.insert("end", f"Angle: {angle}, Distance: {distance}\n")
                        output5.see("end")

                    measurements.append((angle, distance))

                    if angle == 180:
                        Running = False
                        output5.insert("end", "Scan complete.\n")
                        output5.see("end")
                        return

        else:
            print(f"Unknown opcode: {opc}")
            ser.reset_input_buffer()

    # Stop distance mode after receiving 50 values
    if count_data >= 50:
        Running = False
        output2.configure(text="Done")
        return

    # For servo_deg mode: request next data
    if Running and pending_response and count_data < 50:
        ser.write(b'@')
        ser.write(b'M')
        pending_response = False

    if Running:
        root.after(50, wait_for_uart_response)


def serial_reader():
    while not stop_event.is_set():
        try:
            if ser.in_waiting == 0:
                time.sleep(0.01)
                continue

            if ser.in_waiting >= 1:
                opc = ser.read(1)
                print(f"opc: {opc}")
            else:
                continue

            # Distance mode - 'D' + 2 bytes
            if opc == b'D':
                while ser.in_waiting < 2:
                    time.sleep(0.001)
                data = ser.read(2)
                print(f"data: {data}")
                if len(data) != 2:
                    continue
                distance = data[0] | (data[1] << 8)
                uart_q.put(('D', distance))

            # Scan mode - 'S' + angle + 2-byte distance
            elif opc == b'S':
                while ser.in_waiting < 3:
                    time.sleep(0.001)
                data = ser.read(3)
                if len(data) != 3:
                    continue
                angle = data[0]
                distance = data[1] | (data[2] << 8)
                uart_q.put(('S', angle, distance))

            # File mode end signal (optional)
            elif opc == b'F':
                uart_q.put(('F',))

            # Unknown opcode
            else:
                print(f"[WARNING] Unknown OPC: {opc}, ser.in_waiting={ser.in_waiting}")
                uart_q.put(('?', opc))

        except Exception as e:
            uart_q.put(('ERR', str(e)))
            break


def process_uart_queue():
    try:
        while True:
            msg = uart_q.get_nowait()

            if msg[0] == 'D':
                distance = msg[1]
                # output2.configure(text=f"Distance: {distance}")
                output5.insert("end", f"Distance: {distance}\n")
                output5.see("end")

            elif msg[0] == 'S':
                angle, distance = msg[1], msg[2]
                if distance < max_dist:
                    output5.insert("end", f"Angle: {angle}, Distance: {distance}\n")
                    output5.see("end")

                if angle == 180:
                    output5.insert("end", "Scan complete.\n")
                    output5.see("end")
            elif msg[0] == 'F':
                output5.insert("end", "File transfer complete.\n")
                output5.see("end")
            elif msg[0] == 'ERR':
                output5.insert("end", f"[UART error] {msg[1]}\n")
                output5.see("end")

            else:
                output5.insert("end", f"[UART] Unknown: {msg}\n")

                output5.see("end")

    except queue.Empty:
        pass

    if not stop_event.is_set():
        root.after(50, process_uart_queue)


def start_uart_thread():
    global reader_thread
    if reader_thread and reader_thread.is_alive():
        return
    stop_event.clear()
    reader_thread = threading.Thread(target=serial_reader, daemon=True)
    reader_thread.start()
    root.after(50, process_uart_queue)


def stop_uart_thread():
    stop_event.set()


def on_close():
    try:
        stop_uart_thread()
        if ser and ser.is_open:
            ser.close()
    finally:

        root.destroy()


root.protocol("WM_DELETE_WINDOW", on_close)

btn5_text = ctk.CTkButton(frame5, text="Upload text file", command=text_file)
btn5_text.pack(pady=5)
btn5_script = ctk.CTkButton(frame5, text="Upload script file", command=script_file)
btn5_script.pack(pady=5)

# Run
root.mainloop()
