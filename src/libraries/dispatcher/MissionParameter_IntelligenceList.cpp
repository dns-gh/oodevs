// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_IntelligenceList.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_IntelligenceList constructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_IntelligenceList::MissionParameter_IntelligenceList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( int i = 0; i < asn.value().intelligencelist().elem_size(); ++i )
        intelligenceOrders_.push_back( IntelligenceOrder( asn.value().intelligencelist().elem( i ) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_IntelligenceList destructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_IntelligenceList::~MissionParameter_IntelligenceList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_IntelligenceList::Send
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_IntelligenceList::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value ( bNullValue_ );
    for( std::vector< IntelligenceOrder >::const_iterator it = intelligenceOrders_.begin(); it != intelligenceOrders_.end(); ++it )
        it->Send( *asn.mutable_value()->mutable_intelligencelist()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_IntelligenceList::Delete
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_IntelligenceList::Delete( Common::MsgMissionParameter& asn ) const
{
    if( asn.value().intelligencelist().elem_size() > 0 )
        asn.mutable_value()->mutable_intelligencelist()->Clear();
    delete asn.mutable_value()->mutable_intelligencelist();
}
