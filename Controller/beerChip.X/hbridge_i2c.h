/*
 * File:   hbrdg_i2c.h
 * Author: BasementPC
 *
 * Created on January 11, 2014, 2:08 AM
 */


#ifndef HBRDG_I2C_H
#define	HBRDG_I2C_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "hbridge_i2cindex.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define HBRDG_INLINE

#define HBRDG_I2C_ACTIVITY_ENABLE_NOT  /* Enables an I2C Activity LED */
#define HBRDG_I2C_TEST_PATTERN_NOT /* Writes an initial test pattern to I2C
                                  ** data
                                  */
#define HBRDG_I2C_NO_MODIFY /* Do not modify I2C data in ISR. Only report what
                           ** has been changed
                           */

/* Host must match this number to have any hope of compatability */
#define HBRDG_I2C_COMPAT  0x00
#define HBRDG_I2C_VERSION 0x00
#define HBRDG_I2C_BUILD   0x01

#define HBRDG_I2C_CLEAR_CHAR 0xB0


#define HBRDG_I2C_PORT  PORTB
#define HBRDG_I2C_TRIS  TRISB
#define HBRDG_I2C_ANSEL ANSELB
#define HBRDG_I2C_LAT   LATB
#define HBRDG_I2C_PIN_SDA  0x10
#define HBRDG_I2C_PIN_SCL  0x40

#define HBRDG_I2C_ACTIVITY_PORT  PORTA
#define HBRDG_I2C_ACTIVITY_TRIS  TRISA
#define HBRDG_I2C_ACTIVITY_LAT   LATA
#define HBRDG_I2C_ACTIVITY_PIN   0x02
#define HBRDG_I2C_ACTIVITY_ANSEL ANSELA

#ifdef HBRDG_I2C_ACTIVITY_ENABLE
    #define HBRDG_I2C_ACTIVITY_ON()  hbrdg_activityOnI2CSlave()
    #define HBRDG_I2C_ACTIVITY_TOGGLE()  hbrdg_activityToggleI2CSlave()
    #define HBRDG_I2C_ACTIVITY_OFF()  hbrdg_activityOffI2CSlave()
#else
    #define HBRDG_I2C_ACTIVITY_ON()  do {} while(0)
    #define HBRDG_I2C_ACTIVITY_OFF()  do {} while(0)
    #define HBRDG_I2C_ACTIVITY_TOGGLE() do {} while(0)
#endif


#define HBRDG_I2C_ERROR  0xFF

#define HBRDG_INUSE_FLAG  0x01
#define HBRDG_FIRST_WRITE 0x02

typedef struct {
    unsigned char flags;
    unsigned char index;
} hbrdg_i2cInfo_t;

/* Type returned by I2C processing */
#define HBRDG_I2C_STATUS_READ  0x01
#define HBRDG_I2C_STSTUS_WRITE 0x02

typedef union {
    struct {

        uint8_t i2cStatusMasterRead  :1;
        uint8_t i2cStatusMasterWrite :1;
        uint8_t i2cStatusRFFU        :6;
    };
    uint8_t value;

} hbrdg_i2cStatus;

#define BLKN_I2C_STATE_MASK = (_SSP1STAT_R_nW_MASK | _SSP1STAT_D_nA_MASK)

void hbrdg_InitI2CSlave( uint8_t addr );

void hbrdg_ResetI2CSlave( void );

HBRDG_INLINE hbrdg_i2cStatus hbrdg_IsrI2CSlave( uint8_t *index, uint8_t *value );
HBRDG_INLINE void hbrdg_BusColI2CSlave( void );

HBRDG_INLINE void hbrdg_MasterReadI2CData( uint8_t value );

HBRDG_INLINE void hbrdg_activityOnI2CSlave( void );
HBRDG_INLINE void hbrdg_activityToggleI2CSlave( void );
HBRDG_INLINE void hbrdg_activityOffI2CSlave( void );

#ifdef	__cplusplus
}
#endif

#endif	/* HBRDG_I2C_H */

