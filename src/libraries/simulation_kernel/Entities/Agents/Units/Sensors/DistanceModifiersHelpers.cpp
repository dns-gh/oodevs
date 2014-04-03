// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DistanceModifiersHelpers.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"

namespace distance_modifiers
{

std::map< std::string, double > ReadDistanceModifiers( xml::xistream& xis, const std::string& parent )
{
    const std::string attr = "distance-modifier";
    std::map< std::string, double > factors;
    xis >> xml::start( parent )
            >> xml::list( attr, [&]( xml::xistream& xis )
            {
                const double factor = xis.attribute< double >( "value" );
                if( factor < 0 || factor > 1 )
                    throw MASA_EXCEPTION( xis.context() + attr + ": value not in [0..1]" );
                factors[ xis.attribute< std::string >( "type" ) ] = factor;
            })
        >> xml::end;
    return factors;
}

void ReadPostureFactors( xml::xistream& xis, const std::string& parent, std::vector< double >& factors )
{
    auto values = ReadDistanceModifiers( xis, parent );
    for( auto it = values.cbegin(); it != values.cend(); ++it )
    {
        const PHY_Posture* posture = PHY_Posture::FindPosture( it->first );
        if( !posture )
            throw MASA_EXCEPTION( "unknown distance-modifier: " + it->first );
        if( !posture->CanModifyDetection() )
            continue;
        factors.at( posture->GetID() ) = it->second;
    }
}

}
