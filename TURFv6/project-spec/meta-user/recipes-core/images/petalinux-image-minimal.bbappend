
rootfs_postprocess_function() {
    # add configfs
    if [ -e ${IMAGE_ROOTFS}/etc/fstab ]; then
       echo "configfs                 /configfs                configfs       defaults              0  0" >> ${IMAGE_ROOTFS}/etc/fstab
    fi
}

ROOTFS_POSTPROCESS_COMMAND:append = " \
  rootfs_postprocess_function; \
"

