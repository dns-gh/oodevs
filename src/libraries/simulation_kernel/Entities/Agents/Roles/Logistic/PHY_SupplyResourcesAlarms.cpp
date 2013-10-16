// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyResourcesAlarms.h"
#include "MT_Tools/MT_Logger.h"

namespace
{
    std::set< double > levels;

    void ReadThreshold( xml::xistream& xis )
    {
        double ratio;
        xis >> xml::attribute( "availability-threshold", ratio );
        if( ratio < 0 || ratio > 100 )
            throw MASA_EXCEPTION( xis.context() + "resource-availabilty-alert: availability-threshold not in [0..100]" );
        levels.insert( ratio / 100 );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::Initialize
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_SupplyResourcesAlarms::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing supply resource alarms" );
    xis >> xml::start( "supply" )
            >> xml::start( "resource-availability-alerts" )
                >> xml::list( "resource-availability-alert", &ReadThreshold )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::Terminate
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_SupplyResourcesAlarms::Terminate()
{
    levels.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::IsConvoyTransporterResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_SupplyResourcesAlarms::IsConvoyTransporterResourcesLevelReached( double previous, double current )
{
    for( auto it = levels.begin(); it != levels.end(); ++it )
        if( previous > *it && current <= *it )
            return true;
    return false;
}
