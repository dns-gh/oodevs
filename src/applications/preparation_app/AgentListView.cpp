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
#include "clients_kernel/Hierarchies.h"

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
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentListView::Display( const kernel::Hierarchies& hierarchy, gui::ValuedListItem* item )
{
    if( ! hierarchy.GetSuperior() )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentListView::Display( const kernel::Entity_ABC& agent, gui::ValuedListItem* item )
{
    const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >();
    if( decisions )
        item->setPixmap( 0, decisions->IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
    gui::AgentListView::Display( agent, item );
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
    const Entity_ABC* agent = & decisions.GetAgent();
    gui::ValuedListItem* item = gui::FindItem( agent, firstChild() );
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
    const Team_ABC* pTeam = 0;
    // $$$$ AGE 2006-09-20: tuerie le nombre de casts
    if( valuedItem && valuedItem->IsA< const Entity_ABC* >() 
        && ( pTeam = dynamic_cast< const Team_ABC* >( valuedItem->GetValue< const Entity_ABC* >() ) ) )
    {
        Team_ABC& team = const_cast< Team_ABC& >( *pTeam );
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
