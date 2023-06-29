inherit metadata_scm

require recipes-omnect/images/omnect-os-tools.inc

OS_RELEASE_FIELDS += "DISTRO_FEATURES"
OS_RELEASE_FIELDS += "IMAGE_INSTALL"
OS_RELEASE_FIELDS += "MACHINE"
OS_RELEASE_FIELDS += "MACHINE_FEATURES"

OS_RELEASE_FIELDS += "META_OMNECT_GIT_SHA META_OMNECT_GIT_BRANCH"
META_OMNECT_GIT_SHA = "${@base_get_metadata_git_revision('${LAYERDIR_omnect}', d)}"
META_OMNECT_GIT_BRANCH = "${@base_get_metadata_git_branch('${LAYERDIR_omnect}', d)}"
OS_RELEASE_FIELDS += "META_OMNECT_GIT_REPO"
OS_RELEASE_FIELDS += "META_OMNECT_VERSION"

OS_RELEASE_FIELDS += "OMNECT_OS_GIT_SHA"
OS_RELEASE_FIELDS += "OMNECT_OS_GIT_BRANCH"
OS_RELEASE_FIELDS += "OMNECT_OS_GIT_REPO"
OS_RELEASE_FIELDS += "OMNECT_OS_VERSION"

OS_RELEASE_FIELDS += "OMNECT_TOOLS"
OS_RELEASE_FIELDS += "OMNECT_DEVEL_TOOLS"

OS_RELEASE_FIELDS += "OMNECT_RELEASE_IMAGE"

do_compile[nostamp] = "1"
