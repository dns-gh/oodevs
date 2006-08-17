// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_AutomateMissionType.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 17:05 $
// $Revision: 3 $
// $Workfile: MIL_AutomateMissionType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AutomateMissionType.h"
#include "MIL_AutomateMission_ABC.h"

MIL_AutomateMissionType::T_MissionFromIDVector MIL_AutomateMissionType::missionsFromID_;
MIL_AutomateMissionType::T_MissionFromAsnIDMap MIL_AutomateMissionType::missionsFromAsnID_;
MIL_AutomateMissionType::T_MissionFromNameMap  MIL_AutomateMissionType::missionsFromName_;
const MIL_AutomateMissionType*                 MIL_AutomateMissionType::pSurrenderingMissionType_      = 0;
const MIL_AutomateMissionType*                 MIL_AutomateMissionType::pMoveToRefugeeCampMissionType_ = 0;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// ============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Initialize
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_AutomateMissionType::Initialize( MIL_InputArchive& archive )
{
    archive.Section( "Missions" );

    std::string strMission;

    archive.Section( "MissionReddition" );   
    archive.ReadAttribute( "nom", strMission );
    pSurrenderingMissionType_ = MIL_AutomateMissionType::FindAutomateMissionType( strMission );
    if( !pSurrenderingMissionType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid mission name", archive.GetContext() );
    archive.EndSection(); // MissionReddition

    archive.Section( "MissionSeRendreVersCampRefugies" );   
    archive.ReadAttribute( "nom", strMission );
    pMoveToRefugeeCampMissionType_ = MIL_AutomateMissionType::FindAutomateMissionType( strMission );
    if( !pMoveToRefugeeCampMissionType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid mission name", archive.GetContext() );
    archive.EndSection(); // MissionSeRendreVersCampRefugies
    
    archive.EndSection(); // Missions
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::InitializeDIA
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void MIL_AutomateMissionType::InitializeDIA()
{
    MIL_AutomateMission_ABC::InitializeDIA();
    for( CIT_MissionFromIDVector it = missionsFromID_.begin(); it != missionsFromID_.end(); ++it )
    {
        assert( *it );
        const MIL_AutomateMissionType& type = **it;
        type.missionDiaInitializer_( type );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void MIL_AutomateMissionType::Terminate()
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
// Name: MIL_AutomateMissionType constructor
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_AutomateMissionType::MIL_AutomateMissionType( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strMrtBehavior, const std::string& strConduiteBehavior, T_MissionAllocator missionAllocator, T_DiaInitializer diaInitializer )
    : strName_              ( strName )
    , nID_                  ( nID )
    , nAsnID_               ( nAsnID )
    , strDIATypeName_       ( strDIATypeName )
    , strMrtBehavior_       ( strMrtBehavior )
    , strConduiteBehavior_  ( strConduiteBehavior )
    , missionAllocator_     ( missionAllocator )
    , missionDiaInitializer_( diaInitializer )
{

}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
MIL_AutomateMissionType::~MIL_AutomateMissionType()
{

}

