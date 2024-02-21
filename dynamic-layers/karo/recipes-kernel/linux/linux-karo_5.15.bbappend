FILESEXTRAPATHS:prepend := "${THISDIR}/files:${THISDIR}/${BP}:"

## want to use v5.15.102 as phytec does
#SRCREV = "2ddbd0f967b34872290e0f98fae32b91b4de7b87"
#
#SRC_URI:remove = " \
#	file://0010-attiny-regulator-i2c-retries.patch \
#	file://0016-parallel-display-bus-flags-from-display-info.patch \
#"

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
SRC_URI:append = " \
	file://0001-imx6qdl.dtsi-make-usdhc4-mmc0.patch \
	file://defconfig \
"

# KCONFIG_MODE = "alldefconfig"

#ADD_KERNEL_FEATURES = "netfilter.cfg docker.cfg lxc.cfg enable-cifs.cfg disable-kvm.cfg lxc.cfg wireguard.cfg enable-overlayfs.cfg"
#KERNEL_FEATURES += " ${ADD_KERNEL_FEATURES}"

#addtask do_fix_cfgs after do_unpack before do_configre
#
#do_fix_cfgs() {
#    # we need to put all cfg snippets into cfg folder in build directory
#    for cfg in ${ADD_KERNEL_FEATURES}; do
#        ln -s ${WORKDIR}/cfg/$cfg ../$cfg
#    done
#}
