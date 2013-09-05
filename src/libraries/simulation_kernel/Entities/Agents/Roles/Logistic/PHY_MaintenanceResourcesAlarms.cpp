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
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    typedef std::set< double > T_LevelSet;

    T_LevelSet repairerResourcesLevels_;
    T_LevelSet haulerResourcesLevels_;

    void ReadResource( xml::xistream& xis )
    {
        double rRation;
        std::string strType;
        xis >> xml::attribute( "resource", strType )
            >> xml::attribute( "availability-threshold", rRation );
        if( rRation < 0 || rRation > 100 )
            throw MASA_EXCEPTION( xis.context() + "availability-threshold not in [0..100]" );
        rRation /= 100.;
        if( strType == "AlerteDisponibiliteReparateurs" || strType == "repairer" )
            repairerResourcesLevels_.insert( rRation );
        else if( strType == "AlerteDisponibiliteRemorqueurs" || strType == "tug" )
            haulerResourcesLevels_.insert( rRation );
        else
            throw MASA_EXCEPTION( xis.context() + "invalid resource type for maintenance resource-availability-alert" );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::Initialize
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_MaintenanceResourcesAlarms::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing maintenance resources alarms" );
    xis >> xml::start( "maintenance" )
            >> xml::start( "resource-availability-alerts" )
                >> xml::list( "resource-availability-alert", &ReadResource )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::Terminate
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_MaintenanceResourcesAlarms::Terminate()
{
    repairerResourcesLevels_.clear();
    haulerResourcesLevels_.clear();
}

namespace
{
    bool IsLevelReached( const T_LevelSet& levels, double rPreviousRatio, double rCurrentRatio )
    {
        for( auto it = levels.begin(); it != levels.end(); ++it )
            if( rPreviousRatio > *it && rCurrentRatio <= *it )
                return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::IsRepairerResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MaintenanceResourcesAlarms::IsRepairerResourcesLevelReached( double rPreviousRatio, double rCurrentRatio )
{
    return IsLevelReached( repairerResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::IsHaulerResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MaintenanceResourcesAlarms::IsHaulerResourcesLevelReached( double rPreviousRatio, double rCurrentRatio )
{
    return IsLevelReached( haulerResourcesLevels_, rPreviousRatio, rCurrentRatio );
}
