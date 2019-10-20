#!/usr/bin/python
import sys
import time
from datetime import datetime, timedelta

from beerChipDB import beerChipSQLiteDB as beerDB

def usage():
    print( '' )
    print( 'Usage:')
    print( '    python logTemp.py "DatasetName" ')
    print( '' )

if __name__ == '__main__':
    dbConn = beerDB()
    dbConn.connect( '/opt/beermon/data/beermonSQLite3DB.db' )

    if( len(sys.argv) != 2 ):
        print( 'ERROR: Usage' )
        usage()
        sql = "SELECT proj_name FROM Project"
        print( 'Datasets:')
        for proj in dbConn.query( sql ):
            print( '\t%s' % proj[0] )

        dbConn.close()
        sys.exit(1)