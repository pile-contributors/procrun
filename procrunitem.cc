/**
 * @file procrunitem.cc
 * @brief Definitions for ProcRunItem class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procrunitem.h"
#include "procrun-private.h"
#include "procrungroup.h"

#include <QCoreApplication>
#include <QProcess>
#include <QSettings>


/**
 * @class ProcRunItem
 *
 */

/* ------------------------------------------------------------------------- */
/**
 */
ProcRunItem::ProcRunItem (
        const QString & s_program, const QStringList & sl_args,
        const QString & s_crt_path, const QStringList & sl_input) :
    ProcRunItemBase(),
    ProcRunData(s_program, sl_args, s_crt_path, sl_input)
{
    PROCRUN_TRACE_ENTRY;
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 */
ProcRunItem::~ProcRunItem()
{
    PROCRUN_TRACE_ENTRY;
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * The ownership of the instance is passed to the caller.
 *
 * @param kb (optional) callback informed about process finalization
 * @param user_data (optional) some opaque data to pass with the callback
 * @param block_wait (optional) wait for the process to finish or exit right away
 * @return a process instance
 */
ProcRun * ProcRunItem::run (ProcRun::Kb kb, void *user_data, bool block_wait)
{
    ProcRun * result = new ProcRun ();
    result->kb_ = kb;
    result->user_data_ = user_data;
    result->setProgram (s_program_);
    result->setArguments (sl_arguments_);
    result->setWorkingDirectory (s_wrk_dir_);
    result->perform (sl_input_, block_wait);
    return result;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcRunItem::save (QSettings &stg)
{
    bool b_ret = false;
    for (;;) {

        SAVE_VALUE(s_program_);
        SAVE_VALUE(sl_arguments_);
        SAVE_VALUE(s_wrk_dir_);
        SAVE_VALUE(sl_input_);

        b_ret = true;
        break;
    }
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ProcRunItem * ProcRunItem::load (QSettings &stg)
{
    ProcRunItem * result = new ProcRunItem ();
    if (!result->loadMyself (stg)) {
        delete result;
        result = NULL;
    }
    return result;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcRunItem::loadMyself (QSettings &stg)
{
    bool b_ret = false;
    for (;;) {

        LOAD_STRING(s_program_);
        LOAD_STRLIST(sl_arguments_);
        LOAD_STRING(s_wrk_dir_);
        LOAD_STRLIST(sl_input_);

        b_ret = true;
        break;
    }
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int ProcRunItemBase::indexInParent () const
{
    if (parent() == -NULL) {
        return -1;
    } else {
        return parent ()->indexOf (this);
    }
}
/* ========================================================================= */
