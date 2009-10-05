// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_ObjectiveList.h"
#include "game_asn/ASN_Delete.h"
#include "MIL_MissionParameter_ABC.h"

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
bool MIL_ParameterType_ObjectiveList::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value                   = false;
    to.value.t                      = T_MissionParameter_value_missionObjectiveList;
    to.value.u.missionObjectiveList = new ASN1T_MissionObjectiveList();
    
    return from.ToObjectiveList( *to.value.u.missionObjectiveList );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectiveList::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_ObjectiveList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_missionObjectiveList );
    assert( to.value.u.missionObjectiveList );
    ASN_Delete::Delete( *to.value.u.missionObjectiveList );
    delete to.value.u.missionObjectiveList;
}
