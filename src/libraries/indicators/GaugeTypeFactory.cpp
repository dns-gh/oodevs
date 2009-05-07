// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "GaugeTypeFactory.h"
#include "IconsGaugeType.h"
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: GaugeTypeFactory constructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
GaugeTypeFactory::GaugeTypeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GaugeTypeFactory destructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
GaugeTypeFactory::~GaugeTypeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GaugeTypeFactory::Create
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
GaugeType* GaugeTypeFactory::Create( xml::xistream& xis )
{
    const std::string type = xml::attribute< std::string >( xis, "type" );
    if( type == "text" )
        return new GaugeType( xis );
    else if( type == "icons" )
        return new IconsGaugeType( xis );
    return 0;
}
