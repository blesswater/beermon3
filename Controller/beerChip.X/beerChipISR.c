#include <stdint.h>
#include <string.h>  /* For memcpy */

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipI2C.h"
#include "beerChipI2CIndex.h"
#include "beerChipLed.h"
#include "beerLock.h"
#include "beerChipA2D.h"
#include "beerChipTempLookup.h"
#include "beerChipRelay.h"
#include "beerChipBeermon.h"
#include "beerChipBloopDet.h"

uint32_t uptime = BEERMON_USRTIMER_START_CNT;
static uint8_t  secTickCnt = 0x00;

extern a2d_Reading_t a2dProbe[BEERMON_NUM_TEMP_PROBES];
static int16_t tempChan0; 
static int16_t tempChan1;
static int16_t tempChan2;
static int16_t tempChan3;
static uint8_t tempReadingCnt;

extern beerchip_relay_t enableRelay;
extern beerchip_relay_t controlRelay;

extern beermonConfig_t beermonCfg;
beermonConfig_t workingBeermonCfg;
/* beer cfg states */
#define beerCfgClean 0x00  /* No changes */
#define beerCfgDirty 0x01  /* Changes Pending */

/* Command Register */
extern uint8_t beermonStateControlMsg;

extern beermonStats_t beermonStats;
struct {
    uint16_t onCnt;
    uint16_t offCnt;
    uint32_t onTime;
    uint32_t offTime;
} nowStats;
static uint8_t statReadCnt;

extern beermonState_t beermonState;

extern bloopDetState_t bloopDetState;

void __interrupt () ISR( void )
// void interrupt ISR( void )
{
    uint8_t i2cIndex;
    uint8_t i2cValue;
    i2c_i2cStatus i2cStatus;

    static uint32_t uptimeSnapshot;
    
    static uint8_t beerCfgState = beerCfgClean;
    
    static struct {
        uint16_t reading;
        uint16_t count;
    } a2dChan0Snapshot, a2dChan1Snapshot, a2dChan2Snapshot, a2dChan3Snapshot;
    
    // a2dChan0Snapshot.count = 0x89ab;
    // a2dChan0Snapshot.reading = 0xcdef;

    if( TMR1IF )
    {
        bloopDet_Process( &bloopDetState );
        beerChip_KickLED();

        /* Do time tick */
        if( secTickCnt++ == BEERCHIP_TICKS_PER_SECOND )
        {
            /* This should happen once per second */
            uptime++;
            secTickCnt = 0x00;
            // nowStats.onTime = usrStopwatch_GetTime( &(beermonStats.onTime) );
            // nowStats.offTime = usrStopwatch_GetTime( &(beermonStats.offTime) );
        }

        /* Set 16 Bit counter to roll over in 0.1sec */
        TMR1H = 0x3C;
        TMR1L = 0xAF;
        
        TMR1IF = 0x00;
    }


    if( SSP1IF )
    {
        do {

            SSP1IF = 0;

            i2cStatus = i2c_IsrI2CSlave( &i2cIndex, &i2cValue );
            if( i2cStatus.i2cStatusMasterWrite )
            {
                switch( i2cIndex )
                {
                    case BEERCHIP_I2C_LED_MODE:
                        beerChip_SetLEDMode( i2cValue, ledState.cngCnt, ledState.offCnt );
                    break;

                    case BEERCHIP_I2C_LED_CNT_LIMIT:
                        beerChip_SetLEDMode( ledState.mode, i2cValue, ledState.offCnt );
                    break;
                    
                    case BEERCHIP_I2C_LED_CNT_OFF:
                        beerChip_SetLEDMode( ledState.mode, ledState.cngCnt, i2cValue );
                    break;
                    
                    case BEERCHIP_A2D_TRIGGER_READING:
                        if( lock_Check( &(a2dProbe[0].lock) ) )
                        {
                            a2dChan0Snapshot.count = a2dProbe[0].count;
                            a2dChan0Snapshot.reading = a2dProbe[0].reading;
                            tempChan0 = a2dProbe[0].temp;
                            // tempChan0 = 0x1234;
                        }
                        if( lock_Check( &(a2dProbe[1].lock) ) )
                        {
                            a2dChan1Snapshot.count = a2dProbe[1].count;
                            a2dChan1Snapshot.reading = a2dProbe[1].reading;
                            tempChan1 = tempLookup( a2dProbe[1].reading );
                            tempChan1 = a2dProbe[1].temp;
                            // tempChan1 = 0x5678;
                        }
                        if( lock_Check( &(a2dProbe[2].lock) ) )
                        {
                            a2dChan2Snapshot.count = a2dProbe[2].count;
                            a2dChan2Snapshot.reading = a2dProbe[2].reading;
                            tempChan2 = tempLookup( a2dProbe[2].reading );
                            tempChan2 = a2dProbe[2].temp;
                            // tempChan1 = 0x5678;
                        }
                        if( lock_Check( &(a2dProbe[3].lock) ) )
                        {
                            a2dChan3Snapshot.count = a2dProbe[3].count;
                            a2dChan3Snapshot.reading = a2dProbe[3].reading;
                            tempChan3 = tempLookup( a2dProbe[3].reading );
                            tempChan3 = a2dProbe[3].temp;
                            // tempChan1 = 0x5678;
                        }
                        tempReadingCnt++;
                    break;
                    
                    case BEERCHIP_RELAY_ENABLE:
                        if( beermonState.state == beermon_state_extern_cntl )
                        {
                            relay_Switch( &enableRelay, i2cValue );
                        }
                    break;
                    case BEERCHIP_RELAY_CONTROL:
                        if( beermonState.state == beermon_state_extern_cntl )
                        {
                            relay_Switch( &controlRelay, i2cValue );
                        }
                    break;
                    
                    /* Stats */
                    case BEERCHIP_BEERMON_STATS_READ:
                        nowStats.onCnt = beermonStats.onCnt;
                        nowStats.offCnt = beermonStats.offCnt;
                        nowStats.onTime = usrStopwatch_GetTime( &(beermonStats.onTime) );
                        nowStats.offTime = usrStopwatch_GetTime( &(beermonStats.offTime) );
                        
                        statReadCnt++;
                    break;
                    
                    /* Control probe select */
                    case BEERCHIP_BEERMON_CFG_CNTL_PROBE:
                        if( i2cValue < BEERMON_NUM_TEMP_PROBES )
                        {
                            beermonCfg.probe = i2cValue;
                        }
                    break;
                    
                    case BEERCHIP_BEERMON_CNTL_CMD:
                        beermonStateControlMsg = i2cValue;
                    break;
                    
                    /* Configuration */
                    case BEERCHIP_BEERMON_CFG_SETPT:
                        *((uint8_t*)&workingBeermonCfg.setTemp + 0) = i2cValue;
                        beerCfgState = beerCfgDirty;
                    break;
                    case (BEERCHIP_BEERMON_CFG_SETPT + 1):
                        *((uint8_t*)&workingBeermonCfg.setTemp + 1) = i2cValue;
                        beerCfgState = beerCfgDirty;
                    break;
                    case (BEERCHIP_BEERMON_CFG_ON_DEBOUNCE_TIME):
                        *((uint8_t*)&workingBeermonCfg.onDebounceTime + 0) = i2cValue;
                        beerCfgState = beerCfgDirty;
                    break;
                    case (BEERCHIP_BEERMON_CFG_ON_DEBOUNCE_TIME + 1):
                        *((uint8_t*)&workingBeermonCfg.onDebounceTime + 1) = i2cValue;
                        beerCfgState = beerCfgDirty;
                    break;
                    case (BEERCHIP_BEERMON_CFG_OFF_DEBOUNCE_TIME):
                        *((uint8_t*)&workingBeermonCfg.offDebounceTime + 0) = i2cValue;
                        beerCfgState = beerCfgDirty;
                    break;
                    case (BEERCHIP_BEERMON_CFG_OFF_DEBOUNCE_TIME + 1):
                        *((uint8_t*)&workingBeermonCfg.offDebounceTime + 1) = i2cValue;
                        beerCfgState = beerCfgDirty;
                    break;
                    case BEERCHIP_BEERMON_CFG_UPDATE_STATE:
                        /* We write the new configuration */
                        if( (beerCfgState == beerCfgDirty) && (i2cValue == 0x00) )
                        {
                            workingBeermonCfg.seqNum++;
                            workingBeermonCfg.csum = 0x0000;
                            workingBeermonCfg.csum = beermonConfig_CalcCsum( &workingBeermonCfg );
                            memcpy( &beermonCfg, &workingBeermonCfg, sizeof(beermonConfig_t) );
                            beerCfgState = beerCfgClean;
                        }
                    break;
                    
                    /* Bloop detector 
                    ** Writing to this address ACKS a bloop
                    */
                    case BEERCHIP_BLOOPDET_STATE:
                        if( (i2cValue == bloopdet_event_reset) )
                        {
                            bloopDet_ProcessEvent( &bloopDetState, bloopdet_event_reset );
                        }
                        else
                        {
                            bloopDet_ProcessEvent( &bloopDetState, bloopdet_event_bloopack );
                        }
                    break;
                    
                    default:
                        /* Do Nothing */
                    break;
                }
            }

            if( i2cStatus.i2cStatusMasterRead )
            {
                switch( i2cIndex )
                {
                    /*
                    ** Version Information
                    */
                    case BEERCHIP_I2C_PROD_INDEX:
                        i2c_MasterReadI2CData( BEERCHIP_PRODUCT_ID );;
                    break;

                    case BEERCHIP_I2C_MAJ_VER_INDEX:
                        i2c_MasterReadI2CData( BEERCHIP_MAJOR_VERSION );
                    break;

                    case BEERCHIP_I2C_MIN_VER_INDEX:
                        i2c_MasterReadI2CData( BEERCHIP_MINOR_VERSION );
                    break;

                    case BEERCHIP_I2C_PROD_BUILD_INDEX:
                        i2c_MasterReadI2CData( BEERCHIP_BUILD );
                    break;

                    /* Up Time */
                    case BEERCHIP_I2C_UPTIME_BYTE3:
                        uptimeSnapshot = uptime;
                        i2c_MasterReadI2CData( (uint8_t)(uptimeSnapshot & 0xFF) );
                    break;
                    case BEERCHIP_I2C_UPTIME_BYTE2:
                        // i2c_MasterReadI2CData( (uint8_t)((uptimeSnapshot >> 8) & 0xFF) );
                        // i2c_MasterReadI2CData( 0x56 );
                        // temp = 0x56;
                        // temp = (uint8_t)((uptimeSnapshot & 0x0000FF00) >> 8);
                        // temp = *(((uint8_t *)&uptimeSnapshot) + 1);
                        i2c_MasterReadI2CData( *(((uint8_t *)&uptimeSnapshot) + 1) );
                    break;
                        
                    case BEERCHIP_I2C_UPTIME_BYTE1:
                        i2c_MasterReadI2CData( *(((uint8_t *)&uptimeSnapshot) + 2) );
                    break;
                    case BEERCHIP_I2C_UPTIME_BYTE0:
                        // i2c_MasterReadI2CData( (uptimeSnapshot >> (8 * (i2cIndex - BEERCHIP_I2C_UPTIME_BYTE3)) & 0xFF) );
                        i2c_MasterReadI2CData( *(((uint8_t *)&uptimeSnapshot) + 3) );
                    break;

                    /* LED Control */
                    case BEERCHIP_I2C_LED_MODE:
                        i2c_MasterReadI2CData( ledState.mode );
                    break;
                    case BEERCHIP_I2C_LED_CNT:
                        i2c_MasterReadI2CData( ledState.cnt );
                    break;
                    case BEERCHIP_I2C_LED_CNT_LIMIT:
                        i2c_MasterReadI2CData( ledState.cngCnt );
                    break;
                    case BEERCHIP_I2C_LED_CNT_OFF:
                        i2c_MasterReadI2CData( ledState.cngCnt );
                    break;
                    
                    /* 
                    ** A2D 
                    ** Chan 0
                    */
                    
                    case BEERCHIP_A2D_TRIGGER_READING:
                        i2c_MasterReadI2CData( tempReadingCnt );
                    break;
                    case BEERCHIP_A2D_CHAN0_TEMP_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)&(tempChan0) );
                    break;
                    case BEERCHIP_A2D_CHAN0_TEMP_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)&(tempChan0) + 1) );
                    break;
                    case BEERCHIP_A2D_CHAN0_READING_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)&(a2dChan0Snapshot.reading) );
                        // i2c_MasterReadI2CData( 0x12 );
                    break;
                    case BEERCHIP_A2D_CHAN0_READING_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)(&(a2dChan0Snapshot.reading)) + 1) );
                        // i2c_MasterReadI2CData( 0x34 );
                    break;
                    case BEERCHIP_A2D_CHAN0_COUNT_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)(&(a2dChan0Snapshot.count)) );
                        // i2c_MasterReadI2CData( 0x56 );
                    break;
                    case BEERCHIP_A2D_CHAN0_COUNT_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)(&(a2dChan0Snapshot.count)) + 1) );
                        // i2c_MasterReadI2CData( 0x78 );
                    break;
                    
                    /* Chan 1 */  
                    case BEERCHIP_A2D_CHAN1_TEMP_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)&(tempChan1) );
                    break;
                    case BEERCHIP_A2D_CHAN1_TEMP_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)&(tempChan1) + 1) );
                    break;
                    case BEERCHIP_A2D_CHAN1_READING_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)&(a2dChan1Snapshot.reading) );
                        // i2c_MasterReadI2CData( 0x12 );
                    break;
                    case BEERCHIP_A2D_CHAN1_READING_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)(&(a2dChan1Snapshot.reading)) + 1) );
                        // i2c_MasterReadI2CData( 0x34 );
                    break;
                    case BEERCHIP_A2D_CHAN1_COUNT_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)(&(a2dChan1Snapshot.count)) );
                        // i2c_MasterReadI2CData( 0x56 );
                    break;
                    case BEERCHIP_A2D_CHAN1_COUNT_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)(&(a2dChan1Snapshot.count)) + 1) );
                        // i2c_MasterReadI2CData( 0x78 );
                    break;
                    
                    /* Chan 2 */  
                    case BEERCHIP_A2D_CHAN2_TEMP_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)&(tempChan2) );
                    break;
                    case BEERCHIP_A2D_CHAN2_TEMP_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)&(tempChan2) + 1) );
                    break;
                    case BEERCHIP_A2D_CHAN2_READING_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)&(a2dChan2Snapshot.reading) );
                        // i2c_MasterReadI2CData( 0x12 );
                    break;
                    case BEERCHIP_A2D_CHAN2_READING_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)(&(a2dChan2Snapshot.reading)) + 1) );
                        // i2c_MasterReadI2CData( 0x34 );
                    break;
                    case BEERCHIP_A2D_CHAN2_COUNT_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)(&(a2dChan2Snapshot.count)) );
                        // i2c_MasterReadI2CData( 0x56 );
                    break;
                    case BEERCHIP_A2D_CHAN2_COUNT_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)(&(a2dChan2Snapshot.count)) + 1) );
                        // i2c_MasterReadI2CData( 0x78 );
                    break;
                    
                    /* Chan 3 */  
                    case BEERCHIP_A2D_CHAN3_TEMP_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)&(tempChan3) );
                    break;
                    case BEERCHIP_A2D_CHAN3_TEMP_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)&(tempChan3) + 1) );
                    break;
                    case BEERCHIP_A2D_CHAN3_READING_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)&(a2dChan3Snapshot.reading) );
                        // i2c_MasterReadI2CData( 0x12 );
                    break;
                    case BEERCHIP_A2D_CHAN3_READING_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)(&(a2dChan3Snapshot.reading)) + 1) );
                        // i2c_MasterReadI2CData( 0x34 );
                    break;
                    case BEERCHIP_A2D_CHAN3_COUNT_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)(&(a2dChan3Snapshot.count)) );
                        // i2c_MasterReadI2CData( 0x56 );
                    break;
                    case BEERCHIP_A2D_CHAN3_COUNT_BYTE1:
                        i2c_MasterReadI2CData( *((uint8_t *)(&(a2dChan3Snapshot.count)) + 1) );
                        // i2c_MasterReadI2CData( 0x78 );
                    break;
                    
                    /*
                    ** Relays 
                    */
                    case BEERCHIP_RELAY_ENABLE:
                        i2c_MasterReadI2CData( enableRelay.state );
                    break;
                    case BEERCHIP_RELAY_CONTROL:
                        i2c_MasterReadI2CData( controlRelay.state );
                    break;
                    
                    /*
                    ** Beermon Config
                    */
                    
                    case BEERCHIP_BEERMON_CFG_SETPT:
                        if( beerCfgState == beerCfgClean )
                        {
                            memcpy( &workingBeermonCfg, &beermonCfg, sizeof(beermonConfig_t) );
                        }
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.setTemp + 0) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_SETPT + 1):
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.setTemp + 1) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_ON_DEBOUNCE_TIME):
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.onDebounceTime + 0) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_ON_DEBOUNCE_TIME + 1):
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.onDebounceTime + 1) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_OFF_DEBOUNCE_TIME):
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.offDebounceTime + 0) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_OFF_DEBOUNCE_TIME + 1):
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.offDebounceTime + 1) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_SEQNUM):
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.seqNum + 0) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_SEQNUM + 1):
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.seqNum + 1) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_CSUM):
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.csum + 0) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_CSUM + 1):
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.csum + 1) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_CNTL_PROBE):
                        i2c_MasterReadI2CData( *((uint8_t*)&workingBeermonCfg.probe) );
                    break;
                    case (BEERCHIP_BEERMON_CFG_UPDATE_STATE):
                        i2c_MasterReadI2CData( beerCfgState );
                    break;
                    
                    /* Stats */
                    case BEERCHIP_BEERMON_STATS_READ:
                        i2c_MasterReadI2CData( statReadCnt );
                    break;
                    case BEERCHIP_BEERMON_STATS_ON_CNT:
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.onCnt + 0) );
                    break;
                    case (BEERCHIP_BEERMON_STATS_ON_CNT + 1):
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.onCnt + 1) );
                    break;
                    case BEERCHIP_BEERMON_STATS_OFF_CNT:
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.offCnt + 0) );
                    break;
                    case (BEERCHIP_BEERMON_STATS_OFF_CNT + 1):
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.offCnt + 1) );
                    break;
                    case (BEERCHIP_BEERMON_STATS_ON_TIME):
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.onTime + 0) );
                    break;
                    case (BEERCHIP_BEERMON_STATS_ON_TIME + 1):
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.onTime + 1) );
                    break;
                    case (BEERCHIP_BEERMON_STATS_ON_TIME + 2):
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.onTime + 2) );
                    break;
                    case (BEERCHIP_BEERMON_STATS_ON_TIME + 3):
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.onTime + 3) );
                    break;
                    case (BEERCHIP_BEERMON_STATS_OFF_TIME):
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.offTime + 0) );
                    break;
                    case (BEERCHIP_BEERMON_STATS_OFF_TIME + 1):
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.offTime + 1) );
                    break;
                    case (BEERCHIP_BEERMON_STATS_OFF_TIME + 2):
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.offTime + 2) );
                    break;
                    case (BEERCHIP_BEERMON_STATS_OFF_TIME + 3):
                        i2c_MasterReadI2CData( *((uint8_t*)&nowStats.offTime + 3) );
                    break;
                    
                    /* Beemon State */
                    case (BEERCHIP_BEERMON_STATE_STATE):
                        i2c_MasterReadI2CData( beermonState.state );
                    break;
                    
                    case (BEERCHIP_BEERMON_CNTL_CMD):
                        i2c_MasterReadI2CData( beermonStateControlMsg );
                    break;
                    
                    /*
                    ** Bloop detector
                    */
                    case BEERCHIP_BLOOPDET_STATE:
                        i2c_MasterReadI2CData( (uint8_t)bloopDetState.state );
                    break;
                    
                    case BEERCHIP_BLOOPDET_CNT:
                        i2c_MasterReadI2CData( *((uint8_t *)&(bloopDetState.bloopCnt)) );
                    break;
                    case (BEERCHIP_BLOOPDET_CNT + 1):
                        i2c_MasterReadI2CData( *(((uint8_t *)&(bloopDetState.bloopCnt)) + 1) );
                    break;
                    
                    case BEERCHIP_BLOOPDET_ACK:
                        i2c_MasterReadI2CData( *((uint8_t *)&(bloopDetState.bloopAckCnt)) );
                    break;
                    case (BEERCHIP_BLOOPDET_ACK + 1):
                        i2c_MasterReadI2CData( *(((uint8_t *)&(bloopDetState.bloopAckCnt)) + 1) );
                    break;
                    
                    
                    /*
                    ** Test stuff
                    */
#if 0
                    case (BEERCHIP_TEST_START + 1):
                        i2c_MasterReadI2CData( 0x01 );
                    break;
                    case (BEERCHIP_TEST_START + 2):
                        i2c_MasterReadI2CData( 0x02 );
                    break;
                    case (BEERCHIP_TEST_START + 3):
                        i2c_MasterReadI2CData( 0x03);
                    break;
                    case (BEERCHIP_TEST_START + 4):
                        i2c_MasterReadI2CData( 0x04 );
                    break;
                    case (BEERCHIP_TEST_START + 5):
                        i2c_MasterReadI2CData( 0x05 );
                    break;
                    case (BEERCHIP_TEST_START + 6):
                        i2c_MasterReadI2CData( 0x06 );
                    break;
                    case (BEERCHIP_TEST_START + 7):
                        i2c_MasterReadI2CData( 0x07 );
                    break;
                    case (BEERCHIP_TEST_START + 8):
                        i2c_MasterReadI2CData( 0x08 );
                    break;
                    case (BEERCHIP_TEST_START + 9):
                        i2c_MasterReadI2CData( 0x09 );
                    break;
                    case (BEERCHIP_TEST_START + 10):
                        i2c_MasterReadI2CData( 0x0A );
                    break;
                    case (BEERCHIP_TEST_START + 11):
                        i2c_MasterReadI2CData( 0x0B );
                    break;
                    case (BEERCHIP_TEST_START + 12):
                        i2c_MasterReadI2CData( 0x0C );
                    break;
                    case (BEERCHIP_TEST_START + 13):
                        i2c_MasterReadI2CData( 0x0D );
                    break;
                    
                    
                    case (BEERCHIP_TEST_START + 14):
                        i2c_MasterReadI2CData( 0x0E );
                    break;
                    case (BEERCHIP_TEST_START + 15):
                        i2c_MasterReadI2CData( 0x0F );
                    break;
                    case (BEERCHIP_TEST_START + 16):
                        i2c_MasterReadI2CData( 0x10 );
                    break;
                    case (BEERCHIP_TEST_START + 17):
                        i2c_MasterReadI2CData( 0x11 );
                    break;
                    case (BEERCHIP_TEST_START + 18):
                        i2c_MasterReadI2CData( 0x12 );
                    break;
                    case (BEERCHIP_TEST_START + 19):
                        i2c_MasterReadI2CData( 0x13 );
                    break;
                    case (BEERCHIP_TEST_START + 20):
                        i2c_MasterReadI2CData( 0x14 );
                    break;
                    case (BEERCHIP_TEST_START + 21):
                        i2c_MasterReadI2CData( 0x15 );
                    break;
                    case (BEERCHIP_TEST_START + 22):
                        i2c_MasterReadI2CData( 0x16 );
                    break;
                    case (BEERCHIP_TEST_START + 23):
                        i2c_MasterReadI2CData( 0x17 );
                    break;
                    case (BEERCHIP_TEST_START + 24):
                        i2c_MasterReadI2CData( 0x18 );
                    break;
                    case (BEERCHIP_TEST_START + 25):
                        i2c_MasterReadI2CData( 0x19 );
                    break;
                    case (BEERCHIP_TEST_START + 26):
                        i2c_MasterReadI2CData( 0x1A );
                    break;
                    case (BEERCHIP_TEST_START + 27):
                        i2c_MasterReadI2CData( 0x1B );
                    break;
                    case (BEERCHIP_TEST_START + 28):
                        i2c_MasterReadI2CData( 0x1C );
                    break;
                    case (BEERCHIP_TEST_START + 29):
                        i2c_MasterReadI2CData( 0x1D );
                    break;
                    case (BEERCHIP_TEST_START + 30):
                        i2c_MasterReadI2CData( 0x1E );
                    break;
                    case (BEERCHIP_TEST_START + 31):
                        i2c_MasterReadI2CData( 0x1F );
                    break;
                    case (BEERCHIP_TEST_START + 32):
                        i2c_MasterReadI2CData( 0x20 );
                    break;
                    case (BEERCHIP_TEST_START + 33):
                        i2c_MasterReadI2CData( 0x21 );
                    break;
                    case (BEERCHIP_TEST_START + 34):
                        i2c_MasterReadI2CData( 0x22 );
                    break;
                    case (BEERCHIP_TEST_START + 35):
                        i2c_MasterReadI2CData( 0x23 );
                    break;
                    case (BEERCHIP_TEST_START + 36):
                        i2c_MasterReadI2CData( 0x24 );
                    break;
#endif
                    
                    default:
                        i2c_MasterReadI2CData( BEERCHIP_I2C_CLEAR_CHAR );
                    break;
                    
                    
                    
                }
            }
         } while( SSP1IF );
    }

    if( BCL1IF )
    {
        /* I2C bus collision */
        do {
            BCL1IF = 0;

            i2c_BusColI2CSlave();
        } while( BCL1IF );

    }
    

}


