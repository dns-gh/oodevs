// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "stdafx.h"
#include "Resource.h"
#include <iwisedriversink.h>
#include <WISEAttributeGroupConverter.h> 

// -----------------------------------------------------------------------------
// Name: Resource constructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
Resource::Resource( const MsgsSimToClient::ResourceDotations::ResourceDotation& message )
    : type_( message.type().id() )
    , available_( 0 )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: Resource destructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
Resource::~Resource()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Resource::Update
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Resource::Update( const MsgsSimToClient::ResourceDotations::ResourceDotation& message )
{
    available_ = message.quantite_disponible();
}

// -----------------------------------------------------------------------------
// Name: Resource::AddAttributeGroup
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Resource::AddAttributeGroup( std::list< CWISEAttributeGroup >& list, IWISEStringCache& cache ) const
{
    CWISEAttributeGroupTemplate groupTemplate;
    groupTemplate.Add( L"Type", long( type_ ) );
    groupTemplate.Add( L"AvailableQuantity", long( available_ ) );

    CWISEAttributeGroup group;
    CHECK_WISE_RESULT_EX( CWISEAttributeGroupConverter::TemplateToValueGroup( groupTemplate, &cache, L"Resource", group ) );
    list.push_back( group );
}
