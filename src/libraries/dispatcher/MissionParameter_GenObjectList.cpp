// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_GenObjectList.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObjectList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GenObjectList::MissionParameter_GenObjectList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    objects_.reserve( asn.value().plannedworklist().elem_size() );
    for( int i = 0; i != asn.value().plannedworklist().elem_size(); ++i )
        objects_.push_back( GenObject( asn.value().plannedworklist().elem(i) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObjectList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GenObjectList::~MissionParameter_GenObjectList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObjectList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GenObjectList::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value                   ( bNullValue_ );
    if( !objects_.empty() )
    {
        for( T_GenObjectVector::const_iterator it = objects_.begin(); it != objects_.end(); ++it )
            (*it).Send( *asn.mutable_value()->mutable_plannedworklist()->add_elem() );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObjectList::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GenObjectList::Delete( Common::MsgMissionParameter& asn ) const
{
    if( asn.value().plannedworklist().elem_size() > 0 )
        asn.mutable_value()->mutable_plannedworklist()->Clear();
    delete asn.mutable_value()->mutable_plannedworklist();
}
