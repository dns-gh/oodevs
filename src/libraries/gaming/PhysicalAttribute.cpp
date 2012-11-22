// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PhysicalAttribute.h"

#include "Architecture.h"
#include "Usages.h"
#include "clients_kernel/UrbanObject.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute constructor
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
PhysicalAttribute::PhysicalAttribute( const sword::UrbanAttributes& message, kernel::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes,
                                      kernel::UrbanObject& urbanObject, const kernel::ObjectTypes& objectTypes, kernel::Controller& controller )
{
    architecture_.reset( new Architecture( message, urbanObject, dictionary, objectTypes ) );
    usages_.reset( new Usages( message, dictionary, accommodationTypes,
        urbanObject.GetLivingSpace( architecture_->GetFloorNumber(), architecture_->GetOccupation() ), urbanObject, controller ) );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute destructor
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
PhysicalAttribute::~PhysicalAttribute()
{
    // NOTHING
}
