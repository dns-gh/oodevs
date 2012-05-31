// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "PhysicalAttribute.h"
#include "Architecture.h"
#include "Usages.h"
#include "UrbanObject.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute constructor
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
PhysicalAttribute::PhysicalAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes,
                                      UrbanObject& urbanObject, kernel::Controllers& controllers, const kernel::ObjectTypes& objectTypes )
{
    if( xis.has_child( "physical" ) )
    {
        xis >> xml::start( "physical" );
        if( xis.has_child( "architecture" ) )
            architecture_.reset( new Architecture( controllers, xis, dictionary, objectTypes ) );
        else
            architecture_.reset( new Architecture( controllers, dictionary, objectTypes ) );
        if( xis.has_child( "usages" ) )
            usages_.reset( new Usages( xis, dictionary, accommodationTypes, urbanObject.GetLivingSpace( architecture_->GetFloorNumber(), architecture_->GetOccupation() ), urbanObject, controllers.controller_ ) );
        else
            usages_.reset( new Usages( dictionary, accommodationTypes, urbanObject.GetLivingSpace( architecture_->GetFloorNumber(), architecture_->GetOccupation() ), urbanObject, controllers.controller_ ) );
        xis >> xml::end;
    }
    else
        architecture_.reset( new Architecture( controllers, dictionary, objectTypes ) );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute destructor
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
PhysicalAttribute::~PhysicalAttribute()
{
    // NOTHING
}
