/* 
 * File:   beerChipI2C.h
 * Author: BasementPC
 *
 * Created on October 6, 2018, 10:58 PM
 */

#ifndef BEERCHIPI2C_H
#define	BEERCHIPI2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "beerChipConfig.h"

#define BEERCHIP_I2C_ACTIVITY_ENABLE_NOT  /* Enables an I2C Activity LED */
#define BEERCHIP_I2C_TEST_PATTERN_NOT /* Writes an initial test pattern to I2C
                                  ** data
                                  */
#define BEERCHIP_I2C_NO_MODIFY /* Do not modify I2C data in ISR. Only report what
                           ** has been changed
                           */

/* Host must match this number to have any hope of compatability */
#define BEERCHIP_I2C_COMPAT  0x00
#define BEERCHIP_I2C_VERSION 0x00
#define BEERCHIP_I2C_BUILD   0x01

#define BEERCHIP_I2C_CLEAR_CHAR 0xB0


#define BEERCHIP_I2C_PORT  PORTB
#define BEERCHIP_I2C_TRIS  TRISB
#define BEERCHIP_I2C_ANSEL ANSELB
#define BEERCHIP_I2C_LAT   LATB
#define BEERCHIP_I2C_PIN_SDA  0x10
#define BEERCHIP_I2C_PIN_SCL  0x40

/* Pins that allows selection of I2C Address */
#define NOT_BEERCHIP_I2C_SELECT_ENABLE
#define BEERCHIP_I2C_SELECT_PORT  PORTB
#define BEERCHIP_I2C_SELECT_TRIS  TRISB
#define BEERCHIP_I2C_SELECT_ANSEL ANSELB
#define BEERCHIP_I2C_SELECT_PINS 0x00

#define BEERCHIP_I2C_ERROR  0xFF

#define BEERCHIP_INUSE_FLAG  0x01
#define BEERCHIP_FIRST_WRITE 0x02

typedef struct {
    unsigned char flags;
    unsigned char index;
} hbrdg_i2cInfo_t;

/* Type returned by I2C processing */
#define BEERCHIP_I2C_STATUS_READ  0x01
#define BEERCHIP_I2C_STSTUS_WRITE 0x02

typedef union {
    struct {

        uint8_t i2cStatusMasterRead  :1;
        uint8_t i2cStatusMasterWrite :1;
        uint8_t i2cStatusRFFU        :6;
    };
    uint8_t value;

} i2c_i2cStatus;

#define BLKN_I2C_STATE_MASK  (_SSP1STAT_R_nW_MASK | _SSP1STAT_D_nA_MASK)

void i2c_InitI2CSlave( uint8_t addr );

void i2c_ResetI2CSlave( void );

BEERCHIP_INLINE i2c_i2cStatus i2c_IsrI2CSlave( uint8_t *index, uint8_t *value );
BEERCHIP_INLINE void i2c_BusColI2CSlave( void );

BEERCHIP_INLINE void i2c_MasterReadI2CData( uint8_t value );

BEERCHIP_INLINE void hbrdg_activityOnI2CSlave( void );
BEERCHIP_INLINE void hbrdg_activityToggleI2CSlave( void );
BEERCHIP_INLINE void hbrdg_activityOffI2CSlave( void );



#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPI2C_H */

