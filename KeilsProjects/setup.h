#ifndef SETUP_H
#define SETUP_H

#include "NUC100Series.h"

#define LOAD 100000 // Define the LOAD value for TCMPR

// Function Declarations
void GPIO_Config(void);  // Function to configure the General Purpose Input/Output (GPIO) pins
void SystemConfig(void); // Function to configure system settings
void Timer0Config(void); // Function to configure Timer 0

#endif // SETUP_H