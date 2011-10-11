// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AutomatTypeResolver.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/AutomatType.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AutomatTypeResolver constructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
AutomatTypeResolver::AutomatTypeResolver( const tools::Resolver_ABC< kernel::AutomatType, std::string >& automatTypes )
    : automatTypes_( automatTypes )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatTypeResolver destructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
AutomatTypeResolver::~AutomatTypeResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatTypeResolver::Resolve
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
unsigned long AutomatTypeResolver::Resolve( const std::string& automatTypeName ) const
{
    kernel::AutomatType* type = automatTypes_.Find( automatTypeName );
    if( type == 0 )
        return 0;
    return type->GetId();
}
