FILESEXTRAPATHS:prepend := "${THISDIR}/files:${THISDIR}/${BP}:"

# FIXME: kernel configuration should definitely not get copied here!
#SRC_URI += " \
#	file://netfilter.cfg \
#	file://docker.cfg \
#	file://lxc.cfg \
#	file://enable-cifs.cfg \
#	file://disable-kvm.cfg \
#	file://lxc.cfg \
#	file://wireguard.cfg \
#	file://enable-overlayfs.cfg \
#"
#SRC_URI:append = " \
#    file://defconfig \
#"
#INTREE_DEFCONFIG:append = " disable-stuff.cfg"
SRC_URI:append = " \
	file://0001-imx6qdl.dtsi-make-usdhc4-mmc0.patch \
"

COMPATIBLE_MACHINE:tx6 = "(tx6[qsu]-.*)"
RDEPENDS:${KERNEL_PACKAGE_NAME}-base:remove = "phytec-dt-overlays"