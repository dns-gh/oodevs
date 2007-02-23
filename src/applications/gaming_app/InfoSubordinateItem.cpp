// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoSubordinateItem.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Attributes.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem constructor
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
InfoSubordinateItem::InfoSubordinateItem( QIconView* parent, kernel::Controllers& controllers, const QString& text, const QPixmap& icon, const kernel::Entity_ABC& entity )
    : QIconViewItem( parent, text, icon )
    , controllers_( controllers )
    , entity_( entity )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem destructor
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
InfoSubordinateItem::~InfoSubordinateItem()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::paintItem
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
void InfoSubordinateItem::paintItem( QPainter* p, const QColorGroup& cg )
{
    QIconViewItem::paintItem( p, cg );
    if( const Attributes* attributes = static_cast< const Attributes* >( entity_.Retrieve< kernel::Attributes_ABC >() ) )
        DrawLife( p, attributes->nRawOpState_ );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::DrawLife
// Created: SBO 2007-02-23
// -----------------------------------------------------------------------------
void InfoSubordinateItem::DrawLife( QPainter* p, int life )
{
    const QRect rect = pixmapRect( false );
    const QRect lifeRect( rect.left(), rect.bottom() - 5, rect.width(), 5 );
    QColor color;
    color.setHsv( life, 255, 255 );
    p->fillRect( lifeRect, QColor( 255, 100, 100 ) );
    p->fillRect( lifeRect.left(), lifeRect.top(), lifeRect.width() * float( life ) / 100.f, lifeRect.height(), color );
    p->drawRect( lifeRect );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::NotifyUpdated
// Created: SBO 2007-02-23
// -----------------------------------------------------------------------------
void InfoSubordinateItem::NotifyUpdated( const kernel::Attributes_ABC& attributes )
{
    if( &attributes == entity_.Retrieve< kernel::Attributes_ABC >() )
        repaint();
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::GetEntity
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& InfoSubordinateItem::GetEntity() const
{
    return entity_;
}
