Lantronix Bluez Gatt APIs

**Yocto SDK is required to build applications using the APIs. Please first follow the top level instructions to install and use Yocto SDK.**

APIs provide user interaction with bluez package running on PremierWave device. APIs are available to

* Register a service
* Register a characteristic
* Register a descriptor
* Read a characteristic.
* Write to a characteristic.

APIs are defined in detail in the header files which are located in

* $SDKTARGETSYSROOT/usr/include/bluetooth and 
* $SDKTARGETSYSROOT/usr/include/src/shared in Yocto SDK Environment

Example:

* See "example_btgatt-server.c" in the same folder
* Build using Yocto SDK: $CC example_btgatt-server.c -lbtgatt-server -o btgatt
* How to run: scp the executable "btgatt" to PremierWave Device and run it on the device via SSH Terminal.

List of common GATT APIs :

|     API                             |               USAGE                        |                  Header File                           |
|-------------------------------------|--------------------------------------------|--------------------------------------------------------|
| bt_uuid16_create                    | To create a 16 bit attribute.              | $SDKTARGETSYSROOT/usr/include/bluetooth/uuid.h         |
| gatt_db_add_service                 | To add a gatt service.                     | $SDKTARGETSYSROOT/usr/include/src/shared/gatt-db.h     |
| gatt_db_service_add_characteristic  | To add a gatt characteristic.              | $SDKTARGETSYSROOT/usr/include/src/shared/gatt-db.h     |
| gatt_db_service_add_descriptor      | To add a gatt characteristic descriptor.   | $SDKTARGETSYSROOT/usr/include/src/shared/gatt-db.h     |
| gatt_db_service_set_active          | To set the service active and discoverable.| $SDKTARGETSYSROOT/usr/include/src/shared/gatt-db.h     |
| gatt_db_attribute_write             | To write value to the gatt database.       | $SDKTARGETSYSROOT/usr/include/src/shared/gatt-db.h     |
| gatt_db_attribute_read_result       | To send a gatt read response.              | $SDKTARGETSYSROOT/usr/include/src/shared/gatt-db.h     |
| gatt_db_attribute_write_result      | To send a gatt write response.             | $SDKTARGETSYSROOT/usr/include/src/shared/gatt-db.h     |
| bt_gatt_server_send_indication      | To send gatt indication.                   | $SDKTARGETSYSROOT/usr/include/src/shared/gatt-server.h |
| bt_gatt_server_send_notification    | To send gatt notification.                 | $SDKTARGETSYSROOT/usr/include/src/shared/gatt-server.h |
