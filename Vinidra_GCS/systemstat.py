from tkinter import *
import tkinter as tk
import plain
import pandas as pd

def system_statistics():
    frame9=tk.Frame(plain.frame3,width=800,height=30,bg="white")
    frame9.place(x=50,y=80)
    text6 = tk.Label(plain.frame3,text="Launch pad")
    text6.config(background="light slate blue",foreground="white",font=('Arial',20))
    text6.place(x=50,y=140)

    text6 = tk.Label(plain.frame3,text="Ascent")
    text6.config(background="light slate blue",foreground="white",font=('Arial',20))
    text6.place(x=250,y=140)

    text6 = tk.Label(plain.frame3,text="Descent 1")
    text6.config(background="light slate blue",foreground="white",font=('Arial',20))
    text6.place(x=400,y=140)

    text6 = tk.Label(plain.frame3,text="Descent 2")
    text6.config(background="light slate blue",foreground="white",font=('Arial',20))
    text6.place(x=580,y=140)

    text6 = tk.Label(plain.frame3,text="Landed")
    text6.config(background="light slate blue",foreground="white",font=('Arial',20))
    text6.place(x=750,y=140)
    sysstat = pd.read_csv("demo-repo\Data\Book2.csv")
    altitude = sysstat['altitude'].tolist()
    print(altitude)
    for i in altitude:
       if(i<30 and i!=0):
          frame10=tk.Frame(frame9,width=150,height=30,bg="light slate blue")
          frame10.place(x=0,y=0)
          text3 = tk.Label(plain.window,text="")
          text3.config(background="light slate blue",foreground="white",font=('Arial',14))
          text3.place(x=1100,y=140)
       if(i<120):
          frame10=tk.Frame(frame9,width=250,height=30,bg="light slate blue")
          frame10.place(x=0,y=0)
          text3 = tk.Label(plain.window,text="")
          text3.config(background="light slate blue",foreground="white",font=('Arial',14))
          text3.place(x=1100,y=140)
       if(50<i<120):
          frame10=tk.Frame(frame9,width=350,height=30,bg="light slate blue")
          frame10.place(x=0,y=0)
          text3 = tk.Label(plain.window,text="DESCENT1")
          text3.config(background="light slate blue",foreground="white",font=('Arial',14))
          text3.place(x=1100,y=140)
       if(i<50):
          frame10=tk.Frame(frame9,width=400,height=30,bg="light slate blue")
          frame10.place(x=0,y=0)
          text3 = tk.Label(plain.window,text="DESCENT1")
          text3.config(background="light slate blue",foreground="white",font=('Arial',14))
          text3.place(x=1100,y=140)
       if(i==0):
          frame10=tk.Frame(frame9,width=800,height=30,bg="light slate blue")
          frame10.place(x=0,y=0)
          text3 = tk.Label(plain.window,text="")
          text3.config(background="light slate blue",foreground="white",font=('Arial',14))
          text3.place(x=1100,y=140)
