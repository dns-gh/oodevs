// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MeteoModel_ABC.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Meteo.h"
#include "meteo/PHY_Precipitation.h"

using namespace weather;

// -----------------------------------------------------------------------------
// Name: MeteoModel_ABC constructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
MeteoModel_ABC::MeteoModel_ABC( kernel::CoordinateConverter_ABC& converter )
    : converter_  ( converter )
    , globalMeteo_( 0 )
{
    weather::PHY_Precipitation::Initialize();
    weather::PHY_Lighting::Initialize();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel_ABC destructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
MeteoModel_ABC::~MeteoModel_ABC()
{
    weather::PHY_Lighting::Terminate();
    weather::PHY_Precipitation::Terminate();
    Purge();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel_ABC::GetLighting
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
const PHY_Lighting& MeteoModel_ABC::GetLighting() const
{
    assert( globalMeteo_.get() );
    return globalMeteo_->GetLighting();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel_ABC::Purge
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
void MeteoModel_ABC::Purge()
{
    meteos_.clear();
    globalMeteo_.reset();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel_ABC::RegisterMeteo
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
void MeteoModel_ABC::RegisterMeteo( boost::shared_ptr< weather::PHY_Meteo > element )
{
    meteos_.push_front( element );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel_ABC::UnregisterMeteo
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
void MeteoModel_ABC::UnregisterMeteo( boost::shared_ptr< weather::PHY_Meteo > element )
{
    element->SetListener( 0 );
    meteos_.remove( element );
}
