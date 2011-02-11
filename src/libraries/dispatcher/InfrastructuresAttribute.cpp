// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "InfrastructuresAttribute.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: InfrastructuresAttribute constructor
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
InfrastructuresAttribute::InfrastructuresAttribute( const sword::UrbanAttributes& message )
    : enabled_  ( true )
    , threshold_( 0.3f )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: InfrastructuresAttribute destructor
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
InfrastructuresAttribute::~InfrastructuresAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfrastructuresAttribute::Update
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void InfrastructuresAttribute::Update( const sword::UrbanAttributes& message )
{
    if( !message.has_infrastructures() || !message.infrastructures().has_infrastructure() )
        return;
    enabled_ = message.infrastructures().infrastructure().active();
    threshold_ = message.infrastructures().infrastructure().threshold();
    role_ = message.infrastructures().infrastructure().type();
}

// -----------------------------------------------------------------------------
// Name: InfrastructuresAttribute::Send
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void InfrastructuresAttribute::Send( sword::UrbanAttributes& message ) const
{
    message.mutable_infrastructures()->mutable_infrastructure()->set_active( enabled_ );
    message.mutable_infrastructures()->mutable_infrastructure()->set_threshold( threshold_ );
    message.mutable_infrastructures()->mutable_infrastructure()->set_type( role_ );
}
