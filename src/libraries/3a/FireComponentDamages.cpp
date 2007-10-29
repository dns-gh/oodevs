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
float FireComponentDamages::Extract( const ASN1T_MsgsSimToClient& message ) const
{
    const ASN1T_MsgStopUnitFire& stop = *message.msg.u.msg_stop_unit_fire;
    float result = 0;
    for( unsigned u = 0; u < stop.units_damages.n; ++u )
    {
        const ASN1T_UnitFireDamages& damages = stop.units_damages.elem[u];
        for( unsigned e = 0; e < damages.equipments.n; ++e )
        {
            const ASN1T_UnitEquipmentFireDamage& damage = damages.equipments.elem[e];
            if( filter_.IsAllowed( damage.equipement_type ) )
                result += damage.unavailable_nbr;
        }
    }
    return result;
}