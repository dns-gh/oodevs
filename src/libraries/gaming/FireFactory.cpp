// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FireFactory.h"
#include "DirectFire.h"
#include "IndirectFire.h"
#include "Model.h"
#include "StaticModel.h"
#include "AgentsModel.h"
#include "PopulationFire.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

// -----------------------------------------------------------------------------
// Name: FireFactory constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireFactory::FireFactory( Model& model )
    : model_( model )
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
        return new DirectFire( message, model_.agents_, model_.agents_, id );
    if( message.target().has_position() )
        return new IndirectFire( message, model_.agents_, model_.static_.coordinateConverter_ );
    throw std::runtime_error( "Invalid target type" );
}

// -----------------------------------------------------------------------------
// Name: FireFactory::CreateFire
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Fire_ABC* FireFactory::CreateFire( const sword::StartCrowdFire& message, unsigned long /*id*/ )
{
    return new PopulationFire( message, model_.agents_ );
}
