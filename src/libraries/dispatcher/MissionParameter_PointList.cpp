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
#include "ClientPublisher_ABC.h"
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

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_PointList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PointList::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
    if( !points_.empty() )
    {
        for( T_LocalisationVector::const_iterator it = points_.begin(); it != points_.end(); ++it )
            (*it).Send( *asn.mutable_value()->mutable_pointlist()->add_elem()->mutable_location() );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PointList::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PointList::Delete( Common::MsgMissionParameter& asn ) const
{
    if( asn.value().pointlist().elem_size() > 0 )
        asn.mutable_value()->mutable_pointlist()->Clear();
    delete asn.mutable_value()->mutable_pointlist();
}
