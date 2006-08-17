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
#include "clients_kernel/Controllers.h"
#include "Model.h"
#include "StaticModel.h"
#include "CoordinateConverter.h"

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
void WeatherModel::CreateAmmoEffect( const ASN1T_MsgStartFireEffect& message )
{
    Register( message.oid_effet, *new AmmoEffect( message, controllers_.controller_, model_.static_.coordinateConverter_ ) );
}
    
// -----------------------------------------------------------------------------
// Name: WeatherModel::DeleteAmmoEffect
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void WeatherModel::DeleteAmmoEffect( const ASN1T_MsgStopFireEffect& message )
{
    AmmoEffect* effect = Find( message );
    Remove( message );
    delete effect;
}
