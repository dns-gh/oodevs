// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CommunicationListView.h"
#include "ModelBuilder.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "preparation/CommunicationHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CommunicationListView constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
CommunicationListView::CommunicationListView( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder )
    : gui::HierarchyListView< kernel::CommunicationHierarchies >( parent, controllers, factory )
    , factory_( factory )
    , modelBuilder_( modelBuilder )
{
    connect( this, SIGNAL( itemRenamed( QListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( QListViewItem*, int, const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
CommunicationListView::~CommunicationListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Display
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void CommunicationListView::Display( const kernel::CommunicationHierarchies& hierarchy, gui::ValuedListItem* item )
{
    if( ! hierarchy.GetSuperior() )
        item->setRenameEnabled( 0, true );
    item->setDragEnabled( true );
    gui::HierarchyListView< kernel::CommunicationHierarchies >::Display( hierarchy, item );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyUpdated
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyUpdated( const ModelLoaded& )
{
    clear();
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyUpdated
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    gui::ValuedListItem* item = gui::FindItem( &entity, firstChild() );
    if( item )
        item->SetNamed( entity );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyContextMenu
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyContextMenu( const Team_ABC&, ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Commande", tools::translate( "Preparation", "Créer un groupe de connaissance" ), &modelBuilder_, SLOT( OnCreateCommunication() ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Drop
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
bool CommunicationListView::Drop( const Entity_ABC& item, const Entity_ABC& target )
{
    const Agent_ABC*          agent   = dynamic_cast< const Agent_ABC* >         ( &item );
    const KnowledgeGroup_ABC* group   = dynamic_cast< const KnowledgeGroup_ABC* >( &target );
    if( agent && group )
        return Drop( *agent, *group );
    return false;
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Drop
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
bool CommunicationListView::Drop( const Agent_ABC& item,  const KnowledgeGroup_ABC& target )
{
    if( item.GetSuperior() )
        return false;
    kernel::CommunicationHierarchies* com = const_cast< kernel::CommunicationHierarchies* >( item.Retrieve< kernel::CommunicationHierarchies >() );
    if( com )
    {
        KnowledgeGroup_ABC& group = const_cast< KnowledgeGroup_ABC& >( target );
        static_cast< ::CommunicationHierarchies* >( com )->ChangeSuperior( group );
    }
    return true;
}
