/**
 * @file procrun-private.h
 * @brief Declarations for ProcRun class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCRUN_PRIVATE_H_INCLUDE
#define GUARD_PROCRUN_PRIVATE_H_INCLUDE

#include <procrun/procrun-config.h>

#if 0
#    define PROCRUN_DEBUGM printf
#else
#    define PROCRUN_DEBUGM black_hole
#endif

#if 0
#    define PROCRUN_TRACE_ENTRY printf("PROCRUN ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define PROCRUN_TRACE_ENTRY
#endif

#if 0
#    define PROCRUN_TRACE_EXIT printf("PROCRUN EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define PROCRUN_TRACE_EXIT
#endif


static inline void black_hole (...)
{}

#endif // GUARD_PROCRUN_PRIVATE_H_INCLUDE
