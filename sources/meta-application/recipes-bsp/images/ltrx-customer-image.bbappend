# uncomment the following line to build the "helloworld" application into the ROM image
#IMAGE_INSTALL += "helloworld"
IMAGE_INSTALL += "bacnet-stack"
IMAGE_INSTALL_remove = "vsftpd"
IMAGE_INSTALL += "timesync"

inherit extrausers

# uncomment the following line to change "root" password to "rootYocto"
EXTRA_USERS_PARAMS = "usermod -P root root; "
# EXTRA_USERS_PARAMS = "usermod -P <root password> root; "

# uncomment the following line to disable "root" login
#EXTRA_USERS_PARAMS = "userdel -r root; "

append_inittab() {
    echo 'append to inittab'

    # uncomment the following line to start "helloworld" in bootup
    #echo 'null::sysinit:/bin/helloworld &' >> ${IMAGE_ROOTFS}/etc/inittab
    
    # launch script on startup
    echo 'null::sysinit:/ltrx_user/launch.sh &' >> ${IMAGE_ROOTFS}/etc/inittab
    # launch timesync script
    echo 'null::sysinit:/usr/bin/timesync.sh &' >> ${IMAGE_ROOTFS}/etc/inittab
}
ROOTFS_POSTPROCESS_COMMAND += "append_inittab; "

