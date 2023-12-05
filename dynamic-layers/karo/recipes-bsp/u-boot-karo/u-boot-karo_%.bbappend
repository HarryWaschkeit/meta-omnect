FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append:tx6s-8035 = "file://0001-make-compilable-with-cortexa9hf-neon-tune.patch"

EXTRA_OEMAKE:append:tx6s-8035 = " KCFLAGS='-mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9'"
