#include "functions.h" // Include the header file here
void CloseSegments();
const uint8_t DIGIT_PATTERN[] = {
    //   gedbaf_dot_c
    0b10000010, // Number 0          // ---a----
    0b11101110, // Number 1          // |      |
    0b00000111, // Number 2          // f      b
    0b01000110, // Number 3          // |      |
    0b01101010, // Number 4          // ---g----
    0b01010010, // Number 5          // |      |
    0b00010010, // Number 6          // e      c
    0b11100110, // Number 7          // |      |
    0b00000010, // Number 8          // ---d----
    0b01000010, // Number 9
    0b11111111, // Blank LED
    0b01111111, // '-' display (only g)
};
// set up key metrics
void KeyPadScanning()
{
    // Turn all Rows to LOW
    // clear all pins
    PA->DOUT &= ~(1 << 3);
    PA->DOUT &= ~(1 << 4);
    PA->DOUT &= ~(1 << 5);
    // check if the signal of which button is 1 then do the search colum for it
    if (C1_pressed)
    {
        SearchColumn(1);
    }
    else if (C2_pressed)
    {
        SearchColumn(2);
    }
    else if (C3_pressed)
    {
        SearchColumn(3);
    }
}

// add new time in the lap function
void addNewTime(Times lapTimes)
{
    // add lap to currect index of the array
    times[lapTimeIndex] = lapTimes;        // Add lap time
    lapTimeIndex = (lapTimeIndex + 1) % 5; // Cycle from 0 to 4
}

void SearchColumn(int colNum)
{
    if (colNum < 1 || colNum > 3)
        return;

    // Define row pins and corresponding patterns
    uint8_t row_pins[] = {3, 4, 5}; // Pins for rows GPA3, GPA4, GPA5

    for (int i = 0; i < 3; i++)
    {
        // Set the current row as low and others as high
        for (int j = 0; j < 3; j++)
        {
            if (i == j)
                PA->DOUT &= ~(1 << row_pins[j]); // Set the active row as low
            else
                PA->DOUT |= (1 << row_pins[j]); // Set others as high or clear the unused buttons
        }

        // Check if the button in the current row and column is pressed
        volatile bool isPressed = false;

        switch (colNum)
        {
        case 1:
            isPressed = C1_pressed;
            break;
        case 2:
            isPressed = C2_pressed;
            break;
        case 3:
            isPressed = C3_pressed;
            break;
        default:
            return;
        }
        // check for button pressed
        if (isPressed)
        {
            if (colNum == 1 && i == 0) // press k1 -> trigger state count mode
            {
                // change the state to count mode
                state = COUNT_MODE;
                // only pause when in count mode
                if (isCountModeStart)
                {
                    // toggle pause variable to check
                    pause = !pause;
                }
                // if it is not in ciubt mode count mode start.
                isCountModeStart = true;
            }

            if (colNum == 3 && i == 0) // press k3 -> trigger state 1
            {

                state = ALARM_SET_MODE; // update to ALARM_SET_MODE

                if (!setAlarmTime && isAlarmSetModeStart)
                {
                    // if the state one is started and the alarm time hasn't set
                    //-> save alarm time & reset flag
                    setAlarmTime = true;
                }
                isAlarmSetModeStart = true;

                CLK_SysTickDelay(LONG_DELAY_TIME);
            }

            if (colNum == 3 && i == 2)
            {
                if (pause)
                {
                    state = IDLE_MODE;        // Transition to Idle mode
                    pause = false;            // Reset pause state
                    isCountModeStart = false; // Reset count mode start flag
                    isCheckModeStart = false; // Reset check mode flag
                    resetCountTimes();
                    isAlarmSetModeStart = false;
                    alarmTime = 0;
                    setAlarmTime = false;
                    unitsDigit = 0;
                    tensDigit = 0;
                    lapTimeIndex = 0; // Clear time index
                    lapCountPressed = 0;
                    buzzerTriggerCount = 0; // Reset buzzer count

                    // Clear lap times
                    for (int j = 0; j < 5; j++)
                    {
                        times[j].minutes = -1;
                        times[j].tensOfSeconds = -1;
                        times[j].unitsOfSeconds = -1;
                        times[j].tenthsOfSeconds = -1;
                    }

                    CLK_SysTickDelay(LONG_DELAY_TIME); // Debounce delay
                }
                if (isInCountMode && !pause)
                {
                    isK9Pressed = true;
                }
            }

            if (pause)
            {
                if (colNum == 2 && i == 1)
                { // Check key pressed
                    if (!isCheckModeStart)
                    {
                        state = CHECK_MODE; // Enter Check Mode
                        isCheckModeStart = true;
                    }
                    else
                    {
                        state = COUNT_MODE; // Exit Check Mode
                        isCheckModeStart = false;
                        pause = true; // Ensure the clock pauses after exiting
                    }
                }
            }

            CLK_SysTickDelay(LONG_DELAY_TIME); // Add debounce

            return;
        }
    }
}
// turn on Digit at each potition of 7segments
void DisplayDigit(int digit, int segmentNum, bool openDot)
{
    /*
        Order for segment Num

        1  2  3  4
        [] [] [] []
    */
    int pattern = DIGIT_PATTERN[digit]; // get the digit pattern

    if (openDot)
    {
        pattern &= ~(1 << 1); // Clear bit 1
    }

    if (segmentNum == 1)
    {
        // turn on U11
        PC->DOUT &= ~(1 << 4);
        PC->DOUT &= ~(1 << 5);
        PC->DOUT &= ~(1 << 6);
        PC->DOUT |= (1 << 7);
    }
    else if (segmentNum == 2)
    {
        // turn on U12
        PC->DOUT &= ~(1 << 4);
        PC->DOUT &= ~(1 << 5);
        PC->DOUT |= (1 << 6);
        PC->DOUT &= ~(1 << 7);
    }
    else if (segmentNum == 3)
    {
        // turn on U13
        PC->DOUT &= ~(1 << 4);
        PC->DOUT |= (1 << 5);
        PC->DOUT &= ~(1 << 6);
        PC->DOUT &= ~(1 << 7);
    }
    else if (segmentNum == 4)
    {
        // turn on U14
        PC->DOUT |= (1 << 4);
        PC->DOUT &= ~(1 << 5);
        PC->DOUT &= ~(1 << 6);
        PC->DOUT &= ~(1 << 7);
    }

    PE->DOUT = pattern;
}
// close all segments when it is not in needs
void CloseSegments()
{
    PC->DOUT &= ~(1 << 4);
    PC->DOUT &= ~(1 << 5);
    PC->DOUT &= ~(1 << 6);
    PC->DOUT &= ~(1 << 7);
}

void OpenLEDs(int LEDnum)
{
    // Invalid parameter, do nothing
    if (LEDnum < 5 || LEDnum > 8)
        return;

    // Map LED number to GPIO pin (5 -> 12, 6 -> 13, 7 -> 14, 8 -> 15)
    int gpioPin = LEDnum + 7;

    // Turn on the specified LED by writing 0 to it
    PC->DOUT &= ~(1 << gpioPin);

    // Turn off all other LEDs in the range 5 to 8 (by writing 1 to them)
    for (int i = 12; i <= 15; i++)
    {
        if (i != gpioPin) // Skip the specified LED
            PC->DOUT |= (1 << i);
    }
}

void handleTimerUpdate()
{
    if (pause) // Indicate pause state by lighting up an LED
    {
        OpenLEDs(7);
        return; // Do nothing when paused
    }

    if (isOneTenthSecond) // Only update when a tenth of a second has passed
    {
        countTimes.tenthsOfSeconds = (countTimes.tenthsOfSeconds + 1) % 10; // Increment tenths of a second

        if (countTimes.tenthsOfSeconds == 0)
        {
            countTimes.unitsOfSeconds = (countTimes.unitsOfSeconds + 1) % 10; // Increment seconds digit
            if (countTimes.unitsOfSeconds == 0)
            {
                countTimes.tensOfSeconds = (countTimes.tensOfSeconds + 1) % 6; // Increment tens of seconds digit
                if (countTimes.tensOfSeconds == 0)
                {
                    countTimes.minutes = (countTimes.minutes + 1) % 10; // Increment minutes digit
                }
            }
        }
        // Check if the maximum time (9:59) is reached
        if (countTimes.minutes == 9 && countTimes.tensOfSeconds == 5 && countTimes.unitsOfSeconds == 9)
        {
            resetCountTimes();
        }
        isOneTenthSecond = false; // Reset the flag
    }
}
// function to reset count time when needed
void resetCountTimes()
{
    countTimes.minutes = 0;
    countTimes.tensOfSeconds = 0;
    countTimes.unitsOfSeconds = 0;
    countTimes.tenthsOfSeconds = 0;
}
// function to check if the slot/lap or empty
bool isTimeEmpty(Times t)
{
    return (t.minutes == -1 &&
            t.tensOfSeconds == -1 &&
            t.unitsOfSeconds == -1 &&
            t.tenthsOfSeconds == -1);
}
// helping function to calculate times
int calculateTime(int dozenDigit, int secondDigit)
{
    return dozenDigit * 10 + secondDigit;
}

// Buzzer beep function
void BuzzerBeep(int beepTime)
{
    int i;
    // multiply by 2 to allow the delay after each beep
    for (i = 0; i < (beepTime * 2); i++)
    {
        PB->DOUT ^= (1 << 11);
        // ad delay for function
        CLK_SysTickDelay(BUZZER_BEEP_DELAY);
    }
}
