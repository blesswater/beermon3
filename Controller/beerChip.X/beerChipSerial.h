/* 
 * File:   beerChipSerial.h
 * Author: Rob
 *
 * Created on December 31, 2019, 5:59 PM
 */

#ifndef BEERCHIPSERIAL_H
#define	BEERCHIPSERIAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#define BEERMON_TX_BUFSIZE 8
    
typedef struct {
    uint8_t txByte : 5;
    uint8_t txState : 3;
    uint8_t frmType;
    uint8_t frmLen;
    uint8_t *buffer;
} serialTxState;


void initSerial( void );
void initTxState( serialTxState *txState );
bool isTxReady( serialTxState *txState );
void txStart( serialTxState *txState );
void txProcess( serialTxState *txState );

#define BM_SERIAL_STX    0x02
#define BM_SERIAL_ETX    0x03
#define BM_SERIAL_DLE    0x10

/*
**  State Machine
*/
/* States */
#define BM_SERIAL_READY        0x00
#define BM_SERIAL_START        0x01
#define BM_SERIAL_SEND_FRMTYPE 0x02
#define BM_SERIAL_SEND         0x03
#define BM_SERIAL_SEND_ESC     0x04
#define BM_SERIAL_END          0x05

/* Events */
#define BM_SERIAL_EVT_START       0x00
#define BM_SERIAL_EVT_CNTL_CHAR   0x01 /* Tx Complete and next char needs esc */
#define BM_SERIAL_EVT_LAST        0x02 /* Tx Complete and at eof */
#define BM_SERIAL_EVT_TXCMPL      0x03 /* Tx Complete */

void procTxSerialState( serialTxState *txState, uint8_t event );



#ifdef	__cplusplus
}
#endif

#endif	/* BEERCHIPSERIAL_H */

