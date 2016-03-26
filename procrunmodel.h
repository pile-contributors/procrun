/**
 * @file procrunitem.h
 * @brief Declarations for ProcRunModel class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCRUN_MODEL_H_INCLUDE
#define GUARD_PROCRUN_MODEL_H_INCLUDE

#include <procrun/procrun-config.h>
#include <procrun/procrun.h>
#include <procrun/procrungroup.h>

#include <QAbstractItemModel>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

//! A tree model for groups of programs and programs.
class PROCRUN_EXPORT ProcRunModel : public QAbstractItemModel {

public:

    //! Default constructor.
    ProcRunModel (
            QObject * parent = NULL);

    //! Destructor.
    virtual ~ProcRunModel ();


    //! Save this instance from settings.
    bool
    save (
            QSettings & stg);

    //! Load this instance from settings.
    bool
    load (
            QSettings & stg);

    //! The item was updated outside the model.
    void
    itemChanged (
            ProcRunItemBase * item);

    //! Insert the item into given group.
    bool
    insertItem (
            ProcRunItemBase * item,
            int index = -1,
            ProcRunGroup * group = NULL);

    //! Get the item associated with a model index.
    ProcRunItemBase *
    itemFromIndex (
            const QModelIndex & index);

    //! Get the index associated with an item.
    QModelIndex
    indexFromItem (
            ProcRunItemBase * item);

    //! Remove a particular item.
    bool
    removeItem (
            ProcRunItemBase * item);

    /* == == == == == == == == == == == == == == == == */
    /** @name QAbstractItemModel
    *  Reimplemented methods from QAbstractItemModel.
    */
    ///@{

public:

    virtual QModelIndex
    index (
            int row,
            int column,
            const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex
    parent (
            const QModelIndex &child) const;

    virtual QModelIndex
    sibling (
            int row,
            int column,
            const QModelIndex &idx) const;

    virtual int
    rowCount (
            const QModelIndex &parent = QModelIndex()) const;

    virtual int
    columnCount (
            const QModelIndex &parent = QModelIndex()) const;

    virtual bool
    hasChildren (
            const QModelIndex &parent = QModelIndex()) const;

    virtual QVariant
    data (
            const QModelIndex &index,
            int role = Qt::DisplayRole) const;

    virtual QVariant
    headerData (
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags
    flags (
            const QModelIndex &index) const;

    //! Remove some rows.
    virtual bool
    removeRows (
            int row,
            int count,
            const QModelIndex &parent = QModelIndex());

    ///@}
    /* == == == == == == == == == == == == == == == == */

public:

    ProcRunGroup root_; /**< root group */

};


#endif // GUARD_PROCRUN_MODEL_H_INCLUDE
