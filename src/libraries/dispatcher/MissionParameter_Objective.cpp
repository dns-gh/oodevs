// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Objective.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Objective constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Objective::MissionParameter_Objective( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , data_               ( asn.value().missionobjective() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Objective destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Objective::~MissionParameter_Objective()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Objective::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Objective::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value               ( bNullValue_ );
    //asn.value.t                  = T_MissionParameter_value_missionObjective;
//    asn.mutable_value()->mutable_missionobjective() = new MsgMissionObjective();
    data_.Send( *asn.mutable_value()->mutable_missionobjective() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Objective::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Objective::Delete( Common::MsgMissionParameter& asn ) const
{
    delete asn.mutable_value()->mutable_missionobjective();
}
