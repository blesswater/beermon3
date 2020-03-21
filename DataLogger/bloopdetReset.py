#!/usr/bin/python
import sys

# from beerChipI2C import beerChipI2C as beerChip
from beerChipRedis import beerChipRedis as beerChip

if __name__ == '__main__':
    bc = beerChip( 1, 0x2e )
    bc.resetBloopDet()
