// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __LinkItemDelegate_h_
#define __LinkItemDelegate_h_

namespace gui
{

// =============================================================================
/** @class  LinkItemDelegate
    @brief  LinkItemDelegate
*/
// Created: JSR 2012-10-19
// =============================================================================
class LinkItemDelegate : public QStyledItemDelegate
{
Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LinkItemDelegate( QWidget* parent );
    virtual ~LinkItemDelegate();
    //@}

    //! @name Operations
    //@{
    virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual bool editorEvent( QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index );
    //@}

signals:
    //! @name Signals
    //@{
    void LinkClicked( const QString& url, const QModelIndex& index );
    //@}
};

} // namespace gui

#endif // __LinkItemDelegate_h_
