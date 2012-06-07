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
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/Controllers.h"
#include "preparation/UrbanHierarchies.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanListView constructor
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
UrbanListView::UrbanListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, ModelBuilder& modelBuilder )
    : EntityListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
    , controllers_ ( controllers )
    , modelBuilder_( modelBuilder )
{
    addColumn( tr( "Urban" ) );
    //setAcceptDrops( true );
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
    ValuedListItem* item = 0;
    if( superior )
    {
        if( ValuedListItem* superiorItem = FindItem( superior, firstChild() ) )
            item = CreateItem( superiorItem );
    }
    else
        item = CreateItem( this );
    if( item )
    {
        item->SetNamed( static_cast< const kernel::Entity_ABC& >( object ) );
        //item->setDragEnabled( true );
        item->SetToolTip( QString( "%1 [%L2]" ).arg( object.GetName() ).arg( object.GetId() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyDeleted
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyDeleted( const kernel::UrbanObject_ABC& object )
{
    // TODO
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
