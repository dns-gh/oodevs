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
#include "moc_TacticalListView.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/Formation.h"
#include "actions/UnitMagicAction.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Attributes.h"
#include "gaming/StaticModel.h"
#include "gaming/CommandPostAttributes.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Options.h"
#include "protocol/SimulationSenders.h"

using namespace actions;
using namespace gui;
using namespace kernel;

namespace
{
    bool IsCommandPost( const Entity_ABC& entity )
    {
        if( const CommandPostAttributes* pAttributes = entity.Retrieve< CommandPostAttributes >() )
            return pAttributes->IsCommandPost();
        return false;
    }

    int ItemComparator( const ValuedListItem& item1, const ValuedListItem& item2, int /*col*/, bool /*ascending*/ )
    {
        const Entity_ABC* entity1 = item1.GetValue< const Entity_ABC >();
        if( !entity1 || IsCommandPost( *entity1 ) )
            return -1;
        const Entity_ABC* entity2 = item2.GetValue< const Entity_ABC >();
        if( !entity2 || IsCommandPost( *entity2 ) )
            return 1;
        return entity1->GetId() - entity2->GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const Time_ABC& simulation, ItemFactory_ABC& factory, const Profile_ABC& profile, EntitySymbols& icons )
    : HierarchyListView< TacticalHierarchies >( pParent, controllers, factory, profile, icons )
    , actionsModel_        ( actionsModel )
    , static_              ( staticModel )
    , simulation_          ( simulation )
    , lock_                ( MAKE_PIXMAP( lock ) )
    , commandPost_         ( MAKE_PIXMAP( commandpost ) )
    , changeSuperiorDialog_( 0 )
{
    controllers_.Register( *this );
    addColumn( "HiddenPuce", 15 );
    setColumnAlignment( 1, Qt::AlignCenter );
    SetComparator( &ItemComparator );
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
// Name: TacticalListView::hideEvent
// Created: JSR 2011-11-09
// -----------------------------------------------------------------------------
void TacticalListView::hideEvent( QHideEvent* event )
{
    if( changeSuperiorDialog_ )
        changeSuperiorDialog_->hide();
    HierarchyListView< TacticalHierarchies >::hideEvent( event );
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
void TacticalListView::Display( const Entity_ABC& entity, ValuedListItem* item )
{
    if( IsCommandPost( entity ) )
        item->setPixmap( 1, commandPost_ );
    else
    {
        if( const AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >() )
            item->setPixmap( 1, decisions->IsEmbraye() ? lock_ : QPixmap() );
        else
            item->setPixmap( 1, QPixmap() );
    }

    if( const Attributes* attributes = static_cast< const Attributes* >( entity.Retrieve< Attributes_ABC >() ) )
    {
        QColor color;
        if( attributes->bNeutralized_ )
            color = QColor( controllers_.options_.GetOption( "Color/Neutralized", QString( "" ) ).To< QString >() );
        if( attributes->nOpState_ == eOperationalStatus_DetruitTactiquement )
            color = QColor( controllers_.options_.GetOption( "Color/TacticallyDestroyed", QString( "" ) ).To< QString >() );
        if( attributes->nOpState_ == eOperationalStatus_DetruitTotalement )
            color = QColor( controllers_.options_.GetOption( "Color/TotallyDestroyed", QString( "" ) ).To< QString >() );
        item->SetBackgroundColor( color );
    }

    HierarchyListView< TacticalHierarchies >::Display( entity, item );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    const Entity_ABC* agent = & decisions.GetAgent();
    ValuedListItem* item = FindItem( agent, firstChild() );
    if( item )
        item->setPixmap( 1, decisions.IsEmbraye() ? lock_ : QPixmap() );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Entity_ABC& entity, ContextMenu& menu )
{
    if( !isVisible() )
        return;
    if( const TacticalHierarchies* hierarchies = entity.Retrieve< TacticalHierarchies >() )
        if( hierarchies->GetSuperior() != 0 && dynamic_cast< const Object_ABC* >( &entity ) == 0 )
            menu.InsertItem( "Command", tr( "Change superior" ), this, SLOT( OnChangeSuperior() ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::OnChangeSuperior
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void TacticalListView::OnChangeSuperior()
{
    if( ValuedListItem* valuedItem = static_cast< ValuedListItem* >( selectedItem() ) )
    {
        Entity_ABC& entity = *valuedItem->GetValue< Entity_ABC >();
        if( !changeSuperiorDialog_ )
            changeSuperiorDialog_ = new ChangeSuperiorDialog( this, controllers_, *this, false );
        changeSuperiorDialog_->Show( entity );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::CanChangeSuperior
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
bool TacticalListView::CanChangeSuperior( const Entity_ABC& entity, const Entity_ABC& superior ) const
{
    if( dynamic_cast< const Agent_ABC* >( &entity ) )
        return dynamic_cast< const Automat_ABC* >( &superior ) != 0;
    if( dynamic_cast< const Automat_ABC* >( &entity ) )
        return dynamic_cast< const Automat_ABC* >( &superior ) != 0 || dynamic_cast< const Formation_ABC* >( &superior ) != 0;
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::DoChangeSuperior
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void TacticalListView::DoChangeSuperior( Entity_ABC& entity, Entity_ABC& superior )
{
    Drop( entity, superior );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const Entity_ABC& item, const Entity_ABC& target )
{
    if( const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( &item ) )
        if( const Automat_ABC* automat = dynamic_cast< const Automat_ABC* >( &target ) )
            return Drop( *agent, *automat );

    if( const Automat_ABC* automatSource = dynamic_cast< const Automat_ABC* >( &item ) )
        if( const Automat_ABC* automatTarget = dynamic_cast< const Automat_ABC* >( &target ) )
            return Drop( *automatSource, *automatTarget );
        else if( const Formation_ABC* formationTarget = dynamic_cast< const Formation_ABC* >( &target ) )
            return Drop( *automatSource, *formationTarget );

    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const Agent_ABC& item, const Automat_ABC& target )
{
    if( & item.Get< TacticalHierarchies >().GetUp() == &target )
        return false;

    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "unit_change_superior" );
    UnitMagicAction* action = new UnitMagicAction( item, actionType, controllers_.controller_, tr( "Unit Change Superior" ), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
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
bool TacticalListView::Drop( const Automat_ABC& item, const Automat_ABC& target )
{
    if( & item.Get< TacticalHierarchies >().GetUp() == &target || &item == &target )
        return false;

    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_automat_superior" );
    UnitMagicAction* action = new UnitMagicAction( item, actionType, controllers_.controller_, tr( "Change Automat Superior" ), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
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
bool TacticalListView::Drop( const Automat_ABC& item, const Formation_ABC& target )
{
    if( & item.Get< TacticalHierarchies >().GetUp() == &target )
        return false;

    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_formation_superior" );
    UnitMagicAction* action = new UnitMagicAction( item, actionType, controllers_.controller_, tr( "Change Formation Superior" ), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Formation( it.NextElement(), target, controllers_.controller_ ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &item, false ) );
    action->RegisterAndPublish( actionsModel_ );
    return true;
}
