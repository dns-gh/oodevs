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
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
PhysicalAttribute::PhysicalAttribute( const kernel::Entity_ABC* parent, gui::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes,
                                      UrbanObject& urbanObject, kernel::Controllers& controllers, const kernel::ObjectTypes& objectTypes )
{
    architecture_.reset( new Architecture( parent, controllers, urbanObject, dictionary, objectTypes ) );
    usages_.reset( new Usages( parent, dictionary, accommodationTypes, urbanObject.GetLivingSpace( architecture_->GetFloorNumber(), architecture_->GetOccupation() ), urbanObject ) );
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute constructor
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
PhysicalAttribute::PhysicalAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes,
                                      UrbanObject& urbanObject, kernel::Controllers& controllers, const kernel::ObjectTypes& objectTypes )
{
    if( xis.has_child( "physical" ) )
    {
        xis >> xml::start( "physical" );

        if( xis.has_child( "architecture" ) )
            architecture_.reset( new Architecture( controllers, xis, urbanObject, dictionary, objectTypes ) );
        else
            architecture_.reset( new Architecture( 0, controllers, urbanObject, dictionary, objectTypes ) );

        if( xis.has_child( "usages" ) )
            usages_.reset( new Usages( xis, dictionary, accommodationTypes, urbanObject.GetLivingSpace( architecture_->GetFloorNumber(), architecture_->GetOccupation() ), urbanObject ) );
        else
            usages_.reset( new Usages( 0, dictionary, accommodationTypes, urbanObject.GetLivingSpace( architecture_->GetFloorNumber(), architecture_->GetOccupation() ), urbanObject ) );

        xis >> xml::end;
    }
    else
    {
        architecture_.reset( new Architecture( 0, controllers, urbanObject, dictionary, objectTypes ) );
        usages_.reset( new Usages( 0, dictionary, accommodationTypes, urbanObject.GetLivingSpace( architecture_->GetFloorNumber(), architecture_->GetOccupation() ), urbanObject ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute destructor
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
PhysicalAttribute::~PhysicalAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhysicalAttribute::SerializeAttributes
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
void PhysicalAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    assert( architecture_.get() );
    assert( usages_.get() );
    if( architecture_->IsDefault() && usages_->IsDefault() )
        return;
    xos << xml::start( "physical" );
    architecture_->SerializeAttributes( xos );
    usages_->SerializeAttributes( xos );
    xos << xml::end;
}
