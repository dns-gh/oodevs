// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_Bool.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Bool constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Bool::MIL_ParameterType_Bool()
    : MIL_ParameterType_ABC( "bool" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Bool destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Bool::~MIL_ParameterType_Bool()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Bool::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_Bool::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    // $$$$ SBO 2008-05-26: check optional + null
    if( from.value.t != T_MissionParameter_value_aBool ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( to.Type() != eBool ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyBool( from.value.u.aBool, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Bool::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_Bool::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.Type() != eBool )
        return false;

    // Check dest
    if( to.Type() != eBool )
        return false;

    if( !NET_ASN_Tools::CopyBool( from, to ) )
        return false;
    
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Bool::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_Bool::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.Type() != eBool )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_aBool;
    
    if( !NET_ASN_Tools::CopyBool( from, to.value.u.aBool ) )
        return false;

    return true; 
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Bool::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_Bool::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_aBool );
}
