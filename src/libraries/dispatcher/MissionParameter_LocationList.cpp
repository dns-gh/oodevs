// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_LocationList.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_LocationList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_LocationList::MissionParameter_LocationList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    locations_.reserve( asn.value().locationlist().elem_size() );
    for( int i = 0; i != asn.value().locationlist().elem_size(); ++i )
        locations_.push_back( Localisation( asn.value().locationlist().elem( i ) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_LocationList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_LocationList::~MissionParameter_LocationList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_LocationList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_LocationList::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_locationlist();
    for( T_LocalisationVector::const_iterator it = locations_.begin(); it != locations_.end(); ++it )
        it->Send( *asn.mutable_value()->mutable_locationlist()->add_elem() );
}
