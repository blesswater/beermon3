#!/bin/bash

. ./wwwConfig.sh

echo "    cp -r $WWW_LOCATION/*.html ./$PROJECT"
cp -r $WWW_LOCATION/*.html ./$PROJECT
echo "    cp -r $WWW_LOCATION/css/*.css ./$PROJECT/css"
cp -r $WWW_LOCATION/css/*.css ./$PROJECT/css
echo "    cp -r $WWW_LOCATION/js/*.js ./$PROJECT/js"
cp -r $WWW_LOCATION/js/*.js ./$PROJECT/js

# Get Common py files
echo "Get python application files"
for file in ${BASE_PY_FILES[@]}; do
	echo "    cp $WWW_LOCATION/cgi/$file ./DataLogger"
	cp $WWW_LOCATION/cgi/$file ./DataLogger
done

echo "Get python common files"
for file in ${APPL_PY_FILES[@]}; do
	echo "    cp $WWW_LOCATION/cgi/$file $PROJECT/cgi"
	cp $WWW_LOCATION/cgi/$file $PROJECT/cgi
done
