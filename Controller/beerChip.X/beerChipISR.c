#include <stdint.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipI2C.h"
#include "beerChipI2CIndex.h"
#include "beerChipLed.h"
#include "beerLock.h"
#include "beerChipA2D.h"
#include "beerChipTempLookup.h"
#include "beerChipRelay.h"

uint32_t uptime = 0x00000000;
static uint8_t  secTickCnt = 0x00;

extern a2d_Reading_t a2dChan0;
static int16_t tempChan0; 
extern a2d_Reading_t a2dChan1;
static int16_t tempChan1;
static uint8_t tempReadingCnt;

extern beerchip_relay_t enableRelay;
extern beerchip_relay_t controlRelay;

void __interrupt () ISR( void )
// void interrupt ISR( void )
{
    uint8_t i2cIndex;
    uint8_t i2cValue;
    i2c_i2cStatus i2cStatus;

    static uint32_t uptimeSnapshot;
    
    static struct {
        uint16_t reading;
        uint16_t count;
    } a2dChan0Snapshot, a2dChan1Snapshot;
    
    // a2dChan0Snapshot.count = 0x89ab;
    // a2dChan0Snapshot.reading = 0xcdef;

    if( TMR1IF )
    {
        beerChip_KickLED();

        /* Do time tick */
        if( secTickCnt++ == BEERCHIP_TICKS_PER_SECOND )
        {
            /* This should happen once per second */
            uptime++;
            secTickCnt = 0x00;
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
                        beerChip_SetLEDMode( i2cValue, ledState.cngCnt );
                    break;

                    case BEERCHIP_I2C_LED_CNT_LIMIT:
                        beerChip_SetLEDMode( ledState.mode, i2cValue );
                    break;
                    
                    case BEERCHIP_A2D_TRIGGER_READING:
                        if( lock_Check( &(a2dChan0.lock) ) )
                        {
                            a2dChan0Snapshot.count = a2dChan0.count;
                            a2dChan0Snapshot.reading = a2dChan0.reading;
                            tempChan0 = tempLookup( a2dChan0.reading );
                            // tempChan0 = 0x1234;
                        }
                        if( lock_Check( &(a2dChan1.lock) ) )
                        {
                            a2dChan1Snapshot.count = a2dChan1.count;
                            a2dChan1Snapshot.reading = a2dChan1.reading;
                            tempChan1 = tempLookup( a2dChan1.reading );
                            // tempChan1 = 0x5678;
                        }
                        tempReadingCnt++;
                    break;
                    
                    case BEERCHIP_RELAY_ENABLE:
                        relay_Switch( &enableRelay, i2cValue );
                    break;
                    case BEERCHIP_RELAY_CONTROL:
                        relay_Switch( &controlRelay, i2cValue );
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
                    ** Test stuff
                    */
                    
                    case (BEERCHIP_RELAY_CONTROL + 1):
                        i2c_MasterReadI2CData( 0x01 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 2):
                        i2c_MasterReadI2CData( 0x02 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 3):
                        i2c_MasterReadI2CData( 0x03);
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 4):
                        i2c_MasterReadI2CData( 0x04 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 5):
                        i2c_MasterReadI2CData( 0x05 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 6):
                        i2c_MasterReadI2CData( 0x06 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 7):
                        i2c_MasterReadI2CData( 0x07 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 8):
                        i2c_MasterReadI2CData( 0x08 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 9):
                        i2c_MasterReadI2CData( 0x09 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 10):
                        i2c_MasterReadI2CData( 0x0A );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 11):
                        i2c_MasterReadI2CData( 0x0B );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 12):
                        i2c_MasterReadI2CData( 0x0C );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 13):
                        i2c_MasterReadI2CData( 0x0D );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 14):
                        i2c_MasterReadI2CData( 0x0E );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 15):
                        i2c_MasterReadI2CData( 0x0F );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 16):
                        i2c_MasterReadI2CData( 0x10 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 17):
                        i2c_MasterReadI2CData( 0x11 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 18):
                        i2c_MasterReadI2CData( 0x12 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 19):
                        i2c_MasterReadI2CData( 0x13 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 20):
                        i2c_MasterReadI2CData( 0x14 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 21):
                        i2c_MasterReadI2CData( 0x15 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 22):
                        i2c_MasterReadI2CData( 0x16 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 23):
                        i2c_MasterReadI2CData( 0x17 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 24):
                        i2c_MasterReadI2CData( 0x18 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 25):
                        i2c_MasterReadI2CData( 0x19 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 26):
                        i2c_MasterReadI2CData( 0x1A );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 27):
                        i2c_MasterReadI2CData( 0x1B );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 28):
                        i2c_MasterReadI2CData( 0x1C );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 29):
                        i2c_MasterReadI2CData( 0x1D );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 30):
                        i2c_MasterReadI2CData( 0x1E );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 31):
                        i2c_MasterReadI2CData( 0x1F );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 32):
                        i2c_MasterReadI2CData( 0x20 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 33):
                        i2c_MasterReadI2CData( 0x21 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 34):
                        i2c_MasterReadI2CData( 0x22 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 35):
                        i2c_MasterReadI2CData( 0x23 );
                    break;
                    case (BEERCHIP_RELAY_CONTROL + 36):
                        i2c_MasterReadI2CData( 0x24 );
                    break;
                    
                    
                    
                    
                    
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


