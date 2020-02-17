#!/usr/bin/python

import binascii

import redis
from beerChipSerialLink import serialLink

if( __name__ == '__main__' ):
    link = serialLink( '/dev/ttyUSB0', 9600 )
    link.start()
    red = redis.Redis()

    while True:
        frm = link.getFrame()
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
                T = float(frm[3] * 254 + frm[2]) / 256.0
                print( 'INFO: ch: %d Temp: %f F' % (ch, T) )
                red.hset( 'beermonTemp', str(ch), T )
            elif( chr(frm[0]) == 'C' ):
                sp = float((frm[2] * 256 + frm[1])) / 256.0
                probe = str(frm[9])
                red.hset( 'beermonSetpoint', 'temp', sp )
                red.hset( 'beermonSetpoint', 'chan', probe )
                onDebounce = frm[4] * 256 + frm[3]
                offDebounce = frm[6] * 256 + frm[5]
                red.set( 'beermonOndebounce', onDebounce )
                red.set( 'beermonOffdebounce', offDebounce )
            else:
                print( 'INFO: Unknown Frame %s' % (chr(frm[0])) )

            link.txFrame( frm )


    link.stop()


