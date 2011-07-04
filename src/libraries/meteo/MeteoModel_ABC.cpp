// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "MeteoModel_ABC.h"
#include "meteo/Meteo.h"
#include "meteo/PHY_Lighting.h"
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
}

// -----------------------------------------------------------------------------
// Name: MeteoModel_ABC::Purge
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
void MeteoModel_ABC::Purge()
{
    globalMeteo_.reset();
    meteos_.clear();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel_ABC::AddMeteo
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
void MeteoModel_ABC::AddMeteo( weather::Meteo& element )
{
    if( ! meteos_.insert( boost::shared_ptr< weather::Meteo >( &element ) ).second )
        throw std::runtime_error( __FUNCTION__ "Insert failed" );
}
