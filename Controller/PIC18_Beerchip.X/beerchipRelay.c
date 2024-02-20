/* 
 * File:   beerChipRelay.c
 * Author: BasementPC
 *
 * Created on November 22, 2018, 1:03 AM
 */
#include <stdint.h>
#include <string.h> /* For memset */

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipRelay.h"

void relay_Init( beerchip_relay_t *rly, uint8_t pin )
{
    memset( rly, 0x00, sizeof(beerchip_relay_t) );
    rly->portMask = pin;
    BEERCHIP_RLY_PORT = BEERCHIP_RLY_PORT & ~rly->portMask;
}

void relay_Switch( beerchip_relay_t *rly, uint8_t state )
{
    if( state == BEERCHIP_RELAY_ON )
    {
        BEERCHIP_RLY_PORT |= rly->portMask;
        rly->state = BEERCHIP_RELAY_ON;
    }
    else if( state == BEERCHIP_RELAY_OFF )
    {
        BEERCHIP_RLY_PORT &= ~rly->portMask;
        rly->state = BEERCHIP_RELAY_OFF;
    }
}

