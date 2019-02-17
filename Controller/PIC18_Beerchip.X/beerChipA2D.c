/*
 * File:   beerChipA2D.c
 * Author: Rob
 *
 * Created on November 2, 2018, 10:32 PM
 */

#include <string.h>
#include <stdint.h>

#include <xc.h>

#include "mcc_generated_files/adc.h"

#include "beerChipConfig.h"
#include "beerLock.h"
#include "beerChipLed.h"
#include "beerChipA2D.h"
// #include "beerChipTempLookup.h"

void a2d_Init( void )
{    
}

void a2d_InitReading( a2d_Reading_t *reading, uint8_t chan )
{
    memset( reading, 0x00, sizeof( a2d_Reading_t ) );
    lock_Init( &reading->lock );
    reading->chan = chan & 0x1F;
}


void a2d_StartReading( a2d_Reading_t *reading )
{
    uint8_t wt;
    
    if( ((ADCON0 & _ADCON0_CHS0_MASK) >> _ADCON0_CHS0_POSN) != reading->chan )
    {
        /* Write channel */
        ADCON0 = (ADCON0 & ~_ADCON0_CHS0_MASK) | reading->chan << _ADCON0_CHS0_POSN;
        /* Now, we wait for cap to charge? Not sure if this is necessary */
        wt = 0x80;
        while( wt-- != 0x00 );
    }
    /* Go */
    ADCON0 |= (0x1 << _ADCON0_GO_nDONE_POSN);
}

#if 0
bool a2d_PollReading( a2d_Reading_t *reading )
{
    bool result = 0;
    
    if( !(ADCON0 & (0x1 << _ADCON0_GO_nDONE_POSN)) )
    {
        lock_Take( &reading->lock ); /* Should always get the lock */
        reading->reading = ADRESH;
        reading->reading = (reading->reading << 8) | ADRESL;
        reading->count++;
        reading->temp = tempLookup( reading->reading );
        // reading->reading = 0x1234;
        // reading->count = 0x5678;
        lock_Release( &reading->lock );
        result = 1;
    }
    return result;
}
#endif

void a2d_GetTemperatureReading( a2d_Reading_t *reading )
{
    while( lock_Take( &reading->lock ) == 0 );
    
    reading->reading = ADC_GetConversion( (adc_channel_t)reading->chan );
    reading->temp = reading->reading + 3;
    reading->count++;
    
    lock_Release( &reading->lock );
}




