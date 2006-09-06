// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AgentListView.h"
#include "moc_AgentListView.cpp"
#include "ModelBuilder.h"
#include "preparation/Team.h"
#include "preparation/Model.h"
#include "preparation/TeamsModel.h"
#include "preparation/AutomatDecisions.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, Controllers& controllers, gui::ItemFactory_ABC& factory, const Model& model, ModelBuilder& modelBuilder )
    : gui::AgentListView( pParent, controllers, factory )
    , factory_( factory )
    , model_( model )
    , modelBuilder_( modelBuilder )
{
    setRootIsDecorated( false );
    connect( this, SIGNAL( itemRenamed( QListViewItem*, int, const QString& ) ), this, SLOT( OnRename( QListViewItem*, int, const QString& ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
AgentListView::~AgentListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void AgentListView::Display( const Team_ABC& team, gui::ValuedListItem* item )
{
    item->setRenameEnabled( 0, true );
    gui::AgentListView::Display( team, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
void AgentListView::Display( const Agent_ABC& agent, gui::ValuedListItem* item )
{
    const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >();
    if( decisions )
        item->setPixmap( 0, decisions->IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
    gui::AgentListView::Display( agent, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyCreated
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void AgentListView::NotifyCreated( const Team_ABC& team )
{
    gui::ValuedListItem* item = factory_.CreateItem( firstChild() );
    item->SetNamed( team );
    gui::AgentListView::NotifyUpdated( team );
    item->setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyCreated
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AgentListView::NotifyCreated( const KnowledgeGroup_ABC& group )
{
    QListViewItem* item = gui::FindItem( &group.GetTeam(), firstChild() );
    if( item )
        item->setOpen( true );
}
    
// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyCreated
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AgentListView::NotifyCreated( const Agent_ABC& agent )
{
    QListViewItem* item = 0;
    if( agent.GetSuperior() )
        item = gui::FindItem( agent.GetSuperior(), firstChild() );
    else
        item = gui::FindItem< const KnowledgeGroup_ABC* >( &agent.GetKnowledgeGroup(), firstChild() );
    if( item )
        item->setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const ModelLoaded& )
{
    clear();
    gui::ValuedListItem* root = new gui::ValuedListItem( this );
    root->SetValue( &model_ );
    root->setText( 0, tools::translate( "Preparation", "Ordre de Bataille" ) );
    root->setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    gui::ValuedListItem* item = gui::FindItem( & decisions.GetAgent(), firstChild() );
    if( item )
        item->setPixmap( 0, decisions.IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnRename
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void AgentListView::OnRename( QListViewItem* item, int, const QString& text )
{
    gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( item );
    if( valuedItem && valuedItem->IsA< const Team_ABC* >() )
    {
        Team_ABC& team = const_cast< Team_ABC& >( *valuedItem->GetValue< const Team_ABC* >() );
        static_cast< Team& >( team ).Rename( text );
        valuedItem->SetNamed( team );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::keyPressEvent
// Created: SBO 2006-09-05
// -----------------------------------------------------------------------------
void AgentListView::keyPressEvent( QKeyEvent* event )
{
    if( selectedItem() && event->key() == Qt::Key_Delete )
        if( modelBuilder_.OnDelete() )
            return;
    QListView::keyPressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyContextMenu
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AgentListView::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    if( agent.Retrieve< AutomatDecisions >() )
    {
        if( ! agent.Retrieve< AutomatDecisions >()->IsEmbraye() )
            menu.InsertItem( "Commande", tr( "Embrayer" ), this, SLOT( Engage() ) );
        else
            menu.InsertItem( "Commande", tr( "Debrayer" ), this, SLOT( Disengage() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Engage
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AgentListView::Engage()
{
    gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( valuedItem && valuedItem->IsA< const Agent_ABC* >() )
    {
        Agent_ABC& agent = const_cast< Agent_ABC& >( *valuedItem->GetValue< const Agent_ABC* >() );
        agent.Get< AutomatDecisions >().Engage();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Disengage
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AgentListView::Disengage()
{
    gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( valuedItem && valuedItem->IsA< const Agent_ABC* >() )
    {
        Agent_ABC& agent = const_cast< Agent_ABC& >( *valuedItem->GetValue< const Agent_ABC* >() );
        agent.Get< AutomatDecisions >().Disengage();
    }
}
