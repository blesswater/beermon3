/* 
 * File:   beerChipBeermon.h
 * Author: BasementPC
 *
 * Created on November 23, 2018, 9:53 PM
 */

#ifndef BEERCHIPBLOOPDET_H
#define	BEERCHIPBLOOPDET_H

#ifdef	__cplusplus
extern "C" {
#endif

/* Bloop Detect RA1 - Pin 18 */
#define BEERCHIP_BLOOP_PORT  PORTA
#define BEERCHIP_BLOOP_ANSEL ANSELA
#define BEERCHIP_BLOOP_TRIS  TRISA
#define BEERCHIP_BLOOP_LAT LATA
#define BEERCHIP_BLOOP_PIN 0x02

/* Bloop Ack RA0 - pin 19 */
#define BEERCHIP_BLOOPACK_PORT  PORTA
#define BEERCHIP_BLOOPACK_ANSEL ANSELA
#define BEERCHIP_BLOOPACK_TRIS  TRISA
#define BEERCHIP_BLOOPACK_LAT   LATA
#define BEERCHIP_BLOOPACK_PIN   0x01

/* BloopDet States */
#define bloopdet_state_wait        0x00
#define bloopdet_state_bloop       0x01
#define bloopdet_state_bloopack    0x02
    
/* BloopDet Events */
#define bloopdet_event_bloop       (uint8_t)0x00
#define bloopdet_event_notbloop    (uint8_t)0x01
#define bloopdet_event_bloopack    (uint8_t)0x02
#define bloopdet_event_reset       (uint8_t)0x03

typedef struct {
    uint8_t state;
    uint16_t bloopCnt;
    uint16_t bloopAckCnt;
} bloopDetState_t;

void bloopDet_Init(bloopDetState_t *state );

void bloopDet_Process( bloopDetState_t *state );
void bloopDet_ProcessEvent( bloopDetState_t *state, uint8_t event );


#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPBLOOPDET_H */

