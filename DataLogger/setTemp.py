#!/usr/bin/python

import sys

from beerChipI2C import beerChipI2C as beerChip
from beerChipPersistant import beerChipPersistant

def usage():
    print( '' )
    print( 'Usage:')
    print( '    python setTemp.py <temperature in F> ')
    print( '' )

if __name__ == '__main__':
    bc = beerChipPersistant( beerChip( 1, 0x2e ) )

    if( len(sys.argv) != 2 ):
        print( 'ERROR: Usage' )
        usage()
        sys.exit(1)

    try:
        temp = float( sys.argv[1] )
        bc.setSetpoint( temp )
    except:
        print( 'ERROR: Writing setpoint' )
