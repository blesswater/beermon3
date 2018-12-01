/* 
 * File:   beerChipBeermon.h
 * Author: BasementPC
 *
 * Created on November 23, 2018, 9:53 PM
 */

#include <stdint.h>
#include <string.h> /* For memset */

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipUserTimer.h" /* For beerChipBeermon.h */
#include "beerChipRelay.h"     /* For beerChipBeermon.h */
#include "beerChipBeermon.h"

/*
** State Table
*/
typedef void (*beermonAction_ptr_t)(beermonState_t *state);

/* Actions Declaration */
void doNothing( beermonState_t *state );
void gotoOff( beermonState_t *state );
void gotoOffNc( beermonState_t *state );
void gotoExtern( beermonState_t *state );
void gotoSwitchOut( beermonState_t *state );
void gotoOnDbnc( beermonState_t *state );
void gotoOn( beermonState_t *state );
void gotoOnNc( beermonState_t *state );
void gotoOffDbnc( beermonState_t *state );

beermonAction_ptr_t stateTbl[][6] = {
/*                 switch_in      switch_out   TGreater  TLess     TmrExpire ExternEnable */
/* switched_out */ {gotoOff,  doNothing,     doNothing, doNothing,  doNothing,gotoExtern},
/* state_off */    {doNothing,gotoSwitchOut, gotoOnDbnc,doNothing,  doNothing,gotoExtern},
/* on_debounce */  {doNothing,gotoSwitchOut, doNothing, gotoOffNc,  gotoOn,   gotoExtern},
/* state_on */     {doNothing,gotoSwitchOut, doNothing, gotoOffDbnc,doNothing,gotoExtern},
/* off_debounce */ {doNothing,gotoSwitchOut, gotoOnNc,  doNothing,  gotoOff,  gotoExtern},
/* extern_cntl */  {gotoOff,  gotoSwitchOut, doNothing, doNothing,  doNothing,doNothing}
};

/* Actions */
void doNothing( beermonState_t *state ) {}

void gotoOff( beermonState_t *state )
{
    /* Goto the Off state */
    state->state = beermon_state_off;
    relay_Switch( state->enableRelay, BEERCHIP_RELAY_ON );
    relay_Switch( state->controlRelay, BEERCHIP_RELAY_OFF );
    state->stats->offCnt++;
    usrStopwatch_Stop( &(state->stats->onTime) );
    usrStopwatch_Start( &(state->stats->offTime) );
    usrTmr_Stop( &(state->tmr) );
}

void gotoOffNc( beermonState_t *state )
{
    /* Goto the Off state - No count */
    state->state = beermon_state_off;
    relay_Switch( state->enableRelay, BEERCHIP_RELAY_ON );
    relay_Switch( state->controlRelay, BEERCHIP_RELAY_OFF );
    usrStopwatch_Stop( &(state->stats->onTime) );
    usrStopwatch_Start( &(state->stats->offTime) );
    usrTmr_Stop( &(state->tmr) );
}

void gotoExtern( beermonState_t *state )
{
    /* Goto the Extern state */
    state->state = beermon_state_extern_cntl;
    relay_Switch( state->enableRelay, BEERCHIP_RELAY_OFF );
    relay_Switch( state->controlRelay, BEERCHIP_RELAY_OFF );
    usrStopwatch_Stop( &(state->stats->onTime) );
    usrStopwatch_Stop( &(state->stats->offTime) );
    usrTmr_Stop( &(state->tmr) );
}

void gotoSwitchOut( beermonState_t *state )
{
    /* Goto the Switched Out state */
    state->state = beermon_state_switched_out;
    relay_Switch( state->enableRelay, BEERCHIP_RELAY_OFF );
    relay_Switch( state->controlRelay, BEERCHIP_RELAY_OFF );
    usrStopwatch_Stop( &(state->stats->onTime) );
    usrStopwatch_Stop( &(state->stats->offTime) );
    usrTmr_Stop( &(state->tmr) );
}

void gotoOnDbnc( beermonState_t *state )
{
    /* Goto the On Debounce state */
    state->state = beermon_state_on_debounce;
    relay_Switch( state->enableRelay, BEERCHIP_RELAY_ON );
    relay_Switch( state->controlRelay, BEERCHIP_RELAY_OFF );
    usrStopwatch_Stop( &(state->stats->onTime) );
    usrStopwatch_Start( &(state->stats->offTime) );
    usrTmr_Start( &(state->tmr), state->config->onDebounceTime );
}

void gotoOn( beermonState_t *state )
{
    /* Goto the On ON state */
    state->state = beermon_state_on;
    relay_Switch( state->enableRelay, BEERCHIP_RELAY_ON );
    relay_Switch( state->controlRelay, BEERCHIP_RELAY_ON );
    state->stats->onCnt++;
    usrStopwatch_Start( &(state->stats->onTime) );
    usrStopwatch_Stop( &(state->stats->offTime) );
    usrTmr_Stop( &(state->tmr) );
}

void gotoOnNc( beermonState_t *state )
{
    /* Goto the On ON state */
    state->state = beermon_state_on;
    relay_Switch( state->enableRelay, BEERCHIP_RELAY_ON );
    relay_Switch( state->controlRelay, BEERCHIP_RELAY_ON );
    usrStopwatch_Start( &(state->stats->onTime) );
    usrStopwatch_Stop( &(state->stats->offTime) );
    usrTmr_Stop( &(state->tmr) );
}

void gotoOffDbnc( beermonState_t *state )
{
    /* Goto the Off Debounce state */
    state->state = beermon_state_off_debounce;
    relay_Switch( state->enableRelay, BEERCHIP_RELAY_ON );
    relay_Switch( state->controlRelay, BEERCHIP_RELAY_ON );
    usrStopwatch_Start( &(state->stats->onTime) );
    usrStopwatch_Stop( &(state->stats->offTime) );
    usrTmr_Start( &(state->tmr), state->config->offDebounceTime );
}
        
/* API */
void beermonConfig_Init( beermonConfig_t *cfg )
{
    memset( cfg, 0x00, sizeof( beermonConfig_t ) );
    cfg->setTemp = BEERMON_DEFAULT_SETPOINT;
    cfg->offDebounceTime = BEERMON_DEFAULT_OFF_DEBOUNCE_TIME;
    cfg->onDebounceTime = BEERMON_DEFAULT_ON_DEBOUNCE_TIME;
    cfg->probe = 0x00;
    cfg->csum = beermonConfig_CalcCsum( cfg );
}

uint16_t beermonConfig_CalcCsum( beermonConfig_t *cfg )
{
    uint8_t i;
    /* Set to BEERMON_CONFIG_RESULT because we do not want all zeros to be valid */
    uint16_t csum = BEERMON_CONFIG_RESULT; 
    for( i = 0; i < sizeof(beermonConfig_t); i += 2 )
    {
        csum += *((uint16_t *)((uint8_t *)cfg + i));
    }
    csum = ~csum + 1;
    return csum;
}

void beermon_Init( beermonConfig_t *cfg, 
                   beermonState_t *state,
                   beermonStats_t *stats,
                   beerchip_relay_t *enableRelay,
                   beerchip_relay_t *controlRelay )
{
    /* Initialize the state */
    memset( state, 0x00, sizeof(beermonState_t) );
    state->state = beermon_state_switched_out;
    
    state->enableRelay = enableRelay;
    state->controlRelay = controlRelay;
    relay_Switch( state->enableRelay, BEERCHIP_RELAY_OFF );
    relay_Switch( state->controlRelay, BEERCHIP_RELAY_OFF );
    
    usrTmr_Init( &state->tmr );
    
    memset( stats, 0x00, sizeof(beermonStats_t) );
    state->stats = stats;
    usrStopwatch_Init( &(stats->offTime) );
    usrStopwatch_Init( &(stats->onTime) );
    
    state->config = cfg;
}

void beermon_ProcessEvent( beermonState_t *state, uint8_t event )
{
    /* Process the timer */
    if( usrTmr_Check( &(state->tmr) ) )
    {
        stateTbl[state->state][beermon_event_TmrExpire]( state );
    }
    stateTbl[state->state][event]( state );
}