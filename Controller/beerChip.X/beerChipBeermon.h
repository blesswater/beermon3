/* 
 * File:   beerChipBeermon.h
 * Author: BasementPC
 *
 * Created on November 23, 2018, 9:53 PM
 */

#ifndef BEERCHIPBEERMON_H
#define	BEERCHIPBEERMON_H

#include "beerChipUserTimer.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define BEERMON_DEFAULT_SETPOINT          0x2D00  /* 45 F */
#define BEERMON_DEFAULT_ON_DEBOUNCE_TIME  120     /* 2min */
#define BEERMON_DEFAULT_OFF_DEBOUNCE_TIME 120     /* 2min */
    
#define BEERMON_NUM_TEMP_PROBES  2
    
/* Beermon States */
#define beermon_state_switched_out     0x00
#define beermon_state_off              0x01
#define beermon_state_on_debounce      0x02
#define beermon_state_on               0x03
#define beermon_state_off_debounce     0x04
#define beermon_state_extern_cntl      0x05
    
/* Beermon Events */
#define beermon_event_switch_in       0x00
#define beermon_event_switch_out      0x01
#define beermon_event_TGreater        0x02
#define beermon_event_TLess           0x03
#define beermon_event_TmrExpire       0x04
#define beermon_event_ExternEnable    0x05

typedef struct {
    int16_t setTemp;
    uint16_t onDebounceTime;
    uint16_t offDebounceTime;
    uint16_t seqNum;
    uint8_t probe;
    uint8_t rffu;
    uint16_t csum;
} beermonConfig_t;

typedef struct {
    uint16_t onCnt;
    uint16_t offCnt;
    usrStopwatch_t onTime;
    usrStopwatch_t offTime;
} beermonStats_t;

typedef struct {
    uint8_t state;
    userTmr_t tmr;
    beerchip_relay_t *enableRelay;
    beerchip_relay_t *controlRelay;
    beermonStats_t *stats;
    beermonConfig_t *config;
} beermonState_t;

void beermonConfig_Init( beermonConfig_t *cfg );

void beermon_Init( beermonConfig_t *cfg, 
                   beermonState_t *state,
                   beermonStats_t *stats,
                   beerchip_relay_t *enableRelay,
                   beerchip_relay_t *controlRelay );

void beermon_ProcessEvent( beermonState_t *state, uint8_t event );


#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPBEERMON_H */

