require rpi_firmware_settings.inc
require rpi_bootloader_embedded_version.inc

PREFERRED_PROVIDER_virtual/bootloader = "u-boot"
# since u-boot is a file for rpi devices and part of IMAGE_BOOT_FILES, it is not in rootfs
# and not referenced in the wks file
# -> so it doesn't get added to dependency-track sbom
# we add an extra image dependency in this case
DEPENDS:pn-omnect-os-image:append = " virtual/bootloader"

# mask u_boot bbappend from meta-raspberrypi, we handle fw_env.conf in swupdates libubootenv
BBMASK += "/meta-raspberrypi/recipes-bsp/u-boot/u-boot_%.bbappend"

# u-boot to be able to switch between A/B updates
RPI_USE_U_BOOT = "1"

# initramfs type
OMNECT_INITRAMFS_FSTYPE = "cpio.gz.u-boot"

# configure hardware watchdog
# the maximum watchdog deadline depends on the hardware capabilities
SYSTEMD_RuntimeWatchdogSec  = "15"
SYSTEMD_RebootWatchdogSec   = "15"

# rpi bluetooth needs hciattach, which is marked deprecated
PACKAGECONFIG:pn-bluez5:append = " deprecated tools"

# this machine uses uboot
MACHINEOVERRIDES:prepend = "omnect_uboot:"

# optional tpm2 support
MACHINE_FEATURES += "tpm2"

OMNECT_BOOTLOADER_RECIPE_PATH = "${LAYERDIR_core}/recipes-bsp/u-boot/u-boot_2022.01.bb"
# force bootloader version checksum to be old, when sure it's 100% binary compatible
# OMNECT_BOOTLOADER_CHECKSUM_COMPATIBLE:pn-bootloader-versioned = "<newchecksum> <oldchecksum>"
# e.g.:
# OMNECT_BOOTLOADER_CHECKSUM_COMPATIBLE:pn-bootloader-versioned = "2cf3dd5f692d9ed563f06ecf80d0cb566ddc165fa01a37b4eba663dc7b7b9087 ebe0e595a3efb112b7495a14f9562d89d00041390f26a9ad4985435687bf2310"
#
# OMNECT_BOOTLOADER_CHECKSUM_EXPTECTED:pn-bootloader-versioned - build will fail, if the
# computed checksum is different to this; set to <oldchecksum> when
# OMNECT_BOOTLOADER_CHECKSUM_COMPATIBLE:pn-bootloader-versioned is set
OMNECT_BOOTLOADER_CHECKSUM_EXPECTED:pn-bootloader-versioned = "dd60b88d74c8fa852915e3024368f53a4c6ced8fe7578248f57ae65a9d82e48e"
OMNECT_BOOTLOADER_CHECKSUM_COMPATIBLE:pn-bootloader-versioned = "0b6e6acc4bcca419623b86db3bad4bdd02621b0206feeaca619414310541814b dd60b88d74c8fa852915e3024368f53a4c6ced8fe7578248f57ae65a9d82e48e"
