// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_PopulationMissionType.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 17:05 $
// $Revision: 3 $
// $Workfile: MIL_PopulationMissionType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_PopulationMissionType.h"
#include "MIL_PopulationMission_ABC.h"

MIL_PopulationMissionType::T_MissionFromIDVector MIL_PopulationMissionType::missionsFromID_;
MIL_PopulationMissionType::T_MissionFromAsnIDMap MIL_PopulationMissionType::missionsFromAsnID_;
MIL_PopulationMissionType::T_MissionFromNameMap  MIL_PopulationMissionType::missionsFromName_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// ============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::InitializeDIA
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void MIL_PopulationMissionType::InitializeDIA()
{
    MIL_PopulationMission_ABC::InitializeDIA();
    for( CIT_MissionFromIDVector it = missionsFromID_.begin(); it != missionsFromID_.end(); ++it )
    {
        assert( *it );
        const MIL_PopulationMissionType& type = **it;
        type.missionDiaInitializer_( type );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void MIL_PopulationMissionType::Terminate()
{
    for( CIT_MissionFromIDVector it = missionsFromID_.begin(); it != missionsFromID_.end(); ++it )
        delete *it;
    missionsFromID_   .clear();
    missionsFromAsnID_.clear();
    missionsFromName_ .clear();
}


// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType constructor
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_PopulationMissionType::MIL_PopulationMissionType( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strBehavior, T_MissionAllocator missionAllocator, T_DiaInitializer diaInitializer )
    : strName_              ( strName )
    , nID_                  ( nID )
    , nAsnID_               ( nAsnID )
    , strDIATypeName_       ( strDIATypeName )
    , strBehavior_          ( strBehavior )
    , missionAllocator_     ( missionAllocator )
    , missionDiaInitializer_( diaInitializer )
{

}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
MIL_PopulationMissionType::~MIL_PopulationMissionType()
{

}

