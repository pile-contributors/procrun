/**
 * @file procrunitem.h
 * @brief Declarations for ProcRunGroup class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCRUN_GROUP_H_INCLUDE
#define GUARD_PROCRUN_GROUP_H_INCLUDE

#include <procrun/procrun-config.h>
#include <procrun/procrun.h>
#include <procrun/procrunitem.h>

#include <QList>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

//! A group of items.
class PROCRUN_EXPORT ProcRunGroup : public ProcRunItemBase {

    friend class ProcRunGroup;

public:

    //! Default constructor.
    ProcRunGroup (
            const QString & s_name = QString ());

    //! Destructor.
    virtual ~ProcRunGroup ();

    //! Is this a valid index?
    bool
    isIndexValid (int idx) const {
        return ((idx >= 0) && (idx < items_.count ()));
    }

    //! The type of this entry.
    virtual Type
    type () const {
        return GroupType;
    }

    //! Take the ownership of an item and remove it.
    ProcRunItemBase *
    item (
            int idx) const;

    //! Add an item; ownership transferred to this instance.
    void
    append (
            ProcRunItemBase * item);

    //! Add an item; ownership transferred to this instance.
    void
    insertAt (
            int index,
            ProcRunItemBase * item);

    //! Add an item; ownership transferred to this instance.
    int
    indexOf (
            const ProcRunItemBase * item) const;

    //! Number of kids.
    int
    count () const {
        return items_.count ();
    }

    //! Destroy all items.
    void
    clear ();

    //! Take the ownership of an item and remove it.
    ProcRunItemBase *
    take (
            int idx);

    //! Take the ownership of an item and remove it.
    ProcRunItemBase *
    take (
            ProcRunItemBase * item);

    //! Remove an item and delete it.
    void
    remove (
            int idx);

    //! Remove an item and delete it.
    void
    remove (
            ProcRunItemBase * item);

    //! Save this instance from settings.
    virtual bool
    save (
            QSettings & stg);

    //! Create an instance and load it from settings.
    static ProcRunGroup *
    load(
            QSettings & stg);

    //! Load this instance from settings.
    virtual bool
    loadMyself (
            QSettings & stg);

    //! The name of this group.
    const QString
    name () const {
        return s_name_;
    }


public:

    QString s_name_; /**< the name of this group */
    QList<ProcRunItemBase*> items_; /**< the list of items belonging to this group */

};


#endif // GUARD_PROCRUN_GROUP_H_INCLUDE
