import struct

from Queue import Queue
from threading import Thread
import serial

import binascii

class serialPortStateMachine:
    STX = '\x02'
    ETX = '\x03'
    DLE = '\x10'

    def __init__( self, workQ, port, baud ):
        self.port = port
        self.baud = baud
        self.state = 0
        self.running = False
        self.frame = bytearray(b'')
        self.workQ = workQ

        self.serialPort = None
        self.running = False
        try:
            self.serialPort = serial.Serial(self.port, self.baud, timeout=2)
        except:
            print("ERROR: Cannot open serial Port %s for Rx" % (self.port) )

        self.stateMachine = [
        # Events         RxSTX             RxChar         RxDLE          RxETX
            # Wait 0
            [         self._GotoRead,  self._Nothing,    self._Nothing, self._Nothing],
            # Read 1
            [         self._AbortFrame, self._RxChar,    self._RxDLE, self._EndFrame],
            # DLE 2
            [         self._AbortFrame, self._RxCharDLE, self._Nothing, self._AbortFrame],
        ]

    def _Nothing( self, ch ):
        pass

    def _GotoRead( self, ch ):
        self.state = 1

    def _AbortFrame( self, ch ):
        self.frame = bytearray(b'') # Abort the frame
        self.state = 0

    def _RxChar( self, ch ):
        self.frame += bytearray(ch)

    def _RxDLE( self, ch ):
        self.state = 2

    def _RxCharDLE( self, ch ):
        # print( 'RxDLE ch = %s frm = %s' % (binascii.hexlify( ch ), binascii.hexlify(self.frame) ) )
        b = struct.unpack( 'b', ch )[0]
        b = b & 0x7F
        self.frame += bytearray([b])
        self.state = 1

    def _EndFrame( self, ch ):
        # print( binascii.hexlify( self.frame ) )
        self.workQ.put( bytearray(self.frame) )
        self.frame = bytearray(b'')
        self.state = 0

    def run( self ):
        self.running = True
        while self.running:
            c = self.serialPort.read(1)
            if( c ):
                if( c == serialPortStateMachine.STX ):
                    # print( 'STX' )
                    self.stateMachine[self.state][0]( c )
                elif( c == serialPortStateMachine.ETX ):
                    # print( 'ETX' )
                    self.stateMachine[self.state][3]( c )
                elif( c == serialPortStateMachine.DLE ):
                    # print( 'DLE' )
                    self.stateMachine[self.state][2]( c )
                else:
                    # print( c )
                    self.stateMachine[self.state][1]( c )

        if (self.serialPort != None):
            self.serialPort.close()
            self.serialPort = None

    def txFrame( self, frm ):
        txFrame = bytearray( [serialPortStateMachine.STX])
        for byt in frm:
            bytVal = chr(byt)
            if( bytVal == serialPortStateMachine.STX or
                bytVal == serialPortStateMachine.ETX or
                bytVal == serialPortStateMachine.DLE ):
                txFrame += bytearray([serialPortStateMachine.DLE])
                txFrame += bytearray([byt | 0x80])
            else:
                txFrame += bytearray( [byt])
        txFrame += bytearray( [serialPortStateMachine.ETX] )
        if( self.serialPort ):
            self.serialPort.write( txFrame )

    def stop( self ):
        self.running = False

class serialLink:

    def __init__( self, port, baud ):
        self.port = port
        self.baud = baud

        self.rxThread = -1
        self.rxQ = None
        self.rxSm = None

    def start( self ):
        try:
            self.rxQ = Queue()
            self.rxSm = serialPortStateMachine( self.rxQ, self.port, self.baud )
            self.rxThread = Thread( target=self.rxSm.run )
            self.rxThread.start()
        except:
            print( "ERROR: Problem starting Rx Link thread" )
            

    def stop( self ):
        self.rxSm.stop()
        self.rxThread.join()

    def rxFrame( self ):
        try:
            frm = self.rxQ.get( False )
        except:
            # Empty Q
            frm = None
        return frm

    def txFrame( self, frm ):
        self.rxSm.txFrame( frm )


if( __name__ == '__main__' ):
    rxLink = serialLink( '/dev/ttyUSB0', 9600 )
    rxLink.start()

    while True:
        frm = rxLink.rxFrame()
        if( frm ):
            print( "INFO: Received frame %s len: %d" % (chr(frm[0]), len(frm)) )

    rxLink.stop()

    """
    rxQueue = Queue()
    sm = serialPortStateMachine( rxQueue, '/dev/ttyUSB0', 9600 );
    sm.run()
    """
            
        
        

