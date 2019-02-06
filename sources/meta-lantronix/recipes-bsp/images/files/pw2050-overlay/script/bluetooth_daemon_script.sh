mknod -m 666 /dev/rfcomm0 c 216 0
mkdir /var/run/dbus/
dbus-daemon --config-file /etc/dbus-1/system.conf --fork
bluetoothd&
chmod 777 /var/run/sdp
hciconfig hci0 sspmode 0
hciconfig hci0 piscan
sdptool add --channel=3 SP
rfcomm watch /dev/rfcomm0 3&

