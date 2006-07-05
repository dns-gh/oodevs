// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "FireResultFactory.h"
#include "AgentFireResult.h"
#include "PopulationFireResult.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectTypes.h"

// -----------------------------------------------------------------------------
// Name: FireResultFactory constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultFactory::FireResultFactory( Model& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireResultFactory destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultFactory::~FireResultFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireResultFactory::CreateFireResult
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
AgentFireResult* FireResultFactory::CreateFireResult( const ASN1T_FireDamagesPion& message )
{
    return new AgentFireResult( message, model_.agents_, model_.objectTypes_ );
}

// -----------------------------------------------------------------------------
// Name: FireResultFactory::CreateFireResult
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
PopulationFireResult* FireResultFactory::CreateFireResult( const ASN1T_FireDamagesPopulation& message )
{
    return new PopulationFireResult( message, model_.agents_ );
}
