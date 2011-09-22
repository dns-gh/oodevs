// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AccommodationTypes.h"
#include "AccommodationType.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AccommodationTypes constructor
// Created: LDC 2011-03-24
// -----------------------------------------------------------------------------
AccommodationTypes::AccommodationTypes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AccommodationTypes destructor
// Created: LDC 2011-03-24
// -----------------------------------------------------------------------------
AccommodationTypes::~AccommodationTypes()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: AccommodationTypes::Load
// Created: LDC 2011-03-24
// -----------------------------------------------------------------------------
void AccommodationTypes::Load( const tools::ExerciseConfig& config )
{
    DeleteAll();
    const tools::Loader_ABC& loader = config.GetLoader();
    loader.LoadPhysicalFile( "urban", boost::bind( &AccommodationTypes::ReadAccommodations, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: AccommodationTypes::Purge
// Created: JSR 2011-09-22
// -----------------------------------------------------------------------------
void AccommodationTypes::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AccommodationTypes::ReadAccommodations
// Created: LDC 2011-03-24
// -----------------------------------------------------------------------------
void AccommodationTypes::ReadAccommodations( xml::xistream& xis )
{
    xis >> xml::start( "urban" )
            >> xml::optional() >> xml::start( "accommodations" )
            >> xml::list( "accommodation", *this, &AccommodationTypes::ReadAccommodation );
}

// -----------------------------------------------------------------------------
// Name: AccommodationTypes::ReadAccommodation
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void AccommodationTypes::ReadAccommodation( xml::xistream& xis )
{
    std::string role;
    float maxCapacity;
    float nominalCapacity;
    xis >> xml::attribute( "role", role )
        >> xml::attribute( "max-capacity", maxCapacity )
        >> xml::attribute( "nominal-capacity", nominalCapacity );
    AccommodationType* accomodation = new AccommodationType( role, nominalCapacity, maxCapacity );
    Register( role, *accomodation );
}
