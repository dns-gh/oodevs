// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ResourceNetworkSelectionObserver.h"
#include "Controllers.h"
#include "Entity_ABC.h"
#include "ResourceNetwork_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkSelectionObserver constructor
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
ResourceNetworkSelectionObserver::ResourceNetworkSelectionObserver( kernel::Controllers& controllers )
    : controllers_( controllers )
    , selected_( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkSelectionObserver destructor
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
ResourceNetworkSelectionObserver::~ResourceNetworkSelectionObserver()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkSelectionObserver::NotifySelected
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void ResourceNetworkSelectionObserver::NotifySelected( const kernel::Entity_ABC* element )
{
    if( selected_ != 0 )
    {
        const_cast< kernel::ResourceNetwork_ABC* >( selected_->Retrieve< kernel::ResourceNetwork_ABC >() )->Select( false );
        selected_ = 0;
    }
    if( element )
    {
        if( kernel::ResourceNetwork_ABC* resource = const_cast< kernel::ResourceNetwork_ABC* >( element->Retrieve< kernel::ResourceNetwork_ABC >() ) )
        {
            selected_= element;
            resource->Select( true );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkSelectionObserver::NotifyDeleted
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void ResourceNetworkSelectionObserver::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( &entity == selected_ )
        selected_ = 0;
}
