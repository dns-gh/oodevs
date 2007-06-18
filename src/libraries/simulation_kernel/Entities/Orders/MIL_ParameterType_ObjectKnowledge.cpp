// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_ObjectKnowledge.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledge constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_ObjectKnowledge::MIL_ParameterType_ObjectKnowledge()
    : MIL_ParameterType_ABC( "ObjectKnowledge" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledge destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_ObjectKnowledge::~MIL_ParameterType_ObjectKnowledge()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_ObjectKnowledge::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_objectKnowledge ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeConnaissanceObjet( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyObjectKnowledge( from.value.u.objectKnowledge, to, knowledgeResolver ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_ObjectKnowledge::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeConnaissanceObjet( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeConnaissanceObjet( to ) )
        return false;

    NET_ASN_Tools::CopyObjectKnowledge( from, to );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_ObjectKnowledge::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeConnaissanceObjet( from ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_objectKnowledge;
    
    if( !NET_ASN_Tools::CopyObjectKnowledge( from, to.value.u.objectKnowledge, knowledgeResolver ) )
        return false;

    return true;    
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectKnowledge::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_ObjectKnowledge::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_objectKnowledge );
}
