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
    targets_.Clear();
}

// -----------------------------------------------------------------------------
// Name: FiresModel::AddFire
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void FiresModel::AddFire( const sword::StartUnitFire& message )
{
    if( !Find( message.fire().id() ) )
        Register( message.fire().id(), agents_.Get( message.firing_unit().id() ) );
    AddTarget( message );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::AddFire
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void FiresModel::AddFire( const sword::StartCrowdFire& message )
{
    if( !Find( message.fire().id() ) )
        Register( message.fire().id(), populations_.Get( message.firing_crowd().id() ) );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::AddTarget
// Created: ABR 2011-02-16
// -----------------------------------------------------------------------------
void FiresModel::AddTarget( const sword::StartUnitFire& message )
{
    if( !targets_.Find( message.fire().id() ) && !message.target().has_position() )
    {
        if( message.target().has_unit() )
            targets_.Register( message.fire().id(), agents_.Get( message.target().unit().id() ) );
        else if( message.target().has_crowd() )
            targets_.Register( message.fire().id(), populations_.Get( message.target().crowd().id() ) );
    }
}
