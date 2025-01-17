# Clock-Watch-NUC140

This repository contains the implementation of a stopwatch system on the NUC140 development board. The system is designed to emulate the functionality of a digital stopwatch with additional features such as alarm setting, lap time recording, and display management using LEDs and a 7-segment display.

## Features

### 1. Idle Mode

- The system starts in Idle Mode after reset.
- LED 5 is turned on to indicate Idle Mode.
- The system waits for user input to transition into other modes.

### 2. Alarm Set Mode

- Activated by pressing Key K3.
- LED 8 is turned on, while other LEDs are turned off.
- Users can set an alarm time (in seconds) using the PB15 button:
  - Increment the alarm time by 1 second per press, cycling from 0 to 59 seconds.
  - The alarm time is displayed on a 4 x 7-segment LED.
- Press Key K3 again to save the alarm time and return to Idle Mode.

### 3. Count Mode

- Activated by pressing Key K1.
- LED 6 is turned on to indicate Count Mode.
- The stopwatch begins counting with a precision of 1/10 second, using a Timer0 interrupt.
- The elapsed time is displayed on the 4 x 7-segment LED display.
- The buzzer is triggered three times when the elapsed time matches the set alarm time.
- Pausing and resuming:
  - Press Key K1 to pause or resume the stopwatch.
  - LED 7 is turned on during Pause Mode.

### 4. Lap Time Recording

- Press Key K9 during Count Mode to record lap times.
- Up to 5 lap times can be recorded, with new lap times overwriting older ones.
- Recorded lap times can be reviewed in Check Mode.

### 5. Check Mode

- Activated by pressing Key K5 during Pause Mode.
- Displays recorded lap times on the 4 x 7-segment display:
  - The first digit shows the lap number.
  - The remaining three digits show the lap time in tenths of a second.
- Press the PB15 button to cycle through lap times.
- Exit Check Mode by pressing Key K5 again.

### 6. Reset

- Press Key K9 during Pause Mode to reset the stopwatch and return to Idle Mode.

## Implementation Details

### Hardware Setup

- The system uses the NUC140 microcontroller.
- GPIO pins are configured for button input, LED control, and segment display.
- Timer0 and external interrupts are utilized for precision timing and input handling.

### Software Design

- Finite State Machine (FSM): Used to manage transitions between modes (Idle, Alarm Set, Count, Pause, Check).
- Timer0 Interrupt: Generates 1/10-second delays for stopwatch functionality.
- Modular Design: Key functionalities such as GPIO configuration, timing, and state handling are modularized for clarity and maintainability.

## Usage

1. **Clone the Repository**  
   Download or clone this repository to your local machine.

2. **Open the Project in Keil MDK**

   - Ensure you have **Keil µVision IDE** installed on your system.
   - Open the Keil IDE and navigate to **File > Open Project**.
   - Browse to the project directory and select the `.uvproj` file.

3. **Build the Project**

   - In Keil, click on **Project > Build Target** or press the `F7` key to compile the project.
   - Check the output window for any compilation errors or warnings.

4. **Load the Code onto the NUC140 Board**

   - Connect your NUC140 board to the computer using a USB cable.
   - Click on **Flash > Download** or press the `Ctrl+F8` key to program the board.
   - Wait for the flashing process to complete.

5. **Run and Test the Application**

   - After the code is flashed successfully, reset the board.
   - Interact with the application using the keys on the NUC140 board:

     - `K3`: Alarm Set Mode
     - `K1`: Start/Stop
     - `K9`: Record Lap/Reset
     - `K5`: Check Mode
     - `PB15`: Increment alarm or cycle through lap times.

   - Verify the functionality using the 7-segment display, LEDs, and buzzer.

## Demonstration

The system has been tested to meet all functional requirements, including:

- Accurate timing and alarm triggers.
- Smooth transitions between modes.
- Correct handling of lap times and display output.

🎥 **Watch the Demonstration Video**:

[![Clock Watch NUC140](https://img.youtube.com/vi/IP3_0Url3qM/hqdefault.jpg)](https://www.youtube.com/watch?v=IP3_0Url3qM)
