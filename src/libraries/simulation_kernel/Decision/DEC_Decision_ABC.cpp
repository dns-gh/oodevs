// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Decision_ABC.h"
#include "Brain.h"

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC constructor
// Created: RDS 2008-05-05
// -----------------------------------------------------------------------------
DEC_Decision_ABC::DEC_Decision_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC destructor
// Created: RDS 2008-04-30
// -----------------------------------------------------------------------------
DEC_Decision_ABC::~DEC_Decision_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC::GetScriptRef
// Created: MCO 2011-04-30
// -----------------------------------------------------------------------------
directia::tools::binders::ScriptRef DEC_Decision_ABC::GetScriptRef( const std::string& id )
{
    sword::Brain* brain = GetBrain();
    if( !brain )
        throw MASA_EXCEPTION( "Trying to access a deleted brain." );
    return brain->GetScriptRef( id );
}
