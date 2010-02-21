// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ObstacleAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

//using namespace Common;

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , obstacle_ ( asnMsg.obstacle().type() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ObstacleAttribute::~ObstacleAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void ObstacleAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if( asnMsg.has_obstacle() )
        obstacle_ = asnMsg.obstacle().type();
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ObstacleAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_obstacle()->set_type ( obstacle_ );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Delete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObstacleAttribute::Delete( Common::MsgObjectAttributes& /*asnMsg*/ ) const
{
    // NOTHING
}
