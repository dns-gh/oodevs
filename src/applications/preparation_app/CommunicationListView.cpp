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
#include "PreparationProfile.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "preparation/AutomatCommunications.h"
#include "preparation/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CommunicationListView constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
CommunicationListView::CommunicationListView( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, ModelBuilder& modelBuilder )
    : gui::HierarchyListView< kernel::CommunicationHierarchies >( parent, controllers, factory, PreparationProfile::GetProfile(), icons )
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
// Name: CommunicationListView::Display
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void CommunicationListView::Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item )
{
    item->setRenameEnabled( 0, true );
    gui::HierarchyListView< kernel::CommunicationHierarchies >::Display( entity, item );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyContextMenu
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyContextMenu( const Team_ABC&, ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Creation", tools::translate( "CommunicationListView", "Create knowledge group" ), &modelBuilder_, SLOT( OnCreateCommunication() ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyContextMenu
// Created: FHD 2009-11-19
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyContextMenu( const kernel::KnowledgeGroup_ABC& knowledgegroup, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Creation", tools::translate( "CommunicationListView", "Create sub knowledge group" ), &modelBuilder_, SLOT( OnCreateCommunication() ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Drop
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
bool CommunicationListView::Drop( const Entity_ABC& item, const Entity_ABC& target )
{
    const Automat_ABC*        automat = dynamic_cast< const Automat_ABC* >       ( &item );
    const KnowledgeGroup_ABC* group   = dynamic_cast< const KnowledgeGroup_ABC* >( &target );
    if( automat && group )
    {
        CommunicationHierarchies& com = const_cast< CommunicationHierarchies& >( automat->Get< CommunicationHierarchies >() );
        if( &com.GetTop() != &target.Get< CommunicationHierarchies >().GetTop() )
            return false;
        static_cast< AutomatCommunications& >( com ).ChangeSuperior( const_cast< Entity_ABC& >( target ) );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::keyPressEvent
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void CommunicationListView::keyPressEvent( QKeyEvent* event )
{
    if( selectedItem() && event->key() == Qt::Key_Delete )
        modelBuilder_.DeleteEntity( *((gui::ValuedListItem*)selectedItem())->GetValue< const Entity_ABC >() );
    else
        QListView::keyPressEvent( event );
}
