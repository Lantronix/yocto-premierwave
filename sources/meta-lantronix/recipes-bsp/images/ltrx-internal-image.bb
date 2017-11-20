include ltrx-image-common.inc

IMAGE_INSTALL += "bcmdhd"
IMAGE_INSTALL += "ltrx-wpa-supplicant"
IMAGE_INSTALL += "ltrx-apps-dev"
IMAGE_INSTALL += "mqtt-ws-client"

# Build the native files (that run on Host) per ltrx-mkimage.bb
do_rootfs[depends] += "ltrx-mkimage-native:do_build"
