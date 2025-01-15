#include "NUC100Series.h"
#include <stdbool.h>
#include "globals.h"
#include "setup.h"
#include "functions.h"

buzzerTriggerCount = 0;
// Initialize the program in IDLE_MODE
enum programState state = IDLE_MODE;

// Define and initialize global flags and variables
volatile bool isPB15Pressed = false;       // Flag to detect PB15 button press
volatile bool isAlarmSetModeStart = false; // Alarm mode activation flag
int alarmTime = 0;                         // Variable to store alarm time in seconds
volatile bool setAlarmTime = false;        // Flag to indicate alarm time is being set
volatile bool pause = false;               // Flag to pause the system

// Initialize counting times to zero
Times countTimes = {
    .minutes = 0,
    .tensOfSeconds = 0,
    .unitsOfSeconds = 0,
    .tenthsOfSeconds = 0};

volatile bool isOneTenthSecond = false;
volatile bool isCountModeStart = false; // Count mode start flag
int lapTimeIndex = 0;                   // Current index for storing lap times

// Initialize lap times with invalid values (-1)
volatile Times times[5] = {
    {-1, -1, -1, -1},
    {-1, -1, -1, -1},
    {-1, -1, -1, -1},
    {-1, -1, -1, -1},
    {-1, -1, -1, -1}};

volatile bool isCheckModeStart = false; // Check mode start flag
bool isInCountMode = false;             // Flag for counting mode status
int lapCountPressed = 0;                // Count of lap button presses

volatile bool isK9Pressed = false; // Flag for K9 button press

// Digits for displaying values on LEDs (use for alarm set mode)
int unitsDigit = 0; // Units place for display
int tensDigit = 0;  // Tens place for display

// Function declarations
void Idle();
void handleAlarmSetMode();
void handleCountMode();
void addNewTime(Times countTimes);
void handleCheckMode();
void handleTimerUpdate();
void CloseSegments();

int main()
{
    SYS_UnlockReg(); // Unlock protected registers

    SystemConfig(); // Enable System Configuration

    Timer0Config(); // Enable Timer0 Configuration

    GPIO_Config(); // Enable GPIO Configuration

    SYS_LockReg(); // Lock protected registers

    while (1)
    {
        KeyPadScanning(); // Scan keypad for keymatrix button presses

        switch (state)
        {
        case IDLE_MODE:
            Idle(); // Handle idle mode
            break;
        case ALARM_SET_MODE:
            handleAlarmSetMode(); // Handle alarm set mode
            break;
        case COUNT_MODE:
            handleCountMode(); // Handle count mode
            break;
        case CHECK_MODE: // Handle check mode
            handleCheckMode();
            break;
        default:
            break; // Default case for undefined states
        }

        CLK_SysTickDelay(DEBOUNCE_DELAY); // Delay for debounce
    }
}

void Idle()
{
    CloseSegments(); // Turn off all display segments
    OpenLEDs(5);     // Turn on LED 5
}

void handleAlarmSetMode()
{
    OpenLEDs(8); // Open LED 8 (PC15) and close others

    if (setAlarmTime)
    {
        alarmTime = (tensDigit) * 10 + (unitsDigit); // Calculate alarm time
        setAlarmTime = false;                        // Reset flag
        state = IDLE_MODE;                           // Return to idle mode
    }

    if (isAlarmSetModeStart)
    {
        if (isPB15Pressed)
        {
            unitsDigit = (unitsDigit + 1) % 10; // Increment units digit
            if (unitsDigit == 0)
            {
                tensDigit = (tensDigit + 1) % 6; // Increment tens digit (0-5)
            }
            CLK_SysTickDelay(LONG_DELAY_TIME); // Add debounce delay
            isPB15Pressed = false;             // Reset button press flag
        }
    }

    // Display digits for alarm time setting
    DisplayDigit(tensDigit, 2, false); // Display tens digit
    CLK_SysTickDelay(DELAY_TIME);
    DisplayDigit(unitsDigit, 3, false); // Display units digit
    CLK_SysTickDelay(DELAY_TIME);
}

void handleCountMode()
{
    OpenLEDs(6); // Open LED 6 and close others
    isInCountMode = true;

    handleTimerUpdate(); // Update timer values

    // Display time on LEDs
    DisplayDigit(countTimes.tenthsOfSeconds, 4, false); // Tenths of seconds
    CLK_SysTickDelay(DELAY_TIME);

    DisplayDigit(countTimes.unitsOfSeconds, 3, true); // Units digit
    CLK_SysTickDelay(DELAY_TIME);

    DisplayDigit(countTimes.tensOfSeconds, 2, false); // Tens digit
    CLK_SysTickDelay(DELAY_TIME);

    DisplayDigit(countTimes.minutes, 1, false); // Minutes digit
    CLK_SysTickDelay(DELAY_TIME);

    if (isK9Pressed)
    {
        addNewTime(countTimes);            // Add current time as a lap
        CLK_SysTickDelay(LONG_DELAY_TIME); // Debounce delay
        isK9Pressed = false;               // Reset flag
    }

    // Trigger buzzer if current time matches alarm time
    if (calculateTime(countTimes.tensOfSeconds, countTimes.unitsOfSeconds) == alarmTime && buzzerTriggerCount < 3)
    {
        BuzzerBeep(1); // Trigger buzzer
        buzzerTriggerCount++;
        pause = true; // Pause during buzzer operation

        if (buzzerTriggerCount >= 3)
        {
            pause = false; // Resume counting
        }
    }
}

void handleCheckMode()
{

    CloseSegments(); // Turn off all display segments
                     // call out to check if the array is not empty then display digit

    pause = true;
    if (!isTimeEmpty(times[lapCountPressed]))
    {
        DisplayDigit(lapCountPressed + 1, 1, false); // Display lap number
        CLK_SysTickDelay(DELAY_TIME);

        DisplayDigit(times[lapCountPressed].tensOfSeconds, 2, false); // Tens of seconds
        CLK_SysTickDelay(DELAY_TIME);

        DisplayDigit(times[lapCountPressed].unitsOfSeconds, 3, true); // Units of seconds
        CLK_SysTickDelay(DELAY_TIME);

        DisplayDigit(times[lapCountPressed].tenthsOfSeconds, 4, false); // Tenths of seconds
        CLK_SysTickDelay(DELAY_TIME);
    }
    else
    {
        // Display "----" when no data is available for the requested lap
        DisplayDigit(lapCountPressed + 1, 1, false); // Lap number
        CLK_SysTickDelay(DELAY_TIME);

        DisplayDigit(11, 2, false);
        CLK_SysTickDelay(DELAY_TIME);

        DisplayDigit(11, 3, false);
        CLK_SysTickDelay(DELAY_TIME);

        DisplayDigit(11, 4, false);
        CLK_SysTickDelay(DELAY_TIME);
    }
    // if we press Pb15 then move to next lap
    if (isPB15Pressed)
    {
        CLK_SysTickDelay(LONG_DELAY_TIME * 2);       // Debounce delay
        lapCountPressed = (lapCountPressed + 1) % 5; // Cycle through laps
        isPB15Pressed = false;                       // Reset flag
    }
    // turn off check mode
    if (!isCheckModeStart)
    {
        state = COUNT_MODE;       // Return to Count Mode
        isCheckModeStart = false; // Reset flag
        pause = true;             // Pause clock
    }
}

void TMR0_IRQHandler(void)
{
    isOneTenthSecond = true;  // Set flag for tenth-second update
    TIMER0->TISR |= (1 << 0); // Clear Timer 0 interrupt flag
    PC->DOUT ^= (1 << 8);     // Toggle waveform for measurement
}

void EINT1_IRQHandler(void)
{
    if (state == ALARM_SET_MODE || state == CHECK_MODE)
    {
        isPB15Pressed = true; // Set button press flag
    }
    PB->ISRC |= (1 << 15); // Clear interrupt flag for PB15
}
