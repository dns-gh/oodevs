// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_Enumeration.h"

#include "MIL_MissionParameter_ABC.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Enumeration constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Enumeration::MIL_ParameterType_Enumeration()
    : MIL_ParameterType_ABC( "enumeration" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Enumeration destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Enumeration::~MIL_ParameterType_Enumeration()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Enumeration::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Enumeration::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    int id;
    if( !from.ToId( id ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_enumeration;
    
    to.value.u.enumeration = id;

    return true;    
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Enumeration::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_Enumeration::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_enumeration );
}
