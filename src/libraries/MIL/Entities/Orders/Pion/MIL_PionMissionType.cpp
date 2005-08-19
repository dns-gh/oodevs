// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Pion/MIL_PionMissionType.cpp $
// $Author: Nld $
// $Modtime: 23/09/04 11:07 $
// $Revision: 1 $
// $Workfile: MIL_PionMissionType.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_PionMissionType.h"
#include "MIL_PionMission_ABC.h"

MIL_PionMissionType::T_MissionFromIDVector MIL_PionMissionType::missionsFromID_;
MIL_PionMissionType::T_MissionFromAsnIDMap MIL_PionMissionType::missionsFromAsnID_;
MIL_PionMissionType::T_MissionFromNameMap  MIL_PionMissionType::missionsFromName_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// ============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::InitializeDIA
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void MIL_PionMissionType::InitializeDIA()
{
    MIL_PionMission_ABC::InitializeDIA();
    for( CIT_MissionFromIDVector it = missionsFromID_.begin(); it != missionsFromID_.end(); ++it )
    {
        assert( *it );
        const MIL_PionMissionType& type = **it;
        type.missionDiaInitializer_( type );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void MIL_PionMissionType::Terminate()
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
// Name: MIL_PionMissionType constructor
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_PionMissionType::MIL_PionMissionType( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strBehavior, T_MissionAllocator missionAllocator, T_DiaInitializer diaInitializer )
    : strName_              ( strName )
    , nID_                  ( nID )
    , nAsnID_               ( nAsnID )
    , strDIATypeName_       ( strDIATypeName )
    , strBehavior_          ( strBehavior )
    , missionAllocator_     ( missionAllocator )
    , missionDiaInitializer_( diaInitializer )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
MIL_PionMissionType::~MIL_PionMissionType()
{
    // NOTHING
}

