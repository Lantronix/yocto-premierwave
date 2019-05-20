Lantronix Control PIN APIs

**Yocto SDK is required to build and view header files.  Please first follow the top level instructions to install and use Yocto SDK.**

APIs provide user to control the GPIO's as a input or output.  APIs are available to 
* Initialize CP as a input
* Initialize CP as a output
* Get the CP status
* Make active low input
* Make active high input

APIs are defined in detail in the header file "cp_userapi.h", which is located in current directory in Yocto SDK Environment.

Example:
* See the example C file in the same folder
* Build using Yocto SDK: make
* How to run: scp the executable "example_webui" to PremierWave Device and run it on the device via SSH Terminal.

