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
        
        case ledMode_Oneshot:
        case ledMode_Bloop:
            if( ledState.cnt < ledState.cngCnt )
            {
                if( ++ledState.cnt >= ledState.cngCnt )
                {
                    BEERCHIP_LED_PORT = BEERCHIP_LED_PORT & ~BEERCHIP_LED_PIN;
                }
            }
        break;
        
        default: /* Do nothing */
        break;
    }
}

void beerChip_SetLEDMode( uint8_t mode, uint8_t cnt, uint8_t offCnt )
{
    if( (mode >= ledMode_Off) && (mode <= ledMode_Bloop) )
    {
        ledState.mode = mode;
        ledState.cnt = 0x00;
        ledState.cngCnt = cnt;
        ledState.offCnt = offCnt;
        BEERCHIP_LED_PORT = BEERCHIP_LED_PORT & ~BEERCHIP_LED_PIN;

        switch( mode )
        {
            case ledMode_Off:
                BEERCHIP_LED_PORT = BEERCHIP_LED_PORT & ~BEERCHIP_LED_PIN;
            break;

            case ledMode_On:
            case ledMode_Oneshot:
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

void beerChip_BloopEvent( uint8_t event )
{
    /* event 1 -> bloop
    **       0 -> bloop ACK
    */
    if( ledState.mode == ledMode_Bloop )
    {
        if( event == 0x01 )
        {
            BEERCHIP_LED_PORT = BEERCHIP_LED_PORT | BEERCHIP_LED_PIN;
            ledState.cnt = 0x00;
        }
    }
}
