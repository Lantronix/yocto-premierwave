# Using Lantronix PremierWave BSP Yocto Project

## Summary
These instructions show how Customer can use PremierWave BSP Yocto to create a ROM image for pw2050 or sgx5150 that will include their own applications/configuration.

## Prerequisites
Linux OS (Ubuntu 16.04 was tested).  Full Yocto Project Development on a virtual machine is not recommended.  Install necessary packages:
```
sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib \
     build-essential chrpath socat libsdl1.2-dev xterm
sudo apt-get install lzop
```

## Build the ROM image and SDK
Build the ROM image and SDK as follows (assuming the repo is cloned to the folder "~/yocto_premierwave/"):
```
~$ cd yocto_premierwave/sources
~/yocto_premierwave/sources$ git clone -b jethro git://git.yoctoproject.org/poky.git
~/yocto_premierwave/sources$ git clone -b jethro git://git.openembedded.org/meta-openembedded.git
~/yocto_premierwave/sources$ cd ..
~/yocto_premierwave$ ./customer_set_target.sh pw2050
    * This set the target to pw2050.  For sgx5150, change "pw2050" to "sgx5150".
~/yocto_premierwave$ source sources/poky/oe-init-build-env build
~/yocto_premierwave/build$ bitbake ltrx-customer-image
~/yocto_premierwave/build$ bitbake ltrx-customer-image -c populate_sdk
```
The step "bitbake ltrx-customer-image" builds the target ROM image ("PW2050_*.rom" in "build/tmp/deploy/images/pw2050/" for pw2050, or "SGX5150_*.rom" in "build/tmp/deploy/images/sgx5150/" for sgx5150).  The initial build takes about 1 hour.  A build after modification to your application is much faster because it only processes the changes.

The step "bitbake ltrx-customer-image -c populate_sdk" builds the SDK, "poky-glibc-x86_64-ltrx-customer-image-armv5e-toolchain-2.0.3.sh" (together with two other manifest files), under the folder "build/tmp/deploy/sdk/".  This build takes about another 30 minutes, and the built SDK is the same for pw2050 and sgx5150.

## Install SDK
Go to the folder containing the built SDK and install it:
```
~/yocto_premierwave/build/tmp/deploy/sdk$ ./poky-glibc-x86_64-ltrx-customer-image-armv5e-toolchain-2.0.3.sh
Poky (Yocto Project Reference Distro) SDK installer version 2.0.3
=================================================================
Enter target directory for SDK (default: /opt/poky/2.0.3): 
You are about to install the SDK to "/opt/poky/2.0.3". Proceed[Y/n]? Y
Extracting SDK..................................done
Setting it up...done
SDK has been successfully set up and is ready to be used.
Each time you wish to use the SDK in a new shell session, you need to source the environment setup script e.g.
 $ . /opt/poky/2.0.3/environment-setup-armv5e-poky-linux-gnueabi
~/yocto_premierwave/binaries$
```

## Use SDK to build/test your application
1. In your application source folder, run the SDK environment script.  Note the actual script path is determined during the SDK installation in the above step.
```
~/app_premierwave$ . /opt/poky/2.0.3/environment-setup-armv5e-poky-linux-gnueabi
~/app_premierwave$ echo $CC
arm-poky-linux-gnueabi-gcc -march=armv5e -marm -mthumb-interwork --sysroot=/opt/poky/2.0.3/sysroots/armv5e-poky-linux-gnueabi
~/app_premierwave$
```
2. Build your application as a standalone executable
```
~/app_premierwave$ $CC your-app.c -o your-app
~/app_premierwave$
```
3. Load the ROM file built above to the target (See the section "Flashing" below for details).
4. Copy (scp/ftp) the application executable file to the target using the user “root” with the password root.
5. Login the device using the user “root” with the password root, and run/test the application executable.

## Add/Update your application into the ROM image
Once the application is working, you can build your application into the ROM image.
The GIT folder structure is as follows:
```
yocto_premierwave
├── build
│   ├── conf
│   │   ├── bblayers.conf
│   │   └── local.conf
│   └── tmp
│       ├── deploy
│       └── log
├── examples
│   └── ...
├── README.md
└── sources
    ├── meta-application
    │   ├── conf
    │   │   └── layer.conf
    │   ├── recipes-application
    │   │   └── **helloworld**
    │   │       ├── files
    │   │       │   ├── COPYING.MIT
    │   │       │   └── helloworld.c
    │   │       └── helloworld.bb
    │   └── recipes-bsp
    │       └── images
    │           └── **ltrx-customer-image.bbappend**
    ├── meta-lantronix
    │   ├── classes
    │   ├── conf
    │   ├── COPYING.MIT
    │   ├── README
    │   ├── recipes-bsp
    │   ├── ...
```
As shown above, "helloworld" is provided as an example application.  Create a folder in "recipes-application" for your application, put the recipe and code there, and then add your application to "ltrx-customer-image.bbappend" located in "recipes-bsp/images/".

After changes are made, rebuild the ROM image as follows, and the built ROM image will contain your application.  This build is quick because it only processes the changes.
```
~/yocto_premierwave/build$ bitbake ltrx-customer-image -c cleanall
~/yocto_premierwave/build$ bitbake ltrx-customer-image
```

## Flashing
Flash this like any normal PremierWave ROM image.  One way is to follow the instruction on “Upload New Firmware” on Page 110 in the User Guide:https://www.lantronix.com/wp-content/uploads/pdf/SGX5150_UG.pdf.

**Special instructions for pw2050:** if your original firmware version is lower than 8.1.0.0R10 (not including 8.1.0.0R10), you need a special loader "AutoLoader" to upgrade the old firmware to the Yocto build.  Please contact Lantronix for details.

## Examples
Code examples using Lantronix APIs are located in the folder "examples/".

"**ltrx-customer-image.bbappend**" located in "sources/meta-application/recipes-bsp/images/" provides instructions to:
* change root password
* disable root login
* extend /etc/inittab to start an application during bootup

## References
There are many online references.  Here is a good overview:
http://www.yoctoproject.org/bulk/devday-eu-2014/ypdd14-hudson-sdk.pdf

