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
#include "dispatcher/Agent.h"

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterAgent constructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterAgent::MissionParameterAgent( const kernel::OrderParameter& type, const dispatcher::Agent& agent )
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
void MissionParameterAgent::Serialize( ASN1T_MissionParameter& parameter ) const
{
    parameter.null_value = 0;
    parameter.value.t = T_MissionParameter_value_unit;
    parameter.value.u.unit = agent_.GetId();
}

// -----------------------------------------------------------------------------
// Name: MissionParameterAgent::Clean
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionParameterAgent::Clean( ASN1T_MissionParameter& ) const
{
    // NOTHING
}
