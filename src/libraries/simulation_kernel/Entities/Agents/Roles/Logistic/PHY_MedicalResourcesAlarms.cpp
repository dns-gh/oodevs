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
#include <xeumeuleu/xml.h>

PHY_MedicalResourcesAlarms::T_LevelSet PHY_MedicalResourcesAlarms::evacuationResourcesLevels_;
PHY_MedicalResourcesAlarms::T_LevelSet PHY_MedicalResourcesAlarms::collectionResourcesLevels_;
PHY_MedicalResourcesAlarms::T_LevelSet PHY_MedicalResourcesAlarms::doctorsResourcesLevels_;

struct PHY_MedicalResourcesAlarms::LoadingWrapper
{
    void ReadResourceLevel( xml::xistream& xis )
    {
        PHY_MedicalResourcesAlarms::ReadResourceLevel( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::Initialize
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_MedicalResourcesAlarms::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing medical resources alarms" );

    LoadingWrapper loader;

    xis >> xml::start( "health" )
            >> xml::start( "resource-availability-alerts" )
                >> xml::list( "resource-availability-alert", loader, &LoadingWrapper::ReadResourceLevel )
            >> xml::end()
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::ReadResourceLevel
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void PHY_MedicalResourcesAlarms::ReadResourceLevel( xml::xistream& xis )
{
    MT_Float rRatio;
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
bool PHY_MedicalResourcesAlarms::IsLevelReached( const T_LevelSet& levels, MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    for( CIT_LevelSet it = levels.begin(); it != levels.end(); ++it )
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
bool PHY_MedicalResourcesAlarms::IsEvacuationResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( evacuationResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::IsCollectionResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MedicalResourcesAlarms::IsCollectionResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( collectionResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::IsDoctorResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MedicalResourcesAlarms::IsDoctorResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( doctorsResourcesLevels_, rPreviousRatio, rCurrentRatio );
}
