Lantronix Bluez Gatt APIs

**Yocto SDK is required to build applications using the APIs. Please first follow the top level instructions to install and use Yocto SDK.**

APIs provide user interaction with bluez package running on PremierWave device. APIs are available to

* Turn on beacon scan
* Turn off beacon scan
* Removing a scanned device.
* Showing the info of a scanned device.
* Showing the list of scanned devices.

APIs are defined in detail in the header files which are located in

* $SDKTARGETSYSROOT/usr/include/ in Yocto SDK Environment

Example:

* See "ltrx_beacon.c" in the same folder
* Build using Yocto SDK: $CC -Wno-poison-system-directories -Wno-return-local-addr ltrx_beacon_scanner.c -o example_ltrx_beacon -I/usr/include/glib-2.0/ -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include/ -I/usr/include/dbus-1.0/ -lltrx-beacon -lglib-2.0 -ldbus-1 -lreadline
* How to run: scp the executable "example_ltrx_beacon" to PremierWave Device and run it on the device via SSH Terminal.

