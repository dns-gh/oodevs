// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MeteoModel_ABC.h"
#include "meteo/Meteo.h"

using namespace weather;

// -----------------------------------------------------------------------------
// Name: MeteoModel_ABC constructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
MeteoModel_ABC::MeteoModel_ABC( kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MeteoModel_ABC destructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
MeteoModel_ABC::~MeteoModel_ABC()
{
    // NOTHING
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
    meteos_.push_back( boost::shared_ptr< weather::Meteo >( &element ) );
}
