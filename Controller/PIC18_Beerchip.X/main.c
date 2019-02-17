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
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC18F14K50
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
#include "beerChipLed.h"
#include "beerChipConfig.h" /* For beerLock.h */
#include "beerLock.h" /* For beerChipA2D.h */
#include "beerChipA2D.h"
#include "beerChipISRRoutines.h"
#include "beerChipRelay.h"
#include "beerChipUserTimer.h"
#include "beerChipBeermon.h"

/*
** Globals
*/
a2d_Reading_t a2dProbe[BEERMON_NUM_TEMP_PROBES];

beermonConfig_t beermonCfg;
beermonStats_t beermonStats;
beermonState_t beermonState;

/*
                         Main application
 */
void main(void)
{
    char thisChan = 0;
    
    beerchip_relay_t enableRelay;
    beerchip_relay_t controlRelay;
    
    userTmr_t testTimer;
    
    // Initialize the device
    SYSTEM_Initialize();
    
    beerChip_InitLED();
    beerChip_SetLEDMode( ledMode_Blink, 0x32 );
    
    /* Init A/D */
    a2d_Init();
    a2d_InitReading( &a2dProbe[0], BEERCHIP_A2D_CHAN0 );
    a2d_InitReading( &a2dProbe[1], BEERCHIP_A2D_CHAN1 );
    
    /* Init Relays for Beermon */
    relay_Init( &enableRelay, BEERCHIP_RYL0_PIN );
    relay_Init( &controlRelay,  BEERCHIP_RYL1_PIN );
    
    beermonConfig_Init( &beermonCfg );
    beermon_Init( &beermonCfg, &beermonState, &beermonStats, 
                  &enableRelay, &controlRelay );
    
    TMR1_SetInterruptHandler( beerChipTimerISR );

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    while (1)
    {
        // Add your application code
        thisChan = (++thisChan >= BEERMON_NUM_TEMP_PROBES) ? 0 : thisChan;
        a2d_GetTemperatureReading( &a2dProbe[thisChan] );
        if( thisChan == beermonCfg.probe )
        {
            if( a2dProbe[thisChan].temp > beermonCfg.setTemp )
            {
                beermon_ProcessEvent( &beermonState, beermon_event_TGreater );
            }
            else
            {
                beermon_ProcessEvent( &beermonState, beermon_event_TLess );
            }
        }
    }
}
/**
 End of File
*/