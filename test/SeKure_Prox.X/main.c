/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC16LF18446
        Driver Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

#include "mcc_generated_files/mcc.h"

#if 1
/* Timer tick counters */
uint32_t timerLedDisplay;
uint32_t timerLedWalking;
uint32_t timerLedFlashing;
uint32_t timerBatteryCheck;

typedef enum {
    SENSING, LED_WALKING, LED_FLASH, RE_ARM,
} e_controlState; // control states
e_controlState controlState = SENSING;
#endif

void LedTimerISR(void);
void LedWalkingProcess(void);
void LedFlashingProcess(void);
void processProximityActive(enum mtouch_proximity_names proximity);
void processProximityNotActive(enum mtouch_proximity_names proximity);


/*
                         Main application
 */
void main(void) {
    // initialize the device
    SYSTEM_Initialize();
    TMR0_StopTimer(); // stop timer that is started in sys initialize process
    TMR0_SetInterruptHandler(LedTimerISR);

    LED_0_SetLow();
    LED_1_SetLow();
    LED_2_SetLow();
    LED_3_SetLow();
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    MTOUCH_Proximity_SetActivatedCallback(processProximityActive);
    MTOUCH_Proximity_SetNotActivatedCallback(processProximityNotActive);

    while (1) {
        switch (controlState) {
            case SENSING:
                if (MTOUCH_Service_Mainloop()) { // service mtouch library, acquire sensor data and test
                    if (MTOUCH_Proximity_isActivated(0)) {
                        controlState = LED_WALKING; // change state to LED walking display
                        timerLedDisplay = 0; // reset display mode timer counter
                        timerLedWalking = 1; // preload walking timer counter, so LED turns on immediately
                        timerLedFlashing = 2; // preload flashing timer counter
                        TMR0_StartTimer(); // start timer
                    } else {
                        controlState = SENSING; // stay in sensing state
                        LED_0_SetLow(); // turn off LEDs
                        // return to sleep for low power operation
                    }
                }
                break;
            case LED_WALKING:
                LedWalkingProcess();
                // check led walking time exceeded
                if (timerLedDisplay >= 50) { // on time exceeded in 100ms
                    controlState = LED_FLASH; // change state to flash display
                    TMR0_StopTimer(); // stop timer
                    LED_0_SetLow(); // turn off LED
                    // all LEDs off
                }
                break;
            case LED_FLASH:
                LedFlashingProcess();
                // check led flashing time exceeded
                controlState = RE_ARM; // skip through
                break;
            case RE_ARM:
                MTOUCH_Initialize(); // restart mtouch library to clear out any previous activity
                controlState = SENSING; // skip through
                break;
            default:
                controlState = SENSING; // default state
        }
    }
}

/* TMR0 callback function, tick incrementer called every ~100ms*/
void LedTimerISR(void) {
    timerLedDisplay++;
    timerLedWalking++;
    timerLedFlashing++;
    timerBatteryCheck++;
}

/* LED Walking Display Process */
void LedWalkingProcess(void) {
    if (timerLedWalking > 1) {
        LED_0_Toggle(); // toggle LED
        timerLedWalking -= 1; // decrement tick counter
    }
}

/* LED Flashing Display Process */
void LedFlashingProcess(void) {
#if 0
    if (timerLedFlashing > 1) {
        IO_LED_Toggle(); // toggle LED
        timerLedFlashing -= 1; // decrement tick counter
    }
#endif
}

#if 1
void processProximityActive(enum mtouch_proximity_names proximity) {
    switch (proximity) {
        case Proximity0:
            LED_0_SetLow();
            //            LED_1_SetHigh();
            break;
        default: break;
    }
}

void processProximityNotActive(enum mtouch_proximity_names proximity) {
    switch (proximity) {
        case Proximity0:
            LED_0_SetHigh();
            //            LED_1_SetLow();
            break;
        default: break;
    }
}
#endif
/**
 End of File
 */