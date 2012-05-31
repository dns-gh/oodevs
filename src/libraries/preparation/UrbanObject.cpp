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
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: ABR 2012-05-23
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( kernel::Controllers& controllers, const std::string& name, unsigned int id, const kernel::ObjectType& type, const kernel::AccommodationTypes& accommodations, kernel::UrbanDisplayOptions& options)
    : kernel::UrbanObject( controllers, name, id, type, accommodations, options )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: ABR 2012-05-23
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( xml::xistream& xis, kernel::Controllers& controllers, const kernel::ObjectType& type, const kernel::AccommodationTypes& accommodations, kernel::UrbanDisplayOptions& options )
    : kernel::UrbanObject( xis, controllers, type, accommodations, options )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject destructor
// Created: ABR 2012-05-23
// -----------------------------------------------------------------------------
UrbanObject::~UrbanObject()
{
    controllers_.Unregister( *this );
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

// -----------------------------------------------------------------------------
// Name: UrbanObject::NotifyModeChanged
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void UrbanObject::NotifyModeChanged( int newMode )
{
    kernel::ModesObserver_ABC::NotifyModeChanged( newMode );
    if( newMode == ePreparationMode_Exercise )
        CreateDictionary( true );
    else if( newMode == ePreparationMode_Terrain )
        CreateDictionary( false );
}
