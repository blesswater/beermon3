import beerChipI2CAddresses as i2cInfo

class beerChip:
    def getVersion(self):
        raise NotImplementedError("Should have implemented this")

    def getTemperature(self, probeId):
        raise NotImplementedError("Should have implemented this")

