/**
 * @file procrunmodel.cc
 * @brief Definitions for ProcRunModel class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procrunmodel.h"
#include "procrunitem.h"
#include "procrun-private.h"

#include <QCoreApplication>
#include <QProcess>
#include <QSettings>
#include <QFileInfo>
#include <QStyle>
#include <QApplication>

#include <assert.h>

/**
 * @class ProcRunModel
 *
 */

/* ------------------------------------------------------------------------- */
/**
 */
ProcRunModel::ProcRunModel (QObject *parent) :
    QAbstractItemModel (parent),
    root_()
{
    PROCRUN_TRACE_ENTRY;
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 */
ProcRunModel::~ProcRunModel()
{
    PROCRUN_TRACE_ENTRY;
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
QModelIndex ProcRunModel::index (
        int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid ()) {
        if ((row >= 0) && (column >= 0) && (root_.count () > 0)) {
            // This is a top level item.
            return createIndex (row, column, root_.item (row));
        }
    } else {
        // This is not a top level item.
        ProcRunItemBase * itp = static_cast<ProcRunItemBase*> (
                    parent.internalPointer ());
        switch (itp->type ()) {
        case ProcRunItemBase::GroupType: {
            ProcRunGroup * gr = static_cast<ProcRunGroup*> (itp);
            return createIndex (row, column, gr->item (row));
        }
        case ProcRunItemBase::CommandType: {
            // This type does not have any kids.
            break; }
        default:
            PROCRUN_DEBUGM ("Invalid type %d\n", itp->type ());
            break;
        }
    }
    return QModelIndex ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QModelIndex ProcRunModel::parent (
        const QModelIndex & child) const
{
    ProcRunItemBase * itp = static_cast<ProcRunItemBase*> (
                child.internalPointer ());
    if (itp == NULL) {
        DBG_FAILPOINT("Expected an internalPointer");
        return QModelIndex ();
    } else {
        ProcRunGroup * gr = itp->parent();
        if (gr == NULL) {
            // DBG_FAILPOINT("Item should have a valid parent");
            return QModelIndex ();
        } else {
            return createIndex (gr->indexInParent(), 0, gr);
        }
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QModelIndex ProcRunModel::sibling (
        int row, int column, const QModelIndex &idx) const
{
    ProcRunItemBase * itp = static_cast<ProcRunItemBase*> (
                idx.internalPointer ());
    if (itp == NULL) {
        DBG_FAILPOINT("Expected an internalPointer");
    } else {
        ProcRunGroup * gr = itp->parent();
        if (gr == NULL) {
            DBG_FAILPOINT("Item should have a valid parent");
        } else {
            if (!gr->isIndexValid (row)) {
                DBG_FAILPOINT("Item should have a valid row");
            } else {
                return createIndex (row, column, gr->item (row));
            }
        }
    }
    return QModelIndex ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int ProcRunModel::rowCount (
        const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return root_.count ();
    } else {
        ProcRunItemBase * itp = static_cast<ProcRunItemBase*> (
                    parent.internalPointer ());
        switch (itp->type ()) {
        case ProcRunItemBase::GroupType: {
            ProcRunGroup * gr = static_cast<ProcRunGroup*> (itp);
            return gr->count ();
        }
        case ProcRunItemBase::CommandType: {
            // This type does not have any kids.
            break; }
        default:
            PROCRUN_DEBUGM ("Invalid type %d\n", itp->type ());
            break;
        }
    }
    return 0;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int ProcRunModel::columnCount (
        const QModelIndex &parent) const
{
    return 4;
    /*
    if (!parent.isValid()) {
        return root_.count ();
    } else {
        ProcRunItemBase * itp = static_cast<ProcRunItemBase*> (
                    parent.internalPointer ());
        switch (itp->type ()) {
        case ProcRunItemBase::GroupType: {
            return 1;
        }
        case ProcRunItemBase::CommandType: {
            return 4; }
        default:
            PROCRUN_DEBUGM ("Invalid type %d\n", itp->type ());
            break;
        }
    }
    return 0; */
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcRunModel::hasChildren (
        const QModelIndex & parent) const
{
    if (!parent.isValid()) {
        return root_.count() > 0;
    } else {
        ProcRunItemBase * itp = static_cast<ProcRunItemBase*> (
                    parent.internalPointer ());
        switch (itp->type ()) {
        case ProcRunItemBase::GroupType: {
            ProcRunGroup * gr = static_cast<ProcRunGroup*> (itp);
            return gr->count() > 0;
        }
        case ProcRunItemBase::CommandType: {
            return false; }
        default:
            PROCRUN_DEBUGM ("Invalid type %d\n", itp->type ());
            return false;
        }
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QVariant ProcRunModel::data (
        const QModelIndex &index, int role) const
{
    /* MUST be implemented */
    if (!index.isValid()) {
        DBG_FAILPOINT("Expected an internalPointer");
    } else {
        ProcRunItemBase * itp = static_cast<ProcRunItemBase*> (
                    index.internalPointer ());
        switch (itp->type ()) {
        case ProcRunItemBase::GroupType: {
            ProcRunGroup * gr = static_cast<ProcRunGroup*> (itp);
            if ((role == Qt::EditRole) || (Qt::DisplayRole)) {
                if (index.column () == 0) {
                    return gr->name ();
                }
            } else if (role == Qt::DecorationRole) {
                if (index.column () == 0) {
                    return qApp->style()->standardIcon (QStyle::SP_DirIcon);
                }
            }
            break; }
        case ProcRunItemBase::CommandType: {
            ProcRunItem * it = static_cast<ProcRunItem*> (itp);
            if ((role == Qt::EditRole) || (role == Qt::DisplayRole)) {
                switch (index.column ()) {
                case 0: {
                    QFileInfo fi (it->s_program_);
                    return fi.baseName (); }
                case 1: {
                    QFileInfo fi (it->s_program_);
                    return fi.absolutePath (); }
                case 2: {
                    return it->sl_arguments_.join(QChar (' ')); }
                case 3: {
                    return it->s_wrk_dir_; }
                }
            } else if (role == Qt::DecorationRole) {
                if (index.column () == 0) {
                    return qApp->style()->standardIcon (
                                QStyle::SP_ToolBarHorizontalExtensionButton);
                }
            }
            break; }
        default:
            PROCRUN_DEBUGM ("Invalid type %d\n", itp->type ());
            break;
        }
    }
    return QVariant();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QVariant ProcRunModel::headerData (
        int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::EditRole || role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: {
                return tr("Program"); }
            case 1: {
                return tr("Location"); }
            case 2: {
                return tr("Arguments"); }
            case 3: {
                return tr("Working Directory"); }
            }
        }
    }
    return QAbstractItemModel::headerData (section, orientation, role);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Qt::ItemFlags ProcRunModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags (index) |
            Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcRunModel::save (QSettings &stg)
{
    return root_.save (stg);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcRunModel::load (QSettings &stg)
{
    return root_.loadMyself (stg);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRunModel::itemChanged (ProcRunItemBase *item)
{
    QModelIndex topLeft;
    QModelIndex bottomRight;
    int row = item->indexInParent ();
    topLeft = createIndex (row, 0, item);
    if (item->type() == ProcRunItemBase::CommandType) {
        bottomRight = createIndex (row, 3, item);
    } else {
        topLeft = createIndex (row, 0, item);
        bottomRight = topLeft;
    }
    emit dataChanged(topLeft, bottomRight);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcRunModel::insertItem (
        ProcRunItemBase *item, int index, ProcRunGroup *group)
{
    bool b_ret = false;
    for (;;) {
        // Make sure that the item is not already part of a chain.
        if (item->parent() != NULL) {
            PROCRUN_DEBUGM("Item is already part of a group\n");
            break;
        }

        // By default insert at top level.
        QModelIndex parent;
        if (group == NULL) {
            group = &root_;
            parent = createIndex (0, 0, group);
        } else {
            parent = createIndex (group->indexInParent (), 0, group);
        }

        // If the index is wild append.
        if ((index < 0) || (index >= group->count())) {
            index = group->count ();
        }

        beginInsertRows (parent, index, index+1);
        group->insertAt (index, item);
        endInsertRows ();

        b_ret = true;
        break;
    }
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ProcRunItemBase *ProcRunModel::itemFromIndex (const QModelIndex &index)
{
    if (!index.isValid()) {
        return NULL;
    } else {
        ProcRunItemBase * itp = static_cast<ProcRunItemBase*> (
                    index.internalPointer ());
        return itp;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QModelIndex ProcRunModel::indexFromItem (ProcRunItemBase *item)
{
    if (item == NULL) {
        return QModelIndex ();
    }
    return createIndex (item->indexInParent(), 0, item);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcRunModel::removeRows (int row, int count, const QModelIndex &parent)
{
    bool b_ret = false;
    for (;;) {
        ProcRunItemBase * itp = itemFromIndex (parent);
        if (itp == NULL) {
            PROCRUN_DEBUGM("No item assassinated with the index\n");
            break;
        }

        if (itp->type() != ProcRunItemBase::GroupType) {
            PROCRUN_DEBUGM("Items may only be removed from groups\n");
            break;
        }

        ProcRunGroup * grp = static_cast<ProcRunGroup *> (itp);
        if (row < 0) {
            row = 0;
        }
        if (row+count+1 > grp->count()) {
            count = grp->count() - row - 1;
        }

        // Actual removal goes here.
        beginRemoveRows (parent, row, row+count);
        for (int i = row+count-1; i <= row; --i) {
            grp->remove (i);
        }
        endRemoveRows ();

        b_ret = true;
        break;
    }
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcRunModel::removeItem (ProcRunItemBase *item)
{
    bool b_ret = false;
    for (;;) {
        ProcRunGroup * grp = item->parent ();
        if (grp == NULL) {
            PROCRUN_DEBUGM("Item has no parent group\n");
            break;
        }
        int row = grp->indexOf (item);

        beginRemoveRows (createIndex (grp->indexInParent(), 0, grp), row, row+1);
        grp->remove (row);
        endRemoveRows ();

        b_ret = true;
        break;
    }
    return b_ret;
}
/* ========================================================================= */

