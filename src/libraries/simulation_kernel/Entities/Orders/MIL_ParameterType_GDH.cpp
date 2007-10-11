// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_GDH.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GDH constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_GDH::MIL_ParameterType_GDH()
    : MIL_ParameterType_ABC( "GDH" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GDH destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_GDH::~MIL_ParameterType_GDH()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GDH::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_GDH::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_dateTime ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( to.Type() != eFloat )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyGDH( *from.value.u.dateTime, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GDH::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_GDH::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.Type() != eFloat )
        return false;

    // Check dest
    if( to.Type() != eFloat )
        return false;

    NET_ASN_Tools::CopyGDH( from, to );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GDH::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_GDH::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.Type() != eFloat )
        return false;

    to.null_value  = false;
    to.value.t     = T_MissionParameter_value_dateTime;

    if( !NET_ASN_Tools::CopyGDH( from, *to.value.u.dateTime ) )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GDH::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_GDH::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    // NOTHIN
}
