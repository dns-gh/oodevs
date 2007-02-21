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
#include "gaming/ASN_Messages.h"
#include "icons.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : gui::HierarchyListView< kernel::CommunicationHierarchies >( pParent, controllers, factory, profile, icons )
    , publisher_( publisher )
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
        item->setPixmap( 1, decisions->IsEmbraye() ? MAKE_PIXMAP( lock ) : QPixmap() );
    else if( const kernel::CommandPostAttributes* commandPost = entity.Retrieve< kernel::CommandPostAttributes >() )
        item->setPixmap( 1, commandPost->IsCommandPost() ? MAKE_PIXMAP( commandpost ) : QPixmap() );
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
        item->setPixmap( 1, decisions.IsEmbraye() ? MAKE_PIXMAP( lock ) : QPixmap() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target )
{
    const Agent_ABC* agent   = dynamic_cast< const Agent_ABC* >  ( &item );
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

    const Automat_ABC* automat      = dynamic_cast< const Automat_ABC* >       ( &item );
    if( automat )
    {
        const KnowledgeGroup_ABC* group = dynamic_cast< const KnowledgeGroup_ABC* >( &target );
        return group && Drop( *automat, *group );
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

    unsigned int superiorId = target.GetId();
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
bool AgentListView::Drop( const Automat_ABC& item, const KnowledgeGroup_ABC& target )
{
    ASN_MsgChangeGroupeConnaissance asnMsg;
    asnMsg.GetAsnMsg().oid_automate            = item.GetId();
    asnMsg.GetAsnMsg().oid_camp                = target.Get< CommunicationHierarchies >().GetTop().GetId();
    asnMsg.GetAsnMsg().oid_groupe_connaissance = target.GetId();
    asnMsg.Send( publisher_ );
    return true;
}