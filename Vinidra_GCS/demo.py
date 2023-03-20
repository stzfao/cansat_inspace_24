import plain
from tkinter import *
import maps2
import systemstat
import gps
import SerialMonitor

button1 = Button(plain.frame5,wraplength=200, text="SYSTEM STATISTICS")
button1.config(background="light slate blue",foreground="white", font=('Arial', 18),command=lambda:systemstat.system_statistics())
button1.place(x=30, y=50)

button = Button(plain.frame5,wraplength=200, text="SERIAL MONITOR")
button.config(background="light slate blue",foreground="white", font=('Arial', 20),command=lambda:SerialMonitor.read_serial())
button.place(x=30, y=180)

button = Button(plain.frame5,wraplength=200,text="GRAPHED DATA")
button.config(background="light slate blue",foreground="white", font=('Arial', 20),command=lambda:maps2.plot_start())
button.place(x=30, y=310)

button = Button(plain.frame5,wraplength=200, text="GPS MAP")
button.config(background="light slate blue",foreground="white", font=('Arial', 21),command=lambda:gps.loop(gps.map_widget))
button.place(x=30, y=450)

#button = Button(plain.window,text="Quit",command=plain.frame3.destroy)
#button.config(background="light slate blue",foreground="white",font=('Arial',13))
#button.place(x=50,y=10)
plain.window.mainloop()