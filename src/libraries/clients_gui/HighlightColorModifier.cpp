// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "HighlightColorModifier.h"
#include "moc_HighlightColorModifier.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ContextMenu.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier constructor
// Created: AGE 2008-05-15
// -----------------------------------------------------------------------------
HighlightColorModifier::HighlightColorModifier( kernel::Controllers& controllers )
    : controllers_( controllers )
    , selected_   ( controllers )
    , blink_      ( false )
{
    controllers_.Register( *this );
    QTimer* timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), SLOT( OnBlink() ) );
    timer->start( 1000 );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier destructor
// Created: AGE 2008-05-15
// -----------------------------------------------------------------------------
HighlightColorModifier::~HighlightColorModifier()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::Apply
// Created: AGE 2008-05-15
// -----------------------------------------------------------------------------
QColor HighlightColorModifier::Apply( const kernel::Entity_ABC& entity, const QColor& base )
{
    if( highlighted_.find( &entity ) != highlighted_.end() )
        return Highlight( base );
    return base;
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::NotifyContextMenu
// Created: AGE 2008-05-15
// -----------------------------------------------------------------------------
void HighlightColorModifier::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = &entity;
    if( highlighted_.find( &entity ) == highlighted_.end() )
        menu.InsertItem( "Interface", tr( "Highlight" ), this, SLOT( Highlight() ) );
    else
        menu.InsertItem( "Interface", tr( "Remove highlight" ), this, SLOT( Unhighlight() ) );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::Highlight
// Created: AGE 2008-05-15
// -----------------------------------------------------------------------------
void HighlightColorModifier::Highlight()
{
    if( selected_ )
        highlighted_.insert( selected_ );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::Unhighlight
// Created: AGE 2008-05-15
// -----------------------------------------------------------------------------
void HighlightColorModifier::Unhighlight()
{
    if( selected_ )
        highlighted_.erase( selected_ );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::NotifyDeleted
// Created: AGE 2008-05-15
// -----------------------------------------------------------------------------
void HighlightColorModifier::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    highlighted_.erase( &entity );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::Highlight
// Created: AGE 2008-05-15
// -----------------------------------------------------------------------------
QColor HighlightColorModifier::Highlight( const QColor& base )
{
    return blink_ ? base.light( 150 ) : base;
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::OnBlink
// Created: AGE 2008-05-15
// -----------------------------------------------------------------------------
void HighlightColorModifier::OnBlink()
{
    blink_ = ! blink_;
}
