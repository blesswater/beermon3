#!/bin/bash


. ./wwwConfig.sh

if [ `id -u` != 0 ]; then
	echo "ERROR: Must be run as root"
	exit 1
fi


echo "cp ./www/index.html $WEB_LOCATION"
cp ./www/index.html $WEB_LOCATION
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

echo "cp ./www/cgi/*.py $WEB_LOCATION/cgi"
cp ./www/cgi/*.py $WEB_LOCATION/cgi
echo "chown -R $MGMT_USER:$WWW_GROUP $WEB_LOCATION/cgi"
chown -R $MGMT_USER:$WWW_GROUP $WEB_LOCATION/cgi
echo "chmod 640 $WEB_LOCATION/cgi"
chmod 640 $WEB_LOCATION/cgi/*

echo "#"
echo "# Installing Config Files"
echo "#"

echo "cp ./www/conf/beermon3.conf /etc/nginx/conf.d/beermon3.conf"
cp ./www/conf/beermon3.conf /etc/nginx/conf.d/beermon3.conf
echo "chown root:root /etc/nginx/conf.d/beermon3.conf"
chown root:root /etc/nginx/conf.d/beermon3.conf
echo "chmod 644 /etc/nginx/conf.d/beermon3.conf"
chmod 644 /etc/nginx/conf.d/beermon3.conf

echo "cp ./www/conf/nginx.conf /etc/nginx/nginx.conf"
cp ./www/conf/nginx.conf /etc/nginx/nginx.conf
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
cp ./www/conf/uwsgi.ini /etc/uwsgi/uwsgi.ini
chmod 644 /etc/uwsgi/uwsgi.ini

# Upstart files - not used
# cp ./www/conf/uwsgi.conf /etc/init/uwsgi.conf
# chmod 644 /etc/init/uwsgi.conf

# Systemd start up
cp ./www/conf/uwsgi.service /lib/systemd/system/uwsgi.service
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


