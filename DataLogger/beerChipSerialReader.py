#!/usr/bin/python

import binascii

import redis
from beerChipSerialLink import serialLink

def byte8dot8ToFloat( u16Val ):
    if( u16Val & 0x8000 ):
        # Negative
        u16Val = ((u16Val ^ 0xFFFF) & 0xFFFF) + 1
        val = -1.0 * float( u16Val ) / 256.0
    else:
        val = float( u16Val ) / 256.0
    return val

if( __name__ == '__main__' ):
    link = serialLink( '/dev/ttyUSB0', 9600 )
    link.start()
    red = redis.Redis()

    controlStates = {
        0 : 'Switched Out',
        1 : 'Off',
        2 : 'On Debounce',
        3 : 'On',
        4 : 'Off Debounce',
        5 : 'Ext Cntl'
    }

    reverseControlStates = {
        'Switched Out' : 0,
        'Off' : 1,
        'On Debounce' : 2,
        'On' : 3,
        'Off Debounce' : 4,
        'Ext Cntl' : 5
    }

    while True:
        frm = link.rxFrame()
        if( frm ):
            print( "INFO: Received frame %s len: %d" % (frm[0], len(frm)) )
            print( 'INFO: Frame %s' % (binascii.hexlify(frm)) )
            if( chr(frm[0]) == 'V' ):
                ver = '%d.%d.%d' % (frm[2], frm[3], frm[4])
                print( 'INFO: Version = %s' % (ver) )
                red.set( 'beermonVersion', ver )
            elif( chr(frm[0]) == 't' ):
                t = (frm[4] * 16777216) + (frm[3] * 65536) + (frm[2] * 256) + frm[1]
                print( 'INFO: Up time = %d' % (t) )
                red.set( 'beermonTimetick', t )
            elif( chr(frm[0]) == 'T' ):
                ch = frm[1]
                T = byte8dot8ToFloat( (frm[3] * 256) + frm[2] )
                # T = float(frm[3] * 254 + frm[2]) / 256.0
                print( 'INFO: ch: %d Temp: %f F' % (ch, T) )
                red.hset( 'beermonTemp', str(ch), T )
            elif( chr(frm[0]) == 'C' ):
                sp = byte8dot8ToFloat( frm[2] * 256 + frm[1] )
                # sp = float((frm[2] * 256 + frm[1])) / 256.0
                probe = str(frm[9])
                red.hset( 'beermonSetpoint', 'temp', sp )
                red.hset( 'beermonSetpoint', 'chan', probe )
                onDebounce = frm[4] * 256 + frm[3]
                offDebounce = frm[6] * 256 + frm[5]
                red.set( 'beermonOndebounce', onDebounce )
                red.set( 'beermonOffdebounce', offDebounce )
            elif( chr(frm[0]) == 'S' ):
                stateVal = int(frm[1])
                if( stateVal in controlStates ):
                    red.set( 'beermonState', controlStates[stateVal] )
                    print( 'INFO: Beermon State: %s' % (controlStates[stateVal]) )
                else:
                    print( 'INFO: Bad state value %d' % (stateVal) )
            elif( chr(frm[0]) == 'B' ):
                print( 'INFO: Bloop' )
                red.set( 'beermonBloopSet', 1 )
                blp = red.get( 'beermonBloopCnt' )
                if( blp ):
                    red.incr( 'beermonBloopCnt' )
                else:
                    red.set( 'beermonBloopCnt', 1 )
            else:
                print( 'INFO: Unknown Frame %s' % (chr(frm[0])) )

        newSetPoint = red.get( 'beermonSetSetpoint' )
        if( newSetPoint ):
            red.delete( 'beermonSetSetpoint' )
            frm = bytearray( ['s'] )
            fval = float(newSetPoint)
            if( fval >= 0.0 ):
                val = int( fval * 256 )
                if( val < 65535 ):
                    frm += bytearray( [val & 0xFF] )
                    frm += bytearray( [val >> 8] )
            else:
                fval = -fval
                val = int(fval * 256)
                if( val < 65535 ):
                    val = ((val ^ 0xFFFF) & 0xFFFF) + 1
                    frm += bytearray( [val & 0xFF] )
                    frm += bytearray( [val >> 8] )

            print( 'Sending %s' % (binascii.hexlify( frm )) )
            link.txFrame( frm )
        
        newSetProbe = red.get( 'beermonSetProbe' )
        if( newSetProbe ):
            red.delete( 'beermonSetProbe' )
            frm = bytearray( ['p'] )
            frm += bytearray( [int(newSetProbe) & 0xFF] )

            print( 'Sending %s' % (binascii.hexlify( frm )) )
            link.txFrame( frm )

        newSetState = red.get( 'beermonSetState' )
        if( newSetState ):
            red.delete( 'beermonSetState' )
            frm = bytearray( ['m'] )
            if( 'in' in newSetState.lower() ):
                frm += bytearray( [0x01] )
            elif( 'out' in newSetState.lower() ):
                frm += bytearray( [0x02] )
            elif( 'ext' in newSetState.lower() ):
                frm += bytearray( [0x03] )
            else:
                frm += bytearray( [0x00] )

            print( 'Sending %s' % (binascii.hexlify( frm )) )
            link.txFrame( frm )

        cntlRelay = red.hget( 'beermonSetRelay', 'enable' )
        if( cntlRelay ):
            red.hdel( 'beermonSetRelay', 'enable' )
            frm = bytearray( ['r'] )
            frm += bytearray( [0x01] )
            if( 'on' in cntlRelay.lower() ):
                frm += bytearray( [0x01] )
            elif( 'off' in cntlRelay.lower() ):
                frm += bytearray( [0x00] )
            if( len( frm ) == 3 ):
                print( 'Sending %s' % (binascii.hexlify( frm )) )
                link.txFrame( frm )

        cntlRelay = red.hget( 'beermonSetRelay', 'control' )
        if( cntlRelay ):
            red.hdel( 'beermonSetRelay', 'control' )
            frm = bytearray( ['r'] )
            frm += bytearray( [0x02] )
            if( 'on' in cntlRelay.lower() ):
                frm += bytearray( [0x02] )
            elif( 'off' in cntlRelay.lower() ):
                frm += bytearray( [0x00] )
            if( len( frm ) == 3 ):
                print( 'Sending %s' % (binascii.hexlify( frm )) )
                link.txFrame( frm )
                



    link.stop()


