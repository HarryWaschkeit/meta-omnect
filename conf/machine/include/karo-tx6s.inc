# WIC config
WKS_FILE = "omnect-os.karo.wks.in"
WKS_FILES:${MACHINE} = "${WKS_FILE}"

# next one is expected by meta-karo to include overlays
SOC_PREFIX = "tx6s"

# don't install imx-m4-demos
MACHINE_EXTRA_RRECOMMENDS:remove = "imx-m4-demos"

# don't install unnecessary VPU firmware
MACHINE_ESSENTIAL_EXTRA_RDEPENDS:remove = "firmware-imx-vpu-mx6dl"

#SERIAL_CONSOLES = "115200;ttymxc0

# install kernel-devicetree to 'root' partition instead of 'boot'
IMAGE_BOOT_FILES:remove:pn-omnect-os-image = " \
    ${KERNEL_DEVICETREE_FN} \
    oftree \
    bootenv.txt \
"
IMAGE_INSTALL:append:pn-omnect-os-image = " kernel-devicetree dehndetect-udev"
# Todo add 'oftree' to rootfs?

# initramfs type
OMNECT_INITRAMFS_FSTYPE = "cpio.gz.u-boot"

# Set Serial console
SERIAL_CONSOLES ="${@bb.utils.contains('OMNECT_RELEASE_IMAGE', '1', '', '115200;ttymxc2', d)}"
#bootargs console is not part of the devictree, so we have to append it to kernel commandline
APPEND += "console=ttymxc0,115200 uboot=/dev/mmcblk0 cpufreq.default_governor=performance cpuidle.governor=ladder"

# u-boot boot.scr config
KERNEL_IMAGETYPE = "uImage"
KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT}"

KERNEL_BOOTCMD = "bootm"
UBOOT_ARCH = "arm"
UBOOT_FDT_ADDR = "fdt_addr"
UBOOT_FDT_LOAD = "0"

# redundant u-boot env
# This is currently preset by meta-phytec, because we use u-boot-imx instead
# of our patched u-boot here.
OMNECT_PART_OFFSET_UBOOT_ENV1 = "896"
#OMNECT_PART_OFFSET_UBOOT_ENV2 = "1024"
OMNECT_PART_OFFSET_UBOOT_ENV2 = ""
OMNECT_PART_SPARE2 = ""
OMNECT_PART_SIZE_UBOOT_ENV = "128"

# no graphics for TX6
BBMASK:append = " meta-karo-distro/recipes-graphics/wayland/weston_10.0.0.imx.bbappend"
# don't want karo busybox settings
BBMASK:append = " meta-karo-distro/recipes-core/busybox/busybox_%.bbappend"
# prevent karo distro from completely stripping down coreutils
BBMASK:append = " meta-karo-distro/recipes-karo-minimal/recipes-core/coreutils/coreutils_%.bbappend"
# need /var/log which gets deleted here
BBMASK:append = " meta-karo-distro/recipes-core/base-files/base-files_%.bbappend"

# mask otherwise we can not use the latest meta-freescale/meta-imx commits
BBMASK:append = " meta-imx/meta-bsp/recipes-support/opencv/opencv_4.6.0.imx.bb"
BBMASK:append = " meta-imx/meta-sdk/dynamic-layers/openembedded-layer/recipes-multimedia/dvb-apps/dvb-apps_%.bbappend"

# fix search path
FILESEXTRAPATHS:prepend:pn-imx-boot-karo := "${LAYERDIR_freescale-layer}/recipes-bsp/imx-mkimage/files:"

# configure hardware watchdog
# the maximum watchdog deadline depends on the hardware capabilities
SYSTEMD_RuntimeWatchdogSec  = "60"
SYSTEMD_RebootWatchdogSec   = "60"

# this machine uses uboot
MACHINEOVERRIDES:prepend = "omnect_uboot:"
MACHINEOVERRIDES:append = ":karo-tx6s"

# For now remove optee from MACHINE_FEATURES. We don't support secure boot at
# the moment and tee-supplicant will fail, because we don't boot with the
# appropriate device-tree.
# Furthermore we remove feature nxp8987 (set by meta-imx), because
# tauri-l and polis don't have this chip.
MACHINE_FEATURES:remove = " \
    nxp8987 \
    optee \
"
#KERNEL_FEATURES:append = " netfilter.cfg docker.cfg lxc.cfg enable-cifs.cfg disable-kvm.cfg lxc.cfg wireguard.cfg enable-overlayfs.cfg"

KERNEL_FEATURES:remove = "systemd.cfg ipv6.cfg"