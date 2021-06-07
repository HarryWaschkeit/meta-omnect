FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://../../../LICENSE.md;md5=95a70c9e1af3b97d8bde6f7435d535a8"

SRC_URI = " \
  git://github.com/azure/iot-hub-device-update.git;protocol=https;branch=release/2021-q2;tag=0.7.0 \
  file://mindep.patch;patchdir=${WORKDIR}/git \
"

S = "${WORKDIR}/git/src/utils/eis_utils"

DEPENDS = "azure-iot-sdk-c"

inherit cmake

EXTRA_OECMAKE += "-DCMAKE_MODULE_PATH=${WORKDIR}/git/cmake"

do_configure_prepend() {
    mkdir -p ${S}/inc/aduc
    cp -f ${WORKDIR}/git/src/adu_types/inc/aduc/adu_types.h ${S}/inc/aduc
    cp -f ${WORKDIR}/git/src/utils/c_utils/inc/aduc/* ${S}/inc/aduc
}

do_install() {
    install -d ${D}${includedir}
    for i in ${S}/inc/*.h
    do
      install -m 0644 ${i} ${D}${includedir}
    done

    install -d ${D}${includedir}/aduc
    for i in ${S}/inc/aduc/*.h
    do
      install -m 0644 ${i} ${D}${includedir}/aduc
    done

    install -d ${D}${libdir}
    install -m 0644 ${B}/libeis_utils.so ${D}${libdir}
}

FILES_${PN}-dev = "${includedir}"
FILES_${PN} = "${libdir}/libeis_utils.so"