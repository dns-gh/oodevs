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
    if( !message.has_infrastructures() )
        return;
    // Infrastructures
    // TODO
}

// -----------------------------------------------------------------------------
// Name: InfrastructuresAttribute::Send
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void InfrastructuresAttribute::Send( sword::UrbanAttributes& /*message*/ ) const
{
    // TODO
}
