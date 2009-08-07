// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_Direction.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"
#include "MIL_MissionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Direction constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Direction::MIL_ParameterType_Direction()
    : MIL_ParameterType_ABC( "Direction" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Direction destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Direction::~MIL_ParameterType_Direction()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Direction::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_Direction::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_heading;
    
    return from.ToDirection( to.value.u.heading );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Direction::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_Direction::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_heading );
}
