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
// Created: ABR 2012-05-24
// -----------------------------------------------------------------------------
Architecture::Architecture( kernel::PropertiesDictionary& dictionary )
    : kernel::Architecture( dictionary )
{
    Initialize( 20.f, 6, 0, "default", "default", 0.5f, 0.5f ); // $$$$ ABR 2012-05-24: TODO Use RoofShapeType && MaterialCompositionType
}

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Architecture::Architecture( xml::xistream& xis, kernel::PropertiesDictionary& dictionary )
    : kernel::Architecture( dictionary )
{
    unsigned int height = 0;
    unsigned int floorNumber = 0;
    unsigned int parkingFloors = 0;
    std::string roofShape;
    std::string material;
    float occupation = 0.f;
    float trafficability = 0.f;
    // $$$$ _RC_ JSR 2011-05-25: virer les optional (après avoir vérifié les terrains)
    xis >> xml::start( "architecture" )
            >> xml::optional >> xml::attribute( "height", height )
            >> xml::optional >> xml::attribute( "floor-number", floorNumber )
            >> xml::optional >> xml::attribute( "roof-shape", roofShape )
            >> xml::optional >> xml::attribute( "material", material )
            >> xml::optional >> xml::attribute( "occupation", occupation )
            >> xml::optional >> xml::attribute( "trafficability", trafficability )
            >> xml::optional >> xml::attribute( "parking-floors", parkingFloors )
        >> xml::end;
    Initialize( static_cast< float >( height ), floorNumber, parkingFloors, roofShape, material, occupation, trafficability );
}

// -----------------------------------------------------------------------------
// Name: Architecture destructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Architecture::~Architecture()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Architecture::SerializeAttributes
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void Architecture::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "architecture" )
            << xml::attribute( "height", height_ )
            << xml::attribute( "floor-number", floorNumber_ )
            << xml::attribute( "roof-shape", roofShape_ )
            << xml::attribute( "material", material_ )
            << xml::attribute( "occupation", occupation_ / 100.f )
            << xml::attribute( "trafficability", trafficability_ );
    if( parkingFloors_ > 0 )
        xos << xml::attribute( "parking-floors", parkingFloors_ );
    xos << xml::end;
}
