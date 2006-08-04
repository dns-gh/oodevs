// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_MedicalResourcesAlarms.h"

PHY_MedicalResourcesAlarms::T_LevelSet PHY_MedicalResourcesAlarms::evacuationResourcesLevels_;
PHY_MedicalResourcesAlarms::T_LevelSet PHY_MedicalResourcesAlarms::collectionResourcesLevels_;
PHY_MedicalResourcesAlarms::T_LevelSet PHY_MedicalResourcesAlarms::doctorsResourcesLevels_;

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::Initialize
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_MedicalResourcesAlarms::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing medical resources alarms" );

    archive.Section( "Sante" );
    archive.Section( "AlertesDisponibiliteMoyens" );
    
    archive.BeginList( "AlertesDisponibiliteMoyensReleve" );
    while( archive.NextListElement() )
    {
        archive.Section( "AlerteDisponibiliteMoyensReleve" );
        MT_Float rRatio;
        archive.ReadAttribute( "pourcentageMoyensDisponibles", rRatio, CheckValueBound( 0., 100. ) );
        rRatio /= 100.;
        evacuationResourcesLevels_.insert( rRatio );
        archive.EndSection(); // AlerteDisponibiliteMoyensReleve 
    }
    archive.EndList(); // AlertesDisponibiliteMoyensReleve

    archive.BeginList( "AlertesDisponibiliteMoyensRamassage" );
    while( archive.NextListElement() )
    {
        archive.Section( "AlerteDisponibiliteMoyensRamassage" );
        MT_Float rRatio;
        archive.ReadAttribute( "pourcentageMoyensDisponibles", rRatio, CheckValueBound( 0., 100. ) );
        rRatio /= 100.;
        collectionResourcesLevels_.insert( rRatio );
        archive.EndSection(); // AlertesDisponibiliteMoyensRamassage 
    }
    archive.EndList(); // AlertesDisponibiliteMoyensRamassage

    archive.BeginList( "AlertesDisponibiliteMedecins" );
    while( archive.NextListElement() )
    {
        archive.Section( "AlerteDisponibiliteMedecins" );
        MT_Float rRatio;
        archive.ReadAttribute( "pourcentageMoyensDisponibles", rRatio, CheckValueBound( 0., 100. ) );
        rRatio /= 100.;
        doctorsResourcesLevels_.insert( rRatio );
        archive.EndSection(); // AlerteDisponibiliteMedecins 
    }
    archive.EndList(); // AlertesDisponibiliteMedecins

    archive.EndSection(); // AlertesDisponibiliteMoyens
    archive.EndSection(); // Sante
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

// =============================================================================
// OPERATIONS
// =============================================================================

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

