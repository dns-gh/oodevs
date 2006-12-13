// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_NatureAtlas.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_NatureAtlas::MIL_ParameterType_NatureAtlas()
    : MIL_ParameterType_ABC( "NatureAtlas" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_NatureAtlas::~MIL_ParameterType_NatureAtlas()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_NatureAtlas::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_natureAtlas ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeLocalisation( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyLocation( *from.value.u.point, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_NatureAtlas::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( from.Type() != eId )
        return false;

    // Check dest
    if( to.Type() != eId )
        return false;

    NET_ASN_Tools::CopyNatureAtlas( from, to );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_NatureAtlas::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( from.Type() != eId )
        return false;

    to.null_value          = false;
    to.value.t             = T_MissionParameter_value_natureAtlas;
    to.value.u.natureAtlas = new ASN1T_NatureAtlas();
    
    if( !NET_ASN_Tools::CopyNatureAtlas( from, *to.value.u.natureAtlas ) )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_NatureAtlas::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_NatureAtlas::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_natureAtlas );
    assert( to.value.u.natureAtlas );
    delete to.value.u.natureAtlas;
}
