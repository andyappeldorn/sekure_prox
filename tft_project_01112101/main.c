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
#include "main.h"

//#define ENABLE_LOWPOWER   // enable low power operation, serial debug will not operate properly
#define ALL_FLASH_SECONDS   102   // 10 s

/* STATE MACHINES */
typedef enum {
    SENSING, LED_KNIGHT, LED_ALL_FLASH, BAT_CHECK_STATE, RE_ARM
} e_controlState; // main loop control states

e_controlState controlState = RE_ARM;

typedef enum {
    NOTHING_SHOW, KNIGHT_SHOW, ALL_SHOW, TEST_SHOW, HANDS_OFF_SHOW
} e_lightShowState;

e_lightShowState lightShowState = NOTHING_SHOW;

/* VARIABLES */
uint32_t t0tick_seconds_countdown;  // tick counter for seconds elapsed
uint32_t blocking_delay_timer = 0;  // blocking delay timer????
uint32_t bs_timer = 0;  // battery status timer
bool bat_is_low = false;    // low battery flag
bool proximity_detect_status = true;    // proximity detect status flag
bool one_minute_flag = false;   // one minute elapsed flag



void main(void) {
    // initialize the device
    SYSTEM_Initialize();
    
    TMR0_StopTimer(); // stop timer that is started in sys initialize process
    TMR0_SetInterruptHandler(LedTimerISR);
    
//    MTOUCH_Proximity_SetActivatedCallback(processProximityActive);
//    MTOUCH_Proximity_SetNotActivatedCallback(processProximityNotActive);
    MTOUCH_Service_disableLowpower();

    // Turn off FVR
    FVRCONbits.FVREN = 0;

    all_leds_off();

    INTERRUPT_GlobalInterruptEnable();  // Enable the Global Interrupts
    INTERRUPT_PeripheralInterruptEnable();  // Enable the Peripheral Interrupts

    while (1) {

        if (one_minute_flag) {
            one_minute_flag = false;
            controlState = BAT_CHECK_STATE;
            lightShowState = NOTHING_SHOW;
            all_leds_off();
            MTOUCH_Service_disableLowpower();
            TMR0_StartTimer();
        }

        switch (controlState) {
            case SENSING:
                if (MTOUCH_Service_Mainloop()) { // service mtouch library, acquire sensor data and test
                    if (MTOUCH_Proximity_isActivated(0)) {  // was proximity detected
                        MTOUCH_Service_disableLowpower();   // turn off low power
                        TMR0_StartTimer(); // start timer
                        lightShowState = KNIGHT_SHOW;
                        controlState = LED_KNIGHT; // change state to LED walking display
                        t0tick_seconds_countdown = get_switch_dwell_time_setting();
                    } else {
                        controlState = SENSING; // stay in sensing state
                        all_leds_off(); // turn off LEDs
                        // return to sleep for low power operation
                    }
                }
                break;
            case LED_KNIGHT:
                if (!t0tick_seconds_countdown) {
                    controlState = LED_ALL_FLASH;
                    t0tick_seconds_countdown = ALL_FLASH_SECONDS;
                    lightShowState = ALL_SHOW;
                }
                break;

            case LED_ALL_FLASH:
                if (!t0tick_seconds_countdown) {
                    controlState = RE_ARM;
                    lightShowState = NOTHING_SHOW;
                    all_leds_off();
                }
                break;

            case BAT_CHECK_STATE:
                if (MTOUCH_Sensor_isSampling() == false) {
                    MTOUCH_Sensor_Disable(Sensor_ANA4);
                    battery_service();
                    MTOUCH_Sensor_Enable(Sensor_ANA4);
                    MTOUCH_Initialize(); // restart mtouch library to clear out any previous activity
                }
                break;

            case RE_ARM:
            default:
                TMR0_StopTimer(); // stop timer
                MTOUCH_Initialize(); // restart mtouch library to clear out any previous activity
                disable_switches();
                controlState = SENSING; // skip through
#ifdef ENABLE_LOWPOWER
                MTOUCH_Service_enableLowpower();
#endif
                break;
        }
    }
}

// TODO - this needs to be smaller rep rate so that we have
//        more fine-grained control over delays.

/* TMR0 callback function, tick incrementer called every ~100ms*/
void LedTimerISR(void) {
    light_handler();

    if (blocking_delay_timer) {
        blocking_delay_timer--;
    }

    if (t0tick_seconds_countdown) {
        t0tick_seconds_countdown--;
    }

    if (bs_timer) {
        bs_timer--;
    }
}

/********************************************************************
 *   Function Name:  light_handler
 *
 *   Parameters:     none
 *
 *   Return Value:   none
 *
 *   Description:    Sequences LEDs from timer isr. Canned show selected
 *                   by global variable lightShowState.
 *
 ********************************************************************/
void light_handler(void) {
    switch (lightShowState) {
            // Knight rider
        case KNIGHT_SHOW:
            knight_rider();
            break;

            // Blinking in unison
        case ALL_SHOW:
            all_at_50();
            break;

            // All off
        case NOTHING_SHOW:
        default:
            all_leds_off();
            break;

        case TEST_SHOW:
            LATB = LATB & 0x10 ? 0 : 0x10;
            break;

        case HANDS_OFF_SHOW:
            // don't touch anything
            break;
    }
}

void battery_service(void) {
    static uint8_t bs_state = 0;


    switch (bs_state) {
            // initial state
            // Turn on FVR, turn on indicator led if necessary
            // Don't measure until state - this gives 100 ms for FVR to stabilize.
        case 0:
            lightShowState = HANDS_OFF_SHOW;

            // This is a heartbeat led, independent of bat voltage, per Dave A.
            LED_1_SetHigh();

            if (bat_is_low) {
                // This is the low bat indication.
                LED_0_SetHigh();
                bs_state = 2;
            } else {
                FVRCONbits.FVREN = 1;
                bs_state = 1;
            }
            bs_timer = 1;
            break;

            // measure state
            // Turn off indicator led. Assume FVR stable, go measure
            // battery voltage.
        case 1:
            if (!bs_timer) {
                look_at_bat_voltage();

                // Low bat indication off
                LED_0_SetLow();

                // Heartbeat led off
                LED_1_SetLow();
                bs_timer = 1;
                bs_state = 2;
            }
            break;

            // FVR off state
            // Turn off FVR. Go to exit state to allow
            // FVR and ADC to stabilize.
        case 2:
        default:
            if (!bs_timer) {
                bs_state = 3;
                bs_timer = 1;
                FVRCONbits.FVREN = 0;
            }
            break;

            // Exit state
            // Reset led handler and touch stuff.
            // Set next state to initial, see you in one minute.
        case 3:
            if (!bs_timer) {
                bs_state = 0;
                controlState = RE_ARM;
                lightShowState = NOTHING_SHOW;
                all_leds_off();
            }
            break;
    }
}

/********************************************************************
 *   Function Name:  look_at_bat_voltage
 *
 *   Parameters:     none
 *
 *   Return Value:   none
 *
 *   Description:    Measure and smooth battery voltage
 *
 ********************************************************************/
void look_at_bat_voltage(void) {
    uint8_t i;
    uint16_t average_bat_voltage = 0;

    // NOTE: we're actually measuring Vref (ADC reference voltage) by
    //  assuming that Vi (FVR output) is constant. This means that
    //  as Vref decreases, the apparent ADC reading increases. Therefore
    //  the low battery threshold is a LOWER limit for the ADC
    //  measured value.

    // Our Vi is FVR channel 1, which is configured to output 1.024 V

    // Get rid of ADC touch settings.
    ADCC_Initialize();

    PIR1bits.ADTIF = 0; // clear interrupt flag
    PIE1bits.ADTIE = 0; // disable interrupt

    // Take four readings
    for (i = 0; i < 4; i++) {
        // FVR channel 1
        average_bat_voltage += ADCC_GetSingleConversion(0x3e);
    }

    //
    // N -> ADC output reading (12 bit)
    // Vref -> ADC reference (Vbat)
    //
    //  N = (2^12 - 1) * Vi / Vref
    //
    //  Vi = 1.024 V
    //  Vref limit = 2.05 V
    //
    //  N = 4095 * 1.024 / 2.05
    //    = 2045.5
    //    ~= 2046
    //    = 0x7fe 
    //
    // Since we took four readings we can roll the *4
    // factor into the threshold. This saves us the
    // extra step of dividing.
    //
    //  0x7fe * 4 = 0x1ff8
    //

    if (average_bat_voltage >= 0x1ff8) {
        // Note that this can never be reset except by a POR
        bat_is_low = true;
    }

    // For debugger breakpoint (optimizer cheat)
    NOP();
    NOP();
    NOP();
    NOP();

    PIR1bits.ADTIF = 0; // clear interrupt flag
    PIE1bits.ADTIE = 1; // reenable interrupt
}

/********************************************************************
 *   Function Name:  knight_rider
 *
 *   Parameters:     none
 *
 *   Return Value:   none
 *
 *   Description:    Kit!
 *
 ********************************************************************/
void knight_rider(void) {
    uint8_t knight_sequence[NUM_OF_KNIGHT_SEQ_STEPS] = {0x80, 0x40, 0x20, 0x10, 0x20, 0x40};
    static uint8_t next_seq_step = 0;
    uint8_t latch_val = LATB;

    latch_val = LATB;
    latch_val &= LATB_LED_MASK;
    latch_val |= knight_sequence[next_seq_step];
    LATB = latch_val;

    next_seq_step++;
    if (next_seq_step >= NUM_OF_KNIGHT_SEQ_STEPS) {
        next_seq_step = 0;
    }
}

/********************************************************************
 *   Function Name:  all_at_50
 *
 *   Parameters:     none
 *
 *   Return Value:   none
 *
 *   Description:    All blink in unison, 50% DC.
 *
 ********************************************************************/
void all_at_50(void) {
    static uint8_t aa50_state = 0;
    uint8_t latch_val = LATB;

    if (aa50_state) {
        aa50_state = 0;
        latch_val |= ~((uint8_t) LATB_LED_MASK); // all on
    } else {
        aa50_state = 1;
        latch_val &= LATB_LED_MASK; // all off
    }

    LATB = latch_val; // output state to leds
}

/********************************************************************
 *   Function Name:  all_leds_off
 *
 *   Parameters:     none
 *
 *   Return Value:   none
 *
 *   Description:    This function turns off all leds.
 *
 ********************************************************************/
void all_leds_off(void) {
    LED_0_SetLow();
    LED_1_SetLow();
    LED_2_SetLow();
    LED_3_SetLow();
}

/********************************************************************
 *   Function Name:  get_switch_dwell_time_setting
 *
 *   Parameters:     none
 *
 *   Return Value:   uint32_t - dwell time setting derived from switches
 *                             in units of t0 ticks
 *
 *   Description:    This function simply returns a dwell time value
 *                   in t0 ticks that is derived from the current dip
 *                   switch configuration.
 *
 ********************************************************************/
uint32_t get_switch_dwell_time_setting(void) {
    uint32_t ret_val;

    // This is the way it was explained to me - we walk across
    // the dip switches from left to right, and the first one
    // that reads a logic 1 dictates the time.
    //
    // If all four switches read logic 0, we return 0.

    // All times are in units of seconds
    enable_switches(); 

    if (IO_RC7_GetValue()) {
        ret_val = 1221; // 120 s
    } else if (IO_RC6_GetValue()) {
        ret_val = 916; // 90 s
    } else if (IO_RC5_GetValue()) {
        ret_val = 610; // 60 s
    } else if (IO_RC4_GetValue()) {
        ret_val = 305; // 30 s
    } else {
        ret_val = 0; // 0 s
    }

    disable_switches();
    
    return ret_val;
}

void disable_switches(void) {
    IO_RC2_SetLow();
}

void enable_switches(void) {
    IO_RC2_SetHigh();
    blocking_delay_ms(1);       // why blocking delay?  disabling PIE is not advised
}

void blocking_delay_ms(uint32_t num_of_ms) {
    // Disable interrupts while ISR variable is changed.
//    INTERRUPT_PeripheralInterruptDisable();

    blocking_delay_timer = num_of_ms;

    // Reenable interrupts
//    INTERRUPT_PeripheralInterruptEnable();

    while (blocking_delay_timer);
}
