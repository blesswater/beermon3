/* 
 * File:   hbridge_i2cindex.h
 * Author: BasementPC
 *
 * Created on January 19, 2014, 9:58 PM
 */

#ifndef HBRIDGE_I2CINDEX_H
#define	HBRIDGE_I2CINDEX_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
** Indexes that hold version information
*/
#define HBRDG_I2C_PROD_INDEX          0x00
#define HBRDG_I2C_MAJ_VER_INDEX       0x01
#define HBRDG_I2C_MIN_VER_INDEX       0x02
#define HBRDG_I2C_PROD_BUILD_INDEX    0x03

/*
** PWM
*/
#define HBRDG_I2C_LSB_WRITE_FLG  0x01
#define HBRDG_I2C_MSB_WRITE_FLG  0x02

#define HBRDG_I2C_PWM0_FLG_INDEX  0x04
#define HBRDG_I2C_PWM0_LSB_INDEX  0x05
#define HBRDG_I2C_PWM0_MSB_INDEX  0x06
#define HBRDG_I2C_PWM1_FLG_INDEX  0x07
#define HBRDG_I2C_PWM1_LSB_INDEX  0x08
#define HBRDG_I2C_PWM1_MSB_INDEX  0x09

/* H-Bridge */
#define HBRDG_I2C_HBRIDGE_PWM_FLAG        0x08
#define HBRDG_I2C_HBRIDGE_REV_FLAG        0x01
#define HBRDG_I2C_HBRIDGE_STOP_FLAG       0x02
#define HBRDG_I2C_HBRIDGE_INVERT_DIR_FLAG 0x04
#define HBRDG_I2C_HBRIDGE_ENABLE          0x10
#define HBRDG_I2C_HBRIDGE_I2C_CONTROL     0x20

#define HBRDG_I2C_HBRIDGE_INDEX           0x0A

/* Encoder */
#define HBRDG_I2C_ENCOD_RDFLG_INDEX        0x10
#define HBRDG_I2C_ENCOD_POS_BYTE0_INDEX    0x11    /* LSB */
#define HBRDG_I2C_ENCOD_POS_BYTE1_INDEX    0x12
#define HBRDG_I2C_ENCOD_POS_BYTE2_INDEX    0x13
#define HBRDG_I2C_ENCOD_POS_BYTE3_INDEX    0x14    /* MSB */
#define HBRDG_I2C_ENCOD_VEL_LSB_INDEX      0x15
#define HBRDG_I2C_ENCOD_VEL_MSB_INDEX      0x16
#define HBRDG_I2C_ENCOD_CONFIG_INDEX       0x17
#define HBRDG_I2C_ENCOD_ERR_BYTE0_INDEX    0x18
#define HBRDG_I2C_ENCOD_ERR_BYTE1_INDEX    0x19

#define HBRDG_I2C_ENCOD_RDFLG_REV_CNT  0x01
#define HBRDG_I2C_ENCOD_RDFLG_RESET    0x02

#ifdef	__cplusplus
}
#endif

#endif	/* HBRIDGE_I2CINDEX_H */

