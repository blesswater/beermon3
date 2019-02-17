
#include "beerChipLed.h"
#include "beerChipConfig.h"
#include "beerLock.h"
#include "mcc_generated_files/adc.h"
#include "beerChipA2D.h"

extern a2d_Reading_t a2dProbe[BEERMON_NUM_TEMP_PROBES];

uint32_t uptime = 0x00000000;
static uint8_t  secTickCnt = 0x00;
static int test;

void beerChipTimerISR( void )
{
    beerChip_KickLED();
    
    /* Do time tick */
    if( secTickCnt++ == BEERCHIP_TICKS_PER_SECOND )
    {
        /* This should happen once per second */
        uptime++;
        secTickCnt = 0x00;
        // nowStats.onTime = usrStopwatch_GetTime( &(beermonStats.onTime) );
        // nowStats.offTime = usrStopwatch_GetTime( &(beermonStats.offTime) );
    }

}
