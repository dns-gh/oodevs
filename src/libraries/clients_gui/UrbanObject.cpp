// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UrbanObject.h"
#include "UrbanDisplayOptions.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/Usages_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( kernel::Controllers& controllers, const std::string& name, unsigned int id, const kernel::ObjectType& type
                        , const kernel::AccommodationTypes& accommodations, UrbanDisplayOptions& options )
    : kernel::UrbanObject_ABC( controllers.controller_, name, id, type, accommodations )
    , controllers_( controllers )
    , options_    ( options )
{
    UpdateColor();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( xml::xistream& xis, kernel::Controllers& controllers, const kernel::ObjectType& type
                        , const kernel::AccommodationTypes& accommodations, UrbanDisplayOptions& options )
    : kernel::UrbanObject_ABC( xis, controllers.controller_, type, accommodations )
    , controllers_( controllers )
    , options_    ( options )
{
    UpdateColor();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanObject::~UrbanObject()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::UpdateColor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void UrbanObject::UpdateColor()
{
    const kernel::Usages_ABC* pUsages = Retrieve< kernel::Usages_ABC >();
    kernel::UrbanColor_ABC* pColor = Retrieve< kernel::UrbanColor_ABC >();
    if( pUsages && pColor && !options_.SetColor( *pColor, GetLivingSpace(), GetHumansMap(), *pUsages ) )
        pColor->Restore();
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::NotifyUpdated
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void UrbanObject::NotifyUpdated( const UrbanDisplayOptions& )
{
    UpdateColor();
}
