/*******************************************************************************
**
** Filename: beerChipI2C.c
**
** Purpose: I2C driver for the beermon refrigerator controller
**
** References:
**   1 AN734 - Using the PIC Micro for Slave I2C Communications
**
********************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipI2C.h"


static hbrdg_i2cInfo_t i2cInfo;

void i2c_InitI2CSlave( uint8_t addr )
{

    /* Clear SDA/SCL */
    BEERCHIP_I2C_PORT &= ~(BEERCHIP_I2C_PIN_SDA | BEERCHIP_I2C_PIN_SCL);
    BEERCHIP_I2C_LAT &= ~(BEERCHIP_I2C_PIN_SDA | BEERCHIP_I2C_PIN_SCL);
    BEERCHIP_I2C_ANSEL &= ~(BEERCHIP_I2C_PIN_SDA | BEERCHIP_I2C_PIN_SCL);
    /* Set SDA/SCL as Input */
    BEERCHIP_I2C_TRIS = (BEERCHIP_I2C_PIN_SDA | BEERCHIP_I2C_PIN_SCL);

    SSPSTAT = (1 << _SSP1STAT_SMP_POSN) | /* Slew Rate Cntl Disabled */
              (0 << _SSP1STAT_CKE_POSN);  /* Disable SMBus input level */

    SSPCON1 = (0 << _SSP1CON1_WCOL_POSN)  | /* Clear Col Detect */
              (0 << _SSP1CON1_SSPOV_POSN) | /* Clear Overflow bit */
              (1 << _SSP1CON1_SSPEN_POSN) | /* Enable I2C */
              (1 << _SSP1CON1_CKP_POSN)   | /* Enable Clk */
              (0x6 << _SSP1CON1_SSPM_POSN); /* 7-bit slave mode */

    SSPCON2 |= (1 << _SSP1CON2_SEN_POSN); /* Enable clock streaching */

    SSPCON3 |= (1 << _SSP1CON3_BOEN_POSN)  |
               (1 << _SSP1CON3_SDAHT_POSN) | /* SDA holdtime = 300ns min */
               (1 << _SSP1CON3_SBCDE_POSN);  /* Enable slave bus collision detect */

    /* Set the address */
    SSPADD = addr << 1;

    SSP1IF = 0; /* Clear serial port interrupt */
    BCL1IF = 0; /* Clear Bus colloision interrupt */

    BCL1IE = 1; /* Enable bus collision interrult */
    SSP1IE = 1; /* Enable Serial port interrupt */
    PEIE = 1;   /* Enable peripheral interrupts */

}

void i2c_ResetI2CSlave( )
{

    i2cInfo.flags |= BEERCHIP_INUSE_FLAG;

    i2cInfo.index = 0;

}


/* The I2C master is performing a Read - Send it the data */
BEERCHIP_INLINE void i2c_MasterReadI2CData( uint8_t value )
{
    do {
        SSPCON1 &= ~_SSPCON1_WCOL_MASK;
        SSP1BUF = value;
    }while( SSPCON1 & _SSPCON1_WCOL_MASK );

    SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
}


BEERCHIP_INLINE i2c_i2cStatus i2c_IsrI2CSlave( uint8_t *index, uint8_t *value )
{
    uint8_t junk;
    uint8_t stat;
    i2c_i2cStatus result;

    // BEERCHIP_I2C_ACTIVITY_ON();

    result.value = 0x00;

    stat = SSP1STAT;
    switch( stat & BLKN_I2C_STATE_MASK )
    {
        /* Master Read - Last byte was Address - State 3 in Ref[1] */
        case (_SSP1STAT_R_nW_MASK):

            junk = SSP1BUF; /* dummy read to clear BF bit */

            // // *index = i2cInfo.index;
            // // i2cInfo.index++;
            // // *index = i2cInfo.index = 0x00;
            result.i2cStatusMasterRead = 1;
            *index = i2cInfo.index++;

            // SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */

        break;

        /* Master read - Last byte was data - State 4 in Ref[1] */
        case (_SSP1STAT_R_nW_MASK | _SSP1STAT_D_nA_MASK):

            *index = i2cInfo.index;
            result.i2cStatusMasterRead = 1;
            i2cInfo.index++;
            // SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
        break;

        /* Master write - Last byte was an address - State 1 in Ref[1] */
        case 0:

            junk = SSP1BUF;
            i2cInfo.flags |= BEERCHIP_FIRST_WRITE;
            SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
        break;

        /* Master Write - Last byte was data - State 2 in Ref[1] */
        case _SSP1STAT_D_nA_MASK:

            if( !(i2cInfo.flags & BEERCHIP_FIRST_WRITE) )
            {

                *index = i2cInfo.index;
                *value = SSP1BUF;
                result.i2cStatusMasterWrite = 1;

                i2cInfo.index++;
            }
            else
            {
                i2cInfo.index = SSP1BUF;
                *index = i2cInfo.index;
                i2cInfo.flags &= ~BEERCHIP_FIRST_WRITE;
            }

            SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
        break;

        default:
            junk = SSP1BUF;
            SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
        break;

    }
    
    return result;
}

BEERCHIP_INLINE void i2c_BusColI2CSlave( void )
{
    uint8_t junk;

    junk = SSP1BUF;
    SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
}


