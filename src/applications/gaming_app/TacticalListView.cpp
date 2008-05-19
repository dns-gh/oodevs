// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TacticalListView.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Attributes.h"
#include "gaming/SimulationMessages.h"
#include "clients_kernel/CommandPostAttributes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, kernel::Controllers& controllers, Publisher_ABC& publisher, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : gui::HierarchyListView< kernel::TacticalHierarchies >( pParent, controllers, factory, profile, icons )
    , publisher_( publisher )
    , lock_( MAKE_PIXMAP( lock ) )
    , commandPost_( MAKE_PIXMAP( commandpost ) )
{
    addColumn( "HiddenPuce", 15 );
    setColumnAlignment( 1, Qt::AlignCenter );
    controllers.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView destructor
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
TacticalListView::~TacticalListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::viewportResizeEvent
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::viewportResizeEvent( QResizeEvent* e )
{
    QScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::setColumnWidth( int column, int w )
{
    QListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) : w );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Display
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void TacticalListView::Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item )
{
    if( const AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >() )
        item->setPixmap( 1, decisions->IsEmbraye() ? lock_ : QPixmap() );
    else if( const kernel::CommandPostAttributes* commandPost = entity.Retrieve< kernel::CommandPostAttributes >() )
        item->setPixmap( 1, commandPost->IsCommandPost() ? commandPost_ : QPixmap() );
    
    if( const Attributes* attributes = static_cast< const Attributes* >( entity.Retrieve< kernel::Attributes_ABC >() ) )
        item->SetBackgroundColor( attributes->bDead_ ? QColor( 255, 200, 200 ) : QColor() );

    gui::HierarchyListView< kernel::TacticalHierarchies >::Display( entity, item );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    const kernel::Entity_ABC* agent = & decisions.GetAgent();
    gui::ValuedListItem* item = gui::FindItem( agent, firstChild() );
    if( item )
        item->setPixmap( 1, decisions.IsEmbraye() ? lock_ : QPixmap() );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target )
{
    if( const kernel::Agent_ABC* agent = dynamic_cast< const kernel::Agent_ABC* >( &item ) )
        if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &target ) )
            return Drop( *agent, *automat );

    if( const kernel::Automat_ABC* automatSource = dynamic_cast< const kernel::Automat_ABC* >( &item ) )
        if( const kernel::Automat_ABC* automatTarget = dynamic_cast< const kernel::Automat_ABC* >( &target ) )
            return Drop( *automatSource, *automatTarget );
        else if( const kernel::Formation_ABC* formationTarget = dynamic_cast< const kernel::Formation_ABC* >( &target ) )
            return Drop( *automatSource, *formationTarget );

    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const kernel::Agent_ABC& item, const kernel::Automat_ABC& target )
{
    if( & item.Get< kernel::TacticalHierarchies >().GetUp() == &target )
        return false;
    simulation::UnitChangeSuperior asnMsg;
    asnMsg().oid = item.GetId();
    asnMsg().oid_automate = target.GetId();
    asnMsg.Send( publisher_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: NLD 2007-04-12
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const kernel::Automat_ABC& item, const kernel::Automat_ABC& target )
{
    if( & item.Get< kernel::TacticalHierarchies >().GetUp() == &target || &item == &target )
        return false;
    simulation::AutomatChangeSuperior asnMsg;
    asnMsg().oid                     = item.GetId();
    asnMsg().oid_superior.t          = T_MsgAutomatChangeSuperior_oid_superior_automate;
    asnMsg().oid_superior.u.automate = target.GetId();
    asnMsg.Send( publisher_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: NLD 2007-04-12
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const kernel::Automat_ABC& item, const kernel::Formation_ABC& target )
{
    if( & item.Get< kernel::TacticalHierarchies >().GetUp() == &target )
        return false;
    simulation::AutomatChangeSuperior asnMsg;
    asnMsg().oid                      = item.GetId();
    asnMsg().oid_superior.t           = T_MsgAutomatChangeSuperior_oid_superior_formation;
    asnMsg().oid_superior.u.formation = target.GetId();
    asnMsg.Send( publisher_ );
    return true;
}
