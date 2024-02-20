#!/usr/bin/python

import sys
import time

from beermonConfig import beermonConfig
# from beerChipI2C import beerChipI2C as beerChip
from beerChipRedis import beerChipRedis as beerChip
from beerChipPersistant import beerChipPersistant

def usage():
    print ( "" )
    print( "Usage: python beerChipBePersistant.py <checkPeriod>" )
    print( "    Where: checkPeriod -> " )
    print( "                  Number of seconds between reconciliation" )
    print( "                  between database and beerChip" )
    print( "    checkPeriod = 0 means reconcile once, then return ")
    print( "    checkPeriod = 300 -> 5min" )
    print( "    checkPeriod = 1800 -> 1/2hr" )
    print( "    checkPeriod = 3600 -> 1hr" )
    print( "    checkPeriod = 7200 -> 2hr" )
    print( "    checkPeriod = 18000 -> 5hr" )
    print( "    checkPeriod = 36000 -> 10hr" )
    print( "    checkPeriod = 86400 -> 24hr")
    print( "" )

if __name__ == '__main__':
    bc = beerChipPersistant( beerChip( beermonConfig['i2cBus'], beermonConfig['i2cAddr'] ) )

    if( len( sys.argv ) != 2 ):
        usage()
        sys.exit( 1 )

    bc.verifyConfig()
    sleepTime = int( sys.argv[1] )
    if( sleepTime != 0 ):
        while( True ):
           time.sleep( sleepTime )
           bc.verifyConfig()

    sys.exit( 0 )

