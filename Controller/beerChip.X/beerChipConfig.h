/* 
 * File:   beerChipConfig.h
 * Author: BasementPC
 *
 * Created on October 6, 2018, 10:09 PM
 */

#ifndef BEERCHIPCONFIG_H
#define	BEERCHIPCONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
** Version
*/
#define BEERCHIP_PRODUCT_ID    0x0B
#define BEERCHIP_MAJOR_VERSION 0
#define BEERCHIP_MINOR_VERSION 0
#define BEERCHIP_BUILD         0x17

/*
** Different clock values for OSCCON
**/
#define BEERCHIP_CLK_16MHz  (0xF << _OSCCON_IRCF_POSN)
#define BEERCHIP_CLK_8MHz   (0xE << _OSCCON_IRCF_POSN)
#define BEERCHIP_CLK_4MHz   (0xD << _OSCCON_IRCF_POSN)
#define BEERCHIP_CLK_2MHz   (0xC << _OSCCON_IRCF_POSN)
#define BEERCHIP_CLK_1MHz   (0xB << _OSCCON_IRCF_POSN)
#define BEERCHIP_CLK_500KHz (0x7 << _OSCCON_IRCF_POSN) /* Default on reset */
#define BEERCHIP_CLK_31KHz  (0x0 << _OSCCON_IRCF_POSN)

#define BEERCHIP_CLK_SEL    (0x0 << _OSCCON_SCS_POSITION) /* Use FOSC config word */

/* Clock select use in OSCCON */
#define BEERCHIP_CLK (BEERCHIP_CLK_16MHz | BEERCHIP_CLK_SEL)

#define BEERCHIP_INLINE

#define BEERCHIP_BLINK_RATE  0 /* In 1/10 sec */
/*
** I2C Config
*/
#define BEERCHIP_I2C_ADDRESS   0x2E

/*
**
*/
#define BEERCHIP_TICKS_PER_SECOND  10

/*
** Typedefs
*/
typedef unsigned char bool;


#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPCONFIG_H */

