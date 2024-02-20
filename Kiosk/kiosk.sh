#!/bin/bash -x

# sleep 10 

KIOSK_USER=pi

xhost +local:

xset s noblank
xset s off
xset -dpms

unclutter -idle 0.5 -root &

sed -i 's/"exited_cleanly":false/"exited_cleanly":true/' /home/${KIOSK_USER}/.config/chromium/Default/Preferences
sed -i 's/"exit_type":"Crashed"/"exit_type":"Normal"/' /home/${KIOSK_USER}/.config/chromium/Default/Preferences

# Wait til localhost comes up
isLocalUp=`nslookup localhost`
while [ $? -eq 1 ]; do
	sleep 5
	isLocalUp=`nslookup localhost`
done

# isLocalUp=`nslookup www.drudgereport.com`
# while [ $? -eq 1 ]; do
# 	sleep 5
# 	isLocalUp=`nslookup www.drudgereport.com`
# done

isLocalUp=`wget -O - http://localhost > /dev/null 2>&1`
while [ $? -ne 0 ]; do
 	sleep 5
 	isLocalUp=`wget -O - http://localhost > /dev/null 2>&1`
done


/usr/bin/chromium-browser --noerrdialogs --disable-infobars --kiosk http://localhost &

while true; do
 	sleep 10
done
