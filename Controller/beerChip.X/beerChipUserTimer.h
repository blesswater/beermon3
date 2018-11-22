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



#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPUSERTIMER_H */

