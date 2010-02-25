// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_PolygonList.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;


// -----------------------------------------------------------------------------
// Name: MissionParameter_PolygonList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PolygonList::MissionParameter_PolygonList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    polygons_.reserve( asn.value().polygonlist().elem_size() );
    for( int i = 0; i != asn.value().polygonlist().elem_size(); ++i )
        polygons_.push_back( Localisation( asn.value().polygonlist().elem( i ).location() ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PolygonList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PolygonList::~MissionParameter_PolygonList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PolygonList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PolygonList::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_polygonlist();
    for( T_LocalisationVector::const_iterator it = polygons_.begin(); it != polygons_.end(); ++it )
        it->Send( *asn.mutable_value()->mutable_polygonlist()->add_elem()->mutable_location() );
}
