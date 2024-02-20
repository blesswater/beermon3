#!/usr/bin/python
import sys
import os
import getpass

# from beerChipI2C import beerChipI2C as beerChip
from beerChipRedis import beerChipRedis as beerChip
from beerChipDB import beerChipSQLiteDB as beerDB
from beerChipPersistant import beerChipPersistant

def usage():
    print( '' )
    print( 'Usage:')
    print( '    python setActiveProject.py "DatasetName" ')
    print( '' )

if __name__ == '__main__':
    bc = beerChip( 1, 0x2e )
    dbConn = beerDB()
    dbConn.connect( '/opt/beermon/data/beermonSQLite3DB.db' )

    bcPersist = beerChipPersistant( bc )

    if( len(sys.argv) != 2 ):
        print( 'ERROR: Usage' )
        usage()
        sql = "SELECT proj_name FROM Project"
        print( 'Datasets:')
        for proj in dbConn.query( sql ):
            print( '\t%s' % proj[0] )

        dbConn.close()
        sys.exit(1)

    if( getpass.getuser() != 'root' ):
        print( '' )
        print( 'Must be root to run this program' )
        print( '' )
        dbConn.close()
        sys.exit(1)

    projName = sys.argv[1]
    sql = "SELECT id FROM Project WHERE upper(proj_name) = '%s'" % (projName.upper())
    id = None
    for row in dbConn.query( sql ):
        id = row[0]

    if( id == None ):
        print( 'ERROR: Cannot find Dataset %s' % (projName) )
        print( 'Available Datasets' )
        sql = "SELECT proj_name FROM Project"
        for proj in dbConn.query(sql):
            print('\t%s' % (proj[0]) )

        dbConn.close()
        sys.exit(1)

    dbConn.close()

    bcPersist.setProject( projName )

    cmd = 'systemctl restart beermon-datacollect.service'
    os.system( cmd )

