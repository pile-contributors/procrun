/**
 * @file procrundata.cc
 * @brief Definitions for ProcRunData class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procrundata.h"
#include "procrun-private.h"

ProcRunData::ProcRunData (
        const QString &s_program, const QStringList &sl_args,
        const QString &s_crt_path, const QStringList &sl_input) :
    s_program_(s_program),
    sl_arguments_(sl_args),
    s_wrk_dir_(s_crt_path),
    sl_input_(sl_input)
{
}

void ProcRunData::setupProcess (QProcess *proc) const
{
    proc->setProgram (s_program_);
    proc->setArguments (sl_arguments_);
    proc->setWorkingDirectory (s_wrk_dir_);
}
