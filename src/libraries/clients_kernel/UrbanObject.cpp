// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "UrbanObject.h"
#include "Controllers.h"
#include "PhysicalAttribute_ABC.h"
#include "UrbanColor_ABC.h"
#include "UrbanDisplayOptions.h"
#include "Usages_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanObject gaming constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( Controllers& controllers, const std::string& name, unsigned int id, const ObjectType& type
    , const AccommodationTypes& accommodations, UrbanDisplayOptions& options )
    : UrbanObject_ABC( controllers.controller_, name, id, type, accommodations )
    , controllers_( controllers )
    , options_    ( options )
{
    UpdateColor();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject preparation/terrain constructor
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( xml::xistream& xis, Controllers& controllers, const ObjectType& type
    , const AccommodationTypes& accommodations, UrbanDisplayOptions& options )
    : UrbanObject_ABC( xis, controllers.controller_, type, accommodations )
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
    PhysicalAttribute_ABC* pPhysical = Retrieve< PhysicalAttribute_ABC >();
    if( !pPhysical )
        return;
    const Usages_ABC* pUsages = pPhysical->GetUsages();
    UrbanColor_ABC* pColor = Retrieve< UrbanColor_ABC >();
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
