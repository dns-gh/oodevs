// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_ObjectiveList.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectiveList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectiveList::MissionParameter_ObjectiveList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    objects_.reserve( asn.value().missionobjectivelist().elem_size() );
    for( int i = 0; i != asn.value().missionobjectivelist().elem_size(); ++i )
        objects_.push_back( Objective( asn.value().missionobjectivelist().elem(i) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectiveList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectiveList::~MissionParameter_ObjectiveList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectiveList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectiveList::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value               ( bNullValue_ );
    asn.mutable_value()->mutable_missionobjectivelist();
    if( !objects_.empty() )
    {
        for( T_ObjectiveVector::const_iterator it = objects_.begin(); it != objects_.end(); ++it  )
            (*it).Send( *asn.mutable_value()->mutable_missionobjectivelist()->add_elem() );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectiveList::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectiveList::Delete( Common::MsgMissionParameter& asn ) const
{
    if( asn.value().missionobjectivelist().elem_size() > 0 )
        asn.mutable_value()->mutable_missionobjectivelist()->Clear();
    delete asn.mutable_value()->mutable_missionobjectivelist();
}
