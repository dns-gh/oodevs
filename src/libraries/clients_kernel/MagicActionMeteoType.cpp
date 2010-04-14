// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MagicActionMeteoType.h"
#include "OrderParameter.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MagicActionMeteoType constructor
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
MagicActionMeteoType::MagicActionMeteoType()
    : MagicActionType()
{
    name_ = "global_meteo";
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionMeteoType destructor
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
MagicActionMeteoType::~MagicActionMeteoType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicActionMeteoType::Initialize
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
void MagicActionMeteoType::Initialize()
{
    Register( Count(), *new OrderParameter( "Temperature", "numeric", false ) );
    Register( Count(), *new OrderParameter( "WindSpeed", "numeric", false ) );
    Register( Count(), *new OrderParameter( "WindDirection", "direction", false ) );
    Register( Count(), *new OrderParameter( "CloudFloor", "numeric", false ) );
    Register( Count(), *new OrderParameter( "CloudCeiling", "numeric", false ) );
    Register( Count(), *new OrderParameter( "CloudDensity", "numeric", false ) );

    OrderParameter* precipitation = new OrderParameter( "Precipitation", "enumeration", false );
    precipitation->AddValue( 0, "PasDePrecipitation" );
    precipitation->AddValue( 1, "TempeteDeSable"     );
    precipitation->AddValue( 2, "Brouillard"         );
    precipitation->AddValue( 3, "Crachin"            );
    precipitation->AddValue( 4, "Pluie"              );
    precipitation->AddValue( 5, "Neige"              );
    precipitation->AddValue( 6, "Fumigene"           );

    Register( Count(), *precipitation );
}
