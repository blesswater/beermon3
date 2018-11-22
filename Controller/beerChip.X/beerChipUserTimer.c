/* 
 * File:   beerChipUserTimer.c
 * Author: BasementPC
 *
 * Created on November 21, 2018, 4:37 PM
 */
#include <stdint.h>
#include <string.h> /* For memset */

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipUserTimer.h"

#define BEERCHIP_SWI do{}while(0)

/* Global */
extern uint32_t uptime;

void usrTmr_Init( userTmr_t *tmr )
{
    memset( (void *)tmr, 0x00, sizeof( userTmr_t ) );
}

void usrTmr_Start( userTmr_t *tmr, uint32_t ticks )
{
    BEERCHIP_SWI; /* Generate SWI */
    tmr->expireTime = uptime + ticks;
}

bool usrTmr_Check( userTmr_t *tmr )
{
    BEERCHIP_SWI; /* Generate SWI */
    return ( uptime < tmr->expireTime ) ? 0 : 1;
}