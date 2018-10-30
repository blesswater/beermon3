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

#include <xc.h>

#define BEERCHIP_LED_PORT PORTA
#define BEERCHIP_LED_ANSEL ANSELA
#define BEERCHIP_LED_PIN  0x01   /* Port RA0 */
#define BEERCHIP_LED_TRIS TRISA
#define BEERCHIP_LED_LAT  LATA

/*
** Structure for LED Status
*/

/* LED Modes */
#define ledMode_Off        0x00
#define ledMode_On         0x01
#define ledMode_Blink      0x02

typedef struct {
    uint8_t mode;
    uint8_t cnt;
    uint8_t cngCnt;
} beerChip_LEDState_t ;

extern beerChip_LEDState_t ledState;

void beerChip_InitLED();
void beerChip_SetLEDMode( uint8_t mode, uint8_t cnt );
void beerChip_KickLED( void );

#ifdef	__cplusplus
}
#endif

#endif	/* BLNK_H */

