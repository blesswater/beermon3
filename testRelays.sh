#!/bin/bash

echo
echo "Warning: This script disables the beermon record and the"
echo "         beermon persist."
systemctl stop beermon-datacollect.service
systemctl stop beermon-persist.service


addr=0x2e

dump="i2cdump -y 1 $addr"

state=`i2cget -y 1 $addr 0x4e`
while [ "$state" != "0x05" ] ; do
	echo "State = $state"
	i2cset -y 1 0x2e 0x4f 0x03
	sleep 1
	state=`i2cget -y 1 $addr 0x4e`
done

while [ true ] ; do
	echo "Enable Relay ON"
	i2cset -y 1 $addr 0x0e 01
	sleep 1
	echo "Control Relay ON"
	i2cset -y 1 $addr 0x0f 01
	sleep 1
	echo "Enable Relay OFF"
	i2cset -y 1 $addr 0x0e 00
	sleep 1
	echo "Control Relay OFF"
	i2cset -y 1 $addr 0x0f 00
	sleep 1
done

