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
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/ASN_Messages.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher, gui::ItemFactory_ABC& factory )
    : gui::HierarchyListView< kernel::TacticalHierarchies >( pParent, controllers, factory )
    , publisher_( publisher )
{
    // NOTHING
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
// Name: AgentListView::Display
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentListView::Display( const Entity_ABC& agent, gui::ValuedListItem* item )
{
    const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >();
    if( decisions )
        item->setPixmap( 0, decisions->IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
    gui::HierarchyListView< kernel::TacticalHierarchies >::Display( agent, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    const Entity_ABC* entity = & decisions.GetAgent();
    gui::ValuedListItem* item = gui::FindItem( entity, firstChild() );
    if( item )
        item->setPixmap( 0, decisions.IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target )
{
    const Agent_ABC*          agent   = dynamic_cast< const Agent_ABC* >         ( &item );
    const Agent_ABC*          automat = dynamic_cast< const Agent_ABC* >         ( &target );
    const KnowledgeGroup_ABC* group   = dynamic_cast< const KnowledgeGroup_ABC* >( &target );
    if( agent && automat )
        return Drop(*agent, *automat );
    if( agent && group )
        return Drop(*agent, *group );
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Agent_ABC& item, const Agent_ABC& target )
{
    if( item.GetSuperior() == 0 || item.GetSuperior() == &target )
        return false;

    unsigned int superiorId = target.GetId();
    if( target.GetSuperior() != 0 )
        superiorId = target.GetSuperior()->GetId();

    ASN_MsgChangeAutomate asnMsg;
    asnMsg.GetAsnMsg().oid_pion = item.GetId();
    asnMsg.GetAsnMsg().oid_automate = superiorId;
    asnMsg.Send( publisher_ );
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const Agent_ABC& item, const KnowledgeGroup_ABC& target )
{
    if( item.GetSuperior() != 0 )
        return false;
        
    ASN_MsgChangeGroupeConnaissance asnMsg;
    asnMsg.GetAsnMsg().oid_automate = item.GetId();
    asnMsg.GetAsnMsg().oid_camp  = target.GetTeam().GetId();
    asnMsg.GetAsnMsg().oid_groupe_connaissance = target.GetId();
    asnMsg.Send( publisher_ );
    return true;
}