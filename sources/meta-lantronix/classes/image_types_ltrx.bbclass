inherit image_types

IMAGE_DEPENDS_rom = "virtual/kernel:do_deploy"

IMAGE_TYPEDEP_rom = "ubifs"

IMAGE_CMD_rom () {
    cd ${DEPLOY_DIR_IMAGE}

    cat ${KERNEL_IMAGETYPE} ${KERNEL_IMAGETYPE}-${KERNEL_DEVICETREE} > \
        ${KERNEL_IMAGETYPE}_w_dtb

    ltrx-mkimage -m -p ${LTRX_PRODUCT_FAMILY_ID} -t ${LTRX_KERNEL_IMAGE_TYPE} \
        -i ${LTRX_PRODUCT_CODE} \
        -r ${LTRX_PRODUCT_VERSION} -a ${LTRX_BASE_ADDRESS} \
        ${KERNEL_IMAGETYPE}_w_dtb

    ltrx-mkimage -m -p ${LTRX_PRODUCT_FAMILY_ID} -t ${LTRX_ROOTFS_IMAGE_TYPE} \
        -i ${LTRX_PRODUCT_CODE} \
        -r ${LTRX_PRODUCT_VERSION} -a ${LTRX_BASE_ADDRESS} \
        ${IMAGE_BASENAME}-${MACHINE}.ubifs

    ltrx-mkimage -c ${KERNEL_IMAGETYPE}_w_dtb.rom \
        ${IMAGE_BASENAME}-${MACHINE}.ubifs.rom \
        ${LTRX_KERNEL_MACHINE}_${LTRX_PRODUCT_VERSION}.rom
}

