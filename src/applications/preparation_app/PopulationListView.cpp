// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PopulationListView.h"
#include "moc_PopulationListView.cpp"
#include "PreparationProfile.h"
#include "ModelBuilder.h"
#include "clients_gui/ValuedDragObject.h"
#include "preparation/PopulationHierarchies.h"
#include "preparation/ProfileHierarchies.h"

// -----------------------------------------------------------------------------
// Name: PopulationListView constructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
PopulationListView::PopulationListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder )
    : gui::PopulationListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
    , modelBuilder_( modelBuilder )
    , selected_    ( controllers )
{
    setDragAutoScroll( true );
    viewport()->setAcceptDrops( true );
    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), &modelBuilder, SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView destructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
PopulationListView::~PopulationListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::keyPressEvent
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void PopulationListView::keyPressEvent( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
    {
        modelBuilder_.DeleteEntity( *selected_ );
        key->accept();
    }
    else
        gui::PopulationListView::keyPressEvent( key );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifySelectionChanged
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void PopulationListView::NotifySelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements )
{
    // TODO généraliser au cas où y'a plusieurs éléments
    if( elements.size() == 1 )
        selected_ = elements.front();
    else
        selected_ = 0;
    gui::PopulationListView::NotifySelectionChanged( elements );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyCreated
// Created: JSR 2012-12-18
// -----------------------------------------------------------------------------
void PopulationListView::NotifyCreated( const kernel::Population_ABC& population )
{
    gui::PopulationListView::NotifyCreated( population );
    if( gui::ValuedListItem* item = gui::FindItem( static_cast< const kernel::Entity_ABC* >( &population ), firstChild() ) )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyCreated
// Created: SBO 2007-03-23
// -----------------------------------------------------------------------------
void PopulationListView::NotifyCreated( const kernel::Team_ABC& team )
{
    gui::PopulationListView::EntityListView::NotifyCreated( team );
    gui::ValuedListItem* item = gui::FindSibling( (const kernel::Entity_ABC*)&team, firstChild() );
    if( item )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::dragObject
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
Q3DragObject* PopulationListView::dragObject()
{
    if( IsDragAndDropLocked() )
        return 0;
    gui::PopulationListView::dragObject();
    gui::ValuedListItem* pItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;
    if( dynamic_cast< const kernel::Population_ABC* >( &*selected_ ) )
    {
        gui::ValuedListItem* pItem = static_cast< gui::ValuedListItem* >( selectedItem() );
        if( !pItem )
            return 0;
        return new gui::ValuedDragObject( pItem->GetValue< const kernel::Entity_ABC >(), this );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OnContextMenuRequested
// Created: ABR 2011-12-07
// -----------------------------------------------------------------------------
void PopulationListView::OnContextMenuRequested( Q3ListViewItem* item, const QPoint& pos, int index )
{
    gui::PopulationListView::OnContextMenuRequested( item, pos, index );
    if( item || !isVisible() )
        return;
    modelBuilder_.ClearSelection();
    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    menu->insertItem( tr( "Create side" ), &modelBuilder_, SLOT( OnCreate() ) );
    menu->exec( pos, index );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::viewportDragEnterEvent
// Created: JSR 2012-02-24
// -----------------------------------------------------------------------------
void PopulationListView::viewportDragEnterEvent( QDragEnterEvent* pEvent )
{
    EntityListView::viewportDragEnterEvent( pEvent );
    pEvent->accept( gui::ValuedDragObject::Provides< const kernel::Entity_ABC >( pEvent ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::viewportDragMoveEvent
// Created: JSR 2012-02-24
// -----------------------------------------------------------------------------
void PopulationListView::viewportDragMoveEvent( QDragMoveEvent *pEvent )
{
    EntityListView::viewportDragMoveEvent( pEvent );
    pEvent->accept( gui::ValuedDragObject::Provides< const kernel::Entity_ABC >( pEvent ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::viewportDropEvent
// Created: JSR 2012-02-24
// -----------------------------------------------------------------------------
void PopulationListView::viewportDropEvent( QDropEvent* pEvent )
{
    EntityListView::viewportDropEvent( pEvent );
    const kernel::Population_ABC* entity = dynamic_cast< const kernel::Population_ABC* >( gui::ValuedDragObject::GetValue< const kernel::Entity_ABC >( pEvent ) );
    if( entity )
    {
        QPoint position = viewport()->mapFromParent( pEvent->pos() );
        gui::ValuedListItem* targetItem = static_cast< gui::ValuedListItem* >( itemAt( position ) );
        if( !targetItem || !targetItem->IsA< const kernel::Entity_ABC >() )
            pEvent->ignore();
        else
        {
            pEvent->accept();
            kernel::TacticalHierarchies* hierarchies = const_cast< kernel::TacticalHierarchies* >( entity->Retrieve< kernel::TacticalHierarchies >() );
            if( hierarchies )
            {
                const kernel::Entity_ABC& target = *targetItem->GetValue< const kernel::Entity_ABC >();
                if( &target == entity )
                    return;
                gui::PopulationListView::NotifyDeleted( *entity );
                kernel::TacticalHierarchies* targetHierarchies = const_cast< kernel::TacticalHierarchies* >( target.Retrieve< kernel::TacticalHierarchies >() );
                const kernel::Entity_ABC& superior = targetHierarchies ? targetHierarchies->GetTop() : target;
                static_cast< ::PopulationHierarchies* >( hierarchies )->ChangeSuperior( const_cast< kernel::Entity_ABC& >( superior ) );
                if( ProfileHierarchies_ABC* profileHierarchie = const_cast< ProfileHierarchies_ABC* >( entity->Retrieve< ProfileHierarchies_ABC >() ) )
                    static_cast< ::ProfileHierarchies* >( profileHierarchie )->ChangeSuperior( const_cast< kernel::Entity_ABC& >( superior ) );
                gui::PopulationListView::NotifyCreated( *entity );
            }
        }
    }
    else
        pEvent->ignore();
}
