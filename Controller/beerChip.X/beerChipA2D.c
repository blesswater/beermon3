/*
 * File:   beerChipA2D.c
 * Author: Rob
 *
 * Created on November 2, 2018, 10:32 PM
 */

#include <string.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerLock.h"
#include "beerChipLed.h"
#include "beerChipA2D.h"
#include "beerChipTempLookup.h"

void a2d_Init( void )
{
    BEERCHIP_A2D_TRIS0 |= BEERCHIP_A2D_PIN0; /* Set TRIS to Input */
    BEERCHIP_A2D_ANSEL0 |= BEERCHIP_A2D_PIN0;
    
    BEERCHIP_A2D_TRIS1 |= BEERCHIP_A2D_PIN1; /* Set TRIS to Input */
    BEERCHIP_A2D_ANSEL1 |= BEERCHIP_A2D_PIN1;
    
    ADCON1 = (0x01 << _ADCON1_ADFM_POSN) | /* Make right justified MSBs set to 0 */
             (0x05 << _ADCON1_ADCS_POSN) | /* Fosc/16 conversion clock */
             (0x00 << _ADCON1_ADPREF_POSN); /* Use Vdd for Vref */
    
    ADCON2 = (0x00 << _ADCON2_TRIGSEL_POSN); /* No trigger. I will trigger it myself */
    
    ADCON0 = (0x1 << _ADCON0_ADON_POSN); /* Turn A2D ON */
    
    /* Interrupts */
    PIR1 &= ~(0x1 << _PIR1_ADIF_POSN); /* Set A2D interrupt OFF */
    
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



