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
    if( index.column() != 0 )
        return;
    const int w = static_cast< QAbstractScrollArea* >( parent() )->viewport()->width();
    if( w > option.rect.width() )
    {
        QStyleOptionViewItem pmOption = option;
        if( qApp->layoutDirection() == Qt::RightToLeft )
        {
            pmOption.rect.setRight( pmOption.rect.left() );
            pmOption.rect.setLeft( 0 );
        }
        else
        {
            pmOption.rect.setLeft( pmOption.rect.right() );
            pmOption.rect.setRight( w );
        }
        painter->fillRect( pmOption.rect, pmOption.palette.brush( QPalette::Active, QPalette::Base ) );
        drawBackground( painter, pmOption, index );
    }
    if( const kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index ) )
    {
        const std::vector< const QPixmap* > pixmaps = getter_( *entity );
        if( !pixmaps.empty() )
            DrawPixmap( painter, option, pixmaps, index );
    }
}

// -----------------------------------------------------------------------------
// Name: ItemPixmapDelegate::sizeHint
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
QSize ItemPixmapDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    QSize s = QItemDelegate::sizeHint( option, index );
    if( index.column() == 0 )
        s.setWidth( s.width() + 50 );
    s.setHeight( std::max( s.height(), height_ ) );
    return s;
}

// -----------------------------------------------------------------------------
// Name: ItemPixmapDelegate::DrawPixmap
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void ItemPixmapDelegate::DrawPixmap( QPainter* painter, const QStyleOptionViewItem& option, const std::vector< const QPixmap* >& pixmaps, const QModelIndex& index ) const
{
    const QRect rect = option.rect;
    QPoint p;
    QStyleOptionViewItem pmOption = option;
    QAbstractItemView* view = static_cast< QAbstractItemView* >( parent() );
    int pixWidth = 0;
    int pixHeight = 0;
    for( auto it = pixmaps.begin(); it != pixmaps.end(); ++it )
    {
        pixWidth += ( *it )->width() + 5;
        pixHeight = std::max( pixHeight, ( *it )->height() );
    }
    if( qApp->layoutDirection() == Qt::RightToLeft )
    {
        p = rect.topLeft();
        p.rx() += std::max( 0, view->sizeHintForColumn( 0 ) - view->viewport()->width() ) - view->horizontalScrollBar()->value();
        pmOption.rect.setLeft( p.rx() );
        p.rx() += 5;
        pmOption.rect.setRight( p.rx() + pixWidth );
    }
    else
    {
        p = rect.topRight();
        p.rx() = std::min( p.rx(), view->viewport()->width() ) - pixWidth;
        pmOption.rect.setLeft( p.rx() - 5 );
        pmOption.rect.setRight( p.rx() + pixWidth );
    }

    painter->save();
    painter->fillRect( pmOption.rect, option.palette.brush( QPalette::Active, QPalette::Base ) );
    drawBackground( painter, pmOption, index );
    p.ry() += ( rect.height() - pixHeight ) / 2;
    for( auto it = pixmaps.begin(); it != pixmaps.end(); ++it )
    {
        painter->drawPixmap( p, **it );
        p.rx() += ( *it )->width() + 5;
    }
    painter->restore();
}
