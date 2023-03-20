from tkinter import *
import tkinter as tk

#width=1536 height=864
# importing tkinter gui

#creating window
window=tk.Tk()
canvas = Canvas()



#getting screen width and height of display
width= window.winfo_screenwidth()
height= window.winfo_screenheight()
print(width,height)

#setting tkinter window size
window.geometry("%dx%d" % (width, height))
window.config(bg="gray9")
window.title("Splash Screen")
label = tk.Label(window, text="VINIDRA",background="gray9",foreground="white")
label.pack()
#opening text
gcs = tk.Label(window, text="GROUND CONTROL STATION\n\n SOFTWARE")
gcs.config(background="gray9",foreground="white",font = ('Changa' , 44,'bold'))
gcs.place(x=320,y=200)

gcs = tk.Label(window, text="ASI & IN-SPACe's CANSAT India 2022")
gcs.config(background="gray9",foreground="white",font = ('Changa' , 13))
gcs.place(x=600,y=150)

gcs = tk.Label(window, text="Designed and Built by")
gcs.config(background="gray9",foreground="white",font = ('Changa' , 13))
gcs.place(x=650,y=550)


frame1=tk.Frame(window,width=200,height=100,bg="gray16")
frame1.place(x=600,y=600)
# load image to be "edited"
image = tk.PhotoImage(file="demo-repo\Images\inspace.png")
original_image = image.subsample(3,3)  # resize image using subsample
#Label(frame1, image=original_image).grid(row=1, column=0, padx=5, pady=5)

# Display image in right_frame
Label(frame1, image=image).grid(row=0,column=0, padx=5, pady=5)

gcs = tk.Label(window, text="Git Hub:https://github.com/stzfao/CANSAT_InSpace_2022_2023")
gcs.config(background="gray9",foreground="white",font = ('Changa' , 12))
gcs.place(x=520,y=720)
window.mainloop()