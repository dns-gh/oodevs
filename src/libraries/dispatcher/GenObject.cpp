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
GenObject::GenObject( const sword::PlannedWork& msg )
    : type_              ( msg.type() )
    , location_          ( msg.position() )
    , typeObstacle_      ( msg.type_obstacle() )
    , density_           ( msg.density() )
    , tc2_               ( msg.combat_train().id() )
    , delaiActiviteMines_( msg.activity_time() )
    , fireClass_         ( msg.fire_class() )
    , maxCombustion_     ( msg.max_combustion() )
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
    , typeObstacle_      ( sword::ObstacleType_DemolitionTargetType_preliminary )
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
void GenObject::Send( sword::PlannedWork& message ) const
{
    message.set_type( type_ );
    message.set_type_obstacle( typeObstacle_ );
    message.set_density( density_ );
    message.mutable_combat_train()->set_id( tc2_ );
    message.set_activity_time( delaiActiviteMines_ );
    message.set_fire_class( fireClass_ );
    message.set_max_combustion( maxCombustion_ );
    location_.Send( *message.mutable_position() );
}
