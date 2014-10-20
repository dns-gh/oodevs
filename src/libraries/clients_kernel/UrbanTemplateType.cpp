// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "UrbanTemplateType.h"
#include "Architecture_ABC.h"
#include "ObjectTypes.h"
#include "PhysicalAttribute_ABC.h"
#include "MaterialCompositionType.h"
#include "RoofShapeType.h"
#include "UrbanObject_ABC.h"
#include "Usages_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanTemplateType constructor
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
UrbanTemplateType::UrbanTemplateType( xml::xistream& xis, const ObjectTypes& types )
    : types_( types )
    , name_ ( xis.attribute< std::string >( "name" ) )
    , color_( 0, 0, 0, 0 )
{
    float alpha;
    xis >> xml::start( "architecture" )
        >> xml::attribute( "roof-shape", roofShape_ )
        >> xml::attribute( "material", material_ )
        >> xml::attribute( "trafficability", trafficability_ )
        >> xml::attribute( "parking-floors", parkingFloors_ )
        >> xml::attribute( "floor-number", floorNumber_ )
        >> xml::attribute( "occupation", occupation_ )
        >> xml::attribute( "height", height_ )
        >> xml::end
        >> xml::start( "color" )
        >> xml::attribute( "alpha", alpha )
        >> xml::attribute( "blue", color_.blue_ )
        >> xml::attribute( "green", color_.green_ )
        >> xml::attribute( "red", color_.red_ )
        >> xml::end
        >> xml::start( "usages" )
        >> xml::list( "usage", *this, &UrbanTemplateType::ReadUsage )
        >> xml::end;
    color_.alpha_ = static_cast< int >( 255 * alpha + 0.5f );
}

// -----------------------------------------------------------------------------
// Name: UrbanTemplateType destructor
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
UrbanTemplateType::~UrbanTemplateType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanTemplateType::GetName
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
const std::string& UrbanTemplateType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: UrbanTemplateType::ReadUsage
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void UrbanTemplateType::ReadUsage( xml::xistream& xis )
{
    usages_[ xis.attribute< std::string >( "type" ) ] = xis.attribute< float >( "proportion" );
}

// -----------------------------------------------------------------------------
// Name: UrbanTemplateType::Fill
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void UrbanTemplateType::Fill( kernel::UrbanObject_ABC& urbanObject ) const
{
    kernel::PhysicalAttribute_ABC& physical = urbanObject.Get< kernel::PhysicalAttribute_ABC >();

    // usages
    kernel::Usages_ABC& usages = physical.GetUsages();
    usages.ClearAll();
    for( auto it = usages_.begin(); it != usages_.end(); ++it )
        usages.Add( it->first, static_cast< unsigned int >( 100.f * it->second + 0.5f ) );

    // architecture
    kernel::Architecture_ABC& architecture = physical.GetArchitecture();
    MaterialCompositionType* material = types_.tools::StringResolver< MaterialCompositionType >::Find( material_ );
    if( material )
        architecture.SetMaterial( *material );
    RoofShapeType* roof = types_.tools::StringResolver< RoofShapeType >::Find( roofShape_ );
    if( roof )
        architecture.SetRoofShape( *roof );
    architecture.SetFloorNumber( floorNumber_ );
    architecture.SetParkingFloors( parkingFloors_ );
    architecture.SetOccupation( static_cast< unsigned int >( 100.f * occupation_ + 0.5f ) );
    architecture.SetHeight( height_ );
    architecture.SetTrafficability( trafficability_ );

    // Color
    kernel::UrbanColor_ABC& color = urbanObject.Get< kernel::UrbanColor_ABC >();
    color.SetColor( color_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanTemplateType::Matches
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
bool UrbanTemplateType::Matches( const kernel::UrbanObject_ABC& urbanObject ) const
{
    const kernel::PhysicalAttribute_ABC& physical = urbanObject.Get< kernel::PhysicalAttribute_ABC >();

    // architecture
    const kernel::Architecture_ABC& architecture = physical.GetArchitecture();
    if( architecture.GetMaterial().GetName() != material_ )
        return false;
    if( architecture.GetRoofShape().GetName() != roofShape_ )
        return false;
    if( architecture.GetFloorNumber() != floorNumber_ )
        return false;
    if( architecture.GetParkingFloors() != parkingFloors_ )
        return false;
    if( architecture.GetOccupation() != static_cast< unsigned int >( 100.f * occupation_ + 0.5f ) )
        return false;
    if( architecture.GetHeight() != height_ )
        return false;
    if( architecture.GetTrafficability() != trafficability_ )
        return false;

    // usages
    const kernel::T_Usages& usages = physical.GetUsages().GetUsages();
    for( auto it = usages_.begin(); it != usages_.end(); ++it )
    {
        auto found = usages.find( it->first );
        if( found == usages.end() || found->second != static_cast< unsigned int >( 100.f * it->second + 0.5f ) )
            return false;
    }

    // color
    const kernel::UrbanColor_ABC& color = urbanObject.Get< kernel::UrbanColor_ABC >();
    if( color.Red() != color_.red_ || color.Green() != color_.green_ || color.Blue() != color_.blue_ || std::abs( static_cast< int >( 255.f *color.Alpha() ) - color_.alpha_ ) > 1 )
        return false;

    return true;
}
