// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_DotationType.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

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
void MIL_ParameterType_DotationType::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_dotationType ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeDotation( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyDotationType( from.value.u.dotationType, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_DotationType::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_DotationType::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*//*knowledgeResolver*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeDotation( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeDotation( to ) )
        return false;

    NET_ASN_Tools::CopyDotationType( from, to );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_DotationType::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_DotationType::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeDotation( from ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_dotationType;
    
    if( !NET_ASN_Tools::CopyDotationType( from, to.value.u.dotationType ) )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_DotationType::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_DotationType::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_dotationType );
}
