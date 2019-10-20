/* 
 * File:   beerChipBloopDet.c
 * Author: BasementPC
 *
 * Created on November 23, 2018, 9:53 PM
 */

#include <stdint.h>
#include <string.h> /* For memset */

#include <xc.h>

#include "beerChipBloopDet.h"
#include "beerChipLed.h"

/*
** State Table
*/
typedef void (*bloopDetAction_ptr_t)(bloopDetState_t *state);

/* Actions Declaration */
void bloopDet_doNothing( bloopDetState_t *state );
void bloopDet_doBloop( bloopDetState_t *state );
void bloopDet_doReset( bloopDetState_t *state );
void bloopDet_doRmBloop( bloopDetState_t *state );
void bloopDet_doBloopAck( bloopDetState_t *state );
void bloopDet_doBloopAckDrop( bloopDetState_t *state );


bloopDetAction_ptr_t bloopDet_stateTbl[][4] = {
/*                 bloop                not bloop            bloopack            reset */
/* wait */     {bloopDet_doBloop, bloopDet_doNothing, bloopDet_doNothing, bloopDet_doReset },
/* bloop */    {bloopDet_doNothing, bloopDet_doRmBloop, bloopDet_doBloopAck, bloopDet_doReset },
/* bloopack */ {bloopDet_doNothing, bloopDet_doBloopAckDrop, bloopDet_doNothing, bloopDet_doReset },
};

/* Actions */
void bloopDet_doNothing( bloopDetState_t *state ) {}

void bloopDet_doBloop( bloopDetState_t *state )
{
    state->bloopCnt++;
    state->state = bloopdet_state_bloop;
    beerChip_BloopEvent( (uint8_t)0x01 );
}

void bloopDet_doReset( bloopDetState_t *state )
{
    bloopDet_Init( state );
}

void bloopDet_doRmBloop( bloopDetState_t *state )
{
    
    if( state->bloopCnt > 0 )
    {
        state->bloopCnt--;
    }
    state->state = bloopdet_state_wait;
}

void bloopDet_doBloopAck( bloopDetState_t *state )
{
    state->bloopAckCnt++;
    state->state = bloopdet_state_bloopack;
    BEERCHIP_BLOOPACK_PORT = BEERCHIP_BLOOPACK_PORT | BEERCHIP_BLOOPACK_PIN;
    beerChip_BloopEvent( (uint8_t)0x00 );
}

void bloopDet_doBloopAckDrop( bloopDetState_t *state )
{
    state->state = bloopdet_state_wait;
    BEERCHIP_BLOOPACK_PORT = BEERCHIP_BLOOPACK_PORT & ~BEERCHIP_BLOOPACK_PIN;
}

void bloopDet_Init(bloopDetState_t *state )
{
    memset( state, 0x00, sizeof( bloopDetState_t ) );
    
    /* Setup Bloop Pin */
    BEERCHIP_BLOOP_TRIS = BEERCHIP_BLOOP_TRIS | BEERCHIP_BLOOP_PIN;
    BEERCHIP_BLOOP_ANSEL = BEERCHIP_BLOOP_ANSEL & ~BEERCHIP_BLOOP_PIN;
    
    /* Setup Bloop ACK */
    BEERCHIP_BLOOPACK_TRIS = BEERCHIP_BLOOPACK_TRIS & ~BEERCHIP_BLOOPACK_PIN;
    BEERCHIP_BLOOPACK_ANSEL = BEERCHIP_BLOOPACK_ANSEL & ~BEERCHIP_BLOOPACK_PIN;
    BEERCHIP_BLOOPACK_PORT = BEERCHIP_BLOOPACK_PORT & ~BEERCHIP_BLOOPACK_PIN;
}

void bloopDet_ProcessEvent( bloopDetState_t *state, uint8_t event )
{
    bloopDet_stateTbl[state->state][event]( state );
}

void bloopDet_Process( bloopDetState_t *state )
{
    uint8_t event;
    
    event = (BEERCHIP_BLOOP_PORT & BEERCHIP_BLOOP_PIN) ? bloopdet_event_bloop : bloopdet_event_notbloop;
    bloopDet_ProcessEvent( state, event );
}