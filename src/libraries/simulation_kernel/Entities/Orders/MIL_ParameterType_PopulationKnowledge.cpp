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
void MIL_ParameterType_PopulationKnowledge::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_knowledgePopulation ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeConnaissancePopulation( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyPopulationKnowledge( from.value.u.knowledgePopulation, to, knowledgeResolver ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_PopulationKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_PopulationKnowledge::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeConnaissancePopulation( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeConnaissancePopulation( to ) )
        return false;

    NET_ASN_Tools::CopyPopulationKnowledge( from, to );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_PopulationKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_PopulationKnowledge::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeConnaissancePopulation( from ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_knowledgePopulation;
    
    if( !NET_ASN_Tools::CopyPopulationKnowledge( from, to.value.u.knowledgePopulation, knowledgeResolver ) )
        return false;

    return true;    
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_PopulationKnowledge::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_PopulationKnowledge::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_knowledgeObject );
}
