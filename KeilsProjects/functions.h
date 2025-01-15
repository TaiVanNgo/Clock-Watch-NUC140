// define function headers
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "NUC100Series.h"
#include <stdio.h>
#include <stdbool.h>

#include "globals.h"
// set up key metrics by pin a 0 1 2
// check if it is one for each key 
#define C3_pressed !(PA->PIN & (1 << 0)) 
#define C2_pressed !(PA->PIN & (1 << 1))
#define C1_pressed !(PA->PIN & (1 << 2))
// define Global buzzer time to use later
#define BUZZER_BEEP_TIME 3
// Add delay time 
#define BUZZER_BEEP_DELAY 2000000

// add digit pattern for 7segments
extern const uint8_t DIGIT_PATTERN[];


void KeyPadScanning();// function key pad scanning to initialize key matrix
void DisplayDigit(int digit, int segmentNum, bool openDot);// show digit of 7segments 
void SearchColumn(int colNum);// search for col num that the key are pressed
void OpenLEDs(int LEDnum);// function to open LEDs
int calculateTime(int dozenDigit, int secondDigit);// calculate time for second frist and second digit
void BuzzerBeep(int beepTime);//  Function to buzzer beep
void CloseSegments();// close segment to reset the 7segments after set up
void handleTimerUpdate();// update time value of the timer
void resetCountTimes(); // reset every time value to zero
bool isTimeEmpty(Times t);// function that checks if the time object is empty

#endif // FUNCTIONS_H
