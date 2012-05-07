// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Architecture.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Architecture::Architecture( xml::xistream& xis, kernel::PropertiesDictionary& dictionary )
    : gui::Architecture( dictionary )
{
    xis >> xml::optional
            >> xml::start( "physical" );
        if( xis.has_child( "architecture" ) )
        {
            unsigned int height, floorNumber, parkingFloors;
            std::string roofShape, material;
            float occupation, trafficability;
            xis >> xml::start( "architecture" )
                    >> xml::optional >> xml::attribute( "height", height )
                    >> xml::optional >> xml::attribute( "floor-number", floorNumber )
                    >> xml::optional >> xml::attribute( "roof-shape", roofShape )
                    >> xml::optional >> xml::attribute( "material", material )
                    >> xml::optional >> xml::attribute( "occupation", occupation )
                    >> xml::optional >> xml::attribute( "trafficability", trafficability )
                    >> xml::optional >> xml::attribute( "parking-floors", parkingFloors )
                >> xml::end;
            Initialize( height, floorNumber, parkingFloors, roofShape, material, occupation, trafficability );
        }
    xis     >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Architecture destructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Architecture::~Architecture()
{
    // NOTHING
}
