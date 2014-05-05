// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_pch.h"
#include "PlatformDelegateFactory.h"
#include "PlatformDelegate.h"
#include "dispatcher/Agent.h"

using namespace tic;

// -----------------------------------------------------------------------------
// Name: PlatformDelegateFactory constructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
PlatformDelegateFactory::PlatformDelegateFactory( const kernel::CoordinateConverter_ABC& converter, float timeStep )
    : converter_( converter )
    , timeStep_ ( timeStep )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegateFactory destructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
PlatformDelegateFactory::~PlatformDelegateFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PlatformDelegateFactory::Create
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
std::unique_ptr< PlatformDelegate_ABC > PlatformDelegateFactory::Create( dispatcher::Agent_ABC& entity ) const
{
    return std::unique_ptr< PlatformDelegate_ABC >( new PlatformDelegate( entity, converter_, timeStep_ ) );
}
