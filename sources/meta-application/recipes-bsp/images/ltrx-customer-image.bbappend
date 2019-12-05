# uncomment the following line to build the "helloworld" application into the ROM image
#IMAGE_INSTALL += "helloworld"

inherit extrausers

# uncomment the following line to change "root" password to "rootYocto"
#EXTRA_USERS_PARAMS = "usermod -P rootYocto root; "

# uncomment the following line to disable "root" login
#EXTRA_USERS_PARAMS = "userdel -r root; "

append_inittab() {
    echo 'append to inittab'

    # uncomment the following line to start "helloworld" in bootup
    #echo 'null::sysinit:/bin/helloworld &' >> ${IMAGE_ROOTFS}/etc/inittab
}
ROOTFS_POSTPROCESS_COMMAND += "append_inittab; "

postbuild() {
    echo 'root login'

    # uncomment the following line to disable the root login
    #sed -i 's/root:[a-zA-Z0-9$/]*:\([0-9]*\):\([0-9]*\):\([0-9]*\):\([0-9]*\):\([0-9]*\):\([0-9]*\):/root:!!:\1:\2:\3:\4:\5:\6:/g' ${IMAGE_ROOTFS}/etc/shadow
}
ROOTFS_POSTPROCESS_COMMAND += "postbuild; "

