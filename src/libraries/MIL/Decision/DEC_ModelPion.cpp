// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/DEC_ModelPion.cpp $
// $Author: Nld $
// $Modtime: 4/03/05 14:26 $
// $Revision: 5 $
// $Workfile: DEC_ModelPion.cpp $
//
// *****************************************************************************


#include "MIL_pch.h"
#include "DEC_ModelPion.h"

#include "Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "Entities/Orders/Pion/MIL_PionMissionType.h"

// -----------------------------------------------------------------------------
// Name: DEC_ModelPion constructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DEC_ModelPion::DEC_ModelPion( const DEC_Workspace& decWorkspace, const std::string& strName, MIL_InputArchive& input, bool bNeedParsing, const std::string& strArchivePath, const std::string& strSourcePath )
    : DEC_Model_ABC( decWorkspace, strName, input, bNeedParsing, strArchivePath, strSourcePath )
{
    InitializeMissions( input );
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelPion destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
DEC_ModelPion::~DEC_ModelPion()
{

}

// -----------------------------------------------------------------------------
// Name: DEC_ModelPion::InitializeMissionOrdresConduite
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
void DEC_ModelPion::InitializeMissionOrdresConduite( MIL_InputArchive& archive, const MIL_PionMissionType& missionType )
{
    if ( !archive.BeginList( "OrdresConduite", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        if( !archive.Section( "OrdreConduite", MIL_InputArchive::eNothing ) )
            break;

        std::string strOrdreConduite;
        archive.ReadAttribute( "nom", strOrdreConduite );

        const MIL_OrderConduiteType* pType = MIL_OrderConduiteType::FindOrderConduiteType( strOrdreConduite );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown orderConduite type", archive.GetContext() );

        T_IDBitVector& bitVector = orderConduitePerMissionMap_[ &missionType ];
        if( bitVector.size() < pType->GetConduiteID() + 1 )
            bitVector.resize( pType->GetConduiteID() + 1, false );
        bitVector[ pType->GetConduiteID() ] = true;  

        archive.EndSection(); // Mission
    }
    archive.EndList(); // Missions
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelPion::InitializeMissions
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
void DEC_ModelPion::InitializeMissions( MIL_InputArchive& archive )
{
    missionBitset_.clear();
    orderConduitePerMissionMap_.clear();

    if ( !archive.BeginList( "Missions", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        if ( !archive.Section( "Mission", MIL_InputArchive::eNothing ) )
            break;
        
        std::string strMission;
        archive.ReadAttribute( "nom", strMission );

        const MIL_PionMissionType* pType = MIL_PionMissionType::FindPionMissionType( strMission );
        if ( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Invalid mission name '%s' for model '%s'", strMission.c_str(), GetName().c_str() ), archive.GetContext() );

        if ( missionBitset_.size() < pType->GetID() + 1 )
            missionBitset_.resize( pType->GetID() + 1, false );
        missionBitset_[ pType->GetID() ] = true;

        // Check if the DIA behavior is present
        if( !IsMissionAvailable( *pType ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Missing behavior '%s'", pType->GetBehaviorName().c_str() ), archive.GetContext() );

        InitializeMissionOrdresConduite( archive, *pType );

        archive.EndSection(); // Mission
    }
    archive.EndList(); // Missions
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelPion::IsMissionAvailable
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
bool DEC_ModelPion::IsMissionAvailable( const MIL_PionMissionType& missionType ) const
{
    if( missionType.GetID() > missionBitset_.size() || !missionBitset_[ missionType.GetID() ] )
        return false;

    DIA_BehaviorPart& behaviorPart = static_cast< DIA_BehaviorPart& >( GetDIAModel().GetBehaviorTool() );
    return behaviorPart.FindBehavior( missionType.GetBehaviorName() ) != 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelPion::IsOrderConduiteAvailableForMission
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
bool DEC_ModelPion::IsOrderConduiteAvailableForMission( const MIL_PionMissionType& missionType, const MIL_OrderConduiteType& orderConduiteType ) const
{
    if( orderConduiteType.IsAvailableForAllMissions() )
        return true;

    CIT_OrderConduitePerMissionMap itOrderConduite = orderConduitePerMissionMap_.find( &missionType );
    if( itOrderConduite == orderConduitePerMissionMap_.end() )
        return false;

    return orderConduiteType.GetConduiteID() < itOrderConduite->second.size() && (itOrderConduite->second)[ orderConduiteType.GetConduiteID() ];    
}
