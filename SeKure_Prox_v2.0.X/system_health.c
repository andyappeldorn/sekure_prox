/*
 *  Processes for heartbeat led control and battery voltage status
 *
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

#include "system_health.h"

e_batteryMeasureState batteryMeasureState = BATTERY_DONE;
bool systemHealthTimerExpired = false;
bool battery_is_low_flag = false;

/* TMR0 callback function, used for 60sec and 100msec timer for heartbeat and battery measurement */
void PeriodicEventTimerISR(void) {
    systemHealthTimerExpired = true;
}

/* set timer period to 100ms */
void TMR0_Set100msPeriod(void) {
    TMR0H = 0xFF;
    TMR0L = 0x95;
}
/* set timer period to 60s */
void TMR0_Set60sPeriod(void) {
    TMR0H = 0x1C;
    TMR0L = 0xF3;
}

/********************************************************************
 *   Function Name:  heartbeatAndBatteryCheckProcess
 *
 *   Parameters:     none
 *
 *   Return Value:   none
 *
 *   Description:    Measure and smooth battery voltage
 *
 ********************************************************************/
void heartbeatAndBatteryCheckProcess(void) {
    // this is a blocking process
    // blocking from the 100ms heartbeat LED on time
    /* setup next TMR0 event, 100ms heartbeat flash event */
    TMR0_Set100msPeriod();  // set to 100ms period
    systemHealthTimerExpired = false;   // clear time expired flag
    LED_3_SetHigh(); // turn on heartbeat led
    TMR0_StartTimer();  // make sure timer is running
    /* check battery */
    batteryMeasureState = BATTERY_START;
    do{
        batteryMeasureProcess();
    } while(batteryMeasureState < BATTERY_DONE);
    /* wait for 100ms heartbeat timer to expire */
    while (systemHealthTimerExpired == false){  // wait for 100ms time to expire
    }
    LED_3_SetLow(); // turn off heartbeat LED
    /* setup next TMR0 event, 60s event */
    systemHealthTimerExpired = false;    // reset timer flag
    TMR0_Set60sPeriod();    // set timer period to 60s
    TMR0_StartTimer();  // make sure timer is running
    
    controlState = RE_ARM_STATE; // set control state to re arm to resume // @todo - do we need to re arm?
}

/* battery measurement process */
void batteryMeasureProcess(void) {
    switch (batteryMeasureState) {
        case BATTERY_START:
            FVRCONbits.FVREN = 1; // enable FVR
            batteryMeasureState = BATTERY_FVR_WAIT;
            break;
        case BATTERY_FVR_WAIT:
            // data sheet says FVRRDY ready always = 1?
            if (FVRCONbits.FVRRDY == 1) { // wait for FVR to stabilize
                batteryMeasureState = BATTERY_MEASURE;
            }
            break;
        case BATTERY_MEASURE:
            measureBatteryVoltage(); // measure battery voltage  @todo - use return value???
            FVRCONbits.FVREN = 0; // disable FVR when done
            if (battery_is_low_flag == true) {
                LED_0_SetHigh(); // turn on battery low indicator led
            } else {
                LED_0_SetLow(); // turn off battery low indicator led
            }
            batteryMeasureState = BATTERY_DONE;
            break;
        case BATTERY_DONE:
            controlState = RE_ARM_STATE; // re-init machine to continue operation
            break;
        default:
            batteryMeasureState = BATTERY_DONE;
            break;
    }
}

void measureBatteryVoltage(void) {
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

    PIE1bits.ADTIE = 0; // disable interrupt
    PIR1bits.ADTIF = 0; // clear interrupt flag used in mtouch library

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

    PIR1bits.ADTIF = 0; // clear interrupt flag
    PIE1bits.ADTIE = 1; // re-enable interrupt used in mtouch library

    if (average_bat_voltage >= 0x1ff8) {
        // Note that this can never be reset except by a POR
        battery_is_low_flag = true;
    }
    else {
        battery_is_low_flag = false;
    }

    // For debugger breakpoint (optimizer cheat)
    NOP();
    NOP();
    NOP();
    NOP();
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
        ret_val = 1200; // 120 s
    } else if (IO_RC6_GetValue()) {
        ret_val = 9000; // 90 s
    } else if (IO_RC5_GetValue()) {
        ret_val = 6000; // 60 s
    } else if (IO_RC4_GetValue()) {
        ret_val = 3000; // 30 s
    } else {
        ret_val = 0; // 0 s
    }

    disable_switches();

    return ret_val;
}

void enable_switches(void) {
    IO_RC2_SetHigh();
    uint16_t counter;
    for (counter = 0; counter < 0xFF; counter++) { // loop delay to allow voltage rise
        NOP();
    }
    //    blocking_delay_ms(1); // why blocking delay?  disabling PIE is not advised
}

void disable_switches(void) {
    IO_RC2_SetLow();
}
