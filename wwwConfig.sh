#!/bin/bash

PROJECT=RefMonitor
WWW_LOCATION=/var/www/html

WWW_USER=pi
WWW_GROUP=www-data

BASE_PY_FILES=(beerChipDB.py beerChipI2CAddresses.py beerChipI2C.py beerChip.py beermonConfig.py)
APPL_PY_FILES=(wsgi.py configInfo.py temperatureData.py)
