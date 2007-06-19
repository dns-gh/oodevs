// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionTime.h"
#include "Simulation.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionTime constructor
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
ActionTime::ActionTime( const Simulation& simulation )
    : simulation_( simulation )
    , startDay_( simulation.GetDay() )
    , startTime_( simulation.GetTime() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTime destructor
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
ActionTime::~ActionTime()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTime::SetStart
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void ActionTime::SetStart( xml::xistream& xis )
{
    std::string date;
    xis >> xml::attribute( "startdate", date );
    // $$$$ SBO 2007-06-19: 
}

// -----------------------------------------------------------------------------
// Name: ActionTime::Serialize
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void ActionTime::Serialize( xml::xostream& xos ) const
{
    
}
