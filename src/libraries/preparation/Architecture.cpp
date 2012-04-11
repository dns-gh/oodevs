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
Architecture::Architecture( xml::xistream& xis, std::auto_ptr< kernel::Architecture_ABC > pArchitecture )
    : pArchitecture_( pArchitecture )
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

// -----------------------------------------------------------------------------
// Name: Architecture::Initialize
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
void Architecture::Initialize( unsigned int height, unsigned int floorNumber, unsigned int parkingFloors, const std::string& roofShape,
                               const std::string& material, float occupation, float trafficability )
{
    pArchitecture_->Initialize( height, floorNumber, parkingFloors, roofShape, material, occupation, trafficability );
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetMaterial
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
const std::string& Architecture::GetMaterial() const
{
    return pArchitecture_->GetMaterial();
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetFloorNumber
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned int Architecture::GetFloorNumber() const
{
    return pArchitecture_->GetFloorNumber();
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetOccupation
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned int Architecture::GetOccupation() const
{
    return pArchitecture_->GetOccupation();
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetHeight
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
unsigned int Architecture::GetHeight() const
{
    return pArchitecture_->GetHeight();
}
