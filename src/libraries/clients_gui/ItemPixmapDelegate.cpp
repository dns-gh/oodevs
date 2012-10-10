// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ItemPixmapDelegate.h"
#include "StandardModel.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ItemPixmapDelegate constructor
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
ItemPixmapDelegate::ItemPixmapDelegate( const StandardModel& dataModel, T_PixmapGetter getter, QObject *parent, int height /*= 0*/ )
    : QItemDelegate( parent )
    , dataModel_( dataModel )
    , getter_( getter )
    , height_( height )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ItemPixmapDelegate destructor
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
ItemPixmapDelegate::~ItemPixmapDelegate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ItemPixmapDelegate::paint
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void ItemPixmapDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    QItemDelegate::paint( painter, option, index );
    if( index.column() == 1 )
        if( const kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( dataModel_.GetMainModelIndex( index ) ) )
            if( const QPixmap* pixmap = getter_( *entity ) )
                DrawPixmap( painter, option.rect, *pixmap );
}

// -----------------------------------------------------------------------------
// Name: ItemPixmapDelegate::sizeHint
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
QSize ItemPixmapDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    QSize s = QItemDelegate::sizeHint( option, index );
    s.setHeight( std::max( s.height(), height_ ) );
    return s;
}

// -----------------------------------------------------------------------------
// Name: ItemPixmapDelegate::DrawPixmap
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void ItemPixmapDelegate::DrawPixmap( QPainter* painter, const QRect& rect, const QPixmap& pixmap ) const
{
    painter->save();
    QPoint p;
    if( qApp->layoutDirection() == Qt::RightToLeft )
    {
        p = rect.topLeft();
        p.rx() += 5;
    }
    else
    {
        p = rect.topRight();
        p.rx() -= pixmap.width() + 5;
    }
    p.ry() += ( rect.height() - pixmap.height() ) / 2;
    painter->drawPixmap( p, pixmap );
    painter->restore();
}
