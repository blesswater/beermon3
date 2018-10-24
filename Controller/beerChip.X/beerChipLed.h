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

#include <stdint.h>

/*
** Structure for LED Status
*/

/* LED Modes */
#define ledMode_AllowForce 0x01
#define ledMode_Timer      0x02
#define ledMode_stuckOn    0x04
#define ledMode_stuckOff   0x08

typedef struct {
    uint8_t mode;
    uint16_t cnt;
    uint16_t cngCnt;
} beerChip_LEDState_t ;

void beerChip_InitLED( uint8_t portMask, uint8_t mode);
void beerChip_ForceBlink( void );


#ifdef	__cplusplus
}
#endif

#endif	/* BLNK_H */

