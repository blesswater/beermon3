/* 
 * File:   main.c
 * Author: BasementPC
 *
 * Created on January 8, 2014, 12:21 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#include "beerChipConfig.h"
#include "beerChipLed.h"
#include "beerChipI2C.h"
#include "beerLock.h"
#include "beerChipA2D.h"

/*
** Globals
*/

a2d_Reading_t a2dChan0;
a2d_Reading_t a2dChan1;


void blnk_Delay(void)
{
    long i = 524280;
    // long i = 5556;
    while(i--);
}

void beerchip_InitPIC( void )
{
    OSCCON = BEERCHIP_CLK;

    OPTION_REG = (1 << _OPTION_REG_nWPUEN_POSN) | /* Weak pullup disabled */
                 (1 << _OPTION_REG_INTEDG_POSN) | /* Edge triggered ints */
                 (0 << _OPTION_REG_TMR0CS_POSN) | /* TMR0 CLK SRC Fosc/4 */
                 (1 << _OPTION_REG_TMR0SE_POSN) | /* Incr Tmr0 Hi -> low */
                 (0 << _OPTION_REG_PSA_POSN)    | /* Assign pre-sclar to TMR0 */
                 (0x7 << _OPTION_REG_PS_POSN); /* Presclar 1:256 */

    /* Timer 1 Config */
    T1CON = (0x0 << _T1CON_TMR1CS_POSN) | /* Use Fosc/4 */
            (1 << _T1CON_TMR1ON_POSN)   | /* Timer 1 Enabled */
            (0x3 << _T1CON_T1CKPS_POSN) | /* Timer 1 Prescale 1:8 */
            (1 << _T1CON_nT1SYNC_POSN);   /* Not syncronazed */ 

    T1GCON = (0 << _T1GCON_TMR1GE_POSN);    /* Turn off Gate Enable */
    /* Enable Timer 1 interrupt */
    PIE1 = (1 << _PIE1_TMR1IE_POSN);      /* Enable interrupt */
    INTCON = (1 << _INTCON_PEIE_POSN);    /* Timer interrupt Enable */
    /* Set 16 Bit counter to roll over in 0.1sec */
    TMR1H = 0x3C;
    TMR1L = 0xAF;


    WDTCON |= ( 0 << _WDTCON_SWDTEN_POSN); /* Turn WDT Off */

}

uint8_t i2c_ReadI2CSelect()
{
#ifdef BEERCHIP_I2C_SELECT_ENABLE
    uint8_t selValue = 0x00;
    uint8_t cnt = 0;
    long i;
    /* Set I2C Select pin to Input */
    BEERCHIP_I2C_SELECT_PORT |= BEERCHIP_I2C_SELECT_PINS;
    BEERCHIP_I2C_SELECT_ANSEL &= !BEERCHIP_I2C_SELECT_PINS; /* Turn OFF analog select */

    /* Read SELECT_I2C pin. Make sure get 0x20 readings all the same */
    while( cnt < 0x20 )
    {
        /* hbrdg_Delay() */
        i = 5556;
        while(i--);

        cnt++;
        // LATA ^= BEERCHIP_LED_MASK;
        if( (BEERCHIP_I2C_SELECT_PORT & BEERCHIP_I2C_SELECT_PINS) != selValue )
        {
            cnt = 0;
        }
        selValue =  (BEERCHIP_I2C_SELECT_PORT & BEERCHIP_I2C_SELECT_PINS);
    }

    return( (selValue == 0x00) ? 0x00 : 0x0F );
#else
    return( (uint8_t)0x00 );
#endif

}
/*
** main
*/
int main(int argc, char** argv) 
{

    a2d_Reading_t *thisChan; 
    
    beerchip_InitPIC();

    beerChip_InitLED();
    beerChip_SetLEDMode( ledMode_Blink, BEERCHIP_BLINK_RATE );

    /* Init I2C */
    i2c_InitI2CSlave( BEERCHIP_I2C_ADDRESS + (uint8_t)i2c_ReadI2CSelect() );
    i2c_ResetI2CSlave();
    
    /* Init A2D */
    a2d_Init();
    a2d_InitReading( &a2dChan0, BEERCHIP_A2D_CHAN0 );
    a2d_InitReading( &a2dChan1, BEERCHIP_A2D_CHAN1 );
    /* Get a few readings before enabling interrupts */
    thisChan = &a2dChan0;
    a2d_StartReading( thisChan );
    while( !a2d_PollReading( thisChan ) );
    thisChan = &a2dChan1;
    a2d_StartReading( thisChan );
    while( !a2d_PollReading( thisChan ) );
    

    GIE = 1; /* GO! */

    /* Dispatch Loop */
    while( 1 )
    {
        thisChan = (thisChan == &a2dChan0) ? &a2dChan1 : &a2dChan0;
        a2d_StartReading( thisChan );
        while( !a2d_PollReading( thisChan ) );
    }
    return (EXIT_SUCCESS);
}

