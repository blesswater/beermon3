/* 
 * File:   beerChipConfig.h
 * Author: BasementPC
 *
 * Created on October 6, 2018, 9:25 PM
 */

#ifndef BEERCHIPCONFIG_H
#define	BEERCHIPCONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

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

/* I2C Configuration */
#define HBRDG_I2C_SELECT_PORT  PORTC
#define HBRDG_I2C_SELECT_TRIS  TRISC
#define HBRDG_I2C_SELECT_ANSEL ANSELC
#define HBRDG_I2C_SELECT_PIN   0x04  /* RC2 */
#define HBRDG_I2C_BASE_ADDR    0x30


#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPCONFIG_H */

