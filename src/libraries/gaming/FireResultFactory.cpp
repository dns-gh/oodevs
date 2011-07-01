// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FireResultFactory.h"
#include "AgentFireResult.h"
#include "PopulationFireResult.h"
#include "Model.h"
#include "StaticModel.h"
#include "AgentsModel.h"
#include "Simulation.h"
#include "clients_kernel/ObjectTypes.h"

// -----------------------------------------------------------------------------
// Name: FireResultFactory constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultFactory::FireResultFactory( Model& model, const Simulation& simulation )
    : model_( model )
    , simulation_( simulation )
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
AgentFireResult* FireResultFactory::CreateFireResult( const sword::UnitFireDamages& message )
{
    return new AgentFireResult( message, model_.agents_, model_.static_.objectTypes_, simulation_.GetDateTime() );
}

// -----------------------------------------------------------------------------
// Name: FireResultFactory::CreateFireResult
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
PopulationFireResult* FireResultFactory::CreateFireResult( const sword::CrowdFireDamages& message )
{
    return new PopulationFireResult( message, model_.agents_, simulation_.GetDateTime() );
}
