// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_String.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"
#include "MIL_MissionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_String constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_String::MIL_ParameterType_String()
    : MIL_ParameterType_ABC( "string" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_String destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_String::~MIL_ParameterType_String()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_String::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_String::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_aCharStr;
    
    return from.ToString( to.value.u.aCharStr );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_String::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_String::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_aCharStr );
}
