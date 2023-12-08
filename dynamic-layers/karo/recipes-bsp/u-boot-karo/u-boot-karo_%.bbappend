FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:${THISDIR}/${PN}/defconfigs:"

SRC_URI:append:tx6s-8035 = " \
	file://0001-make-compilable-with-cortexa9hf-neon-tune.patch \
	file://0001-compiler-.h-sync-include-linux-compiler-.h-with-Linu.patch \
	file://0002-karo-tx6-fix-duplicate-const-error.patch \
	file://0003-u-boot-dehn-bms.patch \
	file://tx6s-8035_defconfig.template \
	file://u-boot-cfg.generic \
"
SRCREV:mx6 = "9c867060812647af60840df7caf78f6567b2bd29"

EXTRA_OEMAKE:append:tx6s-8035 = " KCFLAGS='-mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9 -mgeneral-regs-only'"
