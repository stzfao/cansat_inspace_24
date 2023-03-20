import tkinter as tk
import serial
from tkinter import *
import plain
window=tk.Tk()
width= 1250
height= 550
window.geometry("%dx%d" % (width, height))
window.config(bg="gray9")
window.title("Serial Monitor Screen")

ser = serial.Serial('COM3', 9600) # replace with your own serial port and baud rate



text_box = tk.Text(window, height=2000, width=1200)
text_box.config(background="white",foreground="black", font=('Arial', 18))
text_box.pack()

def read_serial():
    data = ser.readline()
    # decode the data to a string
    try:
        data_str = data.decode('latin-1')
    except UnicodeDecodeError:
        # handle decoding errors
        print("Error decoding data:", data)
        return
    # strip off any leading bytes before the string starting with "$$"
    start_str = "2022"
    start_idx = data_str.find(start_str)
    if start_idx != -1:
        data_str = data_str[start_idx:]
        # print the string on the Tkinter window
        text_box.insert('end', data_str + '\n')
    text_box.see('end')
    window.after(10, read_serial)




