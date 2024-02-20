FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:${THISDIR}/${PN}/defconfigs:${LAYERDIR_omnect}/recipes-bsp/u-boot/u-boot:"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM:mx6 = "file://Licenses/README;md5=0507cd7da8e7ad6d6701926ec9b84c95"

SRC_URI:append:tx6s-8035 = " \
	file://0001-make-compilable-with-cortexa9hf-neon-tune.patch \
	file://0001-compiler-.h-sync-include-linux-compiler-.h-with-Linu.patch \
	file://0001-net-Use-packed-structures-for-networking.patch \
	file://0001-gcc-9-silence-address-of-packed-member-warning.patch \
	file://0002-karo-tx6-fix-duplicate-const-error.patch \
	file://0003-u-boot-dehn-bms.patch \
	file://omnect_env.patch \
	file://0001-configs-tx6.h-remove-CONFIG_BOOTCMD-defitions-set-ag.patch \
	file://tx6s-8035_defconfig.template \
	file://u-boot-cfg.generic \
	file://omnect_env.h \
	file://omnect_env_karo_tx6s.h \
	file://0004-include-configs-tx6.h-make-user-data-partition-the-o.patch \
	file://0001-increase-CONFIG_SYS_BOOTM_SIZE.patch \
"
SRCBRANCH:mx6 = "master"
SRCREV:mx6 = "9c867060812647af60840df7caf78f6567b2bd29"

EXTRA_OEMAKE:append:tx6s-8035 = " KCFLAGS='-mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9 -mgeneral-regs-only'"

# Appends a string to the name of the local version of the U-Boot image; e.g. "-1"; if you like to update the bootloader via
# swupdate and iot-hub-device-update, the local version must be increased;
#
# Note: `UBOOT_LOCALVERSION` should be also increased on change of variables
#   `APPEND`,
#   `OMNECT_UBOOT_WRITEABLE_ENV_FLAGS`
UBOOT_LOCALVERSION = "-1"
PKGV = "${PV}${UBOOT_LOCALVERSION}"

UBOOT_ENV_FILE:mx6 = ""
SRC_URI:append:mx6 = "${@ " file://${UBOOT_ENV_FILE};subdir=git/board/karo/tx6" if "${UBOOT_ENV_FILE}" != "" else ""}"

UBOOT_BOARD_DIR:mx6 = "board/karo/tx6"

COMPATIBLE_MACHINE:tx6 = "(tx6[qsu]-.*)"
COMPATIBLE_MACHINE:txul = "(txul-.*)"

inherit omnect_uboot_configure_env

do_configure:prepend() {
    omnect_uboot_configure_env
}

do_configure:prepend:tx6s-8035() {
    cp -f ${WORKDIR}/omnect_env_karo_tx6s.h ${S}/include/configs/omnect_env_machine.h
}

do_deploy:append:tx6s-8035() {
    # deploy a binary lacking the initial 1K filler (0xdeadf001) which can be
    # can be used for integration into WIC as well as into SWU
    dd if=${DEPLOYDIR}/u-boot.bin of=${DEPLOYDIR}/u-boot.bin.stripped bs=1K skip=1
}

do_deploy:append() {
  echo "${PKGV}" > ${DEPLOYDIR}/bootloader_version
}
