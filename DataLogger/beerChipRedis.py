"from time import sleep

import redis

from beerChip import beerChip

class beerChipRedis( beerChip ):

    controlCommands = {
        'BEERMON_CONTROL_MSG_ACK' : 0x00,
        'BEERMON_CONTROL_MSG_SWITCH_IN' : 0x01,
        'BEERMON_CONTROL_MSG_SWITCH_OUT' : 0x02,
        'BEERMON_CONTROL_MSG_EXTERN_IN' : 0x03
    }

    controlStates = {
        0 : 'Switched Out',
        1 : 'Off',
        2 : 'On Debounce',
        3 : 'On',
        4 : 'Off Debounce',
        5 : 'Ext Cntl'
    }

    bloopCnt = 0
    bloopAckCnt = 0

    maxTempRange = 114.0
    minTempRange = -20.0

    def __init__(self, notUsed0 = None, notUsed1 = None):
        # Try to communicate
        self.red = None
        try:
            self.red = redis.Redis()
        except:
            print( 'ERROR: Could not connect to Redis Server')

    def getVersion(self):
        ver = '0.0.0'
        try:
            ver = self.red.get( 'beermonVersion' )
        except:
            print( 'ERROR: Cannot get Version from Redis Server')
        return ver

    def getTemperature(self, probeId, type = 'NTC_00'):
        temp = 0.0
        if( type == 'NTC_00'):
            temp = self.red.hget( 'beermonTemp', str(probeId) )
        elif( type == 'setpoint' ):
            temp = self.red.hget( 'beermonSetpoint', 'temp' )
        else:
            print( 'ERROR: Invalid probe type %s' % (type) )
        return float(temp)

    def getControlProbeChan(self ):
        cntlChan = self.red.hget( 'beermonSetpoint', 'chan')
        return int(cntlChan)

    def getUptime(self ):
        uptime = int( self.red.get( 'beermonTimetick') )
        return uptime

    def setControlProbeChan(self, chan ):
        self.red.set( 'beermonSetProbe', chan )
        return True

    def switchIn( self ):
        self.red.set( 'beermonSetState', 'Switch In' )

    def switchOut( self ):
        self.red.set('beermonSetState', 'Switch Out')

    def extCntl(self):
        self.red.set('beermonSetState', 'Ext Control')

    def getState( self ):
        state = self.red.get( 'beermonState' )
        return 'Error'

    def getSetpoint( self, chan = 0 ):
        temp = self.red.hget( 'beermonSetpoint', 'temp' )
        return float(temp)

    def setSetpoint( self, temp ):
        self.red.set( 'beermonSetSetpoint', temp )

    def ackBloopDet(self):
        pass

    def resetBloopDet( self ):
        self.bloopCnt = 0
        self.bloopAckCnt = 0

    def isBloopDet(self):
        blp = 0
        if( blp == 1 ):
            self.bloopCnt += 1
            return True
        else:
            return False

    def getCountBloopDet(self):
        return (self.bloopCnt, self.bloopAckCnt)



if( __name__ == '__main__'):
    bc = beerChipRedis()
    print( 'Version: %s' % (bc.getVersion()) )

    while( True ):
        if( bc.isBloopDet() ):
            (blp, blpAck) = bc.getCountBloopDet()
            print( "BLOOP %d" % blp )
            bc.ackBloopDet()

        # print( '%fF' % (bc.getTemperature(1) ) )

"