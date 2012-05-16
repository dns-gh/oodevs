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
    , controllers_( controllers )
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
void UrbanListView::NotifyCreated( const kernel::Team_ABC& team )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyDeleted
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyDeleted( const kernel::Team_ABC& team )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyCreated
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyCreated( const kernel::Object_ABC& object )
{
    const kernel::Hierarchies* hierarchies = object.Retrieve< kernel::Hierarchies >();
    if( !hierarchies )
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
void UrbanListView::NotifyDeleted( const kernel::Object_ABC& object )
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
