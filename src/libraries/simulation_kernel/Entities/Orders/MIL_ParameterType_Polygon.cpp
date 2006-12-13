// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_Polygon.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Polygon::MIL_ParameterType_Polygon()
    : MIL_ParameterType_ABC( "Polygon" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_Polygon::~MIL_ParameterType_Polygon()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_Polygon::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_polygon ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeLocalisation( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyLocation( *from.value.u.point, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_Polygon::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeLocalisation( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeLocalisation( to ) )
        return false;

    NET_ASN_Tools::CopyLocation( from, to );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_Polygon::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeLocalisation( from ) )
        return false;

    to.null_value      = false;
    to.value.t         = T_MissionParameter_value_polygon;
    to.value.u.polygon = new ASN1T_Polygon();
    
    if( !NET_ASN_Tools::CopyLocation( from, *to.value.u.polygon ) )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_Polygon::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_Polygon::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_polygon );
    assert( to.value.u.polygon );
    NET_ASN_Tools::Delete( *to.value.u.polygon );
    delete to.value.u.polygon;    
}
