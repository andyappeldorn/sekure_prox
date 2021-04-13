/* 
 * File:   led_display.h
 * Author: C13999
 *
 * Created on April 8, 2021, 6:00 PM
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

#ifndef LED_DISPLAY_H
#define	LED_DISPLAY_H

#include "mcc_generated_files/mcc.h"
#include "main.h"

#define NUM_OF_KNIGHT_SEQ_STEPS   6
#define LATB_LED_MASK             0x0f

typedef enum {
    LIGHT_NOTHING_SHOW, LIGHT_KNIGHT_SHOW_START, LIGHT_KNIGHT_SHOW, LIGHT_ALL_SHOW_START, LIGHT_ALL_SHOW
} e_lightShowState;
extern e_lightShowState lightShowState;

extern uint32_t lightingOnTimeCounter;
extern uint32_t lightingFlashTimeCounter;
extern uint32_t lightingDwellTime;

void LedTimerISR(void);
void lightingProcess(void);
void knight_rider(void);
void all_at_50(void);
void all_leds_off(void);

#endif	/* LED_DISPLAY_H */

