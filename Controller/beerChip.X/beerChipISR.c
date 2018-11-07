#include <stdint.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipI2C.h"
#include "beerChipI2CIndex.h"
#include "beerChipLed.h"
#include "beerLock.h"
#include "beerChipA2D.h"

static uint32_t uptime = 0x00000000;
static uint8_t  secTickCnt = 0x00;

extern a2d_Reading_t a2dChan0;
extern a2d_Reading_t a2dChan1;

void __interrupt () ISR( void )
{
    uint8_t i2cIndex;
    uint8_t i2cValue;
    i2c_i2cStatus i2cStatus;

    static uint32_t uptimeSnapshot;
    
    static struct {
        uint16_t reading;
        uint16_t count;
    } a2dChan0Snapshot, a2dChan1Snapshot;

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
                    
                    /* A2D */
                    case BEERCHIP_A2D_CHAN0_READING_BYTE0:
                        if( lock_Take( &(a2dChan0.lock) ) )
                        {
                            a2dChan0Snapshot.count = a2dChan0.count;
                            a2dChan0Snapshot.reading = a2dChan0.reading;
                        }
                        i2c_MasterReadI2CData( *(uint8_t *)&(a2dChan0Snapshot.reading) );
                    break;
                    case BEERCHIP_A2D_CHAN0_READING_BYTE1:
                        i2c_MasterReadI2CData( *(uint8_t *)(&(a2dChan0Snapshot.reading) + 1) );
                    break;
                    case BEERCHIP_A2D_CHAN0_COUNT_BYTE0:
                        i2c_MasterReadI2CData( *(uint8_t *)(&(a2dChan0Snapshot.count)) );
                    break;
                    case BEERCHIP_A2D_CHAN0_COUNT_BYTE1:
                        i2c_MasterReadI2CData( *(uint8_t *)(&(a2dChan0Snapshot.count) + 1) );
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


