// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_AutomatList.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AutomatList::MissionParameter_AutomatList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( int i = 0; i < asn.value().automatlist().elem_size(); ++i )
        automats_.push_back( asn.value().automatlist().elem( i ).oid() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AutomatList::~MissionParameter_AutomatList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AutomatList::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_automatlist();
    for( std::vector< int >::const_iterator it = automats_.begin(); it != automats_.end(); ++it )
        asn.mutable_value()->mutable_automatlist()->add_elem()->set_oid( *it );
}
