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
#include "clients_gui/ValuedDragObject.h" // LTO
#include "clients_gui/ValuedListItem.h" // LTO
#include "preparation/AutomatCommunications.h"
#include "preparation/KnowledgeGroupCommunications.h" // LTO
#include "preparation/Tools.h"
#include <qevent.h> // LTO

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
    controllers_.Register( *this );
    connect( this, SIGNAL( itemRenamed( QListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( QListViewItem*, int, const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
CommunicationListView::~CommunicationListView()
{
    controllers_.Unregister( *this );
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
// LTO
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyContextMenu( const kernel::KnowledgeGroup_ABC& /*knowledgegroup*/, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Creation", tools::translate( "CommunicationListView", "Create sub knowledge group" ), &modelBuilder_, SLOT( OnCreateCommunication() ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::CanDrop
// Created: FHD/SYD 2009-11-30
// LTO
// -----------------------------------------------------------------------------
bool CommunicationListView::CanDrop( const Entity_ABC* draggedEntity, QPoint dropPosition ) const
{
    // $$$$ SYD 2009-12-02: TODO REFACTOR: This method could be used also by Drop method
    //        Base class may have such a "can drop method", along with a dragMoveEvent that use it
    //        More generally, drag n drop validity checking might be more consistent throughout the GUI
    gui::ValuedListItem* targetItem = (gui::ValuedListItem*)itemAt( dropPosition );
    if( targetItem == NULL )
        return false;

    //--- check source of drag n drop
    const KnowledgeGroup_ABC* srcKnowledgeGroup = dynamic_cast< const KnowledgeGroup_ABC* >( draggedEntity );
    const Automat_ABC* srcAutomat = dynamic_cast< const Automat_ABC* >( draggedEntity );
    if( !srcKnowledgeGroup && !srcAutomat )
        return false; // can drop only automats or knowledge group

    //--- check destination of drag n drop
    const Entity_ABC* dstEntity = dynamic_cast< const Entity_ABC* >( targetItem->GetValue< const Entity_ABC >() );
    const KnowledgeGroup_ABC* dstKnowledgeGroup = dynamic_cast< const KnowledgeGroup_ABC* >( dstEntity );
    const Entity_ABC* dstTeam = 0;
    if( const kernel::CommunicationHierarchies* dstHierarchies = dstEntity->Retrieve< kernel::CommunicationHierarchies >() )
        dstTeam = &dstHierarchies->GetTop();

    if( !dstKnowledgeGroup && !dstTeam )
        return false; // can drop only on knowledge group or team

    //--- check compatibility between source and destination of drag n drop
    if( draggedEntity == dstEntity )
        return false; // cannot drop an item on itself

    const Entity_ABC* srcTeam = 0;
    const Entity_ABC* srcSuperior = 0;
    if( const kernel::CommunicationHierarchies* srcHierarchies = draggedEntity->Retrieve< kernel::CommunicationHierarchies >() )
    {
        srcTeam = &srcHierarchies->GetTop();
        srcSuperior = srcHierarchies->GetSuperior();
    }

    if( srcSuperior == dstEntity )
        return false; // cannot drop an item on his parent

    if( !dstKnowledgeGroup && !srcKnowledgeGroup )
        return false; // if source is not a knowledge group, then destination must be a knowledge group

    if( dstTeam == dstEntity && srcAutomat )
        return false; // cannot drop an automat directly on its team

    if( srcTeam != dstTeam )
        return false; // cannot drag n drop between teams

    return true;
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Drop
// Created: SBO 2006-09-26
// LTO
// -----------------------------------------------------------------------------
bool CommunicationListView::Drop( const Entity_ABC& draggedEntity, const Entity_ABC& target )
{
    const Automat_ABC*        automat = dynamic_cast< const Automat_ABC* >       ( &draggedEntity );
    const KnowledgeGroup_ABC* group   = dynamic_cast< const KnowledgeGroup_ABC* >( &target );
    if( automat && group )
    {
        // moving an automat under knowledgegroup
        CommunicationHierarchies& com = const_cast< CommunicationHierarchies& >( automat->Get< CommunicationHierarchies >() );
        if( &com.GetTop() != &target.Get< CommunicationHierarchies >().GetTop() )
            return false;
        static_cast< AutomatCommunications& >( com ).ChangeSuperior( const_cast< Entity_ABC& >( target ) );
        return true;
    }
    else
    {
        // moving a knowledgegroup under knowledgegroup
        const KnowledgeGroup_ABC* knowledgegroup = dynamic_cast< const KnowledgeGroup_ABC* >( &draggedEntity );
        const Entity_ABC* team = dynamic_cast< const Entity_ABC* >( &target );
        if( knowledgegroup && group && ( knowledgegroup != group ) )
        {
            CommunicationHierarchies& com = const_cast< CommunicationHierarchies& >( knowledgegroup->Get< CommunicationHierarchies >() );
            if( &com.GetTop() != &target.Get< CommunicationHierarchies >().GetTop() )
                return false;
            static_cast< AutomatCommunications& >( com ).ChangeSuperior( const_cast< Entity_ABC& >( target ) );
            return true;
        }
        else if( knowledgegroup && team )
        {
            // moving knowledgegroup under side
            CommunicationHierarchies& com = const_cast< CommunicationHierarchies& >( knowledgegroup->Get< CommunicationHierarchies >() );
            if( &com.GetTop() != team )
                return false;
            static_cast< KnowledgeGroupCommunications& >( com ).ChangeSuperior( const_cast< Entity_ABC& >( *team ) );
            return true;
        }
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

// -----------------------------------------------------------------------------
// Name: CommunicationListView::CommunicationListView::dragMoveEvent
// Created: FHD/SYD 2009-12-01
// LTO
// -----------------------------------------------------------------------------
void CommunicationListView::dragMoveEvent( QDragMoveEvent *pEvent )
{
    const Entity_ABC* entity = gui::ValuedDragObject::GetValue< Entity_ABC >( pEvent );
    if( !entity )
    {
        pEvent->ignore();
        return;
    }

    QPoint position = viewport()->mapFromParent( pEvent->pos() );
    pEvent->accept( CanDrop( entity, position ) );
}
