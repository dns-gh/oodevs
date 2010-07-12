// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyResourcesAlarms.h"
#include <xeumeuleu/xml.hpp>

PHY_SupplyResourcesAlarms::T_LevelSet PHY_SupplyResourcesAlarms::convoyTransporterResourcesLevels_;

struct PHY_SupplyResourcesAlarms::LoadingWrapper
{
    void ReadResourceAvailabilityAlert ( xml::xistream& xis )
    {
        PHY_SupplyResourcesAlarms::ReadResourceAvailabilityAlert( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::Initialize
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_SupplyResourcesAlarms::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing supply resources alarms" );
    LoadingWrapper loader;

    xis >> xml::start( "supply" )
            >> xml::start( "resource-availability-alerts" )
                >> xml::list( "resource-availability-alert", loader, &LoadingWrapper::ReadResourceAvailabilityAlert )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::ReadResourceAvailabilityAlert
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void PHY_SupplyResourcesAlarms::ReadResourceAvailabilityAlert( xml::xistream& xis )
{
    MT_Float rRatio;
    xis >> xml::attribute( "availability-threshold", rRatio );
    if( rRatio < 0 || rRatio > 100 )
        xis.error( "resource-availabilty-alert: availability-threshold not in [0..100]" );
    rRatio /= 100.;
    convoyTransporterResourcesLevels_.insert( rRatio );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::Terminate
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_SupplyResourcesAlarms::Terminate()
{
    convoyTransporterResourcesLevels_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::IsLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_SupplyResourcesAlarms::IsLevelReached( const T_LevelSet& levels, MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    for( CIT_LevelSet it = levels.begin(); it != levels.end(); ++it )
    {
        if( rPreviousRatio > *it && rCurrentRatio <= *it )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::IsRepairerResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_SupplyResourcesAlarms::IsConvoyTransporterResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( convoyTransporterResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

