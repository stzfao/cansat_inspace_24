from tkinter import *
import tkinter as tk
import psutil
import datetime as dt
import time
import xbee
import telemetry

#creating window
window=tk.Tk()
canvas = Canvas()
battery = psutil.sensors_battery()
date=dt.datetime.now()
format_date=f"{date:%a, %b %d %Y}"
time2 = time.strftime('%H:%M:%S')

#getting screen width and height of display
width= window.winfo_screenwidth()
height= window.winfo_screenheight()
print(width,height)

#setting tkinter window size
window.geometry("%dx%d" % (width, height))
window.config(bg="gray9")
window.title("System Statistics Screen")
label = tk.Label(window, text="VINIDRA")
label.config(background="gray9",foreground="white")
label.pack()

frame=tk.Frame(window,width=1490,height=150,bg="gray9")
frame.pack(side="top")
frame1=tk.Frame(frame,width=200,height=100,bg="gray16")
frame1.place(x=30,y=25)
# load image to be "edited"
image = tk.PhotoImage(file="demo-repo\Images\inspace.png")
original_image = image.subsample(3,3)  

# Display image in right_frame
Label(frame1, image=image).grid(row=0,column=0, padx=5, pady=5)

frame2=tk.Frame(frame,width=600,height=200,bg="gray18")
frame2.place(x=400,y=20)
#text
com_port = tk.Label(frame2, text="Select COM Port")
com_port.config(background="gray18",foreground="white",font = ('Arial' , 13))
com_port.place(x=65,y=25)

def show():
    label.config( text = clicked.get() ,font = ('Arial' , 10))

# Dropdown menu options
options = [
    "COM1",
    "COM2",
]

# datatype of menu text
clicked = StringVar()


# initial menu text
clicked.set( "Select" )

# Create Dropdown menu
drop = tk.OptionMenu( frame2 , clicked , *options )
drop.config(background="gray9",foreground="white")
drop.place(x=200,y=20)

#button for pan id
button = tk.Button(frame2,text="Set PanId")
button.config(background="gray9",foreground="white",font = ('Arial' , 10),command=lambda:xbee.set_pan_id())
button.place(x=210,y=58)
#text
panid = tk.Label(frame2, text="XBEE Pan ID")
panid.config(background="gray18",foreground="white",font = ('Arial' , 13))
panid.place(x=80,y=60)


sys_date = tk.Label(frame2, text="System Date :")
sys_date.config(background="gray18",foreground="white",font = ('Arial' , 13))
sys_date.place(x=300,y=25)

sys_time= tk.Label(frame2, text="System Time :                   IST")
sys_time.config(background="gray18",foreground="white",font = ('Arial' , 13))
sys_time.place(x=300,y=60)

miss_time = tk.Label(frame2, text="Mission Time :")
miss_time.config(background="gray18",foreground="white",font = ('Arial' , 13))
miss_time.place(x=300,y=90)

frame3=tk.Frame(window,width=1250,height=550,bg="gray18")
frame3.place(x=250,y=200)

frame4=tk.Frame(frame3,width=100,height=100,bg="gray18")
frame4.place(x=40,y=450)

# load image to be "edited"
image1= tk.PhotoImage(file="demo-repo\Images\inspace.png")
original_image1 = image1.subsample(3,3)  


# Inspace image
Label(frame4, image=image1).grid(row=0,column=0, padx=5, pady=5)

frame5=tk.Frame(window,width=200,height=550,bg="gray18")
frame5.place(x=10,y=200)

button = tk.Button(window,text="Start Telemetry",command=lambda:telemetry.recieve_data())
button.config(background="light slate blue",foreground="white",font=('Arial',25))
button.place(x=1200,y=250)

#text
text1= tk.Label(window, text="Battery:         %" )
text1.config(background="gray9",foreground="white",font = ('Arial' , 13))
text1.place(x=1100,y=65)

text2= tk.Label(window, text="CURRENT STATUS" )
text2.config(background="gray9",foreground="white",font = ('Arial' , 13))
text2.place(x=1100,y=95)



text1= tk.Label(window, text=battery.percent)
text1.config(background="gray9",foreground="white",font = ('Arial' , 13))
text1.place(x=1180,y=65)

date=Label(frame2, text=format_date)
date.config(background="gray18",foreground="white", font=("Arial", 13))
date.place(x=420,y=25)

systime= Label (frame2, text=time2)
systime.config(background="gray18",foreground="white", font=("Arial", 13))
systime.place(x=420,y=60)

button = tk.Button(window,text="EXPORT\nCSV")
button.config(background="gray9",foreground="white",font=('Arial',13))
button.place(x=1400,y=110)

button = tk.Button(window,text="EXPORT\nPDF")
button.config(background="gray9",foreground="white",font=('Arial',13))
button.place(x=1400,y=40)

text4= tk.Label(frame3, text="Built for CANSAT '22 India Student Compietition\nheld in joint colaboration between ASI and IN-SPACe. " )
text4.config(background="gray18",foreground="white",font = ('Arial' , 12))
text4.place(x=350,y=460)
window1 = tk.Toplevel(window)
window1.geometry("1250x550+250+200")
window1.withdraw()