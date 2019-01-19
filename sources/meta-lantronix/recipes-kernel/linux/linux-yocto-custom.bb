# This file was derived from the linux-yocto-custom.bb recipe in
# oe-core.
#
# linux-yocto-custom.bb:
#
#   A yocto-bsp-generated kernel recipe that uses the linux-yocto and
#   oe-core kernel classes to apply a subset of yocto kernel
#   management to git managed kernel repositories.
#
# Warning:
#
#   Building this kernel without providing a defconfig or BSP
#   configuration will result in build or boot errors. This is not a
#   bug.
#
# Notes:
#
#   patches: patches can be merged into to the source git tree itself,
#            added via the SRC_URI, or controlled via a BSP
#            configuration.
#
#   example configuration addition:
#            SRC_URI += "file://smp.cfg"
#   example patch addition:
#            SRC_URI += "file://0001-linux-version-tweak.patch
#   example feature addition:
#            SRC_URI += "file://feature.scc"
#

inherit kernel
require recipes-kernel/linux/linux-yocto.inc

# for git:
# SRC_URI = "git://${THISDIR}/${PN}/linux-at91.git;protocol=file;bareclone=1;branch=${KBRANCH}
# KBRANCH = "develop-boottime"
# PV = "${LINUX_VERSION}+git${SRCPV}"

SRC_URI = "file://defconfig"
SRC_URI += "file://0001-atmel_serial_nodma.patch"

# The default kernel config for all the machine should mentioned in defconfig if it is common. If it is related to particular
#  target then update the respective .cfg config file. Ex: for pw2050.cfg for pw2050, sgx5150.cfg for sgx5150 and that will be appended below. 
SRC_URI_append_pw2050 = "file://pw2050.cfg"
SRC_URI_append_sgx5150 = "file://sgx5150.cfg"

# remove this if using git!
S = "${WORKDIR}/linux-at91"

LINUX_VERSION = "3.10.0"
LINUX_VERSION_EXTENSION = ""

PV = "${LINUX_VERSION}"

require ltrx-linux.inc

