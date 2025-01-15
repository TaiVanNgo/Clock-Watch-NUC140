#include "setup.h"

// Function to configure GPIO pins
void GPIO_Config()
{
    // Configure pins PA0 to PA5 as quasi-bidirectional mode
    GPIO_SetMode(PA, BIT0, GPIO_MODE_QUASI); // Set PA0 mode
    GPIO_SetMode(PA, BIT1, GPIO_MODE_QUASI); // Set PA1 mode
    GPIO_SetMode(PA, BIT2, GPIO_MODE_QUASI); // Set PA2 mode
    GPIO_SetMode(PA, BIT3, GPIO_MODE_QUASI); // Set PA3 mode
    GPIO_SetMode(PA, BIT4, GPIO_MODE_QUASI); // Set PA4 mode
    GPIO_SetMode(PA, BIT5, GPIO_MODE_QUASI); // Set PA5 mode

    // Configure PB11 as output mode (used for buzzer)
    GPIO_SetMode(PB, BIT11, GPIO_MODE_OUTPUT); // Set PB11 as output

    // Configure PC4 to PC7 as output mode (used for displaying digits)
    GPIO_SetMode(PC, BIT4, GPIO_MODE_OUTPUT); // Set PC4 as output
    GPIO_SetMode(PC, BIT5, GPIO_MODE_OUTPUT); // Set PC5 as output
    GPIO_SetMode(PC, BIT6, GPIO_MODE_OUTPUT); // Set PC6 as output
    GPIO_SetMode(PC, BIT7, GPIO_MODE_OUTPUT); // Set PC7 as output

    // Configure PC8 as output mode (use for waveform measurement)
    GPIO_SetMode(PC, BIT8, GPIO_MODE_OUTPUT); // Set PC8 as output

    // Configure PC12 to PC15 as output mode
    GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT); // Set PC12 as output
    GPIO_SetMode(PC, BIT13, GPIO_MODE_OUTPUT); // Set PC13 as output
    GPIO_SetMode(PC, BIT14, GPIO_MODE_OUTPUT); // Set PC14 as output
    GPIO_SetMode(PC, BIT15, GPIO_MODE_OUTPUT); // Set PC15 as output

    // Configure PE0 to PE7 as output mode
    GPIO_SetMode(PE, BIT0, GPIO_MODE_OUTPUT); // Set PE0 as output
    GPIO_SetMode(PE, BIT1, GPIO_MODE_OUTPUT); // Set PE1 as output
    GPIO_SetMode(PE, BIT2, GPIO_MODE_OUTPUT); // Set PE2 as output
    GPIO_SetMode(PE, BIT3, GPIO_MODE_OUTPUT); // Set PE3 as output
    GPIO_SetMode(PE, BIT4, GPIO_MODE_OUTPUT); // Set PE4 as output
    GPIO_SetMode(PE, BIT5, GPIO_MODE_OUTPUT); // Set PE5 as output
    GPIO_SetMode(PE, BIT6, GPIO_MODE_OUTPUT); // Set PE6 as output
    GPIO_SetMode(PE, BIT7, GPIO_MODE_OUTPUT); // Set PE7 as output

    // Configure PB15 as input mode (used for button input)
    GPIO_SetMode(PB, BIT15, GPIO_MODE_INPUT); // Set PB15 as input
    PB->IMD &= ~(1 << 15);                    // Configure PB15 for edge-triggered interrupt
    PB->IEN |= (1 << 31);                     // Enable rising edge trigger for PB15

    // Enable NVIC interrupt for PB15
    NVIC->ISER[0] |= 1 << 3;      // Enable interrupt for PB15
    NVIC->IP[0] &= ~(0b11 << 30); // Set interrupt priority for PB15
}

// Function to configure the system clock
void SystemConfig(void)
{
    // Enable the 12MHz external clock source
    CLK->PWRCON |= (0x01 << 0);
    while (!(CLK->CLKSTATUS & (1 << 0))) // Wait for the clock source to stabilize
        ;

    // Select 12MHz external clock source for CPU
    CLK->CLKSEL0 &= (~(0b111 << 0));

    // Set to normal power mode (disable power-down mode)
    CLK->PWRCON &= ~(1 << 7);

    // Set clock division to 1 (no division)
    CLK->CLKDIV &= (~(0xF << 0));
}

// Function to configure Timer 0
void Timer0Config(void)
{
    // Select HCLK as the clock source for Timer 0
    CLK->CLKSEL1 &= ~(0b111 << 8);
    CLK->CLKSEL1 |= (0b010 << 8);

    // Enable the Timer 0 module clock
    CLK->APBCLK |= (1 << 2);

    // Set Timer 0 prescaler to 12
    TIMER0->TCSR &= ~(0xFF << 0);
    TIMER0->TCSR |= 11;

    // Reset Timer 0
    TIMER0->TCSR |= (1 << 26);

    // Configure Timer 0 in periodic mode
    TIMER0->TCSR &= ~(0b11 << 27);
    TIMER0->TCSR |= (0b01 << 27);

    // Disable counter mode for Timer 0
    TIMER0->TCSR &= ~(1 << 24);

    // Enable data load for Timer 0
    TIMER0->TCSR |= (1 << 16);

    // Set the Timer 0 compare value (period)
    TIMER0->TCMPR = LOAD - 1;

    // Start Timer 0 and enable polling mode
    TIMER0->TCSR |= (1 << 30); // Start counting
    TIMER0->TCSR |= (1 << 29); // Enable polling mode

    // Configure NVIC interrupt for Timer 0
    NVIC->ISER[0] |= (1 << 8); // Enable Timer 0 interrupt
    NVIC->IP[2] &= ~(3 << 6);  // Set interrupt priority for Timer 0
}
