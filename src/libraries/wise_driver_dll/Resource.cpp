// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Resource.h"
#pragma warning( push )
#pragma warning( disable: 4201 )
#include <wise/iwisedriversink.h>
#include <wise/WISEAttributeGroupConverter.h> 
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Resource constructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
Resource::Resource( const sword::ResourceDotations::ResourceDotation& message )
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
void Resource::Update( const sword::ResourceDotations::ResourceDotation& message )
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
