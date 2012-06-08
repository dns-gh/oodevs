// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UrbanListView.h"
#include "moc_UrbanListView.cpp"
#include "ModelBuilder.h"
#include "PreparationProfile.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_gui/ValuedDragObject.h"
#include "preparation/UrbanHierarchies.h"

// -----------------------------------------------------------------------------
// Name: UrbanListView constructor
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
UrbanListView::UrbanListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder )
    : EntityListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
    , controllers_ ( controllers )
    , modelBuilder_( modelBuilder )
{
    addColumn( tr( "Urban" ) );
    setDragAutoScroll( true );
    setAcceptDrops( true );
    viewport()->setAcceptDrops( true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView destructor
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
UrbanListView::~UrbanListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyCreated
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyCreated( const kernel::Team_ABC& /*team*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyDeleted
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyDeleted( const kernel::Team_ABC& /*team*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyCreated
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyCreated( const kernel::UrbanObject_ABC& object )
{
    const kernel::Hierarchies* hierarchies = object.Retrieve< kernel::Hierarchies >();
    if( !hierarchies ) // $$$$ ABR 2012-05-22: Only urban block have hierarchies
        return;
    const kernel::Entity_ABC* superior = hierarchies->GetSuperior();
    gui::ValuedListItem* item = 0;
    if( superior )
    {
        if( gui::ValuedListItem* superiorItem = gui::FindItem( superior, firstChild() ) )
            item = CreateItem( superiorItem );
    }
    else
        item = CreateItem( this );
    if( item )
    {
        item->SetNamed( static_cast< const kernel::Entity_ABC& >( object ) );
        item->SetToolTip( QString( "%1 [%L2]" ).arg( object.GetName() ).arg( object.GetId() ) );
        if( superior )
            item->setDragEnabled( true );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyDeleted
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyDeleted( const kernel::UrbanObject_ABC& object )
{
    delete gui::FindItem( static_cast< const kernel::Entity_ABC* >( &object ), firstChild() );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::IsTypeRejected
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
bool UrbanListView::IsTypeRejected( const kernel::Entity_ABC& entity ) const
{
    return entity.GetTypeName() != kernel::UrbanObject_ABC::typeName_;
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyModeChanged
// Created: JSR 2012-05-24
// -----------------------------------------------------------------------------
void UrbanListView::NotifyModeChanged( int newMode )
{
    ModesObserver_ABC::NotifyModeChanged( newMode );
    setSelectionMode( newMode == ePreparationMode_Terrain ? Q3ListView::Extended : Q3ListView::Single );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::OnContextMenuRequested
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanListView::OnContextMenuRequested( Q3ListViewItem* i, const QPoint& pos, int col )
{
    if( !IsReadOnly() )
    {
        if( i == 0 && GetCurrentMode() == ePreparationMode_Terrain )
        {
            QMenu* menu = new QMenu( this );
            menu->addAction( tr( "Create city" ), this, SLOT( OnCreateCity() ) );
            menu->popup( pos );
        }
        else
            gui::EntityListView::OnContextMenuRequested( i, pos, col );
    }

}

// -----------------------------------------------------------------------------
// Name: UrbanListView::OnCreateCity
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanListView::OnCreateCity()
{
    modelBuilder_.CreateCityOrDistrict( 0 );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::keyPressEvent
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
void UrbanListView::keyPressEvent( QKeyEvent* key )
{
    if( GetCurrentMode() == ePreparationMode_Terrain && key->key() == Qt::Key_Delete )
    {
        std::vector< const kernel::UrbanObject_ABC* > blocks;
        Q3ListViewItemIterator it( this );
        while( gui::ValuedListItem* item = dynamic_cast< gui::ValuedListItem* >( *it ) )
        {
            if( item->isSelected() && item->IsA< const kernel::Entity_ABC >() )
                blocks.push_back( static_cast< const kernel::UrbanObject_ABC* >( item->GetValueNoCheck< const kernel ::Entity_ABC >() ) );
            ++it;
        }
        if( !blocks.empty() )
        {
            modelBuilder_.DeleteBlocks( blocks );
            key->accept();
            return;
        }
    }
    gui::EntityListView::keyPressEvent( key );
}

namespace
{
    struct sUrbanDrag {};
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::dragObject
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
Q3DragObject* UrbanListView::dragObject()
{
    if( GetCurrentMode() != ePreparationMode_Terrain )
        return 0;
    gui::EntityListView::dragObject();
    dragged_.clear();
    EUrbanLevel level = static_cast< EUrbanLevel >( -1 );
    Q3ListViewItemIterator it( this );
    while( gui::ValuedListItem* item = dynamic_cast< gui::ValuedListItem* >( *it ) )
    {
        if( item->isSelected() && item->IsA< const kernel::Entity_ABC >() )
        {
            kernel::Entity_ABC* entity = const_cast< kernel::Entity_ABC* >( item->GetValueNoCheck< const kernel::Entity_ABC >() );
            if( entity )
            {
                UrbanHierarchies& hierarchies = static_cast< UrbanHierarchies& >( entity->Get< kernel::Hierarchies >() );
                if( level != static_cast< EUrbanLevel >( -1 ) && level != hierarchies.GetLevel() )
                {
                    dragged_.clear();
                    return 0;
                }
                level = hierarchies.GetLevel();
                dragged_.push_back( entity );
            }
        }
        ++it;
    }
    if( dragged_.empty() )
        return 0;
    return new gui::ValuedDragObject( static_cast< sUrbanDrag* >( 0 ), this );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::Drop
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
kernel::Entity_ABC* UrbanListView::Drop( QDropEvent* pEvent ) const
{
    if( !dragged_.empty() && gui::ValuedDragObject::Provides< const sUrbanDrag >( pEvent ) )
    {
        QPoint position = viewport()->mapFromParent( pEvent->pos() );
        gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( itemAt( position ) );
        if( item && item->IsA< const kernel::Entity_ABC >() )
        {
            kernel::Entity_ABC* entity = const_cast< kernel::Entity_ABC* >( item->GetValueNoCheck< const kernel::Entity_ABC >() );
            if( entity )
            {
                EUrbanLevel level = static_cast< const UrbanHierarchies& >( entity->Get< kernel::Hierarchies >() ).GetLevel();
                EUrbanLevel draggedLevel = static_cast< const UrbanHierarchies& >( dragged_.front()->Get< kernel::Hierarchies >() ).GetLevel();
                if( ( draggedLevel == eUrbanLevelBlock && level == eUrbanLevelDistrict ) || ( draggedLevel == eUrbanLevelDistrict && level == eUrbanLevelCity ) )
                {
                    pEvent->accept();
                    return entity;
                }
            }
        }
    }
    pEvent->ignore();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::viewportDragEnterEvent
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanListView::viewportDragEnterEvent( QDragEnterEvent* pEvent )
{
    EntityListView::viewportDragEnterEvent( pEvent );
    pEvent->accept( gui::ValuedDragObject::Provides< const sUrbanDrag >( pEvent ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::viewportDragMoveEvent
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanListView::viewportDragMoveEvent( QDragMoveEvent *pEvent )
{
    EntityListView::viewportDragMoveEvent( pEvent );
    Drop( pEvent );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::viewportDropEvent
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanListView::viewportDropEvent( QDropEvent* pEvent )
{
    EntityListView::viewportDropEvent( pEvent );
    kernel::Entity_ABC* target = Drop( pEvent );
    if( target )
    {
        for( std::vector< kernel::Entity_ABC* >::const_iterator it = dragged_.begin(); it != dragged_.end(); ++it )
        {
            kernel::UrbanObject_ABC* object = static_cast< kernel::UrbanObject_ABC* >( *it );
            UrbanHierarchies& hierarchies = static_cast< UrbanHierarchies& >( object->Get< kernel::Hierarchies >() );
            kernel::Entity_ABC* superior = const_cast< kernel::Entity_ABC* >( hierarchies.GetSuperior() );
            if( !superior || superior == target )
                continue;
            superior->Get< kernel::UrbanPositions_ABC >().ResetConvexHull();
            NotifyDeleted( *object );
            static_cast< UrbanHierarchies& >( object->Get< kernel::Hierarchies >() ).ChangeSuperior( *target );
            NotifyCreated( *object );
            tools::Iterator< const kernel::Entity_ABC& > subIt = hierarchies.CreateSubordinateIterator();
            while( subIt.HasMoreElements() )
            {
                const kernel::UrbanObject_ABC& child = static_cast< const kernel::UrbanObject_ABC& >( subIt.NextElement() );
                NotifyCreated( child );
            }
        }
        target->Get< kernel::UrbanPositions_ABC >().ResetConvexHull();
        controllers_.actions_.SetSelected( *target, false );
    }
    dragged_.clear();
}
