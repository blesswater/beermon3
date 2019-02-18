/* 
 * File:   beerChipISPRoutines.h
 * Author: BasementPC
 *
 * Created on February 9, 2019, 3:20 PM
 */

#ifndef BEERCHIPISPROUTINES_H
#define	BEERCHIPISPROUTINES_H

#ifdef	__cplusplus
extern "C" {
#endif

void beerChipTimerISR( void );

void beerChipI2CHandler(I2C1_SLAVE_DRIVER_STATUS i2c_bus_state);

#define beerCfgClean 0x00  /* No changes */
#define beerCfgDirty 0x01  /* Changes Pending */

#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPISPROUTINES_H */

