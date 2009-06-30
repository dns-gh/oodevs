// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MaintenanceResourcesAlarms.h"
#include <xeumeuleu/xml.h>

PHY_MaintenanceResourcesAlarms::T_LevelSet PHY_MaintenanceResourcesAlarms::repairerResourcesLevels_;
PHY_MaintenanceResourcesAlarms::T_LevelSet PHY_MaintenanceResourcesAlarms::haulerResourcesLevels_;

struct PHY_MaintenanceResourcesAlarms::LoadingWrapper
{
    void ReadResource( xml::xistream& xis )
    {
        PHY_MaintenanceResourcesAlarms::ReadResource( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::Initialize
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_MaintenanceResourcesAlarms::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing maintenance resources alarms" );

    LoadingWrapper loader;
    xis >> xml::start( "maintenance" )
            >> xml::start( "resource-availability-alerts" )
                >> xml::list( "resource-availability-alert", loader, &LoadingWrapper::ReadResource )
            >> xml::end()
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::ReadResource
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_MaintenanceResourcesAlarms::ReadResource( xml::xistream& xis )
{
    MT_Float rRation;
    std::string strType;
    xis >> xml::attribute( "resource", strType )
        >> xml::attribute( "availability-threshold", rRation );
    if( rRation < 0 || rRation > 100 )
        xis.error( "availability-threshod not in [0..100]" );
    rRation /= 100.;
    if( strType == "repairer" )
        repairerResourcesLevels_.insert( rRation );
    else if( strType == "tug" )
        haulerResourcesLevels_.insert( rRation );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::Terminate
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_MaintenanceResourcesAlarms::Terminate()
{
    repairerResourcesLevels_.clear();
    haulerResourcesLevels_  .clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::IsLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MaintenanceResourcesAlarms::IsLevelReached( const T_LevelSet& levels, MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    for( CIT_LevelSet it = levels.begin(); it != levels.end(); ++it )
        if( rPreviousRatio > *it && rCurrentRatio <= *it )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::IsRepairerResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MaintenanceResourcesAlarms::IsRepairerResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( repairerResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::IsHaulerResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MaintenanceResourcesAlarms::IsHaulerResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( haulerResourcesLevels_, rPreviousRatio, rCurrentRatio );
}
