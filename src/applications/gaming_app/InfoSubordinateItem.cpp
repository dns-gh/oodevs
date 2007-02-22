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
#include "gaming/Attributes.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem constructor
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
InfoSubordinateItem::InfoSubordinateItem( QIconView* parent, const QString& text, const QPixmap& icon, const kernel::Entity_ABC& entity )
    : QIconViewItem( parent, text, icon )
    , entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem destructor
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
InfoSubordinateItem::~InfoSubordinateItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::paintItem
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
void InfoSubordinateItem::paintItem( QPainter* p, const QColorGroup& cg )
{
    QIconViewItem::paintItem( p, cg );
    if( const Attributes* attributes = static_cast< const Attributes* >( entity_.Retrieve< kernel::Attributes_ABC >() ) )
    {
        const float life = attributes->nRawOpState_;
        const QRect rect = pixmapRect( false );
        const QRect lifeRect( rect.left(), rect.bottom() - 5, rect.width(), 5 );
        QColor color;
        color.setHsv( life, 255, 255 );
        p->fillRect( lifeRect, QColor( 255, 100, 100 ) );
        p->fillRect( lifeRect.left(), lifeRect.top(), lifeRect.width() * life / 100.f, lifeRect.height(), color );
        p->drawRect( lifeRect );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::GetEntity
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& InfoSubordinateItem::GetEntity() const
{
    return entity_;
}
