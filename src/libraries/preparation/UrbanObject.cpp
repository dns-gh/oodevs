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

#include "tools/IdManager.h"
#include "UrbanHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController.h"

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( kernel::Controllers& controllers, tools::IdManager& idManager, const std::string& name, const kernel::ObjectType& type,
                          const kernel::AccommodationTypes& accommodations, gui::UrbanDisplayOptions& options )
    : gui::UrbanObject( controllers, name, idManager.GetNextId(), type, accommodations, options )
{
    name_ = QString( "%1 [%2]" ).arg( name.c_str() ).arg( id_ );
    controllers_.modes_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( xml::xistream& xis, kernel::Controllers& controllers, tools::IdManager& idManager, const kernel::ObjectType& type,
                          const kernel::AccommodationTypes& accommodations, gui::UrbanDisplayOptions& options )
    : gui::UrbanObject( xis, controllers, type, accommodations, options )
{
    controllers_.modes_.Register( *this );
    idManager.Lock( id_, true );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject destructor
// Created: ABR 2012-05-23
// -----------------------------------------------------------------------------
UrbanObject::~UrbanObject()
{
    controllers_.modes_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::NotifyModeChanged
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void UrbanObject::NotifyModeChanged( E_Modes newMode )
{
    kernel::ModesObserver_ABC::NotifyModeChanged( newMode );
    if( newMode == eModes_Prepare )
        CreateDictionary( true );
    else if( newMode == eModes_Terrain )
        CreateDictionary( false );
}
