include ltrx-image-common.inc

IMAGE_INSTALL += "ltrx-apps-sdk"

# Build the native files (that run on Host) per ltrx-mkimage-bin.bb
do_rootfs[depends] += "ltrx-mkimage-bin-native:do_build"
