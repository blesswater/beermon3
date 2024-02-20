#include <string.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipLed.h"

/*
** Global instance of the LED State
*/
beerChip_LEDState_t ledState;


void beerChip_InitLED()
{
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
    switch( mode )
    {
        case ledMode_Off:
            ledState.mode = ledMode_Off;
            BEERCHIP_LED_PORT = BEERCHIP_LED_PORT & ~BEERCHIP_LED_PIN;
        break;

        case ledMode_On:
            ledState.mode = ledMode_On;
            BEERCHIP_LED_PORT = BEERCHIP_LED_PORT | BEERCHIP_LED_PIN;
        break;

        case ledMode_Blink:
            ledState.mode = ledMode_Blink;
            ledState.cnt = 0x00;
            ledState.cngCnt = cnt;
        break;

        default:
            /* Do nothing */
        break;
    }
}

void beerChip_ToggleLED( void )
{
    if( (ledState.mode == ledMode_On) )
    {
        ledState.mode = ledMode_Off;
        BEERCHIP_LED_PORT = BEERCHIP_LED_PORT & ~BEERCHIP_LED_PIN;
    }
    else if( ledState.mode == ledMode_Off )
    {
        ledState.mode = ledMode_On;
        BEERCHIP_LED_PORT = BEERCHIP_LED_PORT | BEERCHIP_LED_PIN;
    }
}
