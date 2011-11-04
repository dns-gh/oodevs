// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "icons.h"
#include "TacticalListView.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/Formation.h"
#include "actions/UnitMagicAction.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Attributes.h"
#include "gaming/StaticModel.h"
#include "gaming/CommandPostAttributes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "protocol/SimulationSenders.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : gui::HierarchyListView< kernel::TacticalHierarchies >( pParent, controllers, factory, profile, icons )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , lock_( MAKE_PIXMAP( lock ) )
    , commandPost_( MAKE_PIXMAP( commandpost ) )
{
    controllers_.Register( *this );
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
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::viewportResizeEvent
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::viewportResizeEvent( QResizeEvent* e )
{
    Q3ListView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::setColumnWidth( int column, int w )
{
    Q3ListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) : w );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Display
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void TacticalListView::Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item )
{
    if( const AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >() )
        item->setPixmap( 1, decisions->IsEmbraye() ? lock_ : QPixmap() );
    else if( const CommandPostAttributes* commandPost = entity.Retrieve< CommandPostAttributes >() )
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

    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "unit_change_superior" );
    UnitMagicAction* action = new UnitMagicAction( item, actionType, controllers_.controller_, tr( "Unit Change Superior" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Automat( it.NextElement(), target, controllers_.controller_ ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &item, false ) );
    action->RegisterAndPublish( actionsModel_ );
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

    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "change_automat_superior" );
    UnitMagicAction* action = new UnitMagicAction( item, actionType, controllers_.controller_, tr( "Change Automat Superior" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Automat( it.NextElement(), target, controllers_.controller_ ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &item, false ) );
    action->RegisterAndPublish( actionsModel_ );
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

    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "change_formation_superior" );
    UnitMagicAction* action = new UnitMagicAction( item, actionType, controllers_.controller_, tr( "Change Formation Superior" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Formation( it.NextElement(), target, controllers_.controller_ ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &item, false ) );
    action->RegisterAndPublish( actionsModel_ );
    return true;
}
