// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_PathList.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PathList constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_PathList::MIL_ParameterType_PathList()
    : MIL_ParameterType_ABC( "PathList" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PathList destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_PathList::~MIL_ParameterType_PathList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PathList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_PathList::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_listItineraire ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeListeListesPoints( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyPathList( *from.value.u.listItineraire, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PathList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_PathList::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListeListesPoints( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeListeListesPoints( to ) )
        return false;

    NET_ASN_Tools::CopyPathList( from, to );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PathList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_PathList::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListeListesPoints( from ) )
        return false;

    to.null_value              = false;
    to.value.t                = T_MissionParameter_value_listItineraire;
    to.value.u.listItineraire = new ASN1T_ListItineraire();
    
    if( !NET_ASN_Tools::CopyPathList( from, *to.value.u.listItineraire ) )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_PathList::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_PathList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_listItineraire );
    assert( to.value.u.listItineraire );
    NET_ASN_Tools::Delete( *to.value.u.listItineraire );
    delete to.value.u.listItineraire;
}
