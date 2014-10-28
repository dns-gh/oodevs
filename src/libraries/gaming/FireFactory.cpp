// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentsModel.h"
#include "DirectFire.h"
#include "FireFactory.h"
#include "IndirectFire.h"
#include "Model.h"
#include "PopulationFire.h"
#include "StaticModel.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: FireFactory constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireFactory::FireFactory( Model& model, kernel::Controller& controller, const kernel::Profile_ABC& profile )
    : model_( model )
    , controller_( controller )
    , profile_( profile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireFactory destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireFactory::~FireFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireFactory::CreateFire
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Fire_ABC* FireFactory::CreateFire( const sword::StartUnitFire& message, unsigned long id )
{
    if( message.target().has_unit() || message.target().has_crowd() )
        return new DirectFire( message, controller_, profile_, model_.agents_, model_.agents_, id );
    if( message.target().has_position() )
        return new IndirectFire( message, model_.agents_, model_.static_ );
    throw MASA_EXCEPTION( "Invalid target type" );
}

// -----------------------------------------------------------------------------
// Name: FireFactory::CreateFire
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Fire_ABC* FireFactory::CreateFire( const sword::StartCrowdFire& message, unsigned long /*id*/ )
{
    return new PopulationFire( message, model_.agents_ );
}

Fire_ABC* FireFactory::CreateFire( const sword::StartUnitFireDetection& message, unsigned long /*id*/ )
{
    return new IndirectFire( message, model_.agents_, model_.static_.coordinateConverter_ );
}
