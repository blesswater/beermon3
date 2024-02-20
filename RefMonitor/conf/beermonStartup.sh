#!/bin/bash

/usr/bin/sqlite3 /opt/beermon/data/beermonSQLite3DB.db "UPDATE Project SET collection_state = 'stale' WHERE collection_state != 'old';" ".exit"

/opt/beermon/bin/logTemp.py "default"
