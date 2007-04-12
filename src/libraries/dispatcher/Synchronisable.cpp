// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Synchronisable.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Synchronisable constructor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
Synchronisable::Synchronisable()
    : publisher_( 0 )
    , updated_  ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Synchronisable destructor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
Synchronisable::~Synchronisable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Synchronisable::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Synchronisable::StartSynchronisation( Publisher_ABC& publisher )
{
    publisher_ = &publisher;
    updated_  = false;
}

// -----------------------------------------------------------------------------
// Name: Synchronisable::EndSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Synchronisable::EndSynchronisation( Model& model )
{
    if( ! publisher_ )
        throw std::runtime_error( __FUNCTION__ );
    if( updated_ )
    {
        
        SendFullUpdate( *publisher_ );
        publisher_ = 0;
    }
    else
        model.FlagForDestruction( *this );
}

// -----------------------------------------------------------------------------
// Name: Synchronisable::FlagUpdate
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
bool Synchronisable::FlagUpdate()
{
    updated_ = true;
    return publisher_ != 0;
}

// -----------------------------------------------------------------------------
// Name: Synchronisable::CommitDestruction
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Synchronisable::CommitDestruction()
{
    throw std::runtime_error( __FUNCTION__ );
}


// -----------------------------------------------------------------------------
// Name: Synchronisable::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Synchronisable::StartSynchronisation( Synchronisable& next )
{
    if( publisher_ )
        next.StartSynchronisation( *publisher_ );
}
