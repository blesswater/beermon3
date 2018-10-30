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



#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPI2CINDEX_H */

