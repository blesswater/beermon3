from datetime import datetime, timedelta
import math

from beermonConfig import beermonConfig
from beerChipI2C import beerChipI2C as beerChip
from beerChipDB import beerChipSQLiteDB as beerDB

def getTempStat( data ):
    # import pdb; pdb.set_trace()
    dbConn = None
    result = {}
    try:
        dbConn = beerDB()
        dbConn.connect( beermonConfig['dbLocation'] )

        bc = beerChip( beermonConfig['i2cBus'], beermonConfig['i2cAddr'] )
        result['uptime'] = bc.getUptime()
        cntlChan = bc.getControlProbeChan()
        result['datasetId'] = data['datasetId']
        result['probes'] = []

        sql =  "SELECT id, probe_name, probe_chan, type, min_range, max_range, control_probe FROM Probes "
        sql += "WHERE proj_id = ?"

        for row in dbConn.querySafe( sql, (int(data['datasetId']),) ):
            probeData = { 'id' : row[0],
                          'name' : row[1],
                          'min_range': row[4],
                          'max_range': row[5],
                          'cntl_able' : row[6]}
            if( row[2] == cntlChan ):
                probeData['cntl'] = True
            else:
                probeData['cntl'] = False

            probeWhere = []
            tupleWhere = (data['datasetId'],)
            probeWhere.append( "probe_id = %d" % row[0] )


            startts = datetime.now()
            if( 'startTime' in data ):
                # startts = datetime.fromisoformat( data['startTime'] )
                startts = datetime.strptime(data['startTime'], "%Y-%m-%dT%H:%M:%S.%fZ")
                probeWhere.append( "temp_time <= '%s'" % (startts.isoformat()) )

            if( 'endTime' in data ):
                # ts = datetime.fromisoformat( data['startTime'] )
                ts = datetime.strptime(data['endTime'], "%Y-%m-%dT%H:%M:%S.%fZ")
                probeWhere.append( "temp_time > '%s'" % (ts.isoformat()) )

            if( 'statInterval' in data ):
                ts = startts - timedelta( seconds = int(data['statInterval']) )
                probeWhere.append("temp_time > '%s'" % (ts.isoformat()))

            probeSql  = "SELECT "
            probeSql +=     "COUNT(*) as cnt, "
            probeSql +=     "AVG(temp) as avg, "
            probeSql +=     "SUM(temp * temp) as Ex2, "
            probeSql +=     "MIN(temp) as min, "
            probeSql +=     "MAX(temp) as max "
            probeSql += "FROM Temperature "
            probeSql += "WHERE (proj_id = ?) "
            if( len(probeWhere) > 0 ):
                probeSql += 'AND (' + ') AND ('.join(probeWhere) + ')'

            for prbRow in dbConn.querySafe( probeSql, tupleWhere ):
                if( prbRow[0] > 1 ):
                    probeData['avgTemp'] = prbRow[1]
                    probeData['sdevTemp'] = math.sqrt((prbRow[2] / prbRow[0]) - (probeData['avgTemp'] * probeData['avgTemp']))
                    # probeData['sdevTemp'] = result['uptime'] % 20
                    probeData['minTemp'] = prbRow[3]
                    probeData['maxTemp'] = prbRow[4]

                probeData['currentTemp'] = bc.getTemperature( row[2], row[3] )

                result['probes'].append( probeData )

        result['result'] = 'OK'
    except:
        result['result'] = 'ERROR: GetTempStat()'

    return result


if( __name__ == '__main__' ):
    getConfigInfo( {} )
