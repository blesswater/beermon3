#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerLock.h"

void lock_Init( lock_t *lock )
{
    *lock = 0x00;
}

bool lock_Take( lock_t *lock )
{
    /* Assumes W holds &lock */
    do {
        asm( "MOVF lock_Take@lock, W" );
        asm( "MOVWF FSR1L");
        asm( "CLRF FSR1H" );
        asm( "BSF INDF1, 4" );
        asm( "SWAPF INDF1" );
    } while( *lock == 0x00 );

    return( (*lock & 0x10) ? 0 : 1 );
}

bool lock_Check( lock_t *lock )
{
    return( (*lock & 0x01) ? 0 : 1 );
}

void lock_Release( lock_t *lock )
{
    *lock = 0x00;
}
