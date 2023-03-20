import pandas as pd
import plain
from tkinter import *
import tkintermapview
def loop(map_widget):
    data = pd.read_csv('D:\GENEARL\XBEE Confg\demo-repo\Book2.csv')
    lat = (data['latitude'].iloc[-1])
    lon = (data['longitude'].iloc[-1])
    add = (data['name'].iloc[-1])
    map_widget = tkintermapview.TkinterMapView(new2, width=800, height=600, corner_radius=0)
    map_widget.place(x=280, y=220, width=1100, height=500)
    marker = map_widget.set_marker(lat, lon, text="add")

    map_widget.set_zoom(7)

map_widget = tkintermapview.TkinterMapView(plain.window, width=800, height=600, corner_radius=0)

new2= Toplevel(plain.window)
new2.geometry("1250x550")