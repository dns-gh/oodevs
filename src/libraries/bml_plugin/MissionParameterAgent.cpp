// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterAgent.h"
#include "dispatcher/Agent_ABC.h"

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterAgent constructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterAgent::MissionParameterAgent( const kernel::OrderParameter& type, const dispatcher::Agent_ABC& agent )
    : MissionParameter_ABC( type )
    , agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterAgent destructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterAgent::~MissionParameterAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterAgent::Serialize
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionParameterAgent::Serialize( Common::MsgMissionParameter& parameter ) const
{
    parameter.set_null_value( 0 );
    parameter.mutable_value()->mutable_unit()->set_oid( agent_.GetId() );
}
