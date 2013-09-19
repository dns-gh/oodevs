// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "HighlightColorModifier.h"
#include "moc_HighlightColorModifier.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier constructor
// Created: AGE 2008-05-15
// -----------------------------------------------------------------------------
HighlightColorModifier::HighlightColorModifier( kernel::Controllers& controllers, const kernel::Profile_ABC& profile )
    : controllers_( controllers )
    , profile_    ( profile )
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
QColor HighlightColorModifier::Apply( const kernel::Entity_ABC& entity, const QColor& base ) const
{
    if( highlighted_.find( &entity ) != highlighted_.end() )
        return Highlight( base );
    return base;
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::Apply
// Created: LGY 2013-03-06
// -----------------------------------------------------------------------------
float HighlightColorModifier::Apply( const kernel::Entity_ABC& /*entity*/, float alpha ) const
{
    return alpha;
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::NotifyContextMenu
// Created: AGE 2008-05-15
// Name: HighlightColorModifier::UpdateContextMenu
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void HighlightColorModifier::UpdateContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( !profile_.IsVisible( entity ) )
        return;
    selected_ = &entity;
    if( highlighted_.find( &entity ) == highlighted_.end() )
        menu.InsertItem( "Interface", tools::translate( "gui::HighlightColorModifier", "Highlight" ), this, SLOT( Highlight() ) );
    else
        menu.InsertItem( "Interface", tools::translate( "gui::HighlightColorModifier", "Remove highlight" ), this, SLOT( Unhighlight() ) );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void HighlightColorModifier::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void HighlightColorModifier::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void HighlightColorModifier::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void HighlightColorModifier::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void HighlightColorModifier::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void HighlightColorModifier::NotifyContextMenu( const kernel::AgentKnowledge_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: HighlightColorModifier::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void HighlightColorModifier::NotifyContextMenu( const kernel::ObjectKnowledge_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
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
QColor HighlightColorModifier::Highlight( const QColor& base ) const
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
