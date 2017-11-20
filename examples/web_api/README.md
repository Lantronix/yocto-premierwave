Lantronix Custom Web APIs

*****************************
Yocto SDK is required to build and view header files.  Please first follow the top level instructions to install and use Yocto SDK.
*****************************

APIs provide user interaction with NGINX reverse proxy server running on PremierWave device.  APIs are available to 
* Register a url
* List all registered urls
* Delete a url

APIs are defined in detail in the header file "ltrx_webui.h", which is located in "$SDKTARGETSYSROOT/usr/include/" in Yocto SDK Environment.

Example:
* See "example_web_api.c" in the same folder
* Build using Yocto SDK: make
* How to run: scp the executable "example_web_api" to PremierWave Device and run it on the device via SSH Terminal.

