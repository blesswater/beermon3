import smbus

import beerChipI2CAddresses as i2cInfo
from beerChip import beerChip

class beerChipI2C( beerChip ):
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

    def getTemperature(self, probeId):
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

        return temp

    def getControlProbeChan(self ):
        cntlChan = self.bus.read_byte_data( self.i2cAddr, i2cInfo.beerChipI2CCmdAddr['BEERCHIP_BEERMON_CFG_CNTL_PROBE'] )
        return cntlChan

    def getUptime(self ):
        low = self.bus.read_word_data( self.i2cAddr, i2cInfo.beerChipI2CCmdAddr['BEERCHIP_I2C_UPTIME_BYTE0'] )
        hi = self.bus.read_word_data( self.i2cAddr, i2cInfo.beerChipI2CCmdAddr['BEERCHIP_I2C_UPTIME_BYTE0'] + 2 )
        uptime = 65536 * hi + low
        return uptime


if( __name__ == '__main__'):
    bc = beerChipI2C( 1, 0x2e )
    print( 'Version: %s' % (bc.getVersion()) )

    while( True ):
        print( '%fF' % (bc.getTemperature(1) ) )

