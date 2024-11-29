#!/bin/sh

cd /home/root/sen55-webserver

if [ -f ./server.py ]; then
    echo "Bringup the WiFi interface."
    wpa_supplicant -Dnl80211 -iwlan0 -c /etc/wpa_supplicant.conf -B

    echo "Fetch an IP address from the DHCP server."
    udhcpc -i wlan0

    echo "Start the SEN55 webserver in the background"
    python3 ./server.py &
else
    echo "Set web page location:"
    killall -9 httpd
    httpd -h /home/root/sen55-webserver
fi


