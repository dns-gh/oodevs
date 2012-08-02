// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanPhysicalAttribute.h"
#include "MIL_UrbanMotivationsVisitor_ABC.h"

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalAttribute constructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
UrbanPhysicalAttribute::UrbanPhysicalAttribute( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "physical" )
            >> xml::list( *this, &UrbanPhysicalAttribute::ReadXml )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalAttribute destructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
UrbanPhysicalAttribute::~UrbanPhysicalAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalAttribute::GetArchitecture
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
const UrbanPhysicalAttribute::Architecture& UrbanPhysicalAttribute::GetArchitecture() const
{
    return architecture_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalAttribute::Accept
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void UrbanPhysicalAttribute::Accept( MIL_UrbanMotivationsVisitor_ABC& visitor ) const
{
    for( std::map< std::string, float >::const_iterator it = motivations_.begin(); it != motivations_.end(); ++it )
        visitor.Visit( it->first, it->second );
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalAttribute::GetPathfindCost
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
double UrbanPhysicalAttribute::GetPathfindCost( double weight ) const
{
    if( weight > architecture_.trafficability_ )
        return -1.f;
    return architecture_.occupation_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalAttribute::ReadXml
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void UrbanPhysicalAttribute::ReadXml( const std::string& attribute, xml::xistream& xis )
{
    if( attribute == "architecture" )
    {
        xis >> xml::optional >> xml::attribute( "height", architecture_.height_ )
            >> xml::optional >> xml::attribute( "floor-number", architecture_.floorNumber_ )
            >> xml::optional >> xml::attribute( "roof-shape", architecture_.roofShape_ )
            >> xml::optional >> xml::attribute( "material", architecture_.material_ )
            >> xml::optional >> xml::attribute( "occupation", architecture_.occupation_ )
            >> xml::optional >> xml::attribute( "trafficability", architecture_.trafficability_ )
            >> xml::optional >> xml::attribute( "parking-floors", architecture_.parkingFloors_ );
    }
    else if( attribute == "usages" )
        xis >> xml::list( "usage", *this, &UrbanPhysicalAttribute::ReadMotivation );
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalAttribute::ReadMotivation
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void UrbanPhysicalAttribute::ReadMotivation( xml::xistream& xis )
{
     motivations_[ xis.attribute< std::string >( "type" ) ] = xis.attribute< float >( "proportion" );
}
