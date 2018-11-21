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
#define BEERCHIP_A2D_CHAN0_READING_BYTE0 0x10
#define BEERCHIP_A2D_CHAN0_READING_BYTE1 0x11
#define BEERCHIP_A2D_CHAN0_COUNT_BYTE0   0x12
#define BEERCHIP_A2D_CHAN0_COUNT_BYTE1   0x13
    
#define BEERCHIP_A2D_CHAN1_READING_BYTE0 0x14
#define BEERCHIP_A2D_CHAN1_READING_BYTE1 0x15
#define BEERCHIP_A2D_CHAN1_COUNT_BYTE0   0x16
#define BEERCHIP_A2D_CHAN1_COUNT_BYTE1   0x17
    



#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPI2CINDEX_H */

