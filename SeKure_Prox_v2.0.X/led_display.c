/*
 *  Process for LED control during proximity event
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

#include "led_display.h"
#include "system_health.h"

e_lightShowState lightShowState = LIGHT_NOTHING_SHOW;

uint32_t lightingOnTimeCounter;
uint32_t lightingFlashTimeCounter;
uint32_t lightingDwellTime;

/* TMR4 callback function, tick incrementer called every ~100ms*/
void LedTimerISR(void) {
    if (lightingOnTimeCounter < UINT32_MAX) {
        lightingOnTimeCounter++;
    }
    if (lightingFlashTimeCounter < UINT32_MAX) {
        lightingFlashTimeCounter++;
    }
}

/* Lighting Process Task */
void lightingProcess(void) {
    switch (lightShowState) {
        case LIGHT_KNIGHT_SHOW_START:
            lightingOnTimeCounter = 0; // init timer for on time
            lightingFlashTimeCounter = 0; // init timer for flash time
            TMR4_Start();   // start TMR4
            knight_rider(); // run knight rider sequence first step
            lightingDwellTime = get_switch_dwell_time_setting();
#ifdef LED_DWELL_TIME   // only defined and enabled in quick test mode
            lightingDwellTime = LED_DWELL_TIME; // override dwell time for faster debugging
#endif
            lightShowState = LIGHT_KNIGHT_SHOW; // move to knight show sequence
            break;
        case LIGHT_KNIGHT_SHOW:
            if (lightingFlashTimeCounter > LED_FLASH_RATE) { // flash next light pattern?
                lightingFlashTimeCounter -= LED_FLASH_RATE; // sub flash rate
                knight_rider(); // run knight rider sequence step
            }
            if (lightingOnTimeCounter > lightingDwellTime) { // done with this flash pattern?
                lightingOnTimeCounter = 0; // reset on time counter for next lighting show
                lightShowState = LIGHT_ALL_SHOW_START; // move to all flash state
                controlState = LIGHTING_STATE; // stay in lighting control state
                all_leds_off(); // turn off leds at end of show
            }
            break;
        case LIGHT_ALL_SHOW_START:
            lightingOnTimeCounter = 0; // init timer for on time
            lightingFlashTimeCounter = 0; // init timer for flash time
            //            TMR0_StartTimer(); // start timer
            all_at_50(); // run flashing sequence first step
            lightShowState = LIGHT_ALL_SHOW; // move to all show sequence
            break;
        case LIGHT_ALL_SHOW:
            if (lightingFlashTimeCounter > LED_FLASH_RATE) { // flash next light pattern?
                lightingFlashTimeCounter -= LED_FLASH_RATE; // sub flash rate
                all_at_50(); // run flashing sequence step
            }
            if (lightingOnTimeCounter > LED_FLASH_TIME) { // done with this flash pattern?
                lightingOnTimeCounter = 0; // reset on time counter for next lighting show
//                all_leds_off(); // turn off leds at end of show
                lightShowState = LIGHT_NOTHING_SHOW; // move to all flash state
            }
            break;
        case LIGHT_NOTHING_SHOW:
            all_leds_off(); // turn off all leds
            TMR4_Stop();    // turn off timer when done
            controlState = RE_ARM_STATE; // done with lighting sequence
            break;
        default:
            all_leds_off();
            controlState = RE_ARM_STATE;
            lightShowState = LIGHT_NOTHING_SHOW;
            // systemHealthTimerExpired = TRUE; // need a way to persist the low battery voltage LED
            break;
    }
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