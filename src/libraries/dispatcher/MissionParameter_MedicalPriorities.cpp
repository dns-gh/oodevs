// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_MedicalPriorities.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MedicalPriorities::MissionParameter_MedicalPriorities( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    std::copy( asn.value().logmedicalpriorities().elem().begin(), asn.value().logmedicalpriorities().elem().end()
             , std::back_inserter( medicalPriorities_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MedicalPriorities::~MissionParameter_MedicalPriorities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MedicalPriorities::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
    for (std::vector< int >::const_iterator it(medicalPriorities_.begin()); it != medicalPriorities_.end(); ++it )
        asn.mutable_value()->mutable_logmedicalpriorities()->add_elem( Common::EnumHumanWound( *it ) );        
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MedicalPriorities::Delete( Common::MsgMissionParameter& asn ) const
{
    asn.mutable_value()->clear_logmedicalpriorities();
}
