/* 
 * File:   beerChipI2CIndex.h
 * Author: BasementPC
 *
 * Created on October 6, 2018, 10:57 PM
 */

#ifndef BEERCHIPI2CINDEX_H
#define	BEERCHIPI2CINDEX_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
** Indexes that hold version information
*/
#define BEERCHIP_I2C_PROD_INDEX          0x00
#define BEERCHIP_I2C_MAJ_VER_INDEX       0x01
#define BEERCHIP_I2C_MIN_VER_INDEX       0x02
#define BEERCHIP_I2C_PROD_BUILD_INDEX    0x03

/*
** Up Time seconds
**/
#define BEERCHIP_I2C_UPTIME_BYTE0        0x07 /* MSB */
#define BEERCHIP_I2C_UPTIME_BYTE1        0x06
#define BEERCHIP_I2C_UPTIME_BYTE2        0x05
#define BEERCHIP_I2C_UPTIME_BYTE3        0x04 /* LSB */

/*
** LED Controll
*/
#define BEERCHIP_I2C_LED_MODE               0x08
#define BEERCHIP_I2C_LED_CNT                0x09
#define BEERCHIP_I2C_LED_CNT_LIMIT          0x0A
    
/*
** A2D 
*/
#define BEERCHIP_A2D_TRIGGER_READING     0x10
    
#define BEERCHIP_A2D_CHAN0_TEMP_BYTE0    0x12
#define BEERCHIP_A2D_CHAN0_TEMP_BYTE1    0x13
    
#define BEERCHIP_A2D_CHAN0_READING_BYTE0 0x14
#define BEERCHIP_A2D_CHAN0_READING_BYTE1 0x15
#define BEERCHIP_A2D_CHAN0_COUNT_BYTE0   0x16
#define BEERCHIP_A2D_CHAN0_COUNT_BYTE1   0x17
    
#define BEERCHIP_A2D_CHAN1_TEMP_BYTE0    0x18
#define BEERCHIP_A2D_CHAN1_TEMP_BYTE1    0x19
    
#define BEERCHIP_A2D_CHAN1_READING_BYTE0 0x1A
#define BEERCHIP_A2D_CHAN1_READING_BYTE1 0x1B
#define BEERCHIP_A2D_CHAN1_COUNT_BYTE0   0x1C
#define BEERCHIP_A2D_CHAN1_COUNT_BYTE1   0x1D
    
/*
** Relays
*/
#define BEERCHIP_RELAY_ENABLE           0x20
#define BEERCHIP_RELAY_CONTROL          0x21

/*
** Beermon
*/

/* Config */
    
#define BEERCHIP_BEERMON_CFG_SETPT             0x30
#define BEERCHIP_BEERMON_CFG_ON_DEBOUNCE_TIME  0x32
#define BEERCHIP_BEERMON_CFG_OFF_DEBOUNCE_TIME 0x34
#define BEERCHIP_BEERMON_CFG_SEQNUM            0x36
#define BEERCHIP_BEERMON_CFG_CSUM              0x38
#define BEERCHIP_BEERMON_CFG_CNTL_PROBE        0x3A
#define BEERCHIP_BEERMON_CFG_UPDATE_STATE      0x3B

/* Stats */
#define BEERCHIP_BEERMON_STATS_READ            0x40
#define BEERCHIP_BEERMON_STATS_ON_CNT          0x42
#define BEERCHIP_BEERMON_STATS_OFF_CNT         0x44
#define BEERCHIP_BEERMON_STATS_ON_TIME         0x46
#define BEERCHIP_BEERMON_STATS_OFF_TIME        0x4A

/* State */
#define BEERCHIP_BEERMON_STATE_STATE          0x4E
#define BEERCHIP_BEERMON_CNTL_CMD             0x4F

/*
**
*/
#define BEERCHIP_TEST_START             0x50 /* 0x3E */


#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPI2CINDEX_H */

