#include <string.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipLed.h"

/*
** Global instance of the LED State
*/
static beerChip_LEDState_t ledState;


void beerChip_InitLED()
{
    
    /* Make output */
    BEERCHIP_LED_TRIS = BEERCHIP_LED_TRIS & ~BEERCHIP_LED_PIN;
    BEERCHIP_LED_ANSEL = BEERCHIP_LED_ANSEL & ~BEERCHIP_LED_PIN;
    BEERCHIP_LED_PORT = BEERCHIP_LED_PORT & ~BEERCHIP_LED_PIN;

    memset( &ledState, 0x00, sizeof(beerChip_LEDState_t) );
    ledState.mode = ledMode_Off;

}

void beerChip_KickLED( void )
{
    if( ledState.mode == ledMode_Blink )
    {
        if( ledState.cnt++ == ledState.cngCnt )
        {
            BEERCHIP_LED_PORT = BEERCHIP_LED_PORT ^ BEERCHIP_LED_PIN;
            ledState.cnt = 0x00;
        }
    }
}

void beerChip_SetLEDMode( uint8_t mode, uint8_t cnt )
{
    memset( &ledState, 0x00, sizeof(beerChip_LEDState_t) );
    ledState.mode = mode;
    ledState.cngCnt = cnt;
}
