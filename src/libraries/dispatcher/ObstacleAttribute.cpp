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

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , obstacle_ ( asnMsg.obstacle.type )
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
void ObstacleAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{
    if( asnMsg.m.obstaclePresent )
        obstacle_ = asnMsg.obstacle.type;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ObstacleAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    asnMsg.m.obstaclePresent = 1;
    asnMsg.obstacle.type = obstacle_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObstacleAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
    // NOTHING
}
