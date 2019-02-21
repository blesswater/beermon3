#!/bin/bash

PROJECT=RefMonitor
WWW_LOCATION=/var/www/html

WWW_USER=pi
WWW_GROUP=www-data

MGMT_USER=pi
KIOSK_USER=pi

BASE_PY_FILES=(beerChipDB.py beerChipI2CAddresses.py beerChipI2C.py beerChip.py beermonConfig.py beerChipPersistant.py)
APPL_PY_FILES=(wsgi.py configInfo.py temperatureData.py beerSession.py)
