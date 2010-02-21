// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_PathList.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_PathList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PathList::MissionParameter_PathList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    paths_.reserve( asn.value().pathlist().elem_size() );
    for( int i = 0; i != asn.value().pathlist().elem_size(); ++i )
        paths_.push_back( Localisation( asn.value().pathlist().elem().Get(i).location() ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PathList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PathList::~MissionParameter_PathList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_PathList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PathList::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
    if( !paths_.empty() )
    {
        for( T_LocalisationVector::const_iterator it = paths_.begin(); it != paths_.end(); ++it )
            (*it).Send( *asn.mutable_value()->mutable_pathlist()->add_elem()->mutable_location() );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PathList::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PathList::Delete( Common::MsgMissionParameter& asn ) const
{
    if( asn.value().pathlist().elem_size() > 0 )
        asn.mutable_value()->mutable_pathlist()->Clear();
    delete asn.mutable_value()->mutable_pathlist();
}
