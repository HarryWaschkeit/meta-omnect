#if !defined omnect_env_h
#define omnect_env_h

#include <configs/omnect_env_machine.h>

/* Todo dev and mmcdev are redundant: needs refactoring of bootscript */
/* Todo "env_initialized:do" doesnt work */
#define OMNECT_REQUIRED_WRITEABLE_ENV_FLAGS \
    "bootpart:dw," \
    "data-mount-options:sw," \
    "env_initialized:dw," \
    "factory-reset:dw," \
    "factory-reset-restore-list:sw," \
    "factory-reset-status:sw," \
    "flash-mode:dw," \
    "flash-mode-devpath:sw," \
    "omnect_validate_update:bw," \
    "omnect_validate_update_part:dw," \
    "resized-data:sw"

// activated by either u-boot_%.bbappend or u-boot-imx_%.bbappend
//#define OMNECT_RELEASE_IMAGE
#ifdef OMNECT_RELEASE_IMAGE
#define CONFIG_BOOTCOMMAND "run omnect_update_flow; reset"
#define OMNECT_ENV_SETTINGS \
    "bootdelay=-2\0" \
    "silent=1\0"
#else
#define CONFIG_BOOTCOMMAND "run omnect_update_flow"
#define OMNECT_ENV_SETTINGS
#endif //OMNECT_RELEASE

// set by either u-boot_%.bbappend or u-boot-imx_%.bbappend
#define OMNECT_ENV_EXTRA_BOOTARGS

// u-boot part of omnect update workflow
#define OMNECT_ENV_UPDATE_WORKFLOW \
    "omnect_update_flow=" \
        "if test -n ${omnect_validate_update}; then " \
            "echo \"Update validation failed - booting from partition ${bootpart}\";" \
            "setenv omnect_validate_update_part;" \
            "setenv omnect_validate_update;" \
            "saveenv;" \
            "run distro_bootcmd;" \
        "else " \
            "if test -n ${omnect_validate_update_part}; then " \
                "echo \"Update in progress - booting from partition ${omnect_validate_update_part}\";" \
                "setenv omnect_validate_update 1;" \
                "saveenv;" \
                "setenv bootpart ${omnect_validate_update_part};" \
                "run distro_bootcmd;" \
            "else "\
                "run distro_bootcmd;" \
            "fi;" \
        "fi\0"

// for secureboot, or forcing e.g. silent env var not to be changeable in release image
#ifdef CONFIG_ENV_WRITEABLE_LIST
#define CONFIG_ENV_FLAGS_LIST_STATIC \
    OMNECT_REQUIRED_WRITEABLE_ENV_FLAGS \
    OMNECT_REQUIRED_WRITEABLE_ENV_FLAGS_MACHINE
#endif //CONFIG_ENV_WRITEABLE_LIST

// boot retry enabled, but not configured https://github.com/u-boot/u-boot/blob/master/doc/README.autoboot
#define CONFIG_BOOT_RETRY_TIME -1
#define CONFIG_RESET_TO_RETRY

#endif //omnect_env_h
