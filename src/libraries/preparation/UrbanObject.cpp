// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanObject.h"
#include "UrbanHierarchies.h"

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: ABR 2012-05-23
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( kernel::Controllers& controllers, const std::string& name, unsigned int id, const kernel::ObjectType& type, const kernel::AccommodationTypes& accommodations, kernel::UrbanDisplayOptions& options)
    : kernel::UrbanObject( controllers, name, id, type, accommodations, options )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: ABR 2012-05-23
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( xml::xistream& xis, kernel::Controllers& controllers, const kernel::ObjectType& type, const kernel::AccommodationTypes& accommodations, kernel::UrbanDisplayOptions& options )
    : kernel::UrbanObject( xis, controllers, type, accommodations, options )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObject destructor
// Created: ABR 2012-05-23
// -----------------------------------------------------------------------------
UrbanObject::~UrbanObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::SerializeAttributes
// Created: ABR 2012-05-23
// -----------------------------------------------------------------------------
void UrbanObject::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetId() )
        << xml::attribute( "name", GetName() );
}
