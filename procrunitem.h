/**
 * @file procrunitem.h
 * @brief Declarations for ProcRunItem class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCRUN_ITEM_H_INCLUDE
#define GUARD_PROCRUN_ITEM_H_INCLUDE

#include <procrun/procrun-config.h>
#include <procrun/procrun.h>
#include <procrun/procrundata.h>

#include <QString>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class ProcRunGroup;

//! Common base class for items and groups.
class PROCRUN_EXPORT ProcRunItemBase {

    friend class ProcRunGroup;

public:

    //! The kinds of items we know of.
    enum Type {
        InvalidType = -1,
        GroupType, /**< the group of items */
        CommandType /**< the actual item */
    };

    //! Constructor.
    ProcRunItemBase () :
        parent_ (NULL)
    {}

    //! Virtual destructor.
    virtual ~ProcRunItemBase ()
    {}

    //! The type of this entry.
    virtual Type
    type () const = 0;

    //! Save this instance from settings.
    virtual bool
    save (
            QSettings & stg) = 0;

    //! Load this instance from settings.
    bool
    virtual loadMyself (
            QSettings & stg) = 0;

    //! Get the parent of this item.
    ProcRunGroup *
    parent () const {
        return parent_;
    }

    //! Get the parent of this item.
    int
    indexInParent () const;

protected:

    //! Set the parent of this item.
    void
    setParent (
            ProcRunGroup * parent) {
        parent_ = parent;
    }

private:
    ProcRunGroup * parent_;
};


//! A process, arguments, input and working directory.
class PROCRUN_EXPORT ProcRunItem : public ProcRunItemBase, public ProcRunData {

public:

    //! Default constructor.
    ProcRunItem (
            const QString & s_program = QString (),
            const QStringList & sl_args = QStringList(),
            const QString & s_crt_path = QString (),
            const QStringList & sl_input = QStringList());

    //! Destructor.
    virtual ~ProcRunItem ();

    //! The type of this entry.
    virtual Type
    type () const {
        return CommandType;
    }

    //! Run this instance.
    ProcRun *
    run (
            ProcRun::Kb kb_ = NULL,
            void * user_data_ = NULL,
            bool block_wait = true);

    //! Save this instance from settings.
    virtual bool
    save (
            QSettings & stg);

    //! Create an instance and load it from settings.
    static ProcRunItem *
    load(
            QSettings & stg);

    //! Load this instance from settings.
    bool
    virtual loadMyself (
            QSettings & stg);


};

#endif // GUARD_PROCRUN_ITEM_H_INCLUDE
