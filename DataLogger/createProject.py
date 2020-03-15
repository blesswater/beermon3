#!/usr/bin/python
import sys

# from beerChipI2C import beerChipI2C as beerChip
from beerChipRedis import beerChipRedis as beerChip
from beerChipDB import beerChipSQLiteDB as beerDB

projDescr = {
                'name' : 'RefrigeRun00',
                'probes' : [
                    { 'name' : 'RefrigTemp',
                      'chan' : 0,
                      'type' : 'NTC_00',
                      'control' : True,
                      'min_range' : 20.0,
                      'max_range' : 80.0
                    },
                    { 'name' : 'Setpoint',
                      'type' : 'setpoint',
                    },
                ]
           }
#
# New Project Example
#
"""
projDescr = {
                'name' : 'New Project',
                'probes' : [
                    { 'name' : 'Probe0',
                      'chan' : 0,
                      'type' : 'NTC_00',
                      'control' : True,
                      'min_range' : 20.0,
                      'max_range' : 80.0
                    },
                    { 'name' : 'Probe1',
                      'chan' : 1,
                      'control' : True,
                      'type' : 'NTC_00',
                    },
                    { 'name' : 'Probe2',
                      'chan' : 2,
                      'control' : True,
                      'type' : 'NTC_00',
                    },
                    { 'name' : 'Probe3',
                      'chan' : 3,
                      'control' : True,
                      'type' : 'NTC_00',
                    },
                    { 'name' : 'Setpoint',
                      'type' : 'setpoint',
                    },
                    { 'name' : 'Manual Probe',
                      'type' : 'MANUAL',
                      'control' : False,
                    }
                ]
           }
"""

def usage():
    print( '' )
    print( 'Usage:')
    print( '    python createProject.py')
    print( '' )

if __name__ == '__main__':
    dbConn = beerDB()
    dbConn.connect( '/opt/beermon/data/beermonSQLite3DB.db' )

    sql  = "SELECT * FROM Project WHERE UPPER(proj_name) = UPPER('%s')" % (projDescr['name'])
    row = dbConn.fetchOne( sql, () )
    if( row != None ):
        print( "ERROR: Project name %s already exists" % (projDescr['name']) )
        dbConn.close()
        sys.exit(1)

    sql  = "INSERT INTO Project (proj_name, collection_state) "
    sql += "VALUES( '%s', 'stale' )" % (projDescr['name'])
    dbConn.execute( sql )

    sql  = "SELECT id FROM Project WHERE proj_name = '%s' " % (projDescr['name'])
    row = dbConn.fetchOne( sql, () )
    if( not row ):
        print( "ERROR: Could not find created Project" )
        dbConn.close()
        sys.exit(1)

    projId = row[0]
    for prb in projDescr['probes']:
        if( 'min_range' not in prb ):
            prb['min_range'] = 20.0
        if( 'max_range' not in prb ):
            prb['max_range'] = 80.0
        if( 'control' not in prb ):
            prb['control'] = False

        if( prb['type'].lower() == 'setpoint' ):
            sql  = "INSERT INTO Probes (proj_id, probe_name, probe_chan, type, min_range, max_range, control_probe) "
            sql += "VALUES ( %d, '%s', 0, 'setpoint', %f, %f, 0)" % (projId, prb['name'], prb['min_range'], prb['max_range'])
        elif( prb['type'].lower() == 'manual' ):
            sql  = "INSERT INTO Probes (proj_id, probe_name, probe_chan, type, min_range, max_range, control_probe) "
            sql += "VALUES ( %d, '%s', 0, 'MANUAL', %f, %f, %d)" % (projId, prb['name'], prb['min_range'], prb['max_range'], prb['control'])
        else:
            # Normal
            sql  = "INSERT INTO Probes (proj_id, probe_name, probe_chan, type, min_range, max_range, control_probe) "
            sql += "VALUES ( %d, '%s', %d, '%s', %f, %f, %d)" % (projId, prb['name'], prb['chan'], prb['type'], prb['min_range'], prb['max_range'], prb['control'])

        dbConn.execute( sql )

    dbConn.close()
            
        



