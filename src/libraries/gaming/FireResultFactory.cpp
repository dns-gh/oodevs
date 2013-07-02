// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FireResultFactory.h"
#include "AgentFireResult.h"
#include "AgentsModel.h"
#include "FiresModel.h"
#include "Model.h"
#include "PopulationFireResult.h"
#include "Simulation.h"
#include "StaticModel.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"

// -----------------------------------------------------------------------------
// Name: FireResultFactory constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultFactory::FireResultFactory( Model& model, const Simulation& simulation, const kernel::Profile_ABC& profile )
    : model_( model )
    , simulation_( simulation )
    , profile_( profile )
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
// Name: FireResultFactory::GetObject
// Created: JSR 2012-12-07
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* FireResultFactory::GetFirer( const sword::Explosion& message ) const
{
    return model_.FindObject( message.object().id() );
}

// -----------------------------------------------------------------------------
// Name: FireResultFactory::GetFirer
// Created: JSR 2012-12-07
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* FireResultFactory::GetFirer( const sword::StopUnitFire& message ) const
{
    return model_.fires_.FindFirer( message );
}

// -----------------------------------------------------------------------------
// Name: FireResultFactory::GetFirer
// Created: JSR 2012-12-07
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* FireResultFactory::GetFirer( const sword::StopCrowdFire& message ) const
{
    return model_.fires_.FindFirer( message );
}

// -----------------------------------------------------------------------------
// Name: FireResultFactory::CreateFireResult
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
AgentFireResult* FireResultFactory::CreateFireResult( const sword::UnitFireDamages& message, const kernel::Entity_ABC* firer )
{
    const tools::Resolver<kernel::Agent_ABC,unsigned long>& agents = model_.agents_;
    if( profile_.IsPerceived( agents.Get( message.target().id() ) ) )
        return new AgentFireResult( message, model_.agents_, model_.static_.objectTypes_, simulation_.GetDateTime(), firer );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: FireResultFactory::CreateFireResult
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
PopulationFireResult* FireResultFactory::CreateFireResult( const sword::CrowdFireDamages& message, const kernel::Entity_ABC* firer )
{
    const tools::Resolver<kernel::Population_ABC,unsigned long>& crowds = model_.agents_;
    if( profile_.IsPerceived( crowds.Get( message.target().id() ) ) )
        return new PopulationFireResult( message, model_.agents_, simulation_.GetDateTime(), firer );
    return 0;
}