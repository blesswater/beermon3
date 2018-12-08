import sys
import time

from beerChipI2C import beerChipI2C as beerChip
from beerChipDB import beerChipSQLiteDB as beerDB

def usage():
    print( '' )
    print( 'Usage:')
    print( '    python logTemp.py "DatasetName" ')
    print( '' )

if __name__ == '__main__':
    bc = beerChip( 1, 0x2e )
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

    probes = []
    sql = "SELECT probe_name, probe_chan, id FROM Probes WHERE proj_id = %d" % (id)
    for prb in dbConn.query( sql ):
        probes.append( { 'probe_name' : prb[0], 'chan' : prb[1], 'probe_id' : prb[2], 'data' : 0.0 } )

    if( len( probes ) == 0 ):
        print( 'ERROR: Could not file data sensors for dataset %s' % projName )
        dbConn.close()
        sys.exit(1)

    while( True ):
        for prb in probes:
            prb['data'] = bc.getTemperature(prb['chan'])
            print('%s: %fF' % (prb['probe_name'], prb['data']))

            sql =  "INSERT INTO Temperature (proj_id, probe_id, temp_time, temp) VALUES ( "
            sql += "%d, " % (id)
            sql += "%d, " % (prb['probe_id'])
            sql += "datetime( 'now' ), "
            sql += "%f " % (prb['data'])
            sql += ")"

            dbConn.execute( sql )

        print( '---------' )

        time.sleep(5)








