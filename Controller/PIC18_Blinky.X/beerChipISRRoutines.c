#include "beerChipLed.h"

void beerChipTimerISR( void )
{
    beerChip_KickLED();
}
