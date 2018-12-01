/*
 * File:   blnk.h
 * Author: BasementPC
 *
 * Created on January 11, 2014, 1:43 AM
 */

#ifndef BLNK_H
#define	BLNK_H

#ifdef	__cplusplus
extern "C" {
#endif

#define BLNK_LED_MASK   0x01 /* Port RA0 */

#define BLNK_I2C_ADDR 0x30

#define BLNK_INLINE  inline

#define BLNK_TRUE  1
#define BLNK_FALSE 0

/*
** Structure for Blinky LED
**/

typedef enum {
    blnk_mode_allowForce = 0x01,
    blnk_mode_timer = 0x02,
    blnk_mode_stuckOn = 0x04,
    blnk_mode_stuckOff = 0x08
} blnk_mode_t;

typedef struct {
    blnk_mode_t mode;
    uint16_t cnt;
    uint16_t cngCnt;
} blnk_blinker_t;

void blnk_InitLED( uint8_t portMask, blnk_mode_t mode);
void blnk_ForceBlink( void );

#ifdef	__cplusplus
}
#endif

#endif	/* BLNK_H */

