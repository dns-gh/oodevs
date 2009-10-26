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
#include "Entities/MIL_Entity_ABC.h"

BOOST_CLASS_EXPORT_GUID( DEC_Decision_ABC, "DEC_Decision_ABC" )

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
// Name: template< typename T > T DEC_Decision_ABC::GetScriptVariableRef
// Created: MGD 2009-08-04
// -----------------------------------------------------------------------------
directia::ScriptRef& DEC_Decision_ABC::GetScriptVariableRef( const std::string& name )
{
    return GetBrain().GetScriptVariable( name );
}

