/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC16LF18444
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.30 and above
        MPLAB 	          :  MPLAB X 5.40	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA4 procedures
#define RA4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define RA4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define RA4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define RA4_GetValue()              PORTAbits.RA4
#define RA4_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define RA4_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define RA4_SetPullup()             do { WPUAbits.WPUA4 = 1; } while(0)
#define RA4_ResetPullup()           do { WPUAbits.WPUA4 = 0; } while(0)
#define RA4_SetAnalogMode()         do { ANSELAbits.ANSA4 = 1; } while(0)
#define RA4_SetDigitalMode()        do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set RA5 procedures
#define RA5_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define RA5_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define RA5_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define RA5_GetValue()              PORTAbits.RA5
#define RA5_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define RA5_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define RA5_SetPullup()             do { WPUAbits.WPUA5 = 1; } while(0)
#define RA5_ResetPullup()           do { WPUAbits.WPUA5 = 0; } while(0)
#define RA5_SetAnalogMode()         do { ANSELAbits.ANSA5 = 1; } while(0)
#define RA5_SetDigitalMode()        do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set LED_0 aliases
#define LED_0_TRIS                 TRISBbits.TRISB4
#define LED_0_LAT                  LATBbits.LATB4
#define LED_0_PORT                 PORTBbits.RB4
#define LED_0_WPU                  WPUBbits.WPUB4
#define LED_0_OD                   ODCONBbits.ODCB4
#define LED_0_ANS                  ANSELBbits.ANSB4
#define LED_0_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define LED_0_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define LED_0_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define LED_0_GetValue()           PORTBbits.RB4
#define LED_0_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define LED_0_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define LED_0_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define LED_0_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define LED_0_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define LED_0_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define LED_0_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define LED_0_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set LED_1 aliases
#define LED_1_TRIS                 TRISBbits.TRISB5
#define LED_1_LAT                  LATBbits.LATB5
#define LED_1_PORT                 PORTBbits.RB5
#define LED_1_WPU                  WPUBbits.WPUB5
#define LED_1_OD                   ODCONBbits.ODCB5
#define LED_1_ANS                  ANSELBbits.ANSB5
#define LED_1_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define LED_1_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define LED_1_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define LED_1_GetValue()           PORTBbits.RB5
#define LED_1_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define LED_1_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define LED_1_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define LED_1_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define LED_1_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define LED_1_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define LED_1_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define LED_1_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set LED_2 aliases
#define LED_2_TRIS                 TRISBbits.TRISB6
#define LED_2_LAT                  LATBbits.LATB6
#define LED_2_PORT                 PORTBbits.RB6
#define LED_2_WPU                  WPUBbits.WPUB6
#define LED_2_OD                   ODCONBbits.ODCB6
#define LED_2_ANS                  ANSELBbits.ANSB6
#define LED_2_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define LED_2_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define LED_2_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define LED_2_GetValue()           PORTBbits.RB6
#define LED_2_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define LED_2_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define LED_2_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define LED_2_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define LED_2_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define LED_2_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define LED_2_SetAnalogMode()      do { ANSELBbits.ANSB6 = 1; } while(0)
#define LED_2_SetDigitalMode()     do { ANSELBbits.ANSB6 = 0; } while(0)

// get/set LED_3 aliases
#define LED_3_TRIS                 TRISBbits.TRISB7
#define LED_3_LAT                  LATBbits.LATB7
#define LED_3_PORT                 PORTBbits.RB7
#define LED_3_WPU                  WPUBbits.WPUB7
#define LED_3_OD                   ODCONBbits.ODCB7
#define LED_3_ANS                  ANSELBbits.ANSB7
#define LED_3_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define LED_3_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define LED_3_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define LED_3_GetValue()           PORTBbits.RB7
#define LED_3_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define LED_3_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define LED_3_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define LED_3_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define LED_3_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define LED_3_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define LED_3_SetAnalogMode()      do { ANSELBbits.ANSB7 = 1; } while(0)
#define LED_3_SetDigitalMode()     do { ANSELBbits.ANSB7 = 0; } while(0)

// get/set RC0 procedures
#define RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define RC0_GetValue()              PORTCbits.RC0
#define RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define RC0_SetPullup()             do { WPUCbits.WPUC0 = 1; } while(0)
#define RC0_ResetPullup()           do { WPUCbits.WPUC0 = 0; } while(0)
#define RC0_SetAnalogMode()         do { ANSELCbits.ANSC0 = 1; } while(0)
#define RC0_SetDigitalMode()        do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()              PORTCbits.RC1
#define RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define RC1_SetPullup()             do { WPUCbits.WPUC1 = 1; } while(0)
#define RC1_ResetPullup()           do { WPUCbits.WPUC1 = 0; } while(0)
#define RC1_SetAnalogMode()         do { ANSELCbits.ANSC1 = 1; } while(0)
#define RC1_SetDigitalMode()        do { ANSELCbits.ANSC1 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/