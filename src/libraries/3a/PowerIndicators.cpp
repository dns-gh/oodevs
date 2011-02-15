// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "PowerIndicators.h"
#include "PowerIndicator.h"
#include "tools/PhysicalFileLoader.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace aar;

// -----------------------------------------------------------------------------
// Name: PowerIndicators constructor
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
PowerIndicators::PowerIndicators()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PowerIndicators constructor
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
PowerIndicators::PowerIndicators( const tools::ExerciseConfig& config )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: PowerIndicators destructor
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
PowerIndicators::~PowerIndicators()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: PowerIndicators::Load
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
void PowerIndicators::Load( const tools::ExerciseConfig& config )
{
    Purge();
    std::string invalidSignatureFiles;
    std::string missingSignatureFiles;
    Load( config, invalidSignatureFiles, missingSignatureFiles );
}

// -----------------------------------------------------------------------------
// Name: PowerIndicators::Load
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
void PowerIndicators::Load( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles, std::string& missingSignatureFiles )
{
    tools::PhysicalFileLoader( config, invalidSignatureFiles, missingSignatureFiles )
        .Load( "units", boost::bind( &PowerIndicators::ReadUnit, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: PowerIndicators::Purge
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
void PowerIndicators::Purge()
{
    tools::Resolver< PowerIndicator >::DeleteAll();
    tools::Resolver< PowerIndicator, std::string >::Clear();
}

// -----------------------------------------------------------------------------
// Name: PowerIndicators::ReadUnit
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
void PowerIndicators::ReadUnit( xml::xistream& xis )
{
    xis >> xml::start( "units" )
        >> xml::list( "unit", *this, &PowerIndicators::ReadPowerIndicator );
}

// -----------------------------------------------------------------------------
// Name: PowerIndicators::ReadPowerIndicator
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
void PowerIndicators::ReadPowerIndicator( xml::xistream& xis )
{
    PowerIndicator* powerIndicator = new PowerIndicator( xis );
    tools::Resolver< PowerIndicator >::Register( powerIndicator->GetId(), *powerIndicator );
    tools::Resolver< PowerIndicator, std::string >::Register( powerIndicator->GetName(), *powerIndicator );
}
