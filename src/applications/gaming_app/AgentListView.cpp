// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentListView.h"
#include "actions/Army.h"
#include "actions/Automat.h"
#include "actions/KnowledgeGroup.h"
#include "actions/KnowledgeGroupMagicAction.h"
#include "actions/UnitMagicAction.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/CommandPostAttributes.h"
#include "gaming/ActionTiming.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/KnowledgeGroupHierarchies.h" // LTO
#include "gaming/StaticModel.h"
#include "icons.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : gui::HierarchyListView< kernel::CommunicationHierarchies >( pParent, controllers, factory, profile, icons )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , lock_( MAKE_PIXMAP( lock ) )
    , scisors_( MAKE_PIXMAP( scisors ) ) // LTO
    , commandPost_( MAKE_PIXMAP( commandpost ) )
    , controllers_( controllers ) // LTO
{
    addColumn( "HiddenPuce", 15 );
    setColumnAlignment( 1, Qt::AlignCenter );
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView destructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentListView::~AgentListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentListView::viewportResizeEvent
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void AgentListView::viewportResizeEvent( QResizeEvent* e )
{
    QScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void AgentListView::setColumnWidth( int column, int w )
{
    QListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) : w );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentListView::Display( const Entity_ABC& entity, gui::ValuedListItem* item )
{
    const AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >();
    if( decisions )
        item->setPixmap( 1, decisions->IsEmbraye() ? lock_ : QPixmap() );
    else if( const kernel::CommandPostAttributes* commandPost = entity.Retrieve< kernel::CommandPostAttributes >() )
        item->setPixmap( 1, commandPost->IsCommandPost() ? commandPost_ : QPixmap() );
    // LTO begin
    else if ( const kernel::KnowledgeGroup_ABC* kg = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity ) ) // $$$$ _RC_ SLG 2009-12-21: TEMP
        item->setPixmap( 1, !kg->IsActivated() ? scisors_ : QPixmap() );
    // LTO end

    gui::HierarchyListView< kernel::CommunicationHierarchies >::Display( entity, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    const kernel::Entity_ABC* agent = & decisions.GetAgent();
    gui::ValuedListItem* item = gui::FindItem( agent, firstChild() );
    if( item )
        item->setPixmap( 1, decisions.IsEmbraye() ? lock_ : QPixmap() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: SBO 2006-08-18
// LTO
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const KnowledgeGroup_ABC& knowledgeGroup )
{
    const kernel::Entity_ABC* agent = (const Entity_ABC*)&knowledgeGroup;
    gui::ValuedListItem* item = gui::FindItem( agent, firstChild() );
    if( item )
        item->setPixmap( 1, !knowledgeGroup.IsActivated() ? scisors_ : QPixmap() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target )
{
    const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( &item );
    if( agent )
    {
        const Automat_ABC* automat = dynamic_cast< const Automat_ABC* >( &target );
        if( automat )
            return Drop( *agent, *automat );
        const Agent_ABC* targetAgent = dynamic_cast< const Agent_ABC* >( &target );
        if( targetAgent )
            return Drop( *agent, *targetAgent );
        return false;
    }

    const Automat_ABC* automat = dynamic_cast< const Automat_ABC* >( &item );
    if( automat )
    {
        const KnowledgeGroup_ABC* group = dynamic_cast< const KnowledgeGroup_ABC* >( &target );
        return group && Drop( *automat, *group );
    }

    // LTO begin
    const KnowledgeGroup_ABC* knowledgeGroup = dynamic_cast< const KnowledgeGroup_ABC* >( &item );
    if( knowledgeGroup )
    {
        const KnowledgeGroup_ABC* groupParent = dynamic_cast< const KnowledgeGroup_ABC* >( &target );
        if( groupParent )
            return Drop( *knowledgeGroup, *groupParent );
        const Team_ABC* teamParent = dynamic_cast< const Team_ABC* >( &target );
        if( teamParent )
            return Drop( *knowledgeGroup, *teamParent );
    }
    // LTO end
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const kernel::Agent_ABC& item,  const kernel::Agent_ABC& target )
{
    return Drop( item, target.Get< CommunicationHierarchies >().GetUp() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Agent_ABC& item, const Automat_ABC& target )
{
    if( & item.Get< CommunicationHierarchies >().GetUp() == &target )
        return false;

    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "unit_change_superior" );
    UnitMagicAction* action = new UnitMagicAction( item, actionType, controllers_.controller_, true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Automat( it.NextElement(), target, controllers_.controller_ ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->RegisterAndPublish( actionsModel_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Automat_ABC& item, const KnowledgeGroup_ABC& target )
{
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_knowledge_group" );
    UnitMagicAction* action = new UnitMagicAction( item, actionType, controllers_.controller_, true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::KnowledgeGroup( it.NextElement(), target, controllers_.controller_ ) );
    if( const Team_ABC *team = dynamic_cast< const Team_ABC* >( &target.Get< CommunicationHierarchies >().GetTop() ) )
        action->AddParameter( *new parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->RegisterAndPublish( actionsModel_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: FHD 2010-01-04:
// LTO
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const KnowledgeGroup_ABC& item, const Team_ABC& target )
{
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_update_side" );
    KnowledgeGroupMagicAction* action = new KnowledgeGroupMagicAction( item, actionType, controllers_.controller_, true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    if( const Team_ABC *team = dynamic_cast< const Team_ABC* >( &target.Get< CommunicationHierarchies >().GetTop() ) )
        action->AddParameter( *new parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->RegisterAndPublish( actionsModel_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: FHD 2010-01-04: 
// LTO
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const KnowledgeGroup_ABC& item, const KnowledgeGroup_ABC& target )
{
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_update_side_parent" );
    KnowledgeGroupMagicAction* action = new KnowledgeGroupMagicAction( item, actionType, controllers_.controller_, true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    if( const Team_ABC *team = dynamic_cast< const Team_ABC* >( &target.Get< CommunicationHierarchies >().GetTop() ) )
        action->AddParameter( *new parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
    action->AddParameter( *new parameters::KnowledgeGroup( it.NextElement(), target, controllers_.controller_ ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->RegisterAndPublish( actionsModel_ );
    return true;
}
