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

PHY_SupplyResourcesAlarms::T_LevelSet PHY_SupplyResourcesAlarms::convoyTransporterResourcesLevels_;

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::Initialize
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_SupplyResourcesAlarms::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing supply resources alarms" );

    archive.Section( "Ravitaillement" );
    archive.Section( "AlertesDisponibiliteMoyens" );
    
    archive.BeginList( "AlertesDisponibiliteVecteurs" );
    while( archive.NextListElement() )
    {
        archive.Section( "AlerteDisponibiliteVecteurs" );
        MT_Float rRatio;
        archive.ReadAttribute( "pourcentageMoyensDisponibles", rRatio, CheckValueBound( 0., 100. ) );
        rRatio /= 100.;
        convoyTransporterResourcesLevels_.insert( rRatio );
        archive.EndSection(); // AlerteDisponibiliteVecteurs 
    }
    archive.EndList(); // AlertesDisponibiliteVecteurs

    archive.EndSection(); // AlertesDisponibiliteMoyens
    archive.EndSection(); // Ravitaillement
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::Terminate
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
void PHY_SupplyResourcesAlarms::Terminate()
{
    convoyTransporterResourcesLevels_.clear();
}

// =============================================================================
// OPERATIONS
// =============================================================================

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

