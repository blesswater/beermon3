#!/bin/bash


. ./wwwConfig.sh

if [ `id -u` != 0 ]; then
	echo "ERROR: Must be run as root"
	exit 1
fi


echo "cp ./$PROJECT/www/index.html $WEB_LOCATION"
cp ./$PROJECT/www/index.html $WEB_LOCATION
echo "chown $MGMT_USER:$WWW_GROUP $WEB_LOCATION/index.html"
chown $MGMT_USER:$WWW_GROUP $WEB_LOCATION/index.html
echo "chmod 640 $WEB_LOCATION/index.html"
chmod 640 $WEB_LOCATION/index.html

if [ ! -d "$WEB_LOCATION/cgi" ]; then
	echo "#"
	echo "# Creating CGI directory"
	echo "#"
	echo "mkdir $WEB_LOCATION/cgi"
	mkdir $WEB_LOCATION/cgi
	echo "chown -R $MGMT_USER:$WWW_GROUP $WEB_LOCATION/cgi"
	chown -R $MGMT_USER:$WWW_GROUP $WEB_LOCATION/cgi
	echo "chmod 750 $WEB_LOCATION/cgi"
	chmod 750 $WEB_LOCATION/cgi
fi

echo "cp ./$PROJECT/www/cgi/*.py $WEB_LOCATION/cgi"
cp ./$PROJECT/www/cgi/*.py $WEB_LOCATION/cgi
echo "chown -R $MGMT_USER:$WWW_GROUP $WEB_LOCATION/cgi"
chown -R $MGMT_USER:$WWW_GROUP $WEB_LOCATION/cgi
echo "chmod 640 $WEB_LOCATION/cgi"
chmod 640 $WEB_LOCATION/cgi/*

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

#
# Add beermon startup processes
#
if [ ! -d $BEERMON_DATA_DIR/bin ]; then
	mkdir $BEERMON_DATA_DIR/bin
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
echo "# Copying Apps to bin"
for file in beerChipBePersistant.py logTemp.py; do
	echo "# App file $file"
	echo "cp ./DataLogger/$file $BEERMON_DATA_DIR/bin/$file"
	cp ./DataLogger/$file $BEERMON_DATA_DIR/bin/$file
	echo "chmod 744 $BEERMON_DATA_DIR/bin/$file"
	chmod 744 $BEERMON_DATA_DIR/bin/$file
	echo "chmod root:root $BEERMON_DATA_DIR/bin/$file"
	chmod root:root $BEERMON_DATA_DIR/bin/$file
done

echo "#"
echo "# Adding services"
echo "#"
for file in beermon-persist.service beermon-datacollect.service; do
	echo "# Adding $file service"
	if [ -f /lib/systemd/system/$file ]; then
		echo "# Disabling service $file"
		echo "systemctl stop $file"
		systemctl stop $file
        	echo "systemctl disable $file"
        	systemctl disable $file
        	echo "rm/lib/systemd/system/$file"
        	rm/lib/systemd/system/$file
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
