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

void usrTmr_Stop( userTmr_t *tmr )
{
    tmr->expireTime = 0x00000000;
}

bool usrTmr_Check( userTmr_t *tmr )
{
    BEERCHIP_SWI; /* Generate SWI */
    return ((uptime < tmr->expireTime) || (tmr->expireTime == 0x00000000)) ? 0 : 1;
}

/*
** Stopwatch
*/
void usrStopwatch_Init( usrStopwatch_t *stpw )
{
    memset( stpw, 0x00, sizeof( usrStopwatch_t) );
}

void usrStopwatch_Start( usrStopwatch_t *stpw )
{
    if( !(stpw->flags & BEERMON_USERTIMER_FLAG_RUNNING) )
    {
        BEERCHIP_SWI; /* Generate SWI */
        stpw->startTime = uptime;
        stpw->flags |= BEERMON_USERTIMER_FLAG_RUNNING;
    }
}
void usrStopwatch_Stop( usrStopwatch_t *stpw ) 
{
    BEERCHIP_SWI; /* Generate SWI */
    if( stpw->flags & BEERMON_USERTIMER_FLAG_RUNNING )
    {
        BEERCHIP_SWI; /* Generate SWI */
        stpw->accumTime += (uptime - stpw->startTime);
        stpw->flags &= ~BEERMON_USERTIMER_FLAG_RUNNING;
    }
}
uint32_t usrStopwatch_GetTime( usrStopwatch_t *stpw ) 
{
    uint32_t totalTime;
    if( stpw->flags & BEERMON_USERTIMER_FLAG_RUNNING)
    {
        totalTime = stpw->accumTime + uptime - stpw->startTime;
    }
    else
    {
        totalTime = stpw->accumTime;
    }
    return totalTime;
}