// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Architecture.h"
#include "Tools.h"
#include "clients_kernel/PropertiesDictionary.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Architecture::Architecture( kernel::PropertiesDictionary& dictionary )
    : dictionary_( dictionary )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Architecture destructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Architecture::~Architecture()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Architecture::Initialize
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
void Architecture::Initialize( float height, unsigned int floorNumber, const std::string& roofShape,
                               const std::string& material, float occupation, float trafficability )
{
    roofShape_ = roofShape;
    material_ = material;
    height_ = height;
    floorNumber_ = floorNumber;
    occupation_ = static_cast< unsigned int >( occupation * 100u );
    trafficability_ = trafficability;
    dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/Height" ), height_ );
    dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/floorNumber" ), floorNumber_ );
    dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/roofShape" ), roofShape_ );
    dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/material" ), material_ );
    dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/occupation" ), occupation_ );
    dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/trafficability" ), trafficability_ );
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetMaterial
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
const std::string& Architecture::GetMaterial() const
{
    return material_;
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetFloorNumber
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned int Architecture::GetFloorNumber() const
{
    return floorNumber_;
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetOccupation
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned int Architecture::GetOccupation() const
{
    return occupation_;
}
