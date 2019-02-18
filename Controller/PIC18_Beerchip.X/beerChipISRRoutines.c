
#include <string.h> /* For memcpy */

#include "mcc_generated_files/i2c1.h"

#include "beerChipLed.h"
#include "beerChipConfig.h"
#include "beerLock.h"
#include "mcc_generated_files/adc.h"
#include "beerChipA2D.h"
#include "beerChipI2CIndex.h"
#include "beerChipISRRoutines.h"
#include "beerChipRelay.h"
#include "beerChipBeermon.h"


extern a2d_Reading_t a2dProbe[BEERMON_NUM_TEMP_PROBES];

extern beerchip_relay_t enableRelay;
extern beerchip_relay_t controlRelay;

uint32_t uptime = 0x00000000;
static uint8_t  secTickCnt = 0x00;
static int test;

extern a2d_Reading_t a2dProbe[BEERMON_NUM_TEMP_PROBES];
static int16_t tempChan[BEERMON_NUM_TEMP_PROBES]; 
static struct {
        uint16_t reading;
        uint16_t count;
} a2dChanSnapshot[BEERMON_NUM_TEMP_PROBES];
static uint8_t tempReadingCnt;

extern beermonConfig_t beermonCfg;
static beermonConfig_t workingBeermonCfg;

extern beermonStats_t beermonStats;
struct {
    uint16_t onCnt;
    uint16_t offCnt;
    uint32_t onTime;
    uint32_t offTime;
} nowStats;
static uint8_t statReadCnt;
extern beermonState_t beermonState;

static uint8_t beerCfgState = beerCfgClean;

/* Command Register */
extern uint8_t beermonStateControlMsg;

void beerChipTimerISR( void )
{
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

}

uint8_t beerChipI2CRead( uint8_t i2cIndex )
{
    uint8_t i2cValue;
    static uint32_t uptimeSnapshot;
    
    switch( i2cIndex )
    {
#if 1
        /* Version info */
        case BEERCHIP_I2C_PROD_INDEX:
            i2cValue = BEERCHIP_PRODUCT_ID;
        break;
        case BEERCHIP_I2C_MAJ_VER_INDEX:
            i2cValue = BEERCHIP_MAJOR_VERSION;
        break;
        case BEERCHIP_I2C_MIN_VER_INDEX:
            i2cValue = BEERCHIP_MINOR_VERSION;
        break;
        case BEERCHIP_I2C_PROD_BUILD_INDEX:
            i2cValue = BEERCHIP_BUILD;
        break;
       
        /* Uptime */
        case BEERCHIP_I2C_UPTIME_BYTE0: /* MSB */
            i2cValue = *(((uint8_t *)&uptimeSnapshot) + 3);
        break;
        case BEERCHIP_I2C_UPTIME_BYTE1: 
            i2cValue = *(((uint8_t *)&uptimeSnapshot) + 2);
        break;
        case BEERCHIP_I2C_UPTIME_BYTE2: 
            i2cValue = *(((uint8_t *)&uptimeSnapshot) + 1);
        break;
        case BEERCHIP_I2C_UPTIME_BYTE3: /* LSB */
            uptimeSnapshot = uptime;
            i2cValue = (uint8_t)(uptimeSnapshot & 0xFF);
        break;
        
        /* LED Control */
        case BEERCHIP_I2C_LED_MODE:
            i2cValue = ledState.mode;
        break;
        case BEERCHIP_I2C_LED_CNT:
            i2cValue = ledState.cnt;
        break;
        case BEERCHIP_I2C_LED_CNT_LIMIT:
            i2cValue = ledState.cngCnt;
        break; 
        
        /* A/D Chan 0 */
        case BEERCHIP_A2D_TRIGGER_READING:
            i2cValue = tempReadingCnt;
        break;
        
        case BEERCHIP_A2D_CHAN0_TEMP_BYTE0:
            i2cValue = *(uint8_t *)&(tempChan[0]);
        break;
        case BEERCHIP_A2D_CHAN0_TEMP_BYTE1:
            i2cValue = *((uint8_t *)&(tempChan[0]) + 1);
        break;
        case BEERCHIP_A2D_CHAN0_READING_BYTE0:
            i2cValue = *(uint8_t *)&(a2dChanSnapshot[0].reading);
        break;
        case BEERCHIP_A2D_CHAN0_READING_BYTE1:
            i2cValue = *((uint8_t *)(&(a2dChanSnapshot[0].reading)) + 1);
        break;
        case BEERCHIP_A2D_CHAN0_COUNT_BYTE0:
            i2cValue = *(uint8_t *)(&(a2dChanSnapshot[0].count));
        break;
        case BEERCHIP_A2D_CHAN0_COUNT_BYTE1:
            i2cValue = *((uint8_t *)(&(a2dChanSnapshot[0].count)) + 1);
        break;
        
        /* A/D Chan 1 */
        case BEERCHIP_A2D_CHAN1_TEMP_BYTE0:
            i2cValue = *(uint8_t *)&(tempChan[1]);
        break;
        case BEERCHIP_A2D_CHAN1_TEMP_BYTE1:
            i2cValue = *((uint8_t *)&(tempChan[1]) + 1);
        break;
        case BEERCHIP_A2D_CHAN1_READING_BYTE0:
            i2cValue = *(uint8_t *)&(a2dChanSnapshot[1].reading);
        break;
        case BEERCHIP_A2D_CHAN1_READING_BYTE1:
            i2cValue = *((uint8_t *)(&(a2dChanSnapshot[1].reading)) + 1);
        break;
        case BEERCHIP_A2D_CHAN1_COUNT_BYTE0:
            i2cValue = *(uint8_t *)(&(a2dChanSnapshot[1].count));
        break;
        case BEERCHIP_A2D_CHAN1_COUNT_BYTE1:
            i2cValue = *((uint8_t *)(&(a2dChanSnapshot[1].count)) + 1);
        break;
        
        /* Relays */
        case BEERCHIP_RELAY_ENABLE:
            i2cValue = enableRelay.state;
        break;
        case BEERCHIP_RELAY_CONTROL:
            i2cValue = controlRelay.state;
        break;
        
        /* Beermon config */
        case BEERCHIP_BEERMON_CFG_SETPT:
            if( beerCfgState == beerCfgClean )
            {
                memcpy( &workingBeermonCfg, &beermonCfg, sizeof(beermonConfig_t) );
            }
            i2cValue = *((uint8_t*)&workingBeermonCfg.setTemp + 0);
        break;
        case (BEERCHIP_BEERMON_CFG_SETPT + 1):
            i2cValue = *((uint8_t*)&workingBeermonCfg.setTemp + 1);
        break;
        case (BEERCHIP_BEERMON_CFG_ON_DEBOUNCE_TIME):
            i2cValue = *((uint8_t*)&workingBeermonCfg.onDebounceTime + 0);
        break;
        case (BEERCHIP_BEERMON_CFG_ON_DEBOUNCE_TIME + 1):
            i2cValue = *((uint8_t*)&workingBeermonCfg.onDebounceTime + 1);
        break;
        case (BEERCHIP_BEERMON_CFG_OFF_DEBOUNCE_TIME):
            i2cValue = *((uint8_t*)&workingBeermonCfg.offDebounceTime + 0);
        break;
        case (BEERCHIP_BEERMON_CFG_OFF_DEBOUNCE_TIME + 1):
            i2cValue = *((uint8_t*)&workingBeermonCfg.offDebounceTime + 1);
        break;
        case (BEERCHIP_BEERMON_CFG_SEQNUM):
            i2cValue = *((uint8_t*)&workingBeermonCfg.seqNum + 0);
        break;
        case (BEERCHIP_BEERMON_CFG_SEQNUM + 1):
            i2cValue = *((uint8_t*)&workingBeermonCfg.seqNum + 1);
        break;
        case (BEERCHIP_BEERMON_CFG_CSUM):
            i2cValue = *((uint8_t*)&workingBeermonCfg.csum + 0);
        break;
        case (BEERCHIP_BEERMON_CFG_CSUM + 1):
            i2cValue = *((uint8_t*)&workingBeermonCfg.csum + 1);
        break;
        case (BEERCHIP_BEERMON_CFG_CNTL_PROBE):
            i2cValue = *((uint8_t*)&workingBeermonCfg.probe);
        break;
        case (BEERCHIP_BEERMON_CFG_UPDATE_STATE):
            i2cValue = beerCfgState;
        break;
        
        /* Stats */
        case BEERCHIP_BEERMON_STATS_READ:
            i2cValue = statReadCnt;
        break;
        case BEERCHIP_BEERMON_STATS_ON_CNT:
            i2cValue = *((uint8_t*)&nowStats.onCnt + 0);
        break;
        case (BEERCHIP_BEERMON_STATS_ON_CNT + 1):
            i2cValue = *((uint8_t*)&nowStats.onCnt + 1);
        break;
        case BEERCHIP_BEERMON_STATS_OFF_CNT:
            i2cValue = *((uint8_t*)&nowStats.offCnt + 0);
        break;
        case (BEERCHIP_BEERMON_STATS_OFF_CNT + 1):
            i2cValue = *((uint8_t*)&nowStats.offCnt + 1);
        break;
        case (BEERCHIP_BEERMON_STATS_ON_TIME):
            i2cValue = *((uint8_t*)&nowStats.onTime + 0);
        break;
        case (BEERCHIP_BEERMON_STATS_ON_TIME + 1):
            i2cValue = *((uint8_t*)&nowStats.onTime + 1);
        break;
        case (BEERCHIP_BEERMON_STATS_ON_TIME + 2):
            i2cValue = *((uint8_t*)&nowStats.onTime + 2);
        break;
        case (BEERCHIP_BEERMON_STATS_ON_TIME + 3):
            i2cValue = *((uint8_t*)&nowStats.onTime + 3);
        break;
        case (BEERCHIP_BEERMON_STATS_OFF_TIME):
            i2cValue = *((uint8_t*)&nowStats.offTime + 0);
        break;
        case (BEERCHIP_BEERMON_STATS_OFF_TIME + 1):
            i2cValue = *((uint8_t*)&nowStats.offTime + 1);
        break;
        case (BEERCHIP_BEERMON_STATS_OFF_TIME + 2):
            i2cValue = *((uint8_t*)&nowStats.offTime + 2);
        break;
        case (BEERCHIP_BEERMON_STATS_OFF_TIME + 3):
            i2cValue = *((uint8_t*)&nowStats.offTime + 3);
        break;
        
        /* Beermon State */
        case (BEERCHIP_BEERMON_STATE_STATE):
            i2cValue = beermonState.state;
        break;

        case (BEERCHIP_BEERMON_CNTL_CMD):
            i2cValue = beermonStateControlMsg;
        break;
        
#endif
        
        default:
            i2cValue = 0xBB;
        break;
    }

    return i2cValue;
}

void beerChipI2CWrite( uint8_t i2cIndex, uint8_t i2cValue )
{
    uint8_t chan;
    switch( i2cIndex )
    {
        /* LED Control */
        case BEERCHIP_I2C_LED_MODE:
            beerChip_SetLEDMode( i2cValue, ledState.cngCnt );
        break;
        case BEERCHIP_I2C_LED_CNT_LIMIT:
            beerChip_SetLEDMode( ledState.mode, i2cValue );
        break;
        
        /* Trigger A/D Reading */
        case BEERCHIP_A2D_TRIGGER_READING:
            for( chan = 0; chan < BEERMON_NUM_TEMP_PROBES; chan++ )
            {
                if( lock_Check( &(a2dProbe[chan].lock) ) )
                {
                    a2dChanSnapshot[chan].count = a2dProbe[chan].count;
                    a2dChanSnapshot[chan].reading = a2dProbe[chan].reading;
                    tempChan[chan] = a2dProbe[chan].temp;
                }
            }            
            tempReadingCnt++;
        break;
        
        /* Relay Control */
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
        
        /* Control probe select */
        case BEERCHIP_BEERMON_CFG_CNTL_PROBE:
            if( i2cValue < BEERMON_NUM_TEMP_PROBES )
            {
                beermonCfg.probe = i2cValue;
            }
        break;

        /* Beermon State Machine Control Message */
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
        
        /* Stats */
        case BEERCHIP_BEERMON_STATS_READ:
            nowStats.onCnt = beermonStats.onCnt;
            nowStats.offCnt = beermonStats.offCnt;
            nowStats.onTime = usrStopwatch_GetTime( &(beermonStats.onTime) );
            nowStats.offTime = usrStopwatch_GetTime( &(beermonStats.offTime) );

            statReadCnt++;
        break;
        
        default:
            /* Do nothing */
        break;
        
    }
}

void beerChipI2CHandler(I2C1_SLAVE_DRIVER_STATUS i2c_bus_state)
{
    static uint8_t EEPROM_Buffer[] =
    {
        0xB0,0xBB,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
        0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
        0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
        0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
        0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
        0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
        0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f
    };

    static uint8_t eepromAddress    = 0;
    static uint8_t slaveWriteType   = SLAVE_NORMAL_DATA;


    switch (i2c_bus_state)
    {
        case I2C1_SLAVE_WRITE_REQUEST:
            // the master will be sending the eeprom address next
            slaveWriteType  = SLAVE_DATA_ADDRESS;
            break;


        case I2C1_SLAVE_WRITE_COMPLETED:

            switch(slaveWriteType)
            {
                case SLAVE_DATA_ADDRESS:
                    eepromAddress   = I2C1_slaveWriteData;
                    break;


                case SLAVE_NORMAL_DATA:
                default:
                    // the master has written data to store in the eeprom
                    beerChipI2CWrite( eepromAddress++, I2C1_slaveWriteData );
                    
                break;

            } // end switch(slaveWriteType)

            slaveWriteType  = SLAVE_NORMAL_DATA;
            break;

        case I2C1_SLAVE_READ_REQUEST:
            SSPBUF = beerChipI2CRead( eepromAddress++ );
            break;

        case I2C1_SLAVE_READ_COMPLETED:
        default:;

    } // end switch(i2c_bus_state)

}
