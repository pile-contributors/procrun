/**
 * @file procrundata.h
 * @brief Declarations for ProcRunData class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCRUN_DATA_H_INCLUDE
#define GUARD_PROCRUN_DATA_H_INCLUDE

#include <procrun/procrun-config.h>

#include <QString>
#include <QStringList>
#include <QProcess>

//! All the data for starting and running process.
struct PROCRUN_EXPORT ProcRunData {

    //! Default constructor.
    ProcRunData (
            const QString & s_program = QString (),
            const QStringList & sl_args = QStringList(),
            const QString & s_crt_path = QString (),
            const QStringList & sl_input = QStringList());

    //! Set a QProcess instance with the data inside.
    void
    setupProcess (
            QProcess * proc) const;


    QString s_program_; /**< the path and name of the program to run */
    QStringList sl_arguments_; /**< command line arguments */
    QString s_wrk_dir_; /**< working directory for the program */
    QStringList sl_input_; /**< input to be passed to the program once it starts */
};

#endif // GUARD_PROCRUN_DATA_H_INCLUDE
