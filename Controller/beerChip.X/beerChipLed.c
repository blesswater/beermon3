#include <string.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipLed.h"

/*
** Global instance of the LED State
*/
static beerChip_LEDState_t ledState;


void beerChip_InitLED( uint8_t portMask, uint8_t mode )
{
    PORTA = PORTA & ~portMask;
    /* Make output */
    TRISA = TRISA & ~portMask;

    memset( &ledState, 0x00, sizeof(beerChip_LEDState_t) );
    ledState.mode = mode;
}

void beerChip_ForceBlink( void )
{
    if( (ledState.mode & ledMode_AllowForce) &&
         !(ledState.mode & (ledMode_stuckOn | ledMode_stuckOff )))
    {
        if( LATA & BEERCHIP_LED_MASK )
        {
            PORTA = PORTA & ~BEERCHIP_LED_MASK;
        }
        else
        {
            PORTA = PORTA | BEERCHIP_LED_MASK;
        }
    }
}
