/* 
 * File:   beerChipUserTimer.h
 * Author: BasementPC
 *
 * Created on November 21, 2018, 4:37 PM
 */

#ifndef BEERCHIPUSERTIMER_H
#define	BEERCHIPUSERTIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t expireTime;
} userTmr_t;

void usrTmr_Init( userTmr_t *tmr );
void usrTmr_Start( userTmr_t *tmr, uint32_t ticks );
void usrTmr_Stop( userTmr_t *tmr );
bool usrTmr_Check( userTmr_t *tmr );

/* Stopwatch 
** Keeps running time
*/
#define BEERMON_USERTIMER_FLAG_MASK 0xF000
#define BEERMON_USERTIMER_ACUM_MASK 0x0FFF
typedef struct {
    uint32_t startTime;
    uint32_t accumTime; /* Format bit[31:24] flags, bit[23:0] accumulated time */
} usrStopwatch_t;

void usrStopwatch_Init( usrStopwatch_t *stpw );
void usrStopwatch_Start( usrStopwatch_t *stpw );
void usrStopwatch_Stop( usrStopwatch_t *stpw );
uint32_t usrStopwatch_GetTime( usrStopwatch_t *stpw );



#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPUSERTIMER_H */

