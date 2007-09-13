// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_ObjectiveList.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectiveList constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_ObjectiveList::MIL_ParameterType_ObjectiveList()
    : MIL_ParameterType_ABC( "ObjectiveList" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectiveList destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_ObjectiveList::~MIL_ParameterType_ObjectiveList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectiveList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_ObjectiveList::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.value.t != T_MissionParameter_value_missionObjectiveList ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeListeObjectifs( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    NET_ASN_Tools::CopyObjectiveList( *from.value.u.missionObjectiveList, to );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectiveList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_ObjectiveList::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListeObjectifs( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeListeObjectifs( to ) )
        return false;

    NET_ASN_Tools::CopyObjectiveList( from, to );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectiveList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_ObjectiveList::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListeObjectifs( from ) )
        return false;

    to.null_value                   = false;
    to.value.t                      = T_MissionParameter_value_missionObjectiveList;
    to.value.u.missionObjectiveList = new ASN1T_MissionObjectiveList();
    
    return NET_ASN_Tools::CopyObjectiveList( from, *to.value.u.missionObjectiveList );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectiveList::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_ObjectiveList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_missionObjectiveList );
    assert( to.value.u.missionObjectiveList );
    NET_ASN_Tools::Delete( *to.value.u.missionObjectiveList );
    delete to.value.u.missionObjectiveList;
}
