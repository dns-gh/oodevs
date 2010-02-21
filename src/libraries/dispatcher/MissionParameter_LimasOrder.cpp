// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_LimasOrder.h"
#include "LimaOrder.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_LimasOrder constructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_LimasOrder::MissionParameter_LimasOrder( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( int i = 0; i < asn.value().limasorder().elem_size(); ++i )
        limaOrders_.push_back( LimaOrder( asn.value().limasorder().elem().Get(i) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_LimasOrder destructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_LimasOrder::~MissionParameter_LimasOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_LimasOrder::Send
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_LimasOrder::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value ( bNullValue_ );
    for( std::vector< LimaOrder >::const_iterator it = limaOrders_.begin(); it != limaOrders_.end(); ++it )
        it->Send( *asn.mutable_value()->mutable_limasorder()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_LimasOrder::Delete
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_LimasOrder::Delete( Common::MsgMissionParameter& asn ) const
{
    if( asn.value().limasorder().elem_size() )
        asn.mutable_value()->mutable_limasorder()->Clear();
    delete asn.mutable_value()->mutable_limasorder();
}
