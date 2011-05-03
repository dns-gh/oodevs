// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "FireComponentDamages.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: FireComponentDamages constructor
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
FireComponentDamages::FireComponentDamages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireComponentDamages destructor
// Created: FPO 2011-04-29
// -----------------------------------------------------------------------------
FireComponentDamages::~FireComponentDamages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireComponentDamages constructor
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
FireComponentDamages::FireComponentDamages( xml::xistream& xis )
    : filter_( xis, "components" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireComponentDamages::Extract
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
float FireComponentDamages::Extract( const sword::SimToClient& wrapper ) const
{
    const sword::StopUnitFire& stop = wrapper.message().stop_unit_fire();
    float result = 0;
    for( int u = 0; u < stop.units_damages().elem_size(); ++u )
    {
        const sword::UnitFireDamages& damages = stop.units_damages().elem( u );
        for( int e = 0; e < damages.equipments().elem_size(); ++e )
        {
            const sword::UnitEquipmentFireDamage& damage = damages.equipments().elem( e );
            if( filter_.IsAllowed( damage.equipement().id() ) )
                result += damage.unavailable();
        }
    }
    return result;
}