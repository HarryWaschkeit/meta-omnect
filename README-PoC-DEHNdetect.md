# PoC omnect-OS on device DEHNdetect - current state

With the help of meta-karo, meta-karo-bsp and quite some effort to
find out how the BSP from AST-X builds device artefacts and how they
get applied to the device a first quick and very dirty version of an
omnect OS image could be built.

There are a lot shortcomings, hacks and missing features which need to
be addressed for a first official support of this device.

The new machine is called tx6s-8035, identical to the the article
number of Karo's TX standard module TX6S as used for the DEHNdetect
device.

In first two chapters below there are - most likely incomplete - lists
of hacks, shortcomings and missing stuff of the first implementation,
in no particular order.

And after that another chapter deals with special treatment how to
apply build artifacts to TX6S modules as originally programmed for
DEHNdetect devices.


## Hacks & shortcomings

- overall structure of code additions is quick and very dirty
  - direct meta-karo modifications shouldn't be done
  - instead several things like machine configs should be placed into
    a separate and Dehn specific layer (meta-omnect-dehn/meta-karo-dehn?)
  - placement of various board specific Yocto paramters needs to be
    refactored to meet standards as established for other platforms
- U-Boot setup assumes two environments on the device but originally
  only one is configured, so as a hack the BSP was modified to allow
  for only one environment; this should probably changed to two
  environments also here, because there's enough space in flash
- flash-mode-2 uses U-Boot command `load` to get data from mass
  storage like eMMC, SD card or USB in an  file system agnostic way;
  this is not supported by the old U-Boot version originally used in
  DEHNdetect devices, so these commands were replaced with the fitting
  dedicated load command `fatload` or `ext2load`, respectively.
- another flash-mode-2 change makes use of local WIC xz file for
  integrity check optional depending on variable `verify_before_write`
  which is currently set nowhere (needs to be set platform dependent)
- for compilation of U-Boot comments in `omnect_env.h` were converted
  from C++ to normal C style due to compiler complaints causing
  errors; maybe make the compiler ignore these warnings instead of
  treating them as errors
- another U-Boot shortcoming due to ancient version is the lack to
  handle symbolic links on EXT[234] fileystems; as a crude hack file
  `zImage.bin` gets copied a second time instead of being a symbolic
  link to the right file
- revert udev hack for making all input event files
  (`/dev/input/event[01...]`) world readable so that DEHNdetect
  simulator app can read keyboard events for flash event simulation
- currently the official device tree as contained in vanilla Linux
  kernel's sources is used; it needs to be adapted to reflect the
  hardware settings on DEHNdetect device (including tests for all used
  interfaces)
- review partition layout whether it can be optimized: for the sake of
  simplicity the tauri layout was used with bootloader settings
  removed, so there's a bit eMMC space lost at the moment
- use of swapfile due to little RAM (512MB) which is currently
  implemented as initramfs step; possibly that's the wrong way to do
  it, but certainly the file size shoud be discussed
- for debugging purposes a shell exit was installed during initramfs
  process which can be enabled by providing kernel parameter
  `myshell=1` and which has to be removed again
- kernel driver module `imx_sdma` is called `imx-sdma` for unknown
  reason in linux-karo (which actually refers to vanilla kernel);
  therefore initramfs module `imx-sdma` modprobes also imx-sdma in
  case probing of `imx_sdma` didn't cause kernel module `imx_sdma` to
  get loaded.
- kernel configuration is currently handles as complete defconfig
  which might be not the desired way

## Missing stuff and other ToDos

- handling of bootloader on different storage medium is incomplete
  - multiple images for swupdate needed
  - current abstraction of bootloader device (ubootblk-dev) needs to
    be reviewed
- artefacts suitable to convert original DEHNdetect flash setup to
  omnect setup
- define correct rootfs sizes
- swupdate handling misses several things:
  - correct sw-description file (currently completely lacking boot
    loader section)
  - handling of multiple images in update: one for main data medium
    (here `/dev/mmcblk0`) and another for boot loader medium (here
    `/dev/mmcblk0boot0`)
  - handling of U-Boot environment, also on bootloader medium
  - bootloader file is currently not generated for
    omnect-os-update-image.bb (catchword `SWUPDATE_IMAGES`)
- out-of-tree kernel driver for FPGA needs to be checked whether it
  can be used with current kernel versions (probably not without
  modifications)
- performance of current kernel is much below that of Karo's reference
  kernel (which is based on 4.13); it needs to be checked what this
  gap is caused by: is it really only all of the virtualization stuff
  needed for running containers or are there other kernel options
  enabled now slowing down things?
  (check kernel 5.15 again with respect to eMMC write performance
  which dropped from ~13MB/s in Karo's reference kernel to ~5MB/s in
  current kernel in an unstressed system i.e., with all cloud stuff
  torn down)
- CI/CD is completely missing, of course, only a local concourse
  pipeline was used to ensure that images can be built which work to
  the extent reached with local dobi builds in dirty repositories

## How to re-program TX6S modules from DEHNdetect devices

In order to convert a DEHNdetect TX6S module to an omnect device it
needs to be placed into Karo's evaluation board TX 7, because the
DEHNdetect device lacks a connector for the system console which makes
it impossible to intervene in boot process by stopping and
instrumenting U-Boot bootloader.

Once plugged into eval board with serial interface connected via
USB-2-serial converter to the computer full control over boot process
is guaranteed.

- via flashed bootloader artefacts built by omnect BSP can be loaded
  from network:
  - Linux kernel `zImage`
  - device tree file `imx6dl-tx6s-8035.dtb`
  - initramfs `omnect-os-initramfs-tx6s-8035.cpio.gz.u-boot`
- linux kernel paramters need to be set manually
- before starting linux set flash-mode variable in U-Boot environment
  to 2 so that 

```shell
TX6S U-Boot > echo tftp $kernel_addr_r 
tftp 18000000 tx6s-8035-kirkstone/zImage
TX6S U-Boot > 
Using FEC device
TFTP from server 10.2.1.217; our IP address is 10.2.1.30
Filename 'tx6s-8035-kirkstone/zImage'.
Load address: 0x18000000
Loading: #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         ######
         2.7 MiB/s
done
Bytes transferred = 7713176 (75b198 hex)
TX6S U-Boot > tftp $ramdisk
Using FEC device
TFTP from server 10.2.1.217; our IP address is 10.2.1.30
Filename 'tx6s-8035-kirkstone/omnect-os-initramfs-tx6s-8035.cpio.gz.u-boot'.
Load address: 0x11200000
Loading: #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         #################################################################
         ###################
         2.7 MiB/s
done
Bytes transferred = 17453976 (10a5398 hex)
TX6S U-Boot > setenv bootargs "root=/dev/mmcblk0p2   console=ttymxc0,115200 uboot=/dev/mmcblk0boot0"
TX6S U-Boot > setenv flash-mode 2
TX6S U-Boot > bootz $kernel_addr_r $ramdisk_addr_r $fdtaddr
Kernel image @ 0x18000000 [ 0x000000 - 0x75b198 ]
## Loading init Ramdisk from Legacy Image at 11200000 ...
   Image Name:   hwt-OMNECT-gateway-devel_4.0.14.
   Image Type:   ARM Linux RAMDisk Image (uncompressed)
   Data Size:    17453912 Bytes = 16.6 MiB
   Load Address: 00000000
   Entry Point:  00000000
   Verifying Checksum ... OK
## Flattened Device Tree blob at 11000000
   Booting using the fdt blob at 0x11000000
   Loading Ramdisk to 2e683000, end 2f728358 ... OK
   Loading Device Tree to 2e675000, end 2e6820c5 ... OK

Starting kernel ...

[    0.000000] Booting Linux on physical CPU 0x0
[    0.000000] Linux version 5.15.24-karo+ga0ebea480bb3 (support@karo-electronics.de) (arm-omnect-linux-gnueabi-gcc (GCC) 11.4.0, GNU ld (GNU Binutils) 2.38.20220708) #1 SMP PREEMPT Wed Feb 16 11:56:41 UTC 2022
[...]
```

Note that the boot process will fail nevertheless, because of the
different partition layout on original flash layout.

You will end up in a root shell and can manually execute necessary
parts of /init, source /init.d/05-common and /init.d/87-flash_mode_2
and run now available function `run_flash_mode_2`.

Something like this:

```shell
bash-5.1# (. init.d/05-common_sh; . init.d/87-flash_mode_2; run_flash_mode_2)
```

With this you will end up in a working network where you can supply
WIC `.bmap` and `.xz` files with scp as instructed on the serial
console.
All that under the condition that you have the Ethernet attached to
a network where a DHCP is answering the device's DHCP request.

## Tips

### How to help oneself if boot loader got screwed up

The TX 7 eval board has a boot jumper allowing the TX6 module to be
started via USB OTG interface: the mini USB cable needs to be
connected with the computer, so that a Freescale i.MX device shows up
in USB device list (`lsusb`).

Debian package `imx-usb-loader` contains tool`'imx_usb` which searches
for such a device and sends the binary file provided on command line to
the i.MX6 so that it boots from it.

An invocation on the computer looks like this:

```shell
$ imx_usb u-boot.bin
```

The file `u-boot.bin` shown in command line above is the identically
named artefact generated by the Yocto BSP during `bitbake
omnect-osimage`,
or to be precise by `bitbake virtual/bootloader` or `bitbake
u-boot-karo`.

The serial console can be used to observe the immidiately following
boot process.
