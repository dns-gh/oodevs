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
#include "tools/PhyLoader.h"
#include "tools/ExerciseConfig.h"
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
    config.GetPhyLoader().LoadPhysicalFile( "components", boost::bind( &PowerIndicators::ReadEquipment, this, _1 ) );
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
// Name: PowerIndicators::ReadEquipment
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
void PowerIndicators::ReadEquipment( xml::xistream& xis )
{
    xis >> xml::start( "equipments" )
            >> xml::list( "equipment", *this, &PowerIndicators::ReadPowerIndicator )
        >> xml::end;
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
