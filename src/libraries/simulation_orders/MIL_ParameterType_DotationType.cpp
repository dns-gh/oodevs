// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_DotationType.h"
#include "MIL_MissionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_DotationType constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_DotationType::MIL_ParameterType_DotationType()
    : MIL_ParameterType_ABC( "DotationType" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_DotationType destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_DotationType::~MIL_ParameterType_DotationType()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_DotationType::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_DotationType::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_dotationType;
    
    return from.ToDotationType( to.value.u.dotationType );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_DotationType::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_DotationType::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_dotationType );
}
