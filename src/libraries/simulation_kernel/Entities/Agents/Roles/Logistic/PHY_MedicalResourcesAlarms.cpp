// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MedicalResourcesAlarms.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

PHY_MedicalResourcesAlarms::T_LevelSet PHY_MedicalResourcesAlarms::evacuationResourcesLevels_;
PHY_MedicalResourcesAlarms::T_LevelSet PHY_MedicalResourcesAlarms::collectionResourcesLevels_;
PHY_MedicalResourcesAlarms::T_LevelSet PHY_MedicalResourcesAlarms::doctorsResourcesLevels_;

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::Initialize
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_MedicalResourcesAlarms::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing medical resources alarms" );
    xis >> xml::start( "health" )
            >> xml::start( "resource-availability-alerts" )
                >> xml::list( "resource-availability-alert", &PHY_MedicalResourcesAlarms::ReadResourceLevel )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::ReadResourceLevel
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void PHY_MedicalResourcesAlarms::ReadResourceLevel( xml::xistream& xis )
{
    double rRatio;
    std::string resourceType;
    xis >> xml::attribute( "resource", resourceType )
        >> xml::attribute( "availability-threshold", rRatio );

    if( rRatio < 0 || rRatio > 100 )
        xis.error( "resource-availability-alert: availability-treshold not in [0..100]" );

    rRatio /= 100.;

    if( resourceType == "relieve" )
        evacuationResourcesLevels_.insert( rRatio );
    else if( resourceType == "collection" )
        collectionResourcesLevels_.insert( rRatio );
    else if( resourceType == "doctor" )
        doctorsResourcesLevels_.insert( rRatio );
    else
        xis.error( "resource-availability: unknow resource" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::Terminate
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_MedicalResourcesAlarms::Terminate()
{
    evacuationResourcesLevels_.clear();
    collectionResourcesLevels_.clear();
    doctorsResourcesLevels_   .clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::IsLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MedicalResourcesAlarms::IsLevelReached( const T_LevelSet& levels, double rPreviousRatio, double rCurrentRatio )
{
    for( auto it = levels.begin(); it != levels.end(); ++it )
    {
        if( rPreviousRatio > *it && rCurrentRatio <= *it )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::IsEvacuationResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MedicalResourcesAlarms::IsEvacuationResourcesLevelReached( double rPreviousRatio, double rCurrentRatio )
{
    return IsLevelReached( evacuationResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::IsCollectionResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MedicalResourcesAlarms::IsCollectionResourcesLevelReached( double rPreviousRatio, double rCurrentRatio )
{
    return IsLevelReached( collectionResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::IsDoctorResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MedicalResourcesAlarms::IsDoctorResourcesLevelReached( double rPreviousRatio, double rCurrentRatio )
{
    return IsLevelReached( doctorsResourcesLevels_, rPreviousRatio, rCurrentRatio );
}
