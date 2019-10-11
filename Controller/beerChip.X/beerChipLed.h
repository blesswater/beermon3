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

#define BEERCHIP_LED_PORT  PORTC
#define BEERCHIP_LED_ANSEL ANSELC
#define BEERCHIP_LED_PIN   0x80   /* Port RC7 */
#define BEERCHIP_LED_TRIS  TRISC
#define BEERCHIP_LED_LAT LATC

/*
** Structure for LED Status
*/

/* LED Modes */
#define ledMode_Off        0x00
#define ledMode_On         0x01
#define ledMode_Blink      0x02
#define ledMode_Flash      0x03
#define ledMode_Oneshot    0x04
#define ledMode_Bloop      0x05

typedef struct {
    uint8_t mode;
    uint8_t cnt;
    uint8_t cngCnt;
    uint8_t offCnt; /* Used for flashing */
} beerChip_LEDState_t ;

extern beerChip_LEDState_t ledState;

void beerChip_InitLED();
void beerChip_SetLEDMode( uint8_t mode, uint8_t cnt, uint8_t offCnt );
void beerChip_KickLED( void );
void beerChip_ToggleLED( void );
void beerChip_BloopEvent( uint8_t event );

#ifdef	__cplusplus
}
#endif

#endif	/* BLNK_H */

