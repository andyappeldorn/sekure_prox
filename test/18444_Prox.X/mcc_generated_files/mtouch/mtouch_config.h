/*
    MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:

    You may use this software, and any derivatives created by any person or
    entity by or on your behalf, exclusively with Microchip's products.
    Microchip and its subsidiaries ("Microchip"), and its licensors, retain all
    ownership and intellectual property rights in the accompanying software and
    in all derivatives hereto.

    This software and any accompanying information is for suggestion only. It
    does not modify Microchip's standard warranty for its products.  You agree
    that you are solely responsible for testing the software and determining
    its suitability.  Microchip has no obligation to modify, test, certify, or
    support the software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S
    PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
    (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
    INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
    ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
    FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL
    LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED
    THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR
    THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
    THESE TERMS.
*/
#ifndef MTOUCH_CONFIG_H
#define MTOUCH_CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "mtouch_sensor.h"
#include "mtouch_proximity.h"
#include "mtouch_datastreamer.h"

/*
 * =======================================================================
 * Application / Configuration Settings
 * =======================================================================
 */
    #define MTOUCH_SENSORS          1u
    #define MTOUCH_PROXIMITY        1u
    #define MTOUCH_DEBUG_COMM_ENABLE      1u

    /* 
     * =======================================================================
     * Sensor Parameters
     * =======================================================================
     */

    
    #define MTOUCH_SENSOR_ADPCH_Sensor_ANA4              0x4
    #define MTOUCH_SENSOR_PRECHARGE_Sensor_ANA4          48u
    #define MTOUCH_SENSOR_ACQUISITION_Sensor_ANA4        32u
    #define MTOUCH_SENSOR_OVERSAMPLING_Sensor_ANA4       32u
    #define MTOUCH_SENSOR_ADDITIONALCAP_Sensor_ANA4      6u
    /*-----------------------------------------------------------------------------*/
    #define MTOUCH_SENSOR_ACTIVE_THRESHOLD               100u  

    
    /* 
     * =======================================================================
     * Proximity Parameters
     * =======================================================================
     */
    #define MTOUCH_PROXIMITY_READING_GAIN           ((uint8_t)1u)
    #define MTOUCH_PROXIMITY_BASELINE_GAIN          ((uint8_t)4u)
    #define MTOUCH_PROXIMITY_BASELINE_INIT          ((mtouch_prox_baselinecounter_t)16u)
    #define MTOUCH_PROXIMITY_BASELINE_RATE          ((mtouch_prox_baselinecounter_t)128u)
    #define MTOUCH_PROXIMITY_BASELINE_HOLD          ((mtouch_prox_baselinecounter_t)1024u)
    #define MTOUCH_PROXIMITY_DEVIATION_GAIN         ((uint8_t)1u);
    #define MTOUCH_PROXIMITY_NEGATIVEDEVIATION      ((mtouch_prox_statecounter_t)32u)
    #define MTOUCH_PROXIMITY_ACTIVITYTIMEOUT        ((mtouch_prox_statecounter_t)2000u)

    #define MTOUCH_PROXIMITY_THRESHOLD_Proximity0   20u
    
    #define MTOUCH_PROXIMITY_SCALING_Proximity0     0u
    
    #define MTOUCH_PROXIMITY_HYSTERESIS_Proximity0    HYST_50_PERCENT






#endif // MTOUCH_CONFIG_H
/**
 End of File
*/
