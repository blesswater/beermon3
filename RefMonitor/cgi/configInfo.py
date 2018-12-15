from beermonConfig import beermonConfig
from beerChipI2C import beerChipI2C as beerChip
from beerChipDB import beerChipSQLiteDB as beerDB

def getConfigInfo( data ):

    activityMap = ['stale', 'collecting']
    dbConn = None
    result = {}
    try:
        dbConn = beerDB()
        dbConn.connect( beermonConfig['dbLocation'] )

        result['version'] = '%d.%d.%d' % (beermonConfig['majVersion'], beermonConfig['minVersion'], beermonConfig['buildVersion'])

        bc = beerChip( beermonConfig['i2cBus'], beermonConfig['i2cAddr'] )
        cntlChan = bc.getControlProbeChan()
        result['uptime'] = bc.getUptime()

        dataset = []
        sql =  "SELECT id, proj_name, activity FROM Project "
        sql += "ORDER BY activity desc"
        for row in dbConn.query( sql ):
            probes = []
            if( row[2] < len(activityMap) ):
                act = activityMap[row[2]]
            else:
                act = 'err'
            dat = { 'id' : row[0], 'name' : row[1], 'activity' : act, 'probes' : [] }

            probeSql =  "SELECT id, probe_name, probe_chan, type, min_range, max_range FROM Probes "
            probeSql += "WHERE proj_id = %d" % (row[0])
            for prb in dbConn.query( probeSql ):
                probe = { 'id' : prb[0],
                          'name' : prb[1],
                          'chan' : prb[2],
                          'type' : prb[3],
                          'min_range' : prb[4],
                          'maxRange' : prb[5] }
                if( probe['chan'] == cntlChan ):
                    probe['cntl'] = True
                else:
                    probe['cntl'] = False
                dat['probes'].append( probe )

            dataset.append( dat )
        result['dataset'] = dataset
        result['result'] = 'OK'
    except:
        result['result'] = 'Error'

    return result


if( __name__ == '__main__' ):
    getConfigInfo( {} )
