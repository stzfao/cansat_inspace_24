import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation
import collections
from collections import deque
from extract_data import parse_string

altitude = collections.deque(np.zeros(10))
dummy_data = open("../test_data/xbee_serial_out.txt","r")
all_lines = dummy_data.readlines()


def get_altitude(i):
    if not all_lines:
        return
    line = all_lines.pop(0)
    res = parse_string(line)
    altitude.append(int(res[3]))
    ax.cla()
    ax.plot(altitude)

if __name__ == '__main__':
    fig = plt.figure(figsize=(12,6), facecolor='#DEDEDE')
    ax = plt.subplot(121)
    ax.set_facecolor('#DEDEDE')
    # test
    # get_altitude(dummy_data)
    ani = FuncAnimation(fig, get_altitude, interval=1000)
    plt.show()
    dummy_data.close()
	