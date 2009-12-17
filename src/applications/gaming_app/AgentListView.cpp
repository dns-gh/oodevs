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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/CommandPostAttributes.h"
#include "gaming/AutomatDecisions.h"
#include "game_asn/SimulationSenders.h"
#include "icons.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : gui::HierarchyListView< kernel::CommunicationHierarchies >( pParent, controllers, factory, profile, icons )
    , publisher_( publisher )
    , lock_( MAKE_PIXMAP( lock ) )
    , commandPost_( MAKE_PIXMAP( commandpost ) )
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

    const KnowledgeGroup_ABC* knowledgeGroup = dynamic_cast< const KnowledgeGroup_ABC* >( &item );
    if( knowledgeGroup )
    {
        const KnowledgeGroup_ABC* groupParent = dynamic_cast< const KnowledgeGroup_ABC* >( &target );
        return groupParent && Drop( *knowledgeGroup, *groupParent );
    }
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

    simulation::UnitChangeSuperior asnMsg;
    asnMsg().oid = item.GetId();
    asnMsg().oid_automate = target.GetId();
    asnMsg.Send( publisher_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Automat_ABC& item, const KnowledgeGroup_ABC& target )
{
    simulation::AutomatChangeKnowledgeGroup asnMsg;
    asnMsg().oid                     = item.GetId();
    asnMsg().oid_camp                = target.Get< CommunicationHierarchies >().GetTop().GetId();
    asnMsg().oid_groupe_connaissance = target.GetId();
    asnMsg.Send( publisher_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const KnowledgeGroup_ABC& item, const KnowledgeGroup_ABC& target )
{
    simulation::KnowledgeGroupChangeSuperior asnMsg;
    asnMsg().oid                     = item.GetId();
    asnMsg().oid_camp                = target.Get< CommunicationHierarchies >().GetTop().GetId();
    asnMsg().oid_knowledgegroup_parent = target.GetId();
    asnMsg.Send( publisher_ );
    return true;
}