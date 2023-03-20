import matplotlib.pyplot as plt
from matplotlib import animation
import pandas as pd
import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import plain
from tkinter import *
import os


path = "demo-repo\Data\sample_data.csv"
if os.path.exists(path):
    if os.path.isfile(path):
        data = pd.read_csv(path)
    elif os.path.isdir(path):
        print("directory is existed")

else:
    f = open(path, "w")
    f.close()

cond = False
count = 0
x=[]
y=[]
x1=[]
y1=[]
x2=[]
y2=[]
x3=[]
y3=[]
x4=[]
y4=[]
x5=[]
y5=[]
x6=[]
y6=[]
x7=[]
y7=[]
x8=[]
y8=[]
x9=[]
y9=[]
x10=[]
y10=[]
x11=[]
y11=[]



def plot_start():
    global cond
    cond = True

def draw_graph():
    global cond, y, x, y1, x1, sample_data, count, lines, chart_type
    
    if cond == True and count < (sample_data.size / 2) - 1:
        # Draw temperature graph
        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('TEMPERATURE')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=0, y=0, width=312, height=183)
        count += 1
        a = sample_data['TEMP'][count]
        y.append(a)
        x.append(count)
        arr = np.array(y)
        print(arr)
        ax.plot(x, y)
        chart_type.draw()
        
        # Draw altitude graph
        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('ALTITUDE')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=312, y=0,width=312, height=183)
        count += 1
        a = sample_data['ALTITUDE'][count]
        y1.append(a)
        x1.append(count)
        arr = np.array(y1)
        print(arr)
        ax.plot(x1, y1)
        chart_type.draw()


        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('VOLTAGE')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=624, y=0, width=312, height=183)
        count += 1
        a = sample_data['VOLTAGE'][count]
        y2.append(a)
        x2.append(count)
        arr = np.array(y2)
        print(arr)
        ax.plot(x2, y2)
        chart_type.draw()
    

        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('GNS Time')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=936, y=0, width=312, height=183)
        count += 1
        a = sample_data['GNSS Time'][count]
        y3.append(a)
        x3.append(count)
        arr = np.array(y3)
        print(arr)
        ax.plot(x3, y3)
        chart_type.draw()
    

        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('GNSS LATITUDE')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=0, y=183, width=312, height=183)
        count += 1
        a = sample_data['GNSS LATITUDE'][count]
        y4.append(a)
        x4.append(count)
        arr = np.array(y4)
        print(arr)
        ax.plot(x4, y4)
        chart_type.draw()
    
        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('GNSS LONGITUDE')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=312, y=183, width=312, height=183)
        count += 1
        a = sample_data['GNSS LONGITUDE'][count]
        y5.append(a)
        x5.append(count)
        arr = np.array(y5)
        print(arr)
        ax.plot(x5, y5)
        chart_type.draw()


        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('GNSS ALTITUDE')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=642, y=183, width=312, height=183)
        count += 1
        a = sample_data['GNSS ALTITUDE'][count]
        y6.append(a)
        x6.append(count)
        arr = np.array(y6)
        print(arr)
        ax.plot(x6, y6)
        chart_type.draw()
    
        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('GNSS STATS')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=936, y=183, width=312, height=183)
        count += 1
        a = sample_data['GNSS STATS'][count]
        y7.append(a)
        x7.append(count)
        arr = np.array(y7)
        print(arr)
        ax.plot(x7, y7)
        chart_type.draw()
    
        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('PRESSURE')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=0, y=366, width=312, height=183)
        count += 1
        a = sample_data['PRESSURE'][count]
        y8.append(a)
        x8.append(count)
        arr = np.array(y8)
        print(arr)
        ax.plot(x8, y8)
        chart_type.draw()
    
        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('ACCELEROMETER')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=312, y=366, width=312, height=183)
        count += 1
        a = sample_data['ACCELEROMETER'][count]
        y9.append(a)
        x9.append(count)
        arr = np.array(y9)
        print(arr)
        ax.plot(x9, y9)
        chart_type.draw()
    
        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('GYRO SPIN RATE')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=624, y=366, width=312, height=183)
        count += 1
        a = sample_data['GYRO SPIN RATE'][count]
        y10.append(a)
        x10.append(count)
        arr = np.array(y10)
        print(arr)
        ax.plot(x10, y10)
        chart_type.draw()

        figure = plt.figure()
        ax = figure.add_subplot(111)
        ax.set_title('STATE')
        chart_type = FigureCanvasTkAgg(figure, master=plain.window1)
        chart_type.get_tk_widget().place(x=936, y=366, width=312, height=183)
        count += 1
        a = sample_data['STATE'][count]
        y11.append(a)
        x11.append(count)
        arr = np.array(y10)
        print(arr)
        ax.plot(x11, y11)
        chart_type.draw()
    
        
    

    plain.window1.after(1000, draw_graph)

def plot_start():
    global cond
    cond = True
    plain.window1.deiconify()



plain.window.after(1000, draw_graph)


