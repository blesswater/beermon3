import json

from beermonConfig import beermonConfig
from beerChip import beerChip
from beerChipDB import beerChipSQLiteDB as beerDB

class beerChipPersistant( beerChip ):

    def __init__( self, beerChip ):
        self.bc = beerChip
        self.cfg = self._readConfigFromDb()
        self.dirty = False
        if( self.cfg == None ):
            # No configuration in the DB. Read it from the card.
            self.cfg = {}
            self.cfg['switchIn'] = self._readSwitchInState()
            self.cfg['setpoint'] = self.bc.getSetpoint()
            self.cfg['cntlProbe'] = self.bc.getControlProbeChan()
            if( self.cfg['switchIn'] != 'error' ):
                self._writeConfigToDb()

    def _readSwitchInState( self ):
        stateMap = {
            'Switched Out' : 'Switched Out',
            'Off' : 'Switched In',
            'On Debounce' : 'Switched In',
            'On' : 'Switched In',
            'Off Debounce' : 'Switched In',
            'Ext Cntl' : 'Ext Cntl'
        }
        state = self.bc.getState()
        if( state in stateMap ):
            return stateMap[state]
        else:
            return 'error'

    def _readConfigFromDb( self ):
        dbConn = None
        try:
            dbConn = beerDB()
            dbConn.connect(beermonConfig['dbLocation'])

            sql = "SELECT value FROM Scalar "
            sql += "WHERE type = 'beerchip_config'"

            cfgRow = dbConn.fetchOne( sql, () )
            cfg = None
            if( cfgRow != None ):
                try:
                    cfg = json.loads( cfgRow[0] )
                except:
                    cfg = None
        except:
            cfg = None

        finally:
            if( dbConn != None ):
                dbConn.close()

        return cfg

    def _writeConfigToDb( self ):
        dbConn = None
        try:
            dbConn = beerDB()
            dbConn.connect(beermonConfig['dbLocation'])

            cfgStr = json.dumps( self.cfg )

            sql =  "UPDATE Scalar SET value = '%s' " %(cfgStr)
            sql += "WHERE type = 'beerchip_config'"

            cnt = dbConn.execute( sql )
            if( cnt == 0 ):
                # Entry does not exist
                sql =  "INSERT INTO Scalar (type, value) "
                sql += "VALUES( 'beerchip_config', '%s' ) " % (cfgStr)
                dbConn.execute( sql )

            self.dirty = False
        finally:
            if( dbConn != None ):
                dbConn.close()



    def getVersion(self):
        return self.bc.getVersion()

    def getTemperature(self, probeId):
        return self.bc.getTemperature( probeId )

    def getControlProbeChan(self ):
        return self.bc.getControlProbeChan()

    def getUptime(self):
        return self.bc.getUptime()

    def setControlProbeChan( self, chan ):
        self.cfg['cntlChan'] = chan
        self.dirty = True
        self.bc.setControlProbeChan( chan )
        self._writeConfigToDb()
        self.dirty = False

    def switchIn(self):
        self.cfg['switchIn'] = 'Switched In'
        self.dirty = True
        self.bc.switchIn()
        self._writeConfigToDb()


    def switchOut(self):
        self.cfg['switchIn'] = 'Switched Out'
        self.dirty = True
        self.bc.switchOut()
        self._writeConfigToDb()

    def getState(self):
        return self.bc.getState()

    def getSetpoint(self, chan=0):
        return self.bc.getSetpoint( chan )

    def setSetpoint(self, temp):
        self.cfg['setpoint'] = temp
        self.dirty = True
        self.bc.setSetpoint( temp )
        self._writeConfigToDb()

    def verifyConfig(self ):
        cfg = self._readConfigFromDb()
        if( cfg != None ):
            self.bc.setSetpoint( cfg['setpoint'] )
            self.bc.setControlProbeChan( cfg['cntlProbe'] )
            if( cfg['switchIn'] == 'Switched In' ):
                self.bc.switchIn()
            elif( cfg['switchIn'] == 'Switched Out' ):
                self.bc.switchOut()
            elif( cfg['switchIn'] == 'Ext Cntl' ):
                self.bc.extCntl()
            else:
                pass
