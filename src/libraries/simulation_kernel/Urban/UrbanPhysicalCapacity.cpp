// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanPhysicalCapacity.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( UrbanPhysicalCapacity )

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity constructor
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
UrbanPhysicalCapacity::UrbanPhysicalCapacity()
    : height_( 20 )
    , floorNumber_( 6 )
    , occupation_( 0.5f )
    , trafficability_( 0.5f )
    , parkingFloors_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity constructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
UrbanPhysicalCapacity::UrbanPhysicalCapacity( xml::xistream& xis )
    : height_( 20 )
    , floorNumber_( 6 )
    , occupation_( 0.5f )
    , trafficability_( 0.5f )
    , parkingFloors_( 0 )
{
    xis >> xml::optional >> xml::start( "physical" )
            >> xml::list( *this, &UrbanPhysicalCapacity::ReadXml )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity constructor
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
UrbanPhysicalCapacity::UrbanPhysicalCapacity( const UrbanPhysicalCapacity& from )
    : height_( from.height_ )
    , floorNumber_( from.floorNumber_ )
    , occupation_( from.occupation_ )
    , trafficability_( from.trafficability_ )
    , parkingFloors_( from.parkingFloors_ )
    , roofShape_( from.roofShape_ )
    , material_( from.material_ )
    , motivations_( from.motivations_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity destructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
UrbanPhysicalCapacity::~UrbanPhysicalCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::Register
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
void UrbanPhysicalCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::Instanciate
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
void UrbanPhysicalCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    UrbanPhysicalCapacity* capacity = new UrbanPhysicalCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::SendFullState
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
void UrbanPhysicalCapacity::SendFullState( sword::UrbanAttributes& message ) const
{
    sword::UrbanAttributes_Architecture& msg = *message.mutable_architecture();
    msg.set_height( static_cast< float >( height_ ) );
    msg.set_floor_number( floorNumber_ );
    msg.set_roof_shape( roofShape_.c_str() );
    msg.set_material( material_.c_str() );
    msg.set_occupation( static_cast< float >( occupation_ ) );
    msg.set_trafficability( static_cast< float >( trafficability_ ) );
    msg.set_parking_floors( parkingFloors_ );
    for( auto it = motivations_.begin(); it != motivations_.end(); ++it )
    {
        sword::UrbanUsage& usage = *message.add_usages();
        usage.set_role( it->first );
        usage.set_percentage( static_cast< unsigned int >( it->second * 100 + 0.5f ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::GetHeight
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
unsigned int UrbanPhysicalCapacity::GetHeight() const
{
    return height_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::GetFloorNumber
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
unsigned int UrbanPhysicalCapacity::GetFloorNumber() const
{
    return floorNumber_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::GetOccupation
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
double UrbanPhysicalCapacity::GetOccupation() const
{
    return occupation_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::GetTrafficability
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
double UrbanPhysicalCapacity::GetTrafficability() const
{
    return trafficability_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::GetMaterial
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
const std::string& UrbanPhysicalCapacity::GetMaterial() const
{
    return material_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::GetRoofShape
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
const std::string& UrbanPhysicalCapacity::GetRoofShape() const
{
    return roofShape_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::GetMotivations
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
const std::map< std::string, float >& UrbanPhysicalCapacity::GetMotivations() const
{
    return motivations_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::serialize
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
template< typename Archive >
void UrbanPhysicalCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & height_;
    file & floorNumber_;
    file & occupation_;
    file & trafficability_;
    file & parkingFloors_;
    file & roofShape_;
    file & material_;
    file & motivations_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::WriteUrban
// Created: NPT 2012-09-11
// -----------------------------------------------------------------------------
void UrbanPhysicalCapacity::WriteUrban( xml::xostream& xos ) const
{
    xos << xml::start( "physical" )
            << xml::start( "architecture" )
                << xml::attribute( "height", height_ )
                << xml::attribute( "floor-number", floorNumber_ )
                << xml::attribute( "roof-shape", roofShape_ )
                << xml::attribute( "material", material_ )
                << xml::attribute( "occupation", occupation_ )
                << xml::attribute( "trafficability", trafficability_ )
                << xml::attribute( "parking-floors", parkingFloors_ )
            << xml::end
            << xml::start( "usages" );
            for( auto it = motivations_.begin(); it != motivations_.end(); ++it )
            {
                xos << xml::start( "usage" )
                        << xml::attribute( "type", it->first )
                        << xml::attribute( "proportion", it->second )
                    << xml::end;
            }
   xos      << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::ReadXml
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void UrbanPhysicalCapacity::ReadXml( const std::string& attribute, xml::xistream& xis )
{
    if( attribute == "architecture" )
    {
        xis >> xml::optional >> xml::attribute( "height", height_ )
            >> xml::optional >> xml::attribute( "floor-number", floorNumber_ )
            >> xml::optional >> xml::attribute( "roof-shape", roofShape_ )
            >> xml::optional >> xml::attribute( "material", material_ )
            >> xml::optional >> xml::attribute( "occupation", occupation_ )
            >> xml::optional >> xml::attribute( "trafficability", trafficability_ )
            >> xml::optional >> xml::attribute( "parking-floors", parkingFloors_ );
    }
    else if( attribute == "usages" )
        xis >> xml::list( "usage", *this, &UrbanPhysicalCapacity::ReadMotivation );
}

// -----------------------------------------------------------------------------
// Name: UrbanPhysicalCapacity::ReadMotivation
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void UrbanPhysicalCapacity::ReadMotivation( xml::xistream& xis )
{
     motivations_[ xis.attribute< std::string >( "type" ) ] = xis.attribute< float >( "proportion" );
}
