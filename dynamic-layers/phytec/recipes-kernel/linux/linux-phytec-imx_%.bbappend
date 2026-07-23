FILESEXTRAPATHS:prepend := "${THISDIR}/linux:"

SRC_URI += "\
        file://0001-feat-linux-imx-added-ramoops-for-tauril2.patch \
        file://0001-net-fec-fix-unbalanced-clk-disable-when-register_net.patch \
        file://0002-net-fec-defer-probe-instead-of-failing-on-netdev-nam.patch \
        file://0003-arm64-dts-imx8mm-phycore-som-fix-DP83867-reset-timin.patch \
"
