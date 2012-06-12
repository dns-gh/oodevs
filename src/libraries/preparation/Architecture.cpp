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
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/MaterialCompositionType.h"
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/RoofShapeType.h"
#include "ENT/ENT_Enums_Gen.h"

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: ABR 2012-05-24
// -----------------------------------------------------------------------------
Architecture::Architecture( const kernel::Entity_ABC* parent, kernel::Controllers& controllers, kernel::PropertiesDictionary& dictionary, const kernel::ObjectTypes& objectTypes )
    : kernel::Architecture( dictionary )
    , controllers_( controllers )
{
    if( parent )
    {
        const kernel::PhysicalAttribute_ABC& parentPhysicalAttribute = parent->Get< kernel::PhysicalAttribute_ABC >();
        const kernel::Architecture_ABC& parentArchitecture = parentPhysicalAttribute.GetArchitecture();
        Initialize( objectTypes,
                    parentArchitecture.GetHeight(),
                    parentArchitecture.GetFloorNumber(),
                    parentArchitecture.GetParkingFloors(),
                    static_cast< float >( parentArchitecture.GetOccupation() ) / 100.f,
                    parentArchitecture.GetTrafficability(),
                    parentArchitecture.GetMaterial().GetName(),
                    parentArchitecture.GetRoofShape().GetName() );
    }
    else
        Initialize( objectTypes, 20, 6, 0, 0.5f, 0.5f );

    assert( controllers_.modes_ );
    controllers_.modes_->Register( *this );
    NotifyModeChanged( controllers_.modes_->GetCurrentMode() );
}

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Architecture::Architecture( kernel::Controllers& controllers, xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::ObjectTypes& objectTypes )
    : kernel::Architecture( dictionary )
    , controllers_( controllers )
{
    unsigned int height = 0;
    unsigned int floorNumber = 0;
    unsigned int parkingFloors = 0;
    std::string roofShape;
    std::string material;
    float occupation = 0.f;
    float trafficability = 0.f;
    // $$$$ _RC_ JSR 2011-05-25: virer les optional (apr�s avoir v�rifi� les terrains)
    xis >> xml::start( "architecture" )
            >> xml::optional >> xml::attribute( "height", height )
            >> xml::optional >> xml::attribute( "floor-number", floorNumber )
            >> xml::optional >> xml::attribute( "roof-shape", roofShape )
            >> xml::optional >> xml::attribute( "material", material )
            >> xml::optional >> xml::attribute( "occupation", occupation )
            >> xml::optional >> xml::attribute( "trafficability", trafficability )
            >> xml::optional >> xml::attribute( "parking-floors", parkingFloors )
        >> xml::end;
    Initialize( objectTypes, height, floorNumber, parkingFloors, occupation, trafficability, material, roofShape );
    assert( controllers_.modes_ );
    controllers_.modes_->Register( *this );
    NotifyModeChanged( controllers_.modes_->GetCurrentMode() );
}

// -----------------------------------------------------------------------------
// Name: Architecture destructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Architecture::~Architecture()
{
    assert( controllers_.modes_ );
    controllers_.modes_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Architecture::SerializeAttributes
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void Architecture::SerializeAttributes( xml::xostream& xos ) const
{
    assert( roofShape_ && material_ );
    xos << xml::start( "architecture" )
            << xml::attribute( "height", height_.value_ )
            << xml::attribute( "floor-number", floorNumber_ )
            << xml::attribute( "roof-shape", roofShape_->GetName() )
            << xml::attribute( "material", material_->GetName() )
            << xml::attribute( "occupation", static_cast< float >( occupation_.value_ ) / 100.f )
            << xml::attribute( "trafficability", trafficability_.value_ );
    if( parkingFloors_ > 0 )
        xos << xml::attribute( "parking-floors", parkingFloors_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Architecture::NotifyModeChanged
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void Architecture::NotifyModeChanged( int newMode )
{
    kernel::ModesObserver_ABC::NotifyModeChanged( newMode );
    if( newMode == ePreparationMode_Exercise )
        CreateDictionnary( true );
    else if( newMode == ePreparationMode_Terrain )
        CreateDictionnary( false );
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetMaterial
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetMaterial( kernel::MaterialCompositionType& material )
{
    material_ = &material;
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetRoofShape
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetRoofShape( kernel::RoofShapeType& roofShape )
{
    roofShape_ = &roofShape;
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetFloorNumber
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetFloorNumber( unsigned int floorNumber )
{
    floorNumber_ = floorNumber;
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetParkingFloors
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetParkingFloors( unsigned int parkingFloors )
{
    parkingFloors_ = parkingFloors;
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetOccupation
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetOccupation( unsigned int occupation )
{
    occupation_.value_ = occupation;
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetHeight
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetHeight( unsigned int height )
{
    height_.value_ = height;
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetTrafficability
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetTrafficability( float trafficability)
{
    trafficability_.value_ = trafficability;
}
