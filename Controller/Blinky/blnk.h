/* 
 * File:   blnk.h
 * Author: BasementPC
 *
 * Created on January 11, 2014, 1:43 AM
 */

#ifndef BLNK_H
#define	BLNK_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
** Different clock values for OSCCON
**/
#define BLNK_CLK_16MHz  (0xF << _OSCCON_IRCF_POSN)
#define BLNK_CLK_8MHz   (0xE << _OSCCON_IRCF_POSN)
#define BLNK_CLK_4MHz   (0xD << _OSCCON_IRCF_POSN)
#define BLNK_CLK_2MHz   (0xC << _OSCCON_IRCF_POSN)
#define BLNK_CLK_1MHz   (0xB << _OSCCON_IRCF_POSN)
#define BLNK_CLK_500KHz (0x7 << _OSCCON_IRCF_POSN) /* Default on reset */
#define BLNK_CLK_31KHz  (0x0 << _OSCCON_IRCF_POSN)

#define BLNK_CLK_SEL    (0x0 << _OSCCON_SCS_POSITION) /* Use FOSC config word */

/* Clock select use in OSCCON */
#define BLNK_CLK (BLNK_CLK_16MHz | BLNK_CLK_SEL)

#define BLNK_LED_MASK   0x01 /* Port RA0 */

#define BLNK_I2C_ADDR 0x30

#define BLNK_INLINE  inline

#define BLNK_TRUE  1
#define BLNK_FALSE 0

/*
** Structure for I2C Information
**/


#ifdef	__cplusplus
}
#endif

#endif	/* BLNK_H */

