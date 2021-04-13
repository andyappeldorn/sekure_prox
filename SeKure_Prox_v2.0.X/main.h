/*******************************************************
* File:   main.h
* Author: Mike
*******************************************************/

#ifndef _MAIN_H
#define _MAIN_H

#include <xc.h>

#define ENABLE_LOWPOWER   // enable low power operation, serial debug will not operate properly
#define PRODUCTION_TIMING   // comment this to use quick flash sequences for faster testing

#ifdef PRODUCTION_TIMING
/* PRODUCTION TIMING DEFINITIONS */
#define LED_FLASH_RATE 10   // x10ms - flash rate
#define LED_FLASH_TIME 1000   // x10ms - time to flash at end to signal done
// #define LED_DWELL_TIME 1000   // x10ms - dwell time determined by reading switches - don't use
#else
/* QUICK TEST TIMING DEFINITIONS */
#define LED_FLASH_RATE 1   // x10ms - flash rate
#define LED_FLASH_TIME 20   // x10ms - time to flash at end to signal done
#define LED_DWELL_TIME 30   // x10ms - dwell time determined by reading switches
#endif

#define TRUE                      1
#define FALSE                     0

/* STATE MACHINE DEFINITIONS */
typedef enum {
    SENSING_INIT, SENSING_STATE, LIGHTING_STATE, RE_ARM_STATE
} e_controlState; // main loop control states
extern e_controlState controlState;

/* FUNCTION PROTOTYPES */
void main_process(void);

#endif // _MAIN_H
