// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FiresModel.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FiresModel constructor
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
FiresModel::FiresModel( const tools::Resolver_ABC< Agent_ABC >& agents, const tools::Resolver_ABC< Population_ABC >& populations )
    : agents_( agents )
    , populations_( populations )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FiresModel destructor
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
FiresModel::~FiresModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FiresModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void FiresModel::Purge()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: FiresModel::AddFire
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void FiresModel::AddFire( const sword::StartUnitFire& message )
{
    if( ! Find( message.fire().id() ) )
        Register( message.fire().id(), agents_.Get( message.firing_unit().id() ) );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::AddFire
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void FiresModel::AddFire( const sword::StartCrowdFire& message )
{
    if( ! Find( message.fire().id() ) )
        Register( message.fire().id(), populations_.Get( message.firing_crowd().id() ) );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::FindFirer
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
Entity_ABC* FiresModel::FindFirer( const sword::StopUnitFire& message )
{
    return Find( message.fire().id() );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::FindFirer
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
Entity_ABC* FiresModel::FindFirer( const sword::StopCrowdFire& message )
{
    return Find( message.fire().id() );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::RemoveFire
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void FiresModel::RemoveFire( const sword::StopUnitFire& message )
{
    Remove( message.fire().id() );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::RemoveFire
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void FiresModel::RemoveFire( const sword::StopCrowdFire& message )
{
    Remove( message.fire().id() );
}
