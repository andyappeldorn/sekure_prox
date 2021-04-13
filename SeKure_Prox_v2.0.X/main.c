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
    (c) 2021 Microchip Technology Inc. and its subsidiaries.

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
#include "main.h"
#include "led_display.h"
#include "system_health.h"

/* RESOURCES */
// TMR0 = system health interrupt timer, 60 sec event, operates during sleep
// TMR2 = mtouch scan timer, operates during sleep
// TMR4 = lighting sequence timer, software control on and off

/* STATE MACHINES */
e_controlState controlState = RE_ARM_STATE;

void main(void) {
    // initialize the device
    SYSTEM_Initialize();

    TMR0_StopTimer(); // stop timer that is started in sys initialize process
    TMR4_StopTimer(); // stop timer
    TMR0_SetInterruptHandler(PeriodicEventTimerISR);
    TMR4_SetInterruptHandler(LedTimerISR);

    FVRCONbits.FVREN = 0; // Turn off FVR

    MTOUCH_Service_disableLowpower();
    all_leds_off();

    INTERRUPT_GlobalInterruptEnable(); // Enable the Global Interrupts
    INTERRUPT_PeripheralInterruptEnable(); // Enable the Peripheral Interrupts

    MTOUCH_Initialize(); // restart mtouch library to clear out any previous activity
    // setup TMR0
    TMR0_Set60sPeriod();    // init for 60s heartbeat and battery check
    TMR0_StartTimer(); // start timer   
    
    /* MAIN PROCESS */
    while (1) {
        main_process(); // call main process state machine
    }
}

/* Main Process State Machine */
void main_process(void) {
    switch (controlState) {
        case SENSING_STATE:
            if (MTOUCH_Service_Mainloop()) { // service mtouch library, acquire sensor data and test
                if (MTOUCH_Sensor_isSampling() == false) {
                    if (MTOUCH_Proximity_isActivated(0)) { // proximity event was detected
                        MTOUCH_Sensor_Disable(Sensor_ANA4); // disable proximity sensor to avoid LED interference
                        MTOUCH_Service_disableLowpower(); // turn off low power
                        controlState = LIGHTING_STATE; // switch to lighting state
                        lightShowState = LIGHT_KNIGHT_SHOW_START; // display knight show
                    } else { // proximity event not detected
                        controlState = SENSING_STATE; // stay in sensing state
                        lightShowState = LIGHT_NOTHING_SHOW; // no LEDs to show
                        // return to sleep for low power operation
                    }
                    // service system health heartbeat LED and check battery voltage
                    if (systemHealthTimerExpired == true) { // 60s has occured
                        MTOUCH_Service_disableLowpower(); // turn off low power operation
                        MTOUCH_Sensor_Disable(Sensor_ANA4); // disable proximity sensor to avoid LED interference
                        heartbeatAndBatteryCheckProcess();  // heartbeat and battery status check process
                    }
                }
            }
            break;
        case LIGHTING_STATE:
            lightingProcess();
            break;
        case RE_ARM_STATE:
            MTOUCH_Sensor_Enable(Sensor_ANA4); // enable proximity sensor
            MTOUCH_Initialize(); // restart mtouch library to clear out any previous activity
#ifdef ENABLE_LOWPOWER
            MTOUCH_Service_enableLowpower();
#endif
            controlState = SENSING_STATE; // switch back to proximity sensing
            break;
        default:
            controlState = RE_ARM_STATE; // skip through
            break;
    }
}
