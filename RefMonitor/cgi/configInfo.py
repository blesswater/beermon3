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
        result['state'] = {}
        result['state']['controlState'] = bc.getState()

        dataset = []
        sql =  "SELECT id, proj_name, activity FROM Project "
        sql += "ORDER BY activity desc"
        for row in dbConn.query( sql ):
            if( row[2] < len(activityMap) ):
                act = activityMap[row[2]]
            else:
                act = 'err'
            dat = { 'id' : row[0], 'name' : row[1], 'activity' : act, 'probes' : [] }

            setpoints = []
            hasSp = False

            probeSql =  "SELECT id, probe_name, probe_chan, type, min_range, max_range, control_probe FROM Probes "
            probeSql += "WHERE proj_id = %d" % (row[0])
            for prb in dbConn.query( probeSql ):
                probe = { 'id' : prb[0],
                          'name' : prb[1],
                          'chan' : prb[2],
                          'type' : prb[3],
                          'min_range' : prb[4],
                          'max_range' : prb[5],
                          'cntl_able' : prb[6]}
                if( probe['chan'] == cntlChan ):
                    probe['cntl'] = True
                    hasSp = True

                else:
                    probe['cntl'] = False
                dat['probes'].append( probe )

            if( hasSp ):
                setpoints.append({'chan': 0,
                                  'value': bc.getSetpoint(0),
                                  'min_range': 20.0,
                                  'max_range': 90.0
                                  })
            dat['setpoints'] = setpoints
            dataset.append( dat )

        result['dataset'] = dataset
        result['result'] = 'OK'
    except:
        result['result'] = 'Error'

    if( dbConn != None ):
        dbConn.close()

    return result

def setConfigInfo( data ):
    result = { 'result': 'ERROR: setConfigInfo()'}
    bc = beerChip(beermonConfig['i2cBus'], beermonConfig['i2cAddr'])

    if( 'controlProbe' in data ):
        prbInfo = data['controlProbe']
        if( ('dataSetId' in prbInfo) and ('probeId' in prbInfo) ):
            try:
                dbConn = beerDB()
                dbConn.connect(beermonConfig['dbLocation'])

                sql =  "SELECT id, probe_chan, control_probe FROM Probes "
                sql += "WHERE proj_id = ? AND id = ?"

                row = dbConn.fetchOne( sql, (int(prbInfo['dataSetId']), int(prbInfo['probeId'])) )

                if( row ):
                    if( row[2] == 1 ): # If configured for control
                        if( bc.setControlProbeChan( int(row[1]) ) ):
                            result['result'] = 'OK'
                        else:
                            result['result'] = 'ERROR: Error writing chan %d to control chip' % (int(row[1]))
                    else:
                        result['result'] = 'ERROR: Probe not configured for control'
                else:
                    result['result'] = 'ERROR: Dataset/Probe not found.'

            except:
                result = {'result': 'ERROR: trying setConfigInfo()'}

            finally:
                if (dbConn != None):
                    dbConn.close()

    if( 'activate' in data ):
        if( data['activate'] ):
            bc.switchIn()
        else:
            bc.switchOut()
        result = {'result': 'OK'}

    return result




if( __name__ == '__main__' ):
    getConfigInfo( {} )
