#!/bin/bash

KIOSK_USER=pi

echo "cat kiosk.sh | sed 's/\\\$KIOSK_USER\\\$/$KIOSK_USER/g'"
setStr="s/\\\$KIOSK_USER\\\$/$KIOSK_USER/g"
echo $setStr
# cat kiosk.sh | sed 's/\\\$KIOSK_USER\\\$/$KIOSK_USER/g'
cat kiosk.sh | sed $setStr > /home/$KIOSK_USER/kiosk.sh
