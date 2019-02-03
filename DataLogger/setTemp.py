import sys
import time

from beerChipI2C import beerChipI2C as beerChip
from beerChipDB import beerChipSQLiteDB as beerDB

def usage():
    print( '' )
    print( 'Usage:')
    print( '    python setTemp.py <temperature in F> ')
    print( '' )

if __name__ == '__main__':
    bc = beerChip( 1, 0x2e )

    if( len(sys.argv) != 2 ):
        print( 'ERROR: Usage' )
        usage()
        sys.exit(1)

    try:
        temp = float( sys.argv[1] )
        bc.setPoint( temp )







