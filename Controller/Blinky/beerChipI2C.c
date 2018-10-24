/*******************************************************************************
**
** Filename: hbrdg_i2c.c
**
** Purpose: I2C driver for Woody the Robot.
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



hbrdg_i2cInfo_t i2cInfo;

void hbrdg_InitI2CSlave( uint8_t addr )
{

    /* Clear SDA/SCL */
    HBRDG_I2C_PORT &= ~(HBRDG_I2C_PIN_SDA | HBRDG_I2C_PIN_SCL);
    HBRDG_I2C_LAT &= ~(HBRDG_I2C_PIN_SDA | HBRDG_I2C_PIN_SCL);
    HBRDG_I2C_ANSEL &= ~(HBRDG_I2C_PIN_SDA | HBRDG_I2C_PIN_SCL);
    /* Set SDA/SCL as Input */
    HBRDG_I2C_TRIS = (HBRDG_I2C_PIN_SDA | HBRDG_I2C_PIN_SCL);

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

#ifdef HBRDG_I2C_ACTIVITY_ENABLE

    HBRDG_I2C_ACTIVITY_TRIS &= ~HBRDG_I2C_ACTIVITY_PIN;
    HBRDG_I2C_ACTIVITY_ANSEL &= ~HBRDG_I2C_ACTIVITY_PIN;
    HBRDG_I2C_ACTIVITY_LAT &= ~HBRDG_I2C_ACTIVITY_PIN;

#endif

    BCL1IE = 1; /* Enable bus collision interrult */
    SSP1IE = 1; /* Enable Serial port interrupt */
    PEIE = 1;   /* Enable peripheral interrupts */

}

void hbrdg_ResetI2CSlave( )
{

    i2cInfo.flags |= HBRDG_INUSE_FLAG;

    i2cInfo.index = 0;

}

uint8_t hbrdg_ReadI2CSelect()
{
    uint8_t selValue = 0x00;
    uint8_t cnt = 0;
    /* Set I2C Select pin to Input */
    HBRDG_I2C_SELECT_TRIS |= HBRDG_I2C_SELECT_PIN;
    HBRDG_I2C_SELECT_ANSEL &= !HBRDG_I2C_SELECT_PIN; /* Turn OFF analog select */

    /* Read SELECT_I2C pin. Make sure get 0x20 readings all the same */
    while( cnt < 0x20 )
    {
        hbrdg_Delay();
        cnt++;
        // LATA ^= HBRDG_LED_MASK;
        if( (HBRDG_I2C_SELECT_PORT & HBRDG_I2C_SELECT_PIN) != selValue )
        {
            cnt = 0;
        }
        selValue =  (HBRDG_I2C_SELECT_PORT & HBRDG_I2C_SELECT_PIN);
    }

    return( (selValue == 0x00) ? 0x00 : 0x0F );

}


HBRDG_INLINE void hbrdg_activityOnI2CSlave( void )
{
    HBRDG_I2C_ACTIVITY_LAT |= HBRDG_I2C_ACTIVITY_PIN;
}

HBRDG_INLINE void hbrdg_activityToggleI2CSlave( void )
{
    HBRDG_I2C_ACTIVITY_LAT = HBRDG_I2C_ACTIVITY_LAT ^ HBRDG_I2C_ACTIVITY_PIN;
}

HBRDG_INLINE void hbrdg_activityOffI2CSlave( void )
{
    HBRDG_I2C_ACTIVITY_LAT &= ~HBRDG_I2C_ACTIVITY_PIN;
}

/* The I2C master is performing a Read - Send it data */
HBRDG_INLINE void hbrdg_MasterReadI2CData( uint8_t value )
{
    do {
        SSPCON1 &= ~_SSPCON1_WCOL_MASK;
        SSP1BUF = value;  
    }while( SSPCON1 & _SSPCON1_WCOL_MASK );

    SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
}


HBRDG_INLINE hbrdg_i2cStatus hbrdg_IsrI2CSlave( uint8_t *index, uint8_t *value )
{
    uint8_t junk;
    uint8_t stat;
    hbrdg_i2cStatus result;

    // HBRDG_I2C_ACTIVITY_ON();

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
            i2cInfo.flags |= HBRDG_FIRST_WRITE;
            SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
        break;

        /* Master Write - Last byte was data - State 2 in Ref[1] */
        case _SSP1STAT_D_nA_MASK:

            if( !(i2cInfo.flags & HBRDG_FIRST_WRITE) )
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
                i2cInfo.flags &= ~HBRDG_FIRST_WRITE;
            }

            SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
        break;

        default:
            junk = SSP1BUF;
            SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
        break;

    }
    // HBRDG_I2C_ACTIVITY_OFF();
    return result;
}

HBRDG_INLINE void hbrdg_BusColI2CSlave( void )
{
    uint8_t junk;

    junk = SSP1BUF;
    SSPCON1 |= _SSPCON1_CKP_MASK;  /* Release the clock */
}

