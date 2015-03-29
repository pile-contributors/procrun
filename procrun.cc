/**
 * @file procrun.cc
 * @brief Definitions for ProcRun class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procrun.h"
#include "procrun-private.h"

#include <QCoreApplication>
#include <QProcess>


/**
 * @class ProcRun
 *
 * Detailed description.
 */

/* ------------------------------------------------------------------------- */
/**
 * Constructor connects the slots to Qt object.
 */
ProcRun::ProcRun() :
    QProcess(),
    start_time_(QDateTime::currentDateTime ()),
    end_time_(QDateTime::currentDateTime ()),
    std_err_output_(),
    std_out_output_(),
    all_output_(),
    b_started_(false),
    errors_(),
    states_(),
    ev_loop_()
{
    PROCRUN_TRACE_ENTRY;

    connect (this,
             SIGNAL(error(QProcess::ProcessError)),
             SLOT(errorSlot(QProcess::ProcessError)));
    connect (this,
             SIGNAL(finished(int,QProcess::ExitStatus)),
             SLOT(finishedSlot(int,QProcess::ExitStatus)));
    connect (this,
             SIGNAL(readyReadStandardError()),
             SLOT(readyReadStandardErrorSlot()));
    connect (this,
             SIGNAL(readyReadStandardOutput()),
             SLOT(readyReadStandardOutputSlot()));
    connect (this,
             SIGNAL(started()),
             SLOT(startedSlot()));
    connect (this,
             SIGNAL(stateChanged (QProcess::ProcessState)),
             SLOT(stateChangedSlot (QProcess::ProcessState)));
    connect (this,
             SIGNAL(finished(int,QProcess::ExitStatus)),
             &ev_loop_,
             SLOT(quit()));

    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * Destructor.
 */
ProcRun::~ProcRun()
{
    PROCRUN_TRACE_ENTRY;
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRun::errorSlot (QProcess::ProcessError error)
{
    PROCRUN_TRACE_ENTRY;
    errors_.append (error);
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRun::finishedSlot (int, QProcess::ExitStatus)
{
    PROCRUN_TRACE_ENTRY;
    // no need to cache them as they are available from QProcess
    end_time_ = QDateTime::currentDateTime ();
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRun::readyReadStandardErrorSlot ()
{
    PROCRUN_TRACE_ENTRY;
    QString s (readAllStandardError ());
    std_err_output_.append (s);
    all_output_.append (s);
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRun::readyReadStandardOutputSlot ()
{
    PROCRUN_TRACE_ENTRY;
    QString s (readAllStandardOutput());
    std_out_output_.append (s);
    all_output_.append (s);
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRun::startedSlot ()
{
    PROCRUN_TRACE_ENTRY;
    b_started_ = true;
    start_time_ = QDateTime::currentDateTime ();
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRun::stateChangedSlot (QProcess::ProcessState newState)
{
    PROCRUN_TRACE_ENTRY;
    states_.append (newState);
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRun::perform (const QStringList & input)
{
    PROCRUN_TRACE_ENTRY;

    // start the program
    this->start (QIODevice::ReadWrite);
    if (!this->waitForStarted()) {
        return;
    }

    // provide the input
    foreach (const QString & s, input) {
        this->write (s.toLatin1 ().constData ());
    }
    this->closeWriteChannel();

    // allow it to run
    // this->waitForFinished();
    ev_loop_.exec ();

    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */







