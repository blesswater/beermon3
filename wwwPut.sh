#!/bin/bash

. ./wwwConfig.sh

reCreateSite() {
	echo "Project file not found. Kill everything"
	echo "    rm -r -f $WWW_LOCATION/."
	rm -r -f $WWW_LOCATION/.
	echo "    mkdir $WWW_LOCATION/cgi"
	mkdir $WWW_LOCATION/cgi
	chmod 750 $WWW_LOCATION/cgi
	echo "    mkdir $WWW_LOCATION/css"
	mkdir $WWW_LOCATION/css
	chmod 750 $WWW_LOCATION/css
	echo "    mkdir $WWW_LOCATION/js"
	mkdir $WWW_LOCATION/js
	chmod 750 $WWW_LOCATION/css
}


if [ `id -u` != 0 ]; then
	echo "ERROR: Must be run as root"
	exit 1
fi

if [ ! -d $WWW_LOCATION ]; then
	echo "$WWW_LOCATION not found - creating"
	mkdir $WWW_LOCATION
	chown $WWW_USER:$WWW_GROUP $WWW_LOCATION
	chmod 755 $WWW_LOCATION
fi

if [ ! -f $WWW_LOCATION/proj.sh ]; then
	reCreateSite
else
	source $WWW_LOCATION/proj.sh
	if [ $THIS_PROJECT != $PROJECT ]; then
		reCreateSite
	fi
fi

echo "Copy site $PROJECT"
echo "    cp ./$PROJECT/*.html $WWW_LOCATION"
cp ./$PROJECT/*.html $WWW_LOCATION
echo "    cp ./$PROJECT/proj.sh $WWW_LOCATION"
cp ./$PROJECT/proj.sh $WWW_LOCATION
echo "    cp ./$PROJECT/css/*.css $WWW_LOCATION/css"
cp ./$PROJECT/css/*.css $WWW_LOCATION/css
echo "    cp ./$PROJECT/js/*.js $WWW_LOCATION/js"
cp ./$PROJECT/js/*.js $WWW_LOCATION/js

echo "    cp ./$PROJECT/cgi/*.py $WWW_LOCATION/cgi"
cp ./$PROJECT/cgi/*.py $WWW_LOCATION/cgi
for file in ${BASE_PY_FILES[@]}; do
	echo "    cp ./DataLogger/$file $WWW_LOCATION/cgi"
	cp ./DataLogger/$file $WWW_LOCATION/cgi
done
for file in ${APPL_PY_FILES[@]}; do
	echo "cp ./$PROJECT/cgi/$file $WWW_LOCATION/cgi"
	cp ./$PROJECT/cgi/$file $WWW_LOCATION/cgi
done

echo "    chown -R $WWW_USER:$WWW_GROUP $WWW_LOCATION"
chown -R $WWW_USER:$WWW_GROUP $WWW_LOCATION
echo "    chmod -R 750 $WWW_LOCATION"
chmod -R 750 $WWW_LOCATION

