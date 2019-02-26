/* 
** File:   beerChipA2D.h
** Author: BasementPC
**
** Created on October 6, 2018, 10:09 PM
*/

#ifndef _BEERCHIPA2D_H
#define _BEERCHIPA2D_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/*
** Define the A2D ports
*/

/* Channel 0 is AN4 - RC0 - pin 16 */
#define BEERCHIP_A2D_CHAN0  4
#define BEERCHIP_A2D_PIN0   0x01 /* AN4 */
#define BEERCHIP_A2D_PORT0  PORTC
#define BEERCHIP_A2D_TRIS0  TRISC
#define BEERCHIP_A2D_ANSEL0 ANSELC
    
/* Channel 1 is AN5 - RC1 - pin 15 */
#define BEERCHIP_A2D_CHAN1  5
#define BEERCHIP_A2D_PIN1   0x02 /* AN5 */
#define BEERCHIP_A2D_PORT1  PORTC
#define BEERCHIP_A2D_TRIS1  TRISC
#define BEERCHIP_A2D_ANSEL1 ANSELC
    
/* Channel 2 is AN6 - RC2 - pin 14 */
#define BEERCHIP_A2D_CHAN2  6
#define BEERCHIP_A2D_PIN2   0x04 /* RC2 / AN6 */
#define BEERCHIP_A2D_PORT2  PORTC
#define BEERCHIP_A2D_TRIS2  TRISC
#define BEERCHIP_A2D_ANSEL2 ANSELC

typedef struct {
    lock_t lock;
    uint8_t flags : 3;
    uint8_t chan : 5;
    uint16_t count;
    uint16_t reading;
    int16_t temp;
} a2d_Reading_t;

void a2d_Init( void );
void a2d_InitReading( a2d_Reading_t *reading, uint8_t chan );
void a2d_StartReading( a2d_Reading_t *reading );
bool a2d_PollReading( a2d_Reading_t *reading );

#ifdef	__cplusplus
}
#endif

#endif	/* _BEERCHIPA2D_H */

