#!/bin/bash


. ./wwwConfig.sh

if [ `id -u` != 0 ]; then
	echo "ERROR: Must be run as root"
	exit 1
fi


echo "cp ./$PROJECT/index.html $WWW_LOCATION"
cp ./$PROJECT/index.html $WWW_LOCATION
echo "chown $MGMT_USER:$WWW_GROUP $WWW_LOCATION/index.html"
chown $MGMT_USER:$WWW_GROUP $WWW_LOCATION/index.html
echo "chmod 640 $WWW_LOCATION/index.html"
chmod 640 $WWW_LOCATION/index.html

if [ ! -d "$WWW_LOCATION/cgi" ]; then
	echo "#"
	echo "# Creating CGI directory"
	echo "#"
	echo "mkdir $WWW_LOCATION/cgi"
	mkdir $WWW_LOCATION/cgi
	echo "chown -R $MGMT_USER:$WWW_GROUP $WWW_LOCATION/cgi"
	chown -R $MGMT_USER:$WWW_GROUP $WWW_LOCATION/cgi
	echo "chmod 750 $_LOCATION/cgi"
	chmod 750 $WWW_LOCATION/cgi
fi

echo "cp ./$PROJECT/www/cgi/*.py $WWW_LOCATION/cgi"
cp ./$PROJECT/www/cgi/*.py $WWW_LOCATION/cgi
echo "chown -R $MGMT_USER:$WWW_GROUP $WWW_LOCATION/cgi"
chown -R $MGMT_USER:$WWW_GROUP $WWW_LOCATION/cgi
echo "chmod 640 $WWW_LOCATION/cgi"
chmod 640 $WWW_LOCATION/cgi/*

echo "cp ./$PROJECT/cgi/*.py $WWW_LOCATION/cgi"
cp ./$PROJECT/cgi/*.py $WWW_LOCATION/cgi
echo "chown -R $MGMT_USER:$WWW_GROUP $WWW_LOCATION/cgi"
chown -R $MGMT_USER:$WWW_GROUP $WWW_LOCATION/cgi
echo "chmod 640 $WWW_LOCATION/cgi"
chmod 640 $WWW_LOCATION/cgi/*

if [ ! -d "$WWW_LOCATION/js" ]; then
	echo "#"
	echo "# Creating Javascript(js) directory"
	echo "#"
	echo "mkdir $WWW_LOCATION/js"
	mkdir $WWW_LOCATION/js
	echo "chown -R $MGMT_USER:$WWW_GROUP $WWW_LOCATION/js"
	chown -R $MGMT_USER:$WWW_GROUP $WWW_LOCATION/js
	echo "chmod 750 $_LOCATION/js"
	chmod 750 $WWW_LOCATION/js
fi

echo "cp ./$PROJECT/www/js/*.js $WWW_LOCATION/js"
cp ./$PROJECT/www/js/*.js $WWW_LOCATION/js
echo "cp ./$PROJECT/js/*.js $WWW_LOCATION/js"
cp ./$PROJECT/js/*.js $WWW_LOCATION/js

if [ ! -d "$WWW_LOCATION/css" ]; then
	echo "#"
	echo "# Creating css directory"
	echo "#"
	echo "mkdir $WWW_LOCATION/css"
	mkdir $WWW_LOCATION/css
	echo "chown -R $MGMT_USER:$WWW_GROUP $WWW_LOCATION/css"
	chown -R $MGMT_USER:$WWW_GROUP $WWW_LOCATION/css
	echo "chmod 750 $_LOCATION/css"
	chmod 750 $WWW_LOCATION/css
fi

echo "cp ./$PROJECT/www/css/*.css $WWW_LOCATION/css"
cp ./$PROJECT/www/css/*.css $WWW_LOCATION/css
# echo "cp ./$PROJECT/css/*.css $WWW_LOCATION/css"
# cp ./$PROJECT/css/*.css $WWW_LOCATION/css

# echo "cp ./$PROJECT/www/css/*.css $WWW_LOCATION/css"
# cp ./$PROJECT/www/css/*.css $WWW_LOCATION/css
echo "cp ./$PROJECT/css/*.css $WWW_LOCATION/css"
cp ./$PROJECT/css/*.css $WWW_LOCATION/css

echo "#"
echo "# Installing Config Files"
echo "#"

echo "cp ./$PROJECT/conf/beermon3.conf /etc/nginx/conf.d/beermon3.conf"
cp ./$PROJECT/conf/beermon3.conf /etc/nginx/conf.d/beermon3.conf
echo "chown root:root /etc/nginx/conf.d/beermon3.conf"
chown root:root /etc/nginx/conf.d/beermon3.conf
echo "chmod 644 /etc/nginx/conf.d/beermon3.conf"
chmod 644 /etc/nginx/conf.d/beermon3.conf

echo "cp ./$PROJECT/conf/nginx.conf /etc/nginx/nginx.conf"
cp ./$PROJECT/conf/nginx.conf /etc/nginx/nginx.conf
echo "chown root:root /etc/nginx/nginx.conf"
chown root:root /etc/nginx/nginx.conf
echo "chmod 644 /etc/nginx/nginx.conf"
chmod 644 /etc/nginx/nginx.conf

echo "#"
echo "# Installing uwsgi startup"
echo "#"
if [ ! -d /etc/uwsgi ]; then
	echo "# Creating /etc/uwsgi"
	mkdir /etc/uwsgi
	chmod 755 /etc/uwsgi
fi
echo "cp ./$PROJECT/conf/uwsgi.ini /etc/uwsgi/uwsgi.ini"
cp ./$PROJECT/conf/uwsgi.ini /etc/uwsgi/uwsgi.ini
chmod 644 /etc/uwsgi/uwsgi.ini

# Systemd start up
echo "cp ./$PROJECT/conf/uwsgi.service /lib/systemd/system/uwsgi.service"
cp ./$PROJECT/conf/uwsgi.service /lib/systemd/system/uwsgi.service
chmod 644 /lib/systemd/system/uwsgi.service
if [ ! -f /etc/systemd/system/multi-user.target.wants/uwsgi.service ]; then
	echo "Making multi-user systemd link"
	ln -s /lib/systemd/system/uwsgi.service /etc/systemd/system/multi-user.target.wants/uwsgi.service
fi

#
# Install Kiosk
#
echo "cat ./Kiosk/kiosk.sh | sed 's/\\\$KIOSK_USER\\\$/$KIOSK_USER/g'"
setStr="s/\\\$KIOSK_USER\\\$/$KIOSK_USER/g"
echo $setStr
# cat ./Kiosk/kiosk.sh | sed 's/\\\$KIOSK_USER\\\$/$KIOSK_USER/g'
cat ./Kiosk/kiosk.sh | sed $setStr > /home/$KIOSK_USER/kiosk.sh
echo "chmod a+x /home/$KIOSK_USER/kiosk.sh"
chmod a+x /home/$KIOSK_USER/kiosk.sh

#
# Add beermon startup processes
#
if [ ! -d $BEERMON_DATA_DIR ]; then
	echo "mkdir $BEERMON_DATA_DIR"
	mkdir $BEERMON_DATA_DIR
	echo "chown root:$WWW_GROUP $BEERMON_DATA_DIR"
	chown root:$WWW_GROUP $BEERMON_DATA_DIR
	echo "chmod 775 $BEERMON_DATA_DIR"
	chmod 775 $BEERMON_DATA_DIR
fi

# Install DB
if [ ! -d "$BEERMON_DATA_DIR/data" ]; then
	echo "#"
	echo "# Creating Data directory"
	echo "#"
	echo "mkdir $BEERMON_DATA_DIR/data"
	mkdir $BEERMON_DATA_DIR/data
	echo "chown $MGMT_USER:$WWW_GROUP $BEERMON_DATA_DIR/data"
	chown $MGMT_USER:$WWW_GROUP $BEERMON_DATA_DIR/data
	echo "chmod 770 $BEERMON_DATA_DIR/data"
	chmod 774 $BEERMON_DATA_DIR/data
fi
if [ ! -f $BEERMON_DATA_DIR/data/$BEERMON_DB ]; then
	echo "#"
	echo "# Copying new database"
	echo "#"
	echo "cp ./Data/$BEERMON_DB $BEERMON_DATA_DIR/data/$BEERMON_DB"
	cp ./Data/$BEERMON_DB $BEERMON_DATA_DIR/data/$BEERMON_DB
	echo "chown $WWW_USER:$WWW_GROUP $BEERMON_DATA_DIR/data/$BEERMON_DB"
	chown $WWW_USER:$WWW_GROUP $BEERMON_DATA_DIR/data/$BEERMON_DB
	echo "chmod 774  $BEERMON_DATA_DIR/data/$BEERMON_DB"
	chmod 774  $BEERMON_DATA_DIR/data/$BEERMON_DB
fi

if [ ! -f $BEERMON_DATA_DIR/data/beermonSession.db ]; then
	echo "#"
	echo "# Copying new user database"
	echo "#"
	echo "cp ./Data/beermonSession.db $BEERMON_DATA_DIR/data/beermonSession.db"
	cp ./Data/beermonSession.db $BEERMON_DATA_DIR/data/beermonSession.db
	echo "chown $WWW_USER:$WWW_GROUP $BEERMON_DATA_DIR/data/beermonSession.db"
	chown $WWW_USER:$WWW_GROUP $BEERMON_DATA_DIR/data/beermonSession.db
	echo "chmod 774  $BEERMON_DATA_DIR/data/beermonSession.db"
	chmod 774  $BEERMON_DATA_DIR/data/beermonSession.db
fi

# Create bin directories
if [ ! -d $BEERMON_DATA_DIR/bin ]; then
	mkdir $BEERMON_DATA_DIR/bin
	echo "chown root:root $BEERMON_DATA_DIR/bin"
	chown root:root $BEERMON_DATA_DIR/bin
	echo "chmod 744 $BEERMON_DATA_DIR/bin"
	chmod 744 $BEERMON_DATA_DIR/bin
fi

echo "#"
echo "# Copy base files to bin"
echo "#"
for file in ${BASE_PY_FILES[@]}; do
	echo "# Copying file $file"
	echo "cp ./DataLogger/$file $BEERMON_DATA_DIR/bin/$file"
	cp ./DataLogger/$file $BEERMON_DATA_DIR/bin/$file
	echo "chmod 644 $BEERMON_DATA_DIR/bin/$file"
	chmod 644 $BEERMON_DATA_DIR/bin/$file
	echo "chown root:root $BEERMON_DATA_DIR/bin/$file"
	chown root:root $BEERMON_DATA_DIR/bin/$file
done


echo "# Copy base files to web"
for file in ${BASE_PY_FILES[@]}; do
	echo "# Copying file $file"
	echo "cp ./DataLogger/$file $WWW_LOCATION/cgi/$file"
	cp ./DataLogger/$file $WWW_LOCATION/cgi/$file
	echo "chmod 640 $WWW_LOCATION/cgi/$file"
	chmod 640 $WWW_LOCATION/cgi/$file
	echo "chown $MGMT_USER:$WWW_GROUP $WWW_LOCATION/cgi/$file"
	chown $MGMT_USER:$WWW_GROUP $WWW_LOCATION/cgi/$file
done

echo "# Copying Apps to bin"
for file in beerChipBePersistant.py logTemp.py; do
	echo "# App file $file"
	echo "cp ./DataLogger/$file $BEERMON_DATA_DIR/bin/$file"
	cp ./DataLogger/$file $BEERMON_DATA_DIR/bin/$file
	echo "chmod 744 $BEERMON_DATA_DIR/bin/$file"
	chmod 744 $BEERMON_DATA_DIR/bin/$file
	echo "chown root:root $BEERMON_DATA_DIR/bin/$file"
	chown root:root $BEERMON_DATA_DIR/bin/$file
done

echo "# Set i2c devices priveledge"
echo "chmod 666 /dev/i2c-1"
chmod 666 /dev/i2c-1
echo "usermod -a -G i2c $WWW_GROUP"
usermod -a -G i2c $WWW_GROUP


echo "#"
echo "# Adding services"
echo "#"

echo "# Add beermon startup script"
echo "cp $PROJECT/conf/beermonStartup.sh $BEERMON_DATA_DIR/bin/beermonStartup.sh"
cp $PROJECT/conf/beermonStartup.sh $BEERMON_DATA_DIR/bin/beermonStartup.sh
echo "chmod 744 $BEERMON_DATA_DIR/bin/beermonStartup.sh"
chmod 744 $BEERMON_DATA_DIR/bin/beermonStartup.sh
echo "chown root:root $BEERMON_DATA_DIR/bin/beermonStartup.sh"
chown root:root $BEERMON_DATA_DIR/bin/beermonStartup.sh

for file in beermon-persist.service beermon-datacollect.service kiosk.service; do
	echo "# Adding $file service"
	if [ -f /lib/systemd/system/$file ]; then
		echo "# Disabling service $file"
		echo "systemctl stop $file"
		systemctl stop $file
        	echo "systemctl disable $file"
        	systemctl disable $file
        	echo "rm /lib/systemd/system/$file"
        	rm /lib/systemd/system/$file
        	echo "systemctl daemon-reload" 
        	systemctl daemon-reload 
	fi
	echo "# Adding $file service"
	echo "# Starting service $file"
	echo "cp $PROJECT/conf/$file /lib/systemd/system"
	cp $PROJECT/conf/$file /lib/systemd/system
	echo "chmod 644 /lib/systemd/system/$file"
	chmod 644 /lib/systemd/system/$file
	echo "chown root:root /lib/systemd/system/$file"
	chown root:root /lib/systemd/system/$file
	echo "systemctl daemon-reload"
	systemctl daemon-reload
	echo "systemctl start $file"
	systemctl start $file
	echo "systemctl enable $file"
	systemctl enable $file
done
