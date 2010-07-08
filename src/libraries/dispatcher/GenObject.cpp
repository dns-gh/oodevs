// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "GenObject.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: GenObject constructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
GenObject::GenObject( const Common::MsgPlannedWork& asn )
    : type_              ( asn.type() )
    , location_          ( asn.position() )
    , typeObstacle_      ( asn.type_obstacle() )
    , density_           ( asn.densite() )
    , tc2_               ( asn.tc2() )
    , delaiActiviteMines_( asn.activity_time() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GenObject constructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
GenObject::GenObject()
    : type_              ( "" )
    , location_          ()
    , typeObstacle_      ( Common::ObstacleType_DemolitionTargetType_preliminary )
    , density_           ( 0. )
    , tc2_               ( 0 )
    , delaiActiviteMines_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GenObject destructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
GenObject::~GenObject()
{
    // NOTHING
}

 // -----------------------------------------------------------------------------
// Name: GenObject::Send
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
void GenObject::Send( Common::MsgPlannedWork& message ) const
{
    message.set_type( type_ );
    message.set_type_obstacle( typeObstacle_ );
    message.set_densite( density_ );
    message.set_tc2( tc2_ );
    message.set_activity_time( delaiActiviteMines_ );
    location_.Send( *message.mutable_position() );
}
