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
    
    /* Make output */
    BEERCHIP_LED_TRIS = BEERCHIP_LED_TRIS & ~BEERCHIP_LED_PIN;
    BEERCHIP_LED_ANSEL = BEERCHIP_LED_ANSEL & ~BEERCHIP_LED_PIN;
    BEERCHIP_LED_PORT = BEERCHIP_LED_PORT & ~BEERCHIP_LED_PIN;

    memset( &ledState, 0x00, sizeof(beerChip_LEDState_t) );
    ledState.mode = ledMode_Off;

}

void beerChip_KickLED( void )
{
    uint8_t cngCnt;
    
    switch( ledState.mode )
    {
        case ledMode_Blink:
            if( ledState.cnt++ == ledState.cngCnt )
            {
                BEERCHIP_LED_PORT = BEERCHIP_LED_PORT ^ BEERCHIP_LED_PIN;
                ledState.cnt = 0x00;
            }
        break;
        
        case ledMode_Flash:
            cngCnt = (BEERCHIP_LED_PORT & BEERCHIP_LED_PIN) ? ledState.cngCnt : ledState.offCnt;
            if( ledState.cnt++ == cngCnt )
            {
                BEERCHIP_LED_PORT = BEERCHIP_LED_PORT ^ BEERCHIP_LED_PIN;
                ledState.cnt = 0x00;
            }
        break;
        default: /* Do nothing */
        break;
    }
}

void beerChip_SetLEDMode( uint8_t mode, uint8_t cnt, uint8_t offCnt )
{
    if( (mode >= ledMode_Off) && (mode <= ledMode_Flash) )
    {
        ledState.mode = mode;
        ledState.cnt = 0x00;
        ledState.cngCnt = cnt;
        ledState.offCnt = offCnt;

        switch( mode )
        {
            case ledMode_Off:
                BEERCHIP_LED_PORT = BEERCHIP_LED_PORT & ~BEERCHIP_LED_PIN;
            break;

            case ledMode_On:
                BEERCHIP_LED_PORT = BEERCHIP_LED_PORT | BEERCHIP_LED_PIN;
            break;

            default:
                /* Do nothing */
            break;
        }
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
