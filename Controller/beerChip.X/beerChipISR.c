#include <stdint.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipI2C.h"
#include "beerChipI2CIndex.h"
#include "beerChipLed.h"

#define HBRDG_PROC_LSB  0x01
#define HBRDG_PROC_MSB  0x02

static uint8_t pwmRdCnt;
static uint16_t pwmRdValue;

static uint8_t encodRdCnt;
static uint32_t encodPos;
static uint16_t encodVel;
#ifdef HBRDG_ENCOD_TRACK_ERRORS
static uint16_t encodErr;
#endif

static uint8_t cnts[3];

void interrupt ISR( void )
{
    uint8_t i2cIndex;
    uint8_t i2cValue;
    i2c_i2cStatus i2cStatus;


    if( TMR1IF )
    {
        beerChip_KickLED();

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


