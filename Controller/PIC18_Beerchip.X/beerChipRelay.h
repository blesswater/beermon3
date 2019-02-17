/* 
 * File:   beerChipRelay.h
 * Author: BasementPC
 *
 * Created on November 22, 2018, 1:03 AM
 */

#ifndef BEERCHIPRELAY_H
#define	BEERCHIPRELAY_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define BEERCHIP_RLY_PORT  PORTC
#define BEERCHIP_RLY_ANSEL ANSELH
#define BEERCHIP_RLY_TRIS  TRISC
#define BEERCHIP_RLY_LAT   LATC
    
#define BEERCHIP_RYL0_PIN   0x20   /* Port RC5 */
#define BEERCHIP_RYL0_ANSEL_BIT 0x04
#define BEERCHIP_RYL1_PIN   0x40   /* Port RC6 */
#define BEERCHIP_RYL1_ANSEL_BIT 0x01
    
#define BEERCHIP_RELAY_ON  1
#define BEERCHIP_RELAY_OFF 0
    
typedef struct {
    uint8_t portMask;
    uint8_t state;
} beerchip_relay_t;

void relay_Init( beerchip_relay_t *rly, uint8_t pin );
void relay_Switch( beerchip_relay_t *rly, uint8_t state );




#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPRELAY_H */

