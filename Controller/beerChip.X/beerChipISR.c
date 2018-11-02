#include <stdint.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipI2C.h"
#include "beerChipI2CIndex.h"
#include "beerChipLed.h"

static uint32_t uptime = 0x0000;
static uint8_t  secTickCnt = 0x00;

void __interrupt () ISR( void )
{
    uint8_t i2cIndex;
    uint8_t i2cValue;
    i2c_i2cStatus i2cStatus;

    static uint32_t uptimeSnapshot;


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
                    case BEERCHIP_I2C_UPTIME_BYTE2:
                    case BEERCHIP_I2C_UPTIME_BYTE1:
                    case BEERCHIP_I2C_UPTIME_BYTE0:
                        i2c_MasterReadI2CData( (uptimeSnapshot >> (8 * (i2cIndex - BEERCHIP_I2C_UPTIME_BYTE3)) & 0xFF) );
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


