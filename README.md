# CANSAT India '22-24 - Flight Software & Ground Control Station

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) 
## Overview

This repository contains the source code for the Flight Software (FSW) and Ground Control Station (GCS) developed for the CANSAT India '22 competition, organized by the Astronautical Society of India (ASI) and the Indian National Space Promotion and Authorisation Centre (IN-SPACe).

Our team designed, built, launched, and operated a CanSat (a satellite simulation within the volume of a standard soda can) to gather atmospheric data and perform a defined mission during its descent. This software suite was responsible for controlling the CanSat's operations and communicating with the ground.

**Role:** Team Lead for Flight Software and Ground Control Station.

**Achievement:** Our team was honored with the **Best Teamwork Award** at the competition deployment in Ahmedabad.

## Architecture

The system consists of two primary software components:

1.  **Flight Software (FSW):** Runs on the CanSat's onboard microcontroller/computer.
    * **Responsibilities:**
        * Interface with various sensors (e.g., GPS, IMU, Barometer, Temperature sensor 
        * Process sensor readings.
        * Log critical data onboard to an SD Card retrievable post mission
        * Manage the RF communication module to transmit telemetry packets to the ground.
        * Control any mission-specific actuators or mechanisms.
    * **Language:** C++
    * **Platform:** *ESP32*

2.  **Ground Control Station (GCS):** Runs on a laptop/computer on the ground.
    * **Responsibilities:**
        * Receive telemetry packets via an RF communication module connected to the computer.
        * Parse the incoming data packets.
        * Display real-time telemetry data through a graphical user interface (GUI).
        * Visualize key parameters (e.g., altitude, temperature, GPS coordinates) using graphs and readouts.
        * Log all received data to files for post-mission analysis.
    * **Language:** Python
    * **GUI Framework:** Tkinter
    * **Design Tool:** Figma (for GUI mockups/design)

3.  **Communication Link:**
    * Utilizes RF modules for wireless data transmission between the FSW and GCS.
    * Implements a defined packet structure for telemetry data.

## Key Features

* **FSW:** Real-time multi-sensor data acquisition and processing.
* **FSW:** Onboard data logging for redundancy.
* **FSW:** Robust telemetry packet creation and RF transmission.
* **GCS:** Reliable RF telemetry reception and packet parsing.
* **GCS:** User-friendly GUI (built with Tkinter) for real-time data visualization (graphs, numerical readouts).
* **GCS:** Comprehensive data logging on the ground station for analysis.

## Technology Stack

* **Flight Software:** C++
* **Ground Control Station:** Python 3.x
* **GCS GUI:** Tkinter
* **GUI Design:** Figma
* **Version Control:** Git

## Setup and Installation

*The project contains two components, the onboard computing software and the telemetry software.*

**FSW:**

Load the software onto the ESP32 using the Arduino IDE.

**GCS:**

```bash
# Clone the repository
git clone [https://github.com/stzfao/cansat_inspace_24.git](https://github.com/stzfao/cansat_inspace_24.git)
cd cansat_inspace_24/ground_control_station

# Set up Python environment (e.g., venv)
python -m venv venv
source venv/bin/activate # On Windows use `venv\Scripts\activate`

# Install dependencies
pip install -r requirements.txt 

# Run the GCS application
python main.py
```

## License
This project is licensed under the MIT License - see the LICENSE.md file for details. [Make sure to add a LICENSE.md file with the chosen license text, e.g., MIT or Apache 2.0]

## Acknowledgements
* Astronautical Society of India (ASI)
* Indian National Space Promotion and Authorisation Centre (IN-SPACe)
* Cummins College of Engineering, Pune — for our professors' endless support
* Fellow teammates of Vinidra from 2022-2024 batch. 
