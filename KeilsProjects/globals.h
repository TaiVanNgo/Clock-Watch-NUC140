#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>

// Delay times in microseconds for various operations
#define DELAY_TIME 1000       // Standard delay time for main operations
#define LONG_DELAY_TIME 50000 // Long delay used for extended waiting periods
#define DEBOUNCE_DELAY 2000   // Debounce delay for button presses

// Enum representing the program states
// IDLE_MODE: Default idle mode for the program
// ALARM_SET_MODE: Mode to set the alarm time
// COUNT_MODE: Mode for counting (e.g., stopwatch mode)
// CHECK_MODE: Mode for reviewing stored lap times
enum programState
{
    IDLE_MODE,
    ALARM_SET_MODE,
    COUNT_MODE,
    CHECK_MODE,
};

// Structure for storing time values in the lap
typedef struct
{
    int minutes;         // Minutes component of the time
    int tensOfSeconds;   // Tens of seconds component
    int unitsOfSeconds;  // Units of seconds component
    int tenthsOfSeconds; // Tenths of seconds component
} Times;

// Declaration of global variables used across the program
extern enum programState state;           // Current program state
extern volatile bool isPB15Pressed;       // Flag: Is PB15 button pressed? (*)
extern volatile bool isAlarmSetModeStart; // Flag: Has state alarm set mode started?
extern volatile bool isCountModeStart;    // Flag: Has state count mode started?
extern int alarmTime;                     // Set alarm time in seconds
extern volatile bool setAlarmTime;        // Flag: Is alarm time being set?
extern volatile bool pause;               // Flag: Is the system paused?
extern Times countTimes;                  // Tracking counting times
extern int lapTimeIndex;                  // Index for lap times storage
extern volatile bool isOneTenthSecond;    // Flag for tenths of a second updates
extern volatile bool isCheckModeStart;    // Flag: Has state check mode started?
extern int lapCountPressed;               // Number of times lap button pressed
extern int tensDigit;                     // Tens digit for display
extern int unitsDigit;                    // Units digit for display
extern volatile bool isK9Pressed;         // Flag: Is button K9 pressed?
extern volatile Times times[5];           // Array storing recorded lap times
extern bool isInCountMode;                // Flag: Is the program in counting mode?
extern int buzzerTriggerCount;            // check the time buzzer count

#endif // GLOBALS_H
