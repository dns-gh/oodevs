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
float FireComponentDamages::Extract( const MsgSimToClient& wrapper ) const
{
    const MsgStopUnitFire& stop = wrapper.message().stop_unit_fire();
    float result = 0;
    for( int u = 0; u < stop.units_damages().elem_size(); ++u )
    {
        const MsgUnitFireDamages& damages = stop.units_damages().elem( u );
        for( unsigned e = 0; e < damages.equipments().elem_size(); ++e )
        {
            const MsgUnitEquipmentFireDamage& damage = damages.equipments().elem( e );
            if( filter_.IsAllowed( damage.equipement_type() ) )
                result += damage.unavailable_nbr();
        }
    }
    return result;
}