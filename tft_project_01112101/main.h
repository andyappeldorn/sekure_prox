/*******************************************************
* File:   main.h
* Author: Mike
*******************************************************/

#ifndef _MAIN_H
#define _MAIN_H

#include <xc.h>

#define TRUE                      1
#define FALSE                     0
#define NUM_OF_LEDS               4
#define NUM_OF_KNIGHT_SEQ_STEPS   6
#define LATB_LED_MASK             0x0f
#define LIGHT_SHOW_STEP_INTERVAL  150

#define NOTHING_SHOW    0
#define KNIGHT_SHOW     1
#define ALL_SHOW        2
#define TEST_SHOW       3
#define HANDS_OFF_SHOW  4

void light_handler(void);
void knight_rider(void);
void all_at_50(void);
uint32_t get_switch_dwell_time_setting(void);
void all_leds_off(void);
void disable_switches(void);
void enable_switches(void);
void blocking_delay_ms(uint32_t);
void battery_service(void);
void look_at_bat_voltage(void);

#endif // _MAIN_H