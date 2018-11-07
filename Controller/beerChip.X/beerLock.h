/* 
 * File:   beerLock.h
 * Author: BasementPC
 *
 * Created on October 31, 2018, 6:25 PM
 */

#ifndef BEERLOCK_H
#define	BEERLOCK_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>

typedef volatile uint8_t lock_t;

void lock_Init( lock_t *lock );
bool lock_Take( lock_t *lock );
void lock_Release( lock_t *lock );



#ifdef	__cplusplus
}
#endif

#endif	/* BEERLOCK_H */

