// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_PointList.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_PointList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PointList::MissionParameter_PointList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    points_.reserve( asn.value().pointlist().elem_size() );
    for( int i = 0; i != asn.value().pointlist().elem_size(); ++i )
        points_.push_back( Localisation( asn.value().pointlist().elem(i).location() ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PointList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PointList::~MissionParameter_PointList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PointList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PointList::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_pointlist();
    for( T_LocalisationVector::const_iterator it = points_.begin(); it != points_.end(); ++it )
        it->Send( *asn.mutable_value()->mutable_pointlist()->add_elem()->mutable_location() );
}
