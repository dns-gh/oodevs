// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_PopulationKnowledge.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_PopulationKnowledge constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PopulationKnowledge::MissionParameter_PopulationKnowledge( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , populationKnowledge_( asn.value().populationknowledge().oid() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PopulationKnowledge destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PopulationKnowledge::~MissionParameter_PopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PopulationKnowledge::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PopulationKnowledge::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_populationknowledge()->set_oid( populationKnowledge_ );
}
