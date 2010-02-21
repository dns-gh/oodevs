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
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

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
bool MIL_ParameterType_ObjectiveList::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( false );
    return from.ToObjectiveList( *to.mutable_value()->mutable_missionobjectivelist() );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ObjectiveList::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_ObjectiveList::CleanAfterSerialization( Common::MsgMissionParameter& to ) const
{
    assert( to.value().has_missionobjectivelist() );
    assert( to.mutable_value()->mutable_missionobjectivelist() );
    to.mutable_value()->mutable_missionobjectivelist()->Clear();
    delete to.mutable_value()->mutable_missionobjectivelist();
}
