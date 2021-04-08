/*******************************************************
* File:   main.h
* Author: Mike
*******************************************************/

#ifndef _MAIN_H
#define _MAIN_H

#include <xc.h>

#define ALL_FLASH_SECONDS   102   // 10 s

#define TRUE                      1
#define FALSE                     0
#define NUM_OF_LEDS               4
#define NUM_OF_KNIGHT_SEQ_STEPS   6
#define LATB_LED_MASK             0x0f
#define LIGHT_SHOW_STEP_INTERVAL  150

//void processProximityActive(enum mtouch_proximity_names proximity);
//void processProximityNotActive(enum mtouch_proximity_names proximity);
void LedTimerISR(void);
void light_handler(void);
void knight_rider(void);
void all_at_50(void);
void all_leds_off(void);
uint32_t get_switch_dwell_time_setting(void);
void disable_switches(void);
void enable_switches(void);
//void blocking_delay_ms(uint32_t);
//void battery_service(void);
void batteryMeasureProcess(void);
//void look_at_bat_voltage(void);
void measureBatteryVoltage(void);

#endif // _MAIN_H
