// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_Point.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Point constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Point::MIL_ParameterType_Point()
: MIL_ParameterType_ABC( "Point" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Point destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Point::~MIL_ParameterType_Point()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Point::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_Point::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( from.value.t != T_MissionParameter_value_point ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
    if( !bIsOptional && from.null_value )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypePoint( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyPoint( *from.value.u.point, to, from.null_value ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Point::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Point::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !DEC_Tools::CheckTypePoint( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypePoint( to ) )
        return false;

    if( !NET_ASN_Tools::CopyPoint( from, to ) && !bIsOptional )
        return false;
    
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Point::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Point::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !DEC_Tools::CheckTypePoint( from ) )
        return false;

    to.value.t       = T_MissionParameter_value_point;   
    to.value.u.point = new ASN1T_Point();
    to.null_value    = !NET_ASN_Tools::CopyPoint( from, *to.value.u.point );
    if( !bIsOptional && to.null_value )
        return false;
    return true;    
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Point::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_Point::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_point );
    assert( to.value.u.point );
    if( !to.null_value )
        NET_ASN_Tools::Delete( *to.value.u.point );
    delete to.value.u.point;
}
