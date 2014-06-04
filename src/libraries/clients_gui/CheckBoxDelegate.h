// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui_CheckBoxDelegate_h_
#define __gui_CheckBoxDelegate_h_

#include "CommonDelegate.h"

namespace gui
{
// =============================================================================
/** @class  CheckBoxDelegate
    @brief  CheckBoxDelegate
*/
// Created: LGY 2014-06-03
// =============================================================================
class CheckBoxDelegate : public CommonDelegate
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CheckBoxDelegate( QObject* parent = 0 );
    virtual ~CheckBoxDelegate();
    //@}

    //! @name QItemDelegate operations
    //@{
    virtual void drawCheck( QPainter* painter, const QStyleOptionViewItem& option, const QRect& rect, Qt::CheckState state ) const;
    virtual bool editorEvent( QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index );
    //@}
};

}

#endif // __gui_CheckBoxDelegate_h_
