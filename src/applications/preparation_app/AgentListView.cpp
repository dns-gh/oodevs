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
#include "preparation/Team.h"
#include "preparation/Model.h"
#include "preparation/TeamsModel.h"
#include "clients_gui/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, Controllers& controllers, gui::ItemFactory_ABC& factory, const Model& model )
    : gui::AgentListView( pParent, controllers, factory )
    , factory_( factory )
    , model_( model )
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
void AgentListView::Display( const kernel::Team_ABC& team, gui::ValuedListItem* item )
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
    // $$$$ SBO 2006-08-29: icone embrayé/debrayé
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
// Name: AgentListView::OnRename
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void AgentListView::OnRename( QListViewItem* item, int, const QString& text )
{
    gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( item );
    if( valuedItem->IsA< const Team_ABC* >() )
    {
        Team_ABC& team = const_cast< Team_ABC& >( *valuedItem->GetValue< const Team_ABC* >() );
        static_cast< Team& >( team ).Rename( text );
        valuedItem->SetNamed( team );
    }
}