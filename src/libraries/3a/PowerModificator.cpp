// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "PowerModificator.h"
#include "tools/ExerciseConfig.h"
#include "tools/PhyLoader.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace aar;

// -----------------------------------------------------------------------------
// Name: PowerModificator constructor
// Created: FPO 2011-04-04
// -----------------------------------------------------------------------------
PowerModificator::PowerModificator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PowerModificator constructor
// Created: FPO 2011-04-04
// -----------------------------------------------------------------------------
PowerModificator::PowerModificator( const tools::ExerciseConfig& config )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: PowerModificator destructor
// Created: FPO 2011-04-04
// -----------------------------------------------------------------------------
PowerModificator::~PowerModificator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PowerModificator::Load
// Created: FPO 2011-04-04
// -----------------------------------------------------------------------------
void PowerModificator::Load( const tools::ExerciseConfig& config )
{
    config.GetPhyLoader().LoadPhysicalFile( "decisional", boost::bind( &PowerModificator::ReadPowerModificator, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: PowerModificator::ReadPowerModificator
// Created: FPO 2011-04-04
// -----------------------------------------------------------------------------
void PowerModificator::ReadPowerModificator( xml::xistream& xis )
{
    xis >> xml::start( "decisional" )
            >> xml::start( "wound-effects-modificators" )
            >> xml::attribute( "available", available_ )
            >> xml::attribute( "unavailable", unavailable_ )
            >> xml::attribute( "repairable", repairable_ )
            >> xml::attribute( "repairing", repairing_ )
            >> xml::attribute( "captured", captured_ )
        >> xml::end
      >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PowerModificator::GetAvailableModificator
// Created: FPO 2011-04-04
// -----------------------------------------------------------------------------
double PowerModificator::GetAvailableModificator() const
{
    return available_;
}

// -----------------------------------------------------------------------------
// Name: PowerModificator::GetRepairableModificator
// Created: FPO 2011-04-04
// -----------------------------------------------------------------------------
double PowerModificator::GetRepairableModificator() const
{
    return repairable_;
}

// -----------------------------------------------------------------------------
// Name: PowerModificator::GetRepairingModificator
// Created: FPO 2011-04-04
// -----------------------------------------------------------------------------
double PowerModificator::GetRepairingModificator() const
{
    return repairing_;
}

// -----------------------------------------------------------------------------
// Name: PowerModificator::GetCapturedModificator
// Created: FPO 2011-04-04
// -----------------------------------------------------------------------------
double PowerModificator::GetCapturedModificator() const
{
    return captured_;
}

// -----------------------------------------------------------------------------
// Name: PowerModificator::GetUnavailableModificator
// Created: FPO 2011-04-04
// -----------------------------------------------------------------------------
double PowerModificator::GetUnavailableModificator() const
{
    return unavailable_;
}
