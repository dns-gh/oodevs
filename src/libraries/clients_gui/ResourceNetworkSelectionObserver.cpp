// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ResourceNetworkSelectionObserver.h"

#include "ResourceNetwork_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkSelectionObserver constructor
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
ResourceNetworkSelectionObserver::ResourceNetworkSelectionObserver( Controllers& controllers )
    : controllers_( controllers )
    , selected_   ( 0 )
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
void ResourceNetworkSelectionObserver::NotifySelected( const Entity_ABC* element )
{
    if( selected_ != 0 )
    {
        const_cast< ResourceNetwork_ABC* >( selected_->Retrieve< ResourceNetwork_ABC >() )->Select( false );
        selected_ = 0;
    }
    if( element )
        if( ResourceNetwork_ABC* resource = const_cast< ResourceNetwork_ABC* >( element->Retrieve< ResourceNetwork_ABC >() ) )
        {
            selected_ = element;
            resource->Select( true );
        }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkSelectionObserver::NotifyDeleted
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void ResourceNetworkSelectionObserver::NotifyDeleted( const Entity_ABC& entity )
{
    if( &entity == selected_ )
        selected_ = 0;
}
