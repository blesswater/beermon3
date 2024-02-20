import time

from beerChipRedis import beerChipRedis as beerChip

if __name__ == '__main__':
    bc = beerChip(1, 0x2e)

    bc.extCntl()
    while True:
        bc.setRelay( 'enable', 'on' )
        time.sleep( 1.5 )
        bc.setRelay('control', 'on')
        time.sleep(1.5)
        bc.setRelay('enable', 'off')
        time.sleep(1.5)
        bc.setRelay('control', 'off')
        time.sleep(1.5)