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

    //! Default constructor.
    ProcRun ();

    //! Destructor.
    virtual ~ProcRun();

    qint64
    runDuration() const {
        return end_time_.secsTo (start_time_);
    }

    void perform (
            const QStringList & input);

    int durationInSeconds () {
        return end_time_.secsTo (start_time_);
    }

    int durationInMiliSeconds () {
        return end_time_.msecsTo (start_time_);
    }

private slots:

    void
    errorSlot (
            QProcess::ProcessError error);

    void
    finishedSlot (
            int exitCode,
            QProcess::ExitStatus exitStatus);

    void
    readyReadStandardErrorSlot ();

    void
    readyReadStandardOutputSlot ();

    void
    startedSlot ();

    void
    stateChangedSlot (
            QProcess::ProcessState newState);

public:

    QDateTime start_time_;
    QDateTime end_time_;
    QString std_err_output_;
    QString std_out_output_;
    QString all_output_;
    bool b_started_;
    QList<QProcess::ProcessError> errors_;
    QList<QProcess::ProcessState> states_;

private:
    QEventLoop ev_loop_;

};

#endif // GUARD_PROCRUN_H_INCLUDE
