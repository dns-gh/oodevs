// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "WeatherModel.h"
#include "AmmoEffect.h"
#include "clients_kernel/Controllers.h"
#include "Model.h"
#include "StaticModel.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeatherModel constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
WeatherModel::WeatherModel( Controllers& controllers, Model& model )
     : controllers_( controllers )
     , model_( model )
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
void WeatherModel::CreateAmmoEffect( const MsgsSimToClient::MsgStartFireEffect& message )
{
    Register( message.effect_oid(), *new AmmoEffect( message, controllers_.controller_, model_.static_.coordinateConverter_ ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::DeleteAmmoEffect
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void WeatherModel::DeleteAmmoEffect( const MsgsSimToClient::MsgStopFireEffect& message )
{
    AmmoEffect* effect = Find( message.oid() );
    Remove( message.oid() );
    delete effect;
}
