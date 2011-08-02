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
#include "clients_gui/EntitySymbols.h"
#include "gaming/Attributes.h"
#include <QtGui/qpainter.h>

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem constructor
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
InfoSubordinateItem::InfoSubordinateItem( Q3IconView* parent, kernel::Controllers& controllers, gui::EntitySymbols& icons, const kernel::Entity_ABC& entity )
    : Q3IconViewItem( parent, entity.GetName(), icons.GetSymbol( entity ) )
    , controllers_( controllers )
    , icons_( icons )
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
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::paintItem
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
void InfoSubordinateItem::paintItem( QPainter* p, const QColorGroup& cg )
{
    Q3IconViewItem::paintItem( p, cg );
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
    p->fillRect( lifeRect.left(), lifeRect.top(), int( lifeRect.width() * float( life ) / 100.f ), lifeRect.height(), color );
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
