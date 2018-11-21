#include <stdint.h>
#include <string.h>

#include <xc.h>

#include "blnk.h"
#include "blnk_version.h"

blnk_blinker_t blnk_state;

void blnk_InitLED( uint8_t portMask, blnk_mode_t mode )
{
    PORTA = PORTA & ~portMask;
    /* Make output */
    TRISA = TRISA & ~portMask;

    memset( &blnk_state, 0x00, sizeof(blnk_blinker_t) );
    blnk_state.cnt = 0x0000;
    blnk_state.mode = mode & ~blnk_mode_timer; /* Cannot set to timer mode here */
}

void blnk_ForceBlink( void )
{
    if( (blnk_state.mode & blnk_mode_allowForce) &&
        !(blnk_state.mode & (blnk_mode_stuckOn | blnk_mode_stuckOff)) )
    {
        if( LATA & BLNK_LED_MASK )
        {
            PORTA = PORTA & ~BLNK_LED_MASK;
        }
        else
        {
            PORTA = PORTA | BLNK_LED_MASK;
        }
    }
}
