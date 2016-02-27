/**
 * @file procrun.h
 * @brief Declarations for ProcRun class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCRUN_H_INCLUDE
#define GUARD_PROCRUN_H_INCLUDE

#include <procrun/procrun-config.h>

#include <QList>
#include <QProcess>
#include <QDateTime>
#include <QEventLoop>

//! Helper class to run a process.
class PROCRUN_EXPORT ProcRun : public QProcess {
    Q_OBJECT

public:
    //! The callback used by the class to inform the owner about
    //! program completion.
    typedef void (*Kb) (ProcRun *, void *);

    //! Default constructor.
    ProcRun ();

    //! Destructor.
    virtual ~ProcRun();

    //! Get the duration in seconds.
    qint64
    runDuration() const {
        return end_time_.secsTo (start_time_);
    }

    //! Start the program, optionally inside its own event loop.
    void perform (
            const QStringList & input = QStringList(),
            bool block_wait = true);

    //! Get the duration in seconds.
    int durationInSeconds () {
        return static_cast<int>(end_time_.secsTo (start_time_));
    }

    //! Get the duration in milliseconds.
    int durationInMiliSeconds () {
        return static_cast<int>(end_time_.msecsTo (start_time_));
    }

private slots:

    //! Some output coming out of error channel.
    void
    readyReadStandardErrorSlot ();

    //! Some output coming out of output channel.
    void
    readyReadStandardOutputSlot ();

    //! Connected to keep the started/not started state.
    void
    startedSlot ();

    //! Connected to keep the started/not started state.
    void
    finishedSlot (
            int exitCode,
            QProcess::ExitStatus exitStatus);

    //! Track this slot to accumulate the list of states.
    void
    stateChangedSlot (
            QProcess::ProcessState newState);

    //! Accumulate errors here.
    void
    errorSlot (
            QProcess::ProcessError error);

signals:

    //! Emitted when the process ended.
    void procRunFinish ();

public:

    QDateTime start_time_; /**< the time when the process was started */
    QDateTime end_time_; /**< the time when the process ended */
    QString std_err_output_; /**< the output through error channel */
    QString std_out_output_; /**< the output through output channel */
    QString all_output_; /**< the output through output and error channel */
    bool b_started_; /**< is the process already running? */
    QList<QProcess::ProcessError> errors_; /**< list of errors */
    QList<QProcess::ProcessState> states_; /**< list of states*/
    Kb kb_; /**< function to be called when the process ends */
    void * user_data_; /**< opaque user data */

private:
    QEventLoop ev_loop_; /**< event loop for running the program sequentially */

};

#endif // GUARD_PROCRUN_H_INCLUDE
