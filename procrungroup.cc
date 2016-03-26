/**
 * @file procrungroup.cc
 * @brief Definitions for ProcRunGroup class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procrungroup.h"
#include "procrunitem.h"
#include "procrun-private.h"

#include <QCoreApplication>
#include <QProcess>
#include <QSettings>

#include <assert.h>

/**
 * @class ProcRunGroup
 *
 */

/* ------------------------------------------------------------------------- */
/**
 */
ProcRunGroup::ProcRunGroup (const QString & s_name) :
    ProcRunItemBase (),
    s_name_(s_name),
    items_ ()
{
    PROCRUN_TRACE_ENTRY;
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * Destructor will destroy all items that are part of this group.
 */
ProcRunGroup::~ProcRunGroup()
{
    PROCRUN_TRACE_ENTRY;
    clear ();
    PROCRUN_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ProcRunItemBase *ProcRunGroup::item (int idx) const
{
    if (isIndexValid (idx)) {
        return items_.at (idx);
    } else {
        PROCRUN_DEBUGM ("Index %d is outside valid range [0, %d)\n",
                        idx, items_.count ());
        return NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRunGroup::append (ProcRunItemBase *item)
{
    assert(!items_.contains (item));
    item->setParent (this);
    items_.append (item);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRunGroup::insertAt (int index, ProcRunItemBase *item)
{
    item->setParent (this);
    items_.insert (index, item);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int ProcRunGroup::indexOf (const ProcRunItemBase *item) const
{
    return items_.indexOf (const_cast<ProcRunItemBase *>(item));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRunGroup::clear ()
{
    qDeleteAll (items_);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ProcRunItemBase * ProcRunGroup::take (int idx)
{
    if (!isIndexValid (idx)) {
        PROCRUN_DEBUGM ("Index %d is outside valid range [0, %d)\n",
                        idx, items_.count ());
        return NULL;
    }
    return take (items_.at (idx));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ProcRunItemBase * ProcRunGroup::take (ProcRunItemBase *item)
{
    int idx = items_.indexOf (item);
    if (idx == -1) {
        PROCRUN_DEBUGM ("Item %p is not part of group %p\n",
                        item, this);
        return NULL;
    }

    items_.removeAt (idx);
    item->setParent (NULL);
    return item;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRunGroup::remove (ProcRunItemBase *item)
{
    int idx = items_.indexOf (item);
    if (idx == -1) {
        PROCRUN_DEBUGM ("Item %p is not part of group %p\n",
                        item, this);
        return;
    }

    items_.removeAt (idx);
    delete item;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcRunGroup::remove (int idx)
{
    if (!isIndexValid (idx)) {
        PROCRUN_DEBUGM ("Index %d is outside valid range [0, %d)\n",
                        idx, items_.count ());
        return;
    }
    return remove (items_.at (idx));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcRunGroup::save (QSettings & stg)
{
    bool b_ret = false;
    for (;;) {

        SAVE_VALUE(s_name_);
        stg.beginWriteArray (QSTRINGIFY(items_), items_.count ());
        b_ret = true;
        int i = 0;
        foreach(ProcRunItemBase * item, items_) {
            stg.setArrayIndex (i++);
            stg.setValue (QSTRINGIFY(type), item->type ());
            b_ret = b_ret && item->save (stg);
        }
        stg.endArray ();
        b_ret = true;
        break;
    }
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ProcRunGroup * ProcRunGroup::load (QSettings &stg)
{
    ProcRunGroup * result = new ProcRunGroup ();
    if (!result->loadMyself (stg)) {
        delete result;
        result = NULL;
    }
    return result;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcRunGroup::loadMyself (QSettings &stg)
{
    bool b_ret = false;
    for (;;) {

        LOAD_STRING(s_name_);
        int cnt = stg.beginReadArray (QSTRINGIFY(items_));
        for (int i = 0; i < cnt; ++i) {
            stg.setArrayIndex (i);

            Type ty = static_cast<Type> (
                        stg.value (QSTRINGIFY(type), InvalidType).toInt ());
            switch (ty) {
            case InvalidType: {
                PROCRUN_DEBUGM("Invalid type in settings\n");
                break; }
            case GroupType: {
                ProcRunGroup * gr = ProcRunGroup::load (stg);
                items_.append (gr);
                break; }
            case CommandType: {
                ProcRunItem * it = ProcRunItem::load (stg);
                items_.append (it);
                break; }
            }
        }
        stg.endArray ();
        b_ret = true;
        break;
    }
    return b_ret;
}
/* ========================================================================= */


