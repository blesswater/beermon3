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

#include "blnk.h"
#include "blnk_version.h"




void blnk_InitLED( uint8_t portMask )
{
    PORTA = PORTA & ~portMask;
    /* Make output */
    TRISA = TRISA & ~portMask;
}

void blnk_Delay(void)
{
    long i = 524280;
    // long i = 5556;
    while(i--);
}

void blnk_InitPIC( void )
{
    OSCCON = BLNK_CLK;

    OPTION_REG = (1 << _OPTION_REG_nWPUEN_POSN) | /* Weak pullup disabled */
                 (1 << _OPTION_REG_INTEDG_POSN) | /* Edge triggered ints */
                 (0 << _OPTION_REG_TMR0CS_POSN) | /* TMR0 CLK SRC Fosc/4 */
                 (1 << _OPTION_REG_TMR0SE_POSN) | /* Incr Tmr0 Hi -> low */
                 (0 << _OPTION_REG_PSA_POSN)    | /* Assign pre-sclar to TMR0 */
                 (0x7 << _OPTION_REG_PS_POSN); /* Presclar 1:256 */

    WDTCON |= ( 0 << _WDTCON_SWDTEN_POSN); /* Turn WDT Off */

}
/*
** main
*/
int main(int argc, char** argv) {

    blnk_InitPIC();
    blnk_InitLED( BLNK_LED_MASK );
    
    // GIE = 1; /* GO! */

    while( 1 )
    {
        // PORTA = PORTA ^ BLNK_LED_MASK;
        if( LATA & BLNK_LED_MASK )
        {
            PORTA = PORTA & ~BLNK_LED_MASK;
        }
        else
        {
            PORTA = PORTA | BLNK_LED_MASK;
        }
        blnk_Delay();
    }
    return (EXIT_SUCCESS);
}

