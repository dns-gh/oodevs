// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "FireComponentDamages.h"
#include <xeumeuleu/xml.h>
#include <vector>
#pragma warning (push)
#pragma warning (disable : 4100 4127 4511 4512 )
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

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
{
    const std::string values = xml::attribute( xis, "components", std::string() );
    if( ! values.empty() )
    {
        std::vector< std::string > split;
        boost::algorithm::split( split, values, boost::algorithm::is_any_of( "," ) );
        std::transform( split.begin(), split.end(), std::inserter( filter_, filter_.begin() ), &boost::lexical_cast< int, std::string > );
    }
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
            if( filter_.empty() || filter_.find( damage.equipement_type ) != filter_.end() )
                result += damage.unavailable_nbr;
        }
    }
    return result;
}