// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_MaintenancePriorities.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MaintenancePriorities::MissionParameter_MaintenancePriorities( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    maintenancePriorities_.reserve( asn.value().logmaintenancepriorities().elem_size() );
    for( int i = 0; i < asn.value().logmaintenancepriorities().elem_size(); ++i )
        maintenancePriorities_.push_back( asn.value().logmaintenancepriorities().elem( i ).equipment() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MaintenancePriorities::~MissionParameter_MaintenancePriorities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MaintenancePriorities::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_logmaintenancepriorities();
    for( std::vector< int >::const_iterator it = maintenancePriorities_.begin(); it != maintenancePriorities_.end(); ++it )
        asn.mutable_value()->mutable_logmaintenancepriorities()->add_elem()->set_equipment( *it );
}
