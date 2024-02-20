#!/usr/bin/python
import sys
import time

from beerChipDB import beerChipSQLiteDB as beerDB

def usage():
    print( '' )
    print( 'Usage:')
    print( '    python enterManualTemp.py <probeID> <temperature> ')
    print( '' )

if __name__ == '__main__':
    dbConn = beerDB()
    dbConn.connect( '/opt/beermon/data/beermonSQLite3DB.db' )

    if( len(sys.argv) != 3 ):
        print( 'ERROR: Usage' )
        usage()
        sql  = "SELECT prb.id, proj.proj_name, prb.probe_name "
        sql += "FROM Probes prb "
        sql += "JOIN Project proj ON prb.proj_id=proj.id "
        sql += "WHERE prb.type='MANUAL'"

        print( 'Probe Ids:')
        for row in dbConn.query( sql ):
            print( '\t%d  %s  %s' % (row[0], row[1], row[2]) )

        dbConn.close()
        sys.exit(1)

    try:
        probeId = int( sys.argv[1] )
    except:
        print( 'ERROR: Bad Probe Id: %s' % (sys.argv[1]) )
        dbConn.close()
        sys.exit()

    try:
        tValue = float( sys.argv[2] )
    except:
        print( 'ERROR: Invalid temperature data: %s' % (sys.argv[2]) )
        dbConn.close()
        sys.exit()

    # Get the project id
    sql  = "SELECT proj_id FROM Probes "
    sql += "WHERE id = %d" % (probeId)
    projId = None
    for row in dbConn.query( sql ):
        projId = row[0]

    if( projId == None ):
        print( 'ERROR: Cannot find project Id for probe %d' % (probeId) )
        dbConn.close()
        sys.exit()


    sql  = "INSERT INTO Temperature (proj_id, probe_id, temp_time, temp) "
    sql += "VALUES ( "
    sql += "%d, " % (projId)
    sql += "%d, " % (probeId)
    sql += "datetime( 'now' ), "
    sql += "%f " % (tValue)
    sql += ")"

    dbConn.execute( sql )

    dbConn.close()






