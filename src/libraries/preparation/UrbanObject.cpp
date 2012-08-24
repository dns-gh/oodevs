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

#include "IdManager.h"
#include "UrbanHierarchies.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( kernel::Controllers& controllers, IdManager& idManager, const std::string& name, const kernel::ObjectType& type,
                          const kernel::AccommodationTypes& accommodations, kernel::UrbanDisplayOptions& options )
    : kernel::UrbanObject( controllers, name, idManager.GetNextId(), type, accommodations, options )
{
    name_ = QString( "%1 [%2]" ).arg( name.c_str() ).arg( id_ );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( xml::xistream& xis, kernel::Controllers& controllers, IdManager& idManager, const kernel::ObjectType& type,
                          const kernel::AccommodationTypes& accommodations, kernel::UrbanDisplayOptions& options )
    : kernel::UrbanObject( xis, controllers, type, accommodations, options )
{
    controllers_.Update( *this );
    idManager.Lock( id_ );
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
