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
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/RoofShapeType.h"
#include "clients_kernel/MaterialCompositionType.h"
#include "ENT/ENT_Enums_Gen.h"

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: ABR 2012-05-24
// -----------------------------------------------------------------------------
Architecture::Architecture( kernel::Controllers& controllers, kernel::PropertiesDictionary& dictionary, const kernel::ObjectTypes& objectTypes )
    : kernel::Architecture( dictionary )
    , controllers_( controllers )
{
    Initialize( objectTypes, 20.f, 6, 0, 0.5f, 0.5f );
    if( controllers_.modes_ )
        controllers_.modes_->Register( *this );
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
    Initialize( objectTypes, static_cast< float >( height ), floorNumber, parkingFloors, occupation, trafficability, material, roofShape );
    if( controllers_.modes_ )
        controllers_.modes_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Architecture destructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Architecture::~Architecture()
{
    if( controllers_.modes_ )
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
            << xml::attribute( "height", height_ )
            << xml::attribute( "floor-number", floorNumber_ )
            << xml::attribute( "roof-shape", roofShape_->GetName() )
            << xml::attribute( "material", material_->GetName() )
            << xml::attribute( "occupation", occupation_ / 100.f )
            << xml::attribute( "trafficability", trafficability_ );
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
