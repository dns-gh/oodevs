// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "FireFactory.h"
#include "DirectFire.h"
#include "IndirectFire.h"
#include "Model.h"
#include "AgentsModel.h"
#include "PopulationFire.h"

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
Fire_ABC* FireFactory::CreateFire( const ASN1T_MsgStartPionFire& message )
{
    switch( message.cible.t )
    {
    case T_MsgStartPionFire_cible_pion:
    case T_MsgStartPionFire_cible_population:
        return new DirectFire( message, model_.agents_, model_.agents_ );
    case T_MsgStartPionFire_cible_position:
        return new IndirectFire( message, model_.agents_ );
    default: 
        throw std::runtime_error( "Invalid target type" );

    };
}

// -----------------------------------------------------------------------------
// Name: FireFactory::CreateFire
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Fire_ABC* FireFactory::CreateFire( const ASN1T_MsgStartPopulationFire& message )
{
    return new PopulationFire( message, model_.agents_ );
}
