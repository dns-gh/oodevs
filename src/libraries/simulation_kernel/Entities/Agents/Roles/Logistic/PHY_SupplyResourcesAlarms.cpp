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

PHY_SupplyResourcesAlarms::T_LevelSet PHY_SupplyResourcesAlarms::convoyTransporterResourcesLevels_;

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::Initialize
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_SupplyResourcesAlarms::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing supply resources alarms" );
    xis >> xml::start( "supply" )
            >> xml::start( "resource-availability-alerts" )
                >> xml::list( "resource-availability-alert", &PHY_SupplyResourcesAlarms::ReadResourceAvailabilityAlert )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::ReadResourceAvailabilityAlert
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void PHY_SupplyResourcesAlarms::ReadResourceAvailabilityAlert( xml::xistream& xis )
{
    double rRatio;
    xis >> xml::attribute( "availability-threshold", rRatio );
    if( rRatio < 0 || rRatio > 100 )
        throw MASA_EXCEPTION( xis.context() + "resource-availabilty-alert: availability-threshold not in [0..100]" );
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
bool PHY_SupplyResourcesAlarms::IsLevelReached( const T_LevelSet& levels, double rPreviousRatio, double rCurrentRatio )
{
    for( auto it = levels.begin(); it != levels.end(); ++it )
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
bool PHY_SupplyResourcesAlarms::IsConvoyTransporterResourcesLevelReached( double rPreviousRatio, double rCurrentRatio )
{
    return IsLevelReached( convoyTransporterResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

