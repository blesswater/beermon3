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
#define BEERCHIP_PRODUCT_ID    0xB0
#define BEERCHIP_MAJOR_VERSION 0
#define BEERCHIP_MINOR_VERSION 0
#define BEERCHIP_BUILD         0

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
/*
** LED PIN Configuration
*/
#define BEERCHIP_LED_MASK   0x01 /* Port RA0 */

/*
** I2C Config
*/
#define BEERCHIP_I2C_ADDRESS   0x23


#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPCONFIG_H */

