if (NOT PROCRUN_BUILD_MODE)
    set (PROCRUN_BUILD_MODE STATIC)
endif ()

include(pile_support)
pileInclude (ProcRun)
procrunInit(${PROCRUN_BUILD_MODE})
