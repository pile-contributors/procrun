
# enable/disable cmake debug messages related to this pile
set (PROCRUN_DEBUG_MSG OFF)

# make sure support code is present; no harm
# in including it twice; the user, however, should have used
# pileInclude() from pile_support.cmake module.
include(pile_support)

# initialize this module
macro    (procrunInit
          ref_cnt_use_mode)

    # default name
    if (NOT PROCRUN_INIT_NAME)
        set(PROCRUN_INIT_NAME "ProcRun")
    endif ()

    # Compose the list of headers and sources.

    set(PROCRUN_HEADERS
        "procrunmodel.h"
        "procrun.h"
        "procrunitem.h"
        "procrungroup.h")

    set(PROCRUN_SOURCES
        "procrunmodel.cc"
        "procrun.cc"
        "procrunitem.cc"
        "procrungroup.cc")

    pileSetSources(
        "${PROCRUN_INIT_NAME}"
        "${PROCRUN_HEADERS}"
        "${PROCRUN_SOURCES}")

    set(PROCRUN_QT_MODS Core Widgets)
    set(CMAKE_AUTOMOC ON)

    pileSetCommon(
        "${PROCRUN_INIT_NAME}"
        "0;0;1;d"
        "ON"
        "${ref_cnt_use_mode}"
        "AppLib"
        "runtime"
        "nogui;process")

endmacro ()
