// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_MaintenanceResourcesAlarms.h"

PHY_MaintenanceResourcesAlarms::T_LevelSet PHY_MaintenanceResourcesAlarms::repairerResourcesLevels_;
PHY_MaintenanceResourcesAlarms::T_LevelSet PHY_MaintenanceResourcesAlarms::haulerResourcesLevels_;

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::Initialize
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_MaintenanceResourcesAlarms::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing maintenance resources alarms" );

    archive.Section( "Maintenance" );
    archive.Section( "AlertesDisponibiliteMoyens" );
    
    archive.BeginList( "AlertesDisponibiliteReparateurs" );
    while( archive.NextListElement() )
    {
        archive.Section( "AlerteDisponibiliteReparateurs" );
        MT_Float rRatio;
        archive.ReadAttribute( "pourcentageMoyensDisponibles", rRatio, CheckValueBound( 0., 100. ) );
        rRatio /= 100.;
        repairerResourcesLevels_.insert( rRatio );
        archive.EndSection(); // AlerteDisponibiliteReparateurs 
    }
    archive.EndList(); // AlertesDisponibiliteReparateurs

    archive.BeginList( "AlertesDisponibiliteRemorqueurs" );
    while( archive.NextListElement() )
    {
        archive.Section( "AlerteDisponibiliteRemorqueurs" );
        MT_Float rRatio;
        archive.ReadAttribute( "pourcentageMoyensDisponibles", rRatio, CheckValueBound( 0., 100. ) );
        rRatio /= 100.;
        haulerResourcesLevels_.insert( rRatio );
        archive.EndSection(); // AlerteDisponibiliteRemorqueurs 
    }
    archive.EndList(); // AlertesDisponibiliteRemorqueurs

    archive.EndSection(); // AlertesDisponibiliteMoyens
    archive.EndSection(); // Maintenance
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

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::IsLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
bool PHY_MaintenanceResourcesAlarms::IsLevelReached( const T_LevelSet& levels, MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    for( CIT_LevelSet it = levels.begin(); it != levels.end(); ++it )
    {
        if( rPreviousRatio > *it && rCurrentRatio <= *it )
            return true;
    }
    return false;
}

