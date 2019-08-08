mknod -m 666 /dev/rfcomm0 c 216 0
mkdir /var/run/dbus/
dbus-daemon --config-file /etc/dbus-1/system.conf --fork
/usr/libexec/bluetooth/bluetoothd --noplugin=sap --compat&
chmod 777 /var/run/sdp
