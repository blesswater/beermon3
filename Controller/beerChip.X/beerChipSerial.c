#include <string.h>
#include <stdint.h>

#include <xc.h>

#include "beerChipConfig.h"
#include "beerChipSerial.h"

#ifdef BEERCHIP_USE_SERIAL

void initSerial( void )
{
 
    BAUDCON = ( 0 << _BAUDCON_BRG16_POSN) | /* 8-bit Baud Rate Generator is used */
              ( 0 << _BAUDCON_SCKP_POSN );   /* non-invert */
    
    // SPBRG = 0x81; /* 9600 baud - Fosc = 20M */
    SPBRG = 0x67; /* 9600 baud - Fosc = 16M */
    
    TRISB = TRISB & ~(_TRISB_TRISB7_MASK); /* Set RB7 (pin10) to output */
    TRISB = TRISB |  (_TRISB_TRISB5_MASK); /* Set RB5 to input */
    ANSELB = ANSELB & (~_ANSELB_ANSB5_MASK);
    
    TXSTA = (1 << _TXSTA_TXEN_POSN) |   /* Transmit enabled */
            (0 << _TXSTA_SYNC_POSN) |
            (0 << _TXSTA_TX9_POSN)  |
            (1 << _TXSTA_BRGH_POSN);   /* high baud rate select */
    
    RCSTA = (1 << _RCSTA_SPEN_POSN) |
            (1 << _RCSTA_CREN_POSN) | /* Enable reciever */
            (0 << _RCSTA_RX9_POSN); 
    
    /* Interrupts */
    // PIR1 = PIR1 | (1 << _PIR1_TXIF_POSN);
    PIE1 = PIE1 | (0 << _PIE1_TXIE_POSN) |
                  (1 << _PIE1_RCIE_POSN);
}

void initTxState( serialTxState *txState )
{
    memset( txState, 0x00, sizeof( serialTxState ) ); 
}

bool isTxReady( serialTxState *txState )
{
    return ((txState->txState == BM_SERIAL_READY) && 
        (TXIF == 1) && 
        (TXSTA & _TXSTA_TRMT_MASK)) ? 1 : 0;
}

void txStart( serialTxState *txState )
{
    procTxSerialState( txState, (uint8_t)BM_SERIAL_EVT_START );
}

void txProcess( serialTxState *txState )
{
    uint8_t event;
    if( (TXIF == 1) && (TXSTA & _TXSTA_TRMT_MASK) )
    {
        event = BM_SERIAL_EVT_TXCMPL;
        if( txState->txByte >= txState->frmLen )
            event = BM_SERIAL_EVT_LAST;
        else if( (txState->buffer[txState->txByte] == BM_SERIAL_STX) ||
                 (txState->buffer[txState->txByte] == BM_SERIAL_ETX) ||
                 (txState->buffer[txState->txByte] == BM_SERIAL_DLE)
                )
            event = BM_SERIAL_EVT_CNTL_CHAR;
        procTxSerialState( txState, event );
        
    }
}

/*
** State Table Actions
*/

void doNothingSerial( serialTxState *txState ) {}

void startSerial( serialTxState *txState ) {
    txState->txByte = 0;
    txState->txState = BM_SERIAL_START;
}

/* The START (BM_SERIAL_START) State */
void startFrmSerial( serialTxState *txState ) {
    TXREG = (uint8_t)BM_SERIAL_STX;
    txState->txState = BM_SERIAL_SEND_FRMTYPE;
}

/* The Send Frame Type (BM_SERIAL_SEND_FRMTYPE) State */
void frameTypeSerial_Send( serialTxState *txState ) {
    TXREG = (uint8_t)txState->frmType;
    txState->txState = BM_SERIAL_SEND;
}
void frameTypeSerial_Ctl( serialTxState *txState ) 
{
    TXREG = BM_SERIAL_DLE;
    txState->txState = BM_SERIAL_SEND_ESC;
}

/* The Send State (BM_SERIAL_SEND) State */
void sendSerial_Send( serialTxState *txState ) {
    TXREG = (txState->buffer)[txState->txByte];
    txState->txByte++;
}

void sendSerial_Last( serialTxState *txState )
{
    TXREG = BM_SERIAL_ETX;
    txState->txState = BM_SERIAL_END;
}

/* The Send Esc (BM_SERIAL_SEND_ESC) State */
void escSerial_Send( serialTxState *txState )
{
    TXREG = (txState->buffer)[txState->txByte] | 0x80;
    txState->txByte++;
    txState->txState = BM_SERIAL_SEND;
}

void escSerial_SendLast( serialTxState *txState )
{
    TXREG = (txState->buffer)[txState->txByte] | 0x80;
    txState->txByte++;
    txState->txState = BM_SERIAL_END;
}

/* The End (BM_SERIAL_END) State */
void endSerial_Cmpl( serialTxState *txState )
{
    txState->txState = BM_SERIAL_READY;
}

typedef void (*serialPortAction_ptr_t)( serialTxState *txState );

serialPortAction_ptr_t serialPortStateTbl[][4] = {
/*                   Start           ctlChar           last            TxCmpl */
/* READY    */ {startSerial,     doNothingSerial, doNothingSerial, doNothingSerial},
/* START    */ {doNothingSerial, startFrmSerial, startFrmSerial, startFrmSerial },
/* FRMTYPE  */ {doNothingSerial, frameTypeSerial_Send, doNothingSerial, frameTypeSerial_Send},
/* SEND     */ {doNothingSerial, frameTypeSerial_Ctl, sendSerial_Last, sendSerial_Send},
/* SEND_ESC */ {doNothingSerial, escSerial_Send, escSerial_SendLast, escSerial_Send},
/* END      */ {doNothingSerial, endSerial_Cmpl, endSerial_Cmpl, endSerial_Cmpl}
};

void procTxSerialState( serialTxState *txState, uint8_t event )
{
    serialPortStateTbl[txState->txState][event]( txState );
}

/*
** Rx Serial Port
*/

/*
** Serial Rx Action Functions
*/

typedef void (*serialRxPortAction_ptr_t)( serialRxState *rxState, uint8_t byt );

void doNothingRxSerial( serialRxState *rxState, uint8_t byt )
{
    /* Do nothing */
}

/* Rx WAIT State */
void startRxSerial( serialRxState *rxState, uint8_t byt )
{
    rxState->rxByte = 0;
    rxState->rxState = BM_RX_SERIAL_READ;
}

/* Rx READ State */
void readSTXRxSerial( serialRxState *rxState, uint8_t byt )
{
    /* This is an error */
    rxState->rxState = BM_RX_SERIAL_WAIT;
}

void readCharRxSerial( serialRxState *rxState, uint8_t byt )
{
    if( rxState->rxByte < BEERMON_RX_BUFSIZE )
    {
        rxState->buffer[rxState->rxByte++] = byt;
    }
    else
    {
        /* Buffer overflow */
        rxState->rxState = BM_RX_SERIAL_WAIT;
    }
}

void readDLERxSerial( serialRxState *rxState, uint8_t byt )
{
    rxState->rxState = BM_RX_SERIAL_DLE;
}

void readETXRxSerial( serialRxState *rxState, uint8_t byt )
{
    rxState->rxState = BM_RX_SERIAL_END;
}

/* Rx DLE State */
void dleCharRxSerial( serialRxState *rxState, uint8_t byt )
{
    if( rxState->rxByte < BEERMON_RX_BUFSIZE )
    {
        rxState->buffer[rxState->rxByte++] = (byt & 0x7F);
        rxState->rxState = BM_RX_SERIAL_READ;
    }
    else
    {
        /* Buffer overflow */
        rxState->rxState = BM_RX_SERIAL_WAIT;
    }
}

/* Rx END State */
void endProcFrmRxSerial( serialRxState *rxState, uint8_t byt )
{
    rxState->rxState = BM_RX_SERIAL_WAIT;
}

serialRxPortAction_ptr_t serialPortRxStateTbl[][5] = {
    /*                STX            RxChar           DLE                ETX        ProcFrm*/
    /* WAIT */ {startRxSerial, doNothingRxSerial, doNothingRxSerial ,doNothingRxSerial, doNothingRxSerial},
    /* READ */ {readSTXRxSerial, readCharRxSerial, readDLERxSerial ,readETXRxSerial, doNothingRxSerial},
    /* DLE  */ {readSTXRxSerial, dleCharRxSerial, readSTXRxSerial ,readSTXRxSerial, doNothingRxSerial},
    /* END  */ {doNothingRxSerial, doNothingRxSerial, doNothingRxSerial ,doNothingRxSerial, endProcFrmRxSerial}
};

void initRxState( serialRxState *rxState )
{
    memset( rxState, 0x00, sizeof(serialRxState) );
}

void procRxSerialState( serialRxState *rxState, uint8_t event, uint8_t byt )
{
    serialPortRxStateTbl[rxState->rxState][event]( rxState, byt );
}

void procRxSerial( serialRxState *rxState, uint8_t byt )
{
    if( byt == BM_SERIAL_STX )
    {
        procRxSerialState( rxState, BM_RX_SERIAL_EVT_STX, byt );
    }
    else if(byt == BM_SERIAL_ETX )
    {
        procRxSerialState( rxState, BM_RX_SERIAL_EVT_ETX, byt );
    }
    else if( byt == BM_SERIAL_DLE )
    {
        procRxSerialState( rxState, BM_RX_SERIAL_EVT_DLE, byt );
    }
    else
    {
        procRxSerialState( rxState, BM_RX_SERIAL_EVT_RXCHAR, byt );
    }
}

void doneRxSerial( serialRxState *rxState )
{
    procRxSerialState( rxState, BM_RX_SERIAL_EVT_PROCFRM, 0x00 );
}

#endif /* BEERCHIP_USE_SERIAL */
