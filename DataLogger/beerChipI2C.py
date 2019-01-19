import smbus
from time import sleep

import beerChipI2CAddresses as i2cInfo
from beerChip import beerChip

class beerChipI2C( beerChip ):
    errorCnt = 4

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

    def __init__(self, i2cDev, i2cAddr):
        self.i2cAddr = i2cAddr
        self.i2cDev = i2cDev
        self.bus = None

        # Try to communicate
        try:
            self.bus = smbus.SMBus(self.i2cDev)
            product = self.bus.read_byte_data( self.i2cAddr,
                                               i2cInfo.beerChipI2CCmdAddr['BEERCHIP_I2C_PROD_INDEX'])
            if( product == i2cInfo.beerChipI2CConstants['BEERCHIP_PRODUCT_ID'] ):
                print( 'INFO: Connected to beerChip!' )
            else:
                print( 'ERROR: Could not connect to beerChip product')
        except:
            print( 'ERROR: Could not connect to beerChip I2C')

    def getVersion(self):
        maj = self.bus.read_byte_data( self.i2cAddr,
                                       i2cInfo.beerChipI2CCmdAddr['BEERCHIP_I2C_MAJ_VER_INDEX'])
        min = self.bus.read_byte_data( self.i2cAddr,
                                       i2cInfo.beerChipI2CCmdAddr['BEERCHIP_I2C_MIN_VER_INDEX'])
        bld = self.bus.read_byte_data( self.i2cAddr,
                                       i2cInfo.beerChipI2CCmdAddr['BEERCHIP_I2C_PROD_BUILD_INDEX'])
        return '%d.%d.%d' % (maj,min,bld)

    def getTemperature(self, probeId, type = 'NTC_00'):
        temp = 0.0
        if( type == 'NTC_00'):
            if( probeId < len(i2cInfo.beerChipTemperatureProbe) ):
                self.bus.write_byte_data(self.i2cAddr,
                                         i2cInfo.beerChipTemperatureProbe[probeId]['Trigger'],
                                         0x01)
                tempWord = self.bus.read_word_data( self.i2cAddr,
                                                    i2cInfo.beerChipTemperatureProbe[probeId]['TempIndex'] )
                if( tempWord & 0x8000 ):
                    # Temperature is negative
                    tempWord = (~tempWord + 1) & 0xFFFF
                    temp = -1.0 * float(tempWord) / 256.0
                else:
                    temp = float(tempWord) / 256.0
            else:
                print( 'ERROR: Bad probe Id: %d' % (probeId) )

        elif( type == 'setpoint' ):
            tempWord = self.bus.read_word_data( self.i2cAddr, i2cInfo.beerChipI2CCmdAddr[BEERCHIP_BEERMON_CFG_SETPT] )
            if (tempWord & 0x8000):
                # Temperature is negative
                tempWord = (~tempWord + 1) & 0xFFFF
                temp = -1.0 * float(tempWord) / 256.0
            else:
                temp = float(tempWord) / 256.0

        else:
            print( 'ERROR: Invalid probe type %s' % (type) )


        return temp

    def getControlProbeChan(self ):
        self.bus.read_word_data(self.i2cAddr, i2cInfo.beerChipI2CCmdAddr['BEERCHIP_BEERMON_CFG_SETPT'])
        cntlChan = self.bus.read_byte_data( self.i2cAddr, i2cInfo.beerChipI2CCmdAddr['BEERCHIP_BEERMON_CFG_CNTL_PROBE'] )
        return cntlChan

    def getUptime(self ):
        low = self.bus.read_word_data( self.i2cAddr, i2cInfo.beerChipI2CCmdAddr['BEERCHIP_I2C_UPTIME_BYTE0'] )
        hi = self.bus.read_word_data( self.i2cAddr, i2cInfo.beerChipI2CCmdAddr['BEERCHIP_I2C_UPTIME_BYTE0'] + 2 )
        uptime = 65536 * hi + low
        return uptime

    def setControlProbeChan(self, chan ):
        self.bus.write_byte_data( self.i2cAddr,
                                  i2cInfo.beerChipI2CCmdAddr['BEERCHIP_BEERMON_CFG_CNTL_PROBE'],
                                  chan )

        val = self.getControlProbeChan()
        errCnt = 0
        while( (val != chan) and (errCnt < self.errorCnt) ):
            sleep( 1.0 )
            val = self.getControlProbeChan()
            errCnt = errCnt + 1
            print( 'chan = %d val = %d errcnt = %d' % (chan, val, errCnt) )
        val = self.getControlProbeChan()
        if( val == chan ):
            return True
        else:
            return False

    def switchIn( self ):
        self.bus.write_byte_data( self.i2cAddr,
                                  i2cInfo.beerChipI2CCmdAddr['BEERCHIP_BEERMON_CNTL_CMD'],
                                  self.controlCommands['BEERMON_CONTROL_MSG_SWITCH_IN'] )

    def switchOut( self ):
        self.bus.write_byte_data( self.i2cAddr,
                                  i2cInfo.beerChipI2CCmdAddr['BEERCHIP_BEERMON_CNTL_CMD'],
                                  self.controlCommands['BEERMON_CONTROL_MSG_SWITCH_OUT'] )

    def getState( self ):
        dat = self.bus.read_word_data( self.i2cAddr, i2cInfo.beerChipI2CCmdAddr['BEERCHIP_BEERMON_STATE_STATE'] )
        if( dat in self.controlStates ):
            return self.controlStates[dat]
        else:
            return 'Error'




if( __name__ == '__main__'):
    bc = beerChipI2C( 1, 0x2e )
    print( 'Version: %s' % (bc.getVersion()) )

    while( True ):
        print( '%fF' % (bc.getTemperature(1) ) )

