// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/DEC_ModelAutomate.cpp $
// $Author: Nld $
// $Modtime: 4/03/05 16:18 $
// $Revision: 5 $
// $Workfile: DEC_ModelAutomate.cpp $
//
// *****************************************************************************


#include "simulation_kernel_pch.h"
#include "DEC_ModelAutomate.h"

#include "Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "Entities/Orders/Conduite/MIL_OrderConduiteType.h"

// -----------------------------------------------------------------------------
// Name: DEC_ModelAutomate constructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DEC_ModelAutomate::DEC_ModelAutomate( const DEC_Workspace& decWorkspace, const std::string& strName, MIL_InputArchive& input, bool bNeedParsing, const std::string& strArchivePath, const std::string& strSourcePath )
    : DEC_Model_ABC( decWorkspace, strName, input, bNeedParsing, strArchivePath, strSourcePath )
{
    InitializeMissions( input );
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelAutomate destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
DEC_ModelAutomate::~DEC_ModelAutomate()
{

}

// -----------------------------------------------------------------------------
// Name: DEC_ModelAutomate::InitializeMissionOrdresConduite
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
void DEC_ModelAutomate::InitializeMissionOrdresConduite( MIL_InputArchive& archive, const MIL_AutomateMissionType& missionType )
{
    if( !archive.BeginList( "OrdresConduite", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        if( !archive.Section( "OrdreConduite", MIL_InputArchive::eNothing ) )
            break;

        std::string strOrdreConduite;
        archive.ReadAttribute( "nom", strOrdreConduite );

        const MIL_OrderConduiteType* pType = MIL_OrderConduiteType::Find( strOrdreConduite );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknow orderConduite type", archive.GetContext() );

        T_IDBitVector& bitVector = orderConduitePerMissionMap_[ &missionType ];
        if( bitVector.size() < pType->GetConduiteID() + 1 )
            bitVector.resize( pType->GetConduiteID() + 1, false );
        bitVector[ pType->GetConduiteID() ] = true; 

        archive.EndSection(); // Mission
    }
    archive.EndList(); // Missions
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelAutomate::InitializeMissions
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
void DEC_ModelAutomate::InitializeMissions( MIL_InputArchive& archive )
{
    missionBitset_.clear();
    orderConduitePerMissionMap_.clear();

    // Ajout de la mission globale obligatoire 'SeRendre'
    const MIL_AutomateMissionType& surrenderingMissionType = MIL_AutomateMissionType::GetSurrenderingMissionType();
    if( missionBitset_.size() < surrenderingMissionType.GetID() + 1 )
        missionBitset_.resize( surrenderingMissionType.GetID() + 1, false );
    missionBitset_[ surrenderingMissionType.GetID() ] = true;
    if( !IsMissionAvailable( surrenderingMissionType ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Missing behavior '%s' or '%s'", surrenderingMissionType.GetMrtBehaviorName().c_str(), surrenderingMissionType.GetConduiteBehaviorName().c_str() ), archive.GetContext() );

    if ( !archive.BeginList( "Missions", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        if ( !archive.Section( "Mission", MIL_InputArchive::eNothing ) )
            break;

        std::string strMission;
        archive.ReadAttribute( "nom", strMission );

        const MIL_AutomateMissionType* pType = MIL_AutomateMissionType::FindAutomateMissionType( strMission );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Invalid mission name '%s' for model '%s'", strMission.c_str(), GetName().c_str() ), archive.GetContext() );

        if( missionBitset_.size() < pType->GetID() + 1 )
            missionBitset_.resize( pType->GetID() + 1, false );
        missionBitset_[ pType->GetID() ] = true;
        
        // Check if the DIA behaviors are presents
        if( !IsMissionAvailable( *pType ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Missing behavior '%s' or '%s'", pType->GetMrtBehaviorName().c_str(), pType->GetConduiteBehaviorName().c_str() ), archive.GetContext() );

        InitializeMissionOrdresConduite( archive, *pType );

        archive.EndSection(); // Mission
    }
    archive.EndList(); // Missions
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelAutomate::IsMissionAvailable
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
bool DEC_ModelAutomate::IsMissionAvailable( const MIL_AutomateMissionType& missionType ) const
{
    if( missionType.GetID() >= missionBitset_.size() || !missionBitset_[ missionType.GetID() ] )
        return false;

    DIA_BehaviorPart& behaviorPart = static_cast< DIA_BehaviorPart& >( GetDIAModel().GetBehaviorTool() );
    if( behaviorPart.FindBehavior( missionType.GetMrtBehaviorName() ) == 0 )
        return false;

    if( behaviorPart.FindBehavior( missionType.GetConduiteBehaviorName() ) == 0 )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelAutomate::IsOrderConduiteAvailableForMission
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
bool DEC_ModelAutomate::IsOrderConduiteAvailableForMission( const MIL_AutomateMissionType& missionType, const MIL_OrderConduiteType& orderConduiteType ) const
{
    if( orderConduiteType.IsAvailableForAllMissions() )
        return true;

    CIT_OrderConduitePerMissionMap itOrderConduite = orderConduitePerMissionMap_.find( &missionType );
    if( itOrderConduite == orderConduitePerMissionMap_.end() )
        return false;

    return orderConduiteType.GetConduiteID() < itOrderConduite->second.size() && (itOrderConduite->second)[ orderConduiteType.GetConduiteID() ];    
}
