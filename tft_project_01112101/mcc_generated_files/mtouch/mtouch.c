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

#include <xc.h>
#include <stdint.h>

#include "mtouch.h"
#include "../mcc.h"

/* ======================================================================= *
 * Definitions
 * ======================================================================= */
#define MTOUCH_SCAN_TIMER_TICK                  1032.258 //unit us
#define MTOUCH_MAX_TIMER_PERIOD                 264 //unit ms
#define MTOUCH_SCAN_PERIOD                      (uint16_t)MTOUCH_SCAN_INTERVAL
#define MTOUCH_TMR2_POST_SCALER                 (uint8_t)(MTOUCH_SCAN_PERIOD / MTOUCH_MAX_TIMER_PERIOD)
#define MTOUCH_TMR2_PERIOD                      (uint8_t)((MTOUCH_SCAN_PERIOD*1000.0)/(MTOUCH_SCAN_TIMER_TICK*(MTOUCH_TMR2_POST_SCALER+1))-1u)
#define MTOUCH_LOWPOWER_SCAN_PERIOD             (uint16_t)MTOUCH_LOWPOWER_SCAN_INTERVAL
#define MTOUCH_TMR2_PERIOD_LOWPOWER             (uint8_t)((MTOUCH_LOWPOWER_SCAN_PERIOD*1000.0)/(MTOUCH_SCAN_TIMER_TICK*(MTOUCH_TMR2_POST_SCALER_LOWPOWER+1))-1u)
#define MTOUCH_TMR2_POST_SCALER_LOWPOWER        (uint8_t)(MTOUCH_LOWPOWER_SCAN_PERIOD / MTOUCH_MAX_TIMER_PERIOD)
#define MTOUCH_LOWPOWER_INACTIVE_TIMEOUT_CYCLE  (uint16_t)(MTOUCH_LOWPOWER_INACTIVE_TIMEOUT/MTOUCH_SCAN_INTERVAL)
#define MTOUCH_LOWPOWER_BASELINEUPDATE_CYCLE    (uint16_t)(MTOUCH_LOWPOWER_BASELINEUPDATE_TIME/MTOUCH_LOWPOWER_SCAN_INTERVAL)


/* ======================================================================= *
 * Local Variable
 * ======================================================================= */

static bool mtouch_time_toScan = false;
static bool mtouch_request_init = false;
static bool mtouch_lowpowerEnabled = true;
static bool mtouch_lowpowerActivated = false;
static uint16_t mtouch_inactive_counter = 0;
static uint16_t mtouch_sleep_baseline_counter = 0;
const  uint8_t mtouch_sleep_sensors[] = MTOUCH_LOWPOWER_SENSOR_LIST;

/*
 * =======================================================================
 *  Local Functions
 * =======================================================================
 */
static void MTOUCH_ScanScheduler(void); 
static void MTOUCH_SetTimer2Period(uint16_t scanPeriod); 
static void MTOUCH_Lowpower_Initialize();
static bool MTOUCH_needReburst(void);

/*
 * =======================================================================
 * MTOUCH_ScanScheduler()
 * =======================================================================
 *  The interrupt handler callback for scanrate timer  
 */
static void MTOUCH_ScanScheduler(void)         
{
    
    //schedule the scan
    mtouch_time_toScan = true;  

}

/*
 * =======================================================================
 * MTOUCH_SetTimer2Config
 * =======================================================================
 *  set Timer2 period register and the postscaler   
 */
static void MTOUCH_SetTimer2Config(void) 
{
    if(mtouch_lowpowerActivated)
    {
        /* Set timer period and control register */
        T2CON = 0x00;                        /* Timer OFF */
        TMR2_LoadPeriodRegister(MTOUCH_TMR2_PERIOD_LOWPOWER);     /* set period */
        T2CON = 0xD0 + (MTOUCH_TMR2_POST_SCALER_LOWPOWER & 0x0F);  /* set post-scaler, Timer ON */
    }
    else
    {
        T2CON = 0x00; 
        TMR2_LoadPeriodRegister(MTOUCH_TMR2_PERIOD);     
        T2CON = 0xD0 + (MTOUCH_TMR2_POST_SCALER & 0x0F);  
    }
} 

/*
 * =======================================================================
 * MTOUCH_Service_isInProgress()
 * =======================================================================
 *  indicate the mTouch service status
 */

bool MTOUCH_Service_isInProgress()
{
    return mtouch_time_toScan;
}

/*
 * =======================================================================
 * MTOUCH_Lowpower_Initialize()
 * =======================================================================
 *  initialize the registers and settings for low power operation
 */
static void MTOUCH_Lowpower_Initialize()
{
}

/*
 * =======================================================================
 * MTOUCH_Init()
 * =======================================================================
 *  Root initialization routine for all enabled mTouch library modules.
 */
void MTOUCH_Initialize(void)
{   
    MTOUCH_Sensor_InitializeAll();
    MTOUCH_Proximity_InitializeAll();
#if (MTOUCH_DEBUG_COMM_ENABLE == 1u)
    MTOUCH_DataStreamer_Initialize();    
#endif      
    MTOUCH_Sensor_Sampled_ResetAll();
    MTOUCH_Sensor_Scan_Initialize();
    MTOUCH_Lowpower_Initialize();
    MTOUCH_SetTimer2Config();
    TMR2_SetInterruptHandler(MTOUCH_ScanScheduler);

}

/*
 * =======================================================================
 * MTOUCH_Service_Mainloop()
 * =======================================================================
 *  Root mainloop service routine for all enabled mTouch library modules.
 */

#define ONE_MIN_TICKS   720
extern bool one_minute_flag;
uint32_t one_minute_countdown_timer = ONE_MIN_TICKS;

bool MTOUCH_Service_Mainloop(void)
{
    if(mtouch_request_init)
    {
        MTOUCH_Initialize();
        mtouch_request_init = false;
    }
    /* In free running mode, the mTouch service will be executed once MTOUCH_Service_Mainloop gets called.*/
    mtouch_time_toScan = true;
    
    if(mtouch_time_toScan)               
    {
        if(mtouch_lowpowerActivated && mtouch_lowpowerEnabled)
        {
            if(MTOUCH_Sensor_isSamplingComplete())
            {
                mtouch_time_toScan = false;
                MTOUCH_Sensor_Sampled_ResetAll();  
                
                if(MTOUCH_Sensor_isAnySensorActive())
                {
                    MTOUCH_Service_exitLowpower();
                    mtouch_inactive_counter = 0;
                }
                else
                {
                    /* Exit low power temporarily for baseline update */
                    if((0u != MTOUCH_LOWPOWER_BASELINEUPDATE_CYCLE) &&
                       (++mtouch_sleep_baseline_counter == MTOUCH_LOWPOWER_BASELINEUPDATE_CYCLE)) 
                    {
                        MTOUCH_Proximity_Baseline_ForceUpdate();
                        MTOUCH_Service_exitLowpower();
                        mtouch_sleep_baseline_counter = 0;
                        mtouch_inactive_counter = 
                                MTOUCH_LOWPOWER_INACTIVE_TIMEOUT_CYCLE - 1;
                    }
                    SLEEP();
                    NOP();
                    NOP();
                    
                }
                
                
            }
            return true;
        }
        else
        {
            if(MTOUCH_Sensor_SampleAll() == false)     
                return false;           
            MTOUCH_Proximity_ServiceAll();          /* Execute state machine for all proximity w/scanned sensors */
        #if (MTOUCH_DEBUG_COMM_ENABLE == 1u)
            MTOUCH_DataStreamer_Service();          /* Execute Data Visualizer module */  
        #endif
            mtouch_time_toScan = MTOUCH_needReburst();
            MTOUCH_Sensor_Sampled_ResetAll();  
            MTOUCH_Tick();
            if(mtouch_lowpowerEnabled)
            {
                if(MTOUCH_Proximity_Proximitymask_Get())
                {
                    mtouch_inactive_counter = 0;
                }
                else
                {
                    if(++mtouch_inactive_counter == 
                       MTOUCH_LOWPOWER_INACTIVE_TIMEOUT_CYCLE)
                    {
                        MTOUCH_Service_enterLowpower();
                        mtouch_sleep_baseline_counter = 0;
                        SLEEP();
                        NOP();
                        NOP();
                        if (one_minute_countdown_timer)
                        {
                          one_minute_countdown_timer--;
                        }
                        else
                        {
                          one_minute_countdown_timer = ONE_MIN_TICKS;
                          one_minute_flag = true;
                        }
                    }
                }
            }
            return true;
        }   
    }
    else                              
    {
        return false;                
    }
}

/*
 * =======================================================================
 * MTOUCH_Tick
 * =======================================================================
 */
void MTOUCH_Tick(void)
{
    MTOUCH_Proximity_Tick();
}

/*
 * =======================================================================
 * MTOUCH_Reburst
 * =======================================================================
 */
 static bool MTOUCH_needReburst(void)
 {
    bool needReburst = false;
    
    return needReburst;
 }

/*
 * =======================================================================
 * MTOUCH_Service_enterLowpower
 * =======================================================================
 */
void MTOUCH_Service_enterLowpower(void)
{
    uint8_t i;

    mtouch_lowpowerActivated = true;
    
    for(i=0;i<MTOUCH_SENSORS;i++)
    {
        MTOUCH_Sensor_Disable (i);
    }
    
    for(i=0;i < sizeof(mtouch_sleep_sensors);i++)
    {
        MTOUCH_Sensor_Enable(mtouch_sleep_sensors[i]);
    }

    MTOUCH_SetTimer2Config();  
    
    MTOUCH_Sensor_startLowpower();
}

/*
 * =======================================================================
 * MTOUCH_Service_exitLowpower
 * =======================================================================
 */
void MTOUCH_Service_exitLowpower(void)
{
    uint8_t i;
    
    mtouch_lowpowerActivated = false;

    MTOUCH_SetTimer2Config(); 
    
    for(i=0;i < MTOUCH_SENSORS;i++)
    {
        MTOUCH_Sensor_Enable (i);
    }
    MTOUCH_Sensor_exitLowpower ();
}

/*
 * =======================================================================
 * MTOUCH_Service_LowpowerState_Get
 * =======================================================================
 */
bool MTOUCH_Service_LowpowerState_Get(void)
{
    return mtouch_lowpowerActivated;
}
 

/*
 * =======================================================================
 * MTOUCH_Service_disableLowpower
 * =======================================================================
 */
void MTOUCH_Service_disableLowpower(void)
{
     mtouch_lowpowerEnabled = false;
}
 
/*
 * =======================================================================
 * MTOUCH_Service_enableLowpower
 * =======================================================================
 */
void MTOUCH_Service_enableLowpower(void)
{
    mtouch_lowpowerEnabled = true;
}


/*
 * =======================================================================
 * Request the initialization of mTouch library
 * note: this is designed to use in the Interrupt context so that the compiler
 *  does not duplicate the MTOUCH_Initialize() and causing possible hardware 
 *  stack overflow.
 * =======================================================================
 */
 void MTOUCH_requestInitSet(void)
 {
     mtouch_request_init = true;
 }

 bool MTOUCH_requestInitGet(void)
 {
     return mtouch_request_init;
 }
