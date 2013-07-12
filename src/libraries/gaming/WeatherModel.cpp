// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "WeatherModel.h"
#include "AmmoEffect.h"
#include "Model.h"
#include "StaticModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeatherModel constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
WeatherModel::WeatherModel( Controller& controller, Model& model, kernel::Profile_ABC& profile, const Simulation& simulation )
     : controller_( controller )
     , model_     ( model )
     , profile_   ( profile )
     , simulation_( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherModel destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
WeatherModel::~WeatherModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void WeatherModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::CreateAmmoEffect
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void WeatherModel::CreateAmmoEffect( const sword::StartFireEffect& message )
{
    Register( message.fire_effect().id(), *new AmmoEffect( message, controller_, model_.static_.coordinateConverter_, model_.GetAgentResolver(), profile_, simulation_ ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::DeleteAmmoEffect
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void WeatherModel::DeleteAmmoEffect( const sword::StopFireEffect& message )
{
    AmmoEffect* effect = Find( message.fire_effect().id() );
    Remove( message.fire_effect().id() );
    delete effect;
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::UpdateFireEffectPerception
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
void WeatherModel::UpdateFireEffectPerception( const sword::IndirectFirePerception& message )
{
    for( int i = 0; i < message.fire_effects_size(); ++i )
    {
        AmmoEffect* effect = Find( message.fire_effects( i ).id() );
        if( effect )
            effect->SetPerceivers( message.perceivers() );
    }
}
