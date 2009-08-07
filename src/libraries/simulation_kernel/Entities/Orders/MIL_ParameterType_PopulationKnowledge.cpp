// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_PopulationKnowledge.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"
#include "MIL_MissionParameter_ABC.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_PopulationKnowledge constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_PopulationKnowledge::MIL_ParameterType_PopulationKnowledge()
    : MIL_ParameterType_ABC( "PopulationKnowledge" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_PopulationKnowledge destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_PopulationKnowledge::~MIL_ParameterType_PopulationKnowledge()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_PopulationKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_PopulationKnowledge::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.value.t    = T_MissionParameter_value_populationKnowledge;
    to.null_value = !from.ToPopulationKnowledge( to.value.u.populationKnowledge );
    
    return !to.null_value || bIsOptional;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_PopulationKnowledge::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_PopulationKnowledge::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_populationKnowledge );
}
