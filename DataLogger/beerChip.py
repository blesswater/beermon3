import beerChipI2CAddresses as i2cInfo

class beerChip:
    def getVersion(self):
        raise NotImplementedError("Should have implemented this")

    def getTemperature(self, probeId):
        raise NotImplementedError("Should have implemented this")

    def getControlProbeChan(self):
        raise NotImplementedError("Should have implemented this")

    def getUptime(self):
        raise NotImplementedError("Should have implemented this")

    def setControlProbeChan(self, chan):
        raise NotImplementedError("Should have implemented this")

    def switchIn(self):
        raise NotImplementedError("Should have implemented this")

    def switchOut(self):
        raise NotImplementedError("Should have implemented this")

    def extCntl(self):
        raise NotImplementedError("Should have implemented this")

    def getState(self):
        raise NotImplementedError("Should have implemented this")

    def getSetpoint(self, chan=0):
        raise NotImplementedError("Should have implemented this")

    def setSetpoint(self, temp):
        raise NotImplementedError("Should have implemented this")

    def resetBloopDet(self):
        raise NotImplementedError("Should have implemented this")

    def isBloopDet(self):
        raise NotImplementedError("Should have implemented this")

    def ackBloopDet(self):
        raise NotImplementedError("Should have implemented this")

    def getCountBloopDet(self):
        raise NotImplementedError("Should have implemented this")



