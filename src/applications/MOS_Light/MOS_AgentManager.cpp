//*****************************************************************************
// 
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentManager.cpp $
// $Author: Nld $
// $Modtime: 14/02/05 15:56 $
// $Revision: 35 $
// $Workfile: MOS_AgentManager.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_AgentManager.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentManager.inl"
#endif

#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_Tools.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_History.h"
#include "MOS_AgentModel.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"
#include "MOS_AgentEditor.h"
#include "MOS_AgentListView.h"
#include "MOS_Gtia.h"
#include "MOS_LogMaintenanceConsign.h"
#include "MOS_LogSupplyConsign.h"
#include "MOS_LogMedicalConsign.h"
#include "MOS_TypePion.h"
#include "MOS_TypeAutomate.h"

#include "MT_Tools/MT_ScipioException.h"

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_AgentManager::MOS_AgentManager()
{

}

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_AgentManager::~MOS_AgentManager()
{
    RemoveAllAgents();
    RemoveAllGtias();
    RemoveAllTeams();
    MT_DELETEOWNED( vAvailableModels_ );
    vAvailableModels_.clear();
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::Initialize
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_AgentManager::Initialize()
{
    MT_XXmlInputArchive scipioArchive;

    if( ! scipioArchive.Open( "scipio.xml" ) )
        throw MT_ScipioException( "MOS_AgentManager::Initialize", __FILE__, __LINE__, "Can not open file scipio.xml" , scipioArchive.RetrieveLastError()->GetInfo() );
    if( ! scipioArchive.Section( "Scipio" ) )
        throw MT_ScipioException( "MOS_AgentManager::Initialize", __FILE__, __LINE__, "Section SCIPIO does not exist", scipioArchive.RetrieveLastError()->GetInfo() );
    if( ! scipioArchive.Section( "Donnees" ) )
        throw MT_ScipioException( "MOS_AgentManager::Initialize", __FILE__, __LINE__, "Section SCIPIO does not exist", scipioArchive.RetrieveLastError()->GetInfo() );

    InitializeModels       ( scipioArchive );
    InitializeTypesPion    ( scipioArchive );
    InitializeTypesAutomate( scipioArchive );

    scipioArchive.EndSection(); // Donnees
    scipioArchive.EndSection(); // Scipio
    scipioArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::InitializeTypesPion
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentManager::InitializeTypesPion( MT_InputArchive_ABC& scipioArchive )
{
    std::string strTypesPionFile;
    if( !scipioArchive.ReadField( "Pions", strTypesPionFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    MT_XXmlInputArchive archive;
    if( !archive.Open( strTypesPionFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
       
    if( !archive.BeginList( "Pions" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    while( archive.NextListElement() )
    {
        if( !archive.Section( "Unite" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        std::string strName;
        if( !archive.ReadAttribute( "nom", strName ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        MOS_TypePion* pTypePion = new MOS_TypePion( strName, archive );
        bool bOut = typesPion_.insert( std::make_pair( pTypePion->GetID(), pTypePion ) ).second;
        assert( bOut );

        archive.EndSection(); // Unite
    }
    archive.EndList(); // Pions

    archive.Close();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::InitializeTypesAutomate
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentManager::InitializeTypesAutomate( MT_InputArchive_ABC& scipioArchive )
{
    std::string strTypesAutomateFile;
    if( !scipioArchive.ReadField( "Automates", strTypesAutomateFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    MT_XXmlInputArchive archive;
    if( !archive.Open( strTypesAutomateFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
       
    if( !archive.BeginList( "Automates" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    while( archive.NextListElement() )
    {
        if( !archive.Section( "Unite" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        std::string strName;
        if( !archive.ReadAttribute( "nom", strName ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        MOS_TypeAutomate* pTypeAutomate = new MOS_TypeAutomate( strName, archive );
        bool bOut = typesAutomate_.insert( std::make_pair( pTypeAutomate->GetID(), pTypeAutomate ) ).second;
        assert( bOut );

        archive.EndSection(); // Unite
    }
    archive.EndList(); // Automates

    archive.Close();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::InitializeModels
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
void MOS_AgentManager::InitializeModels( MT_InputArchive_ABC& scipioArchive )
{    
    // Modeles
    std::string strDecFile;
    if( !scipioArchive.ReadField( "Decisionnel", strDecFile ) )
        throw MT_ScipioException( "MOS_AgentManager::Initialize", __FILE__, __LINE__, "Field SCIPIO::DIA::Camps does not exist", scipioArchive.RetrieveLastError()->GetInfo() );
    MT_XXmlInputArchive decArchive;
    if( !decArchive.Open( strDecFile ) )
        throw MT_ScipioException( "MOS_AgentManager::Initialize", __FILE__, __LINE__, "Field SCIPIO::DIA::Camps does not exist", decArchive.RetrieveLastError()->GetInfo() );
    if( !decArchive.Section( "Decisionnel" ) )
        throw MT_ScipioException( "MOS_AgentManager::Initialize", __FILE__, __LINE__, "Field SCIPIO::DIA::Camps does not exist", decArchive.RetrieveLastError()->GetInfo() );
    if( !decArchive.Section( "DirectIA" ) )
        throw MT_ScipioException( "MOS_AgentManager::Initialize", __FILE__, __LINE__, "Field SCIPIO::DIA::Camps does not exist", decArchive.RetrieveLastError()->GetInfo() );
    std::string strModelFile;
    if( !decArchive.ReadField( "Modeles", strModelFile ) )
        throw MT_ScipioException( "MOS_AgentManager::Initialize", __FILE__, __LINE__, "Field SCIPIO::DIA::Camps does not exist", decArchive.RetrieveLastError()->GetInfo() );
    decArchive.EndSection(); // Decisionnel
    decArchive.EndSection(); // DirectIA
    decArchive.Close();

    std::string strModelsDir;
    MT_ExtractFilePath( strDecFile, strModelsDir );    

    MT_XXmlInputArchive modelArchive;
    if( !modelArchive.Open( strModelsDir + strModelFile ) )
        throw MT_ScipioException( "MOS_AgentManager::Initialize", __FILE__, __LINE__, "" );
    
    if( !modelArchive.Section( "Modeles" ) )
        throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, "List 'Models' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );

    if( !modelArchive.BeginList( "Pions" ) )
        throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, "List 'Models' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );

    while( modelArchive.NextListElement() )
    {
        if( ! modelArchive.Section( "Modele" ) )
            throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, "Section 'Models::Model' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );

        std::string strModelName;
        if( ! modelArchive.ReadAttribute( "nom", strModelName ) )
            throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, "Section 'Models::Model' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );

        IT_Model_Vector found = std::find_if( vAvailableModels_.begin(), vAvailableModels_.end(), MOS_AgentModel::Cmp( strModelName ) );
        if( found != vAvailableModels_.end() )
            throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, MT_FormatString( "Le modèle %s existe déjà.", strModelName.c_str() ).c_str() );

        MOS_AgentModel* pModel = new MOS_AgentModel( false, strModelName );
        vAvailableModels_.push_back( pModel );
        pModel->Initialize( modelArchive );
        modelArchive.EndSection(); // Model
    }
    modelArchive.EndList();     // Pions

    if( !modelArchive.BeginList( "Automates" ) )
        throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, "List 'Models' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );

    while( modelArchive.NextListElement() )
    {
        if( ! modelArchive.Section( "Modele" ) )
            throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, "Section 'Models::Model' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );

        std::string strModelName;
        if( ! modelArchive.ReadAttribute( "nom", strModelName ) )
            throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, "Section 'Models::Model' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );

        IT_Model_Vector found = std::find_if( vAvailableModels_.begin(), vAvailableModels_.end(), MOS_AgentModel::Cmp( strModelName ) );
        if( found != vAvailableModels_.end() )
            throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, MT_FormatString( "Le modèle %s existe déjà.", strModelName.c_str() ).c_str() );

        MOS_AgentModel* pModel = new MOS_AgentModel( true, strModelName );
        vAvailableModels_.push_back( pModel );
        pModel->Initialize( modelArchive );
        modelArchive.EndSection(); // Model
    }
    modelArchive.EndList();     // Automates

    modelArchive.EndSection(); // Modeles

    modelArchive.Close();    
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::Terminate
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_AgentManager::Terminate()
{

}

//=============================================================================
// AGENTS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterGtia
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentManager::RegisterGtia( MOS_Gtia& gtia )
{
    bool bOut = gtiaMap_.insert( std::make_pair( gtia.GetID(), &gtia ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RemoveAllGtias
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentManager::RemoveAllGtias()
{
    for( IT_GtiaMap itGtia = gtiaMap_.begin(); itGtia != gtiaMap_.end(); ++itGtia )
        delete itGtia->second;
    gtiaMap_.clear();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterTeam
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentManager::RegisterTeam( MOS_Team& team )
{
    bool bOut = teamMap_.insert( std::make_pair( team.GetID(), &team ) ).second;
    assert( bOut );

    teams_.push_back( &team );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RemoveAllTeams
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentManager::RemoveAllTeams()
{
    for( IT_TeamMap itTeam = teamMap_.begin(); itTeam != teamMap_.end(); ++itTeam )
        delete itTeam->second;
    teamMap_.clear();
    teams_.clear();
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterAgent
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_AgentManager::RegisterAgent( MOS_Agent& agent )
{
    bool bOut = agentMap_.insert( std::make_pair( agent.GetAgentID(), &agent ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::UnregisterAgent
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_AgentManager::UnregisterAgent( MOS_Agent& agent )
{
    uint nOut = agentMap_.erase( agent.GetAgentID() );
    assert( nOut == 1 );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::RemoveAllAgents
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
void MOS_AgentManager::RemoveAllAgents()
{
    while( agentMap_.size() )
    {
        MOS_Agent& agent = *agentMap_.begin()->second;
        MOS_App::GetApp().GetMainWindow().NotifyDeleteAgent( agent );

        for( CIT_AgentListViewSet itListView = agentListViewSet_.begin() ; itListView != agentListViewSet_.end() ; ++itListView )
            agent.DeleteListView( **itListView );

        delete &agent;
    }
    agentMap_.clear();
    MOS_App::GetApp().GetMainWindow().GetHistory().Reset();

}

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindAgent
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_Agent* MOS_AgentManager::FindAgent( MIL_AgentID nAgentID )
{
    IT_AgentMap itAgent = agentMap_.find( nAgentID );
    if( itAgent == agentMap_.end() )
        return 0;

    return itAgent->second;
}


//=============================================================================
// DRAW
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::Draw
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_AgentManager::Draw()
{
    for( IT_AgentMap itAgent = agentMap_.begin(); itAgent != agentMap_.end(); ++itAgent )
        itAgent->second->Draw();
    for( IT_GtiaMap itGtia = gtiaMap_.begin(); itGtia != gtiaMap_.end(); ++itGtia )
        itGtia->second->Draw();
    for( IT_TeamMap itTeam = teamMap_.begin(); itTeam != teamMap_.end(); ++itTeam )
        itTeam->second->Draw();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::DrawVision
// Created: FBD 03-02-12
//-----------------------------------------------------------------------------
void MOS_AgentManager::DrawVision()
{
    for( IT_AgentMap itAgent = agentMap_.begin(); itAgent != agentMap_.end(); ++itAgent )
        itAgent->second->DrawVision();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::DrawVisionSurface
// Created: FBD 03-02-12
// Modified: JVT 2004-09-27
//-----------------------------------------------------------------------------
void MOS_AgentManager::DrawVisionSurface()
{
    MOS_Agent* pSelectedAgent = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().GetSelectedAgent();

    if ( pSelectedAgent )
        pSelectedAgent->DrawVisionSurface();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::DrawVisionCone
// Created: FBD 03-02-12
//-----------------------------------------------------------------------------
void MOS_AgentManager::DrawVisionCone()
{
    for( IT_AgentMap itAgent = agentMap_.begin(); itAgent != agentMap_.end(); ++itAgent )
        itAgent->second->DrawVisionCone();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterAgentListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_AgentManager::RegisterAgentListView( MOS_AgentListView& agentListView )
{
    agentListViewSet_.insert( &agentListView );

    for( CIT_AgentMap itAgent = agentMap_.begin() ; itAgent != agentMap_.end() ; ++itAgent )
        itAgent->second->RegisterListView( agentListView );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::UnregisterAgentListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_AgentManager::UnregisterAgentListView( MOS_AgentListView& agentListView )
{
    agentListViewSet_.erase( &agentListView );

    for( CIT_AgentMap itAgent = agentMap_.begin() ; itAgent != agentMap_.end() ; ++itAgent )
        itAgent->second->UnregisterListView( agentListView );
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterListViewsForAgent
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_AgentManager::RegisterListViewsForAgent( MOS_Agent& agent )
{
    for( CIT_AgentListViewSet itListView = agentListViewSet_.begin() ; itListView != agentListViewSet_.end() ; ++itListView )
        agent.RegisterListView( **itListView );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindModel
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
const MOS_AgentModel* MOS_AgentManager::FindModel( const std::string& strModelName ) const
{
    CIT_Model_Vector found = std::find_if( vAvailableModels_.begin(), vAvailableModels_.end(), MOS_AgentModel::Cmp( strModelName ) );
    if( found == vAvailableModels_.end() )
        return 0;
    else
        return (*found);
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindTypePion
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
const MOS_TypePion* MOS_AgentManager::FindTypePion( const std::string& strName ) const
{
    for( CIT_TypePionMap it = typesPion_.begin(); it != typesPion_.end(); ++it )
    {
        const MOS_TypePion& typePion = *it->second;
        if( typePion.GetName() == strName )
            return &typePion;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindTypePion
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
const MOS_TypePion* MOS_AgentManager::FindTypePion( uint nID ) const
{
    CIT_TypePionMap it = typesPion_.find( nID );
    if( it == typesPion_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindTypeAutomate
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
const MOS_TypeAutomate* MOS_AgentManager::FindTypeAutomate( uint nID ) const
{
    CIT_TypeAutomateMap it = typesAutomate_.find( nID );
    if( it == typesAutomate_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentManager::RegisterMaintenanceConsign( MOS_LogMaintenanceConsign& consign )
{
    bool bOut = maintenanceConsigns_.insert( std::make_pair( consign.GetID(), &consign ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::UnregisterMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentManager::UnregisterMaintenanceConsign( MOS_LogMaintenanceConsign& consign )
{
    int nOut = maintenanceConsigns_.erase( consign.GetID() );
    assert( nOut == 1 );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMaintenanceConsign* MOS_AgentManager::FindMaintenanceConsign( uint nID ) const
{
    CIT_MaintenanceConsignMap it = maintenanceConsigns_.find( nID );
    if( it == maintenanceConsigns_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentManager::RegisterSupplyConsign( MOS_LogSupplyConsign& consign )
{
    bool bOut = supplyConsigns_.insert( std::make_pair( consign.GetID(), &consign ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::UnregisterSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentManager::UnregisterSupplyConsign( MOS_LogSupplyConsign& consign )
{
    int nOut = supplyConsigns_.erase( consign.GetID() );
    assert( nOut == 1 );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogSupplyConsign* MOS_AgentManager::FindSupplyConsign( uint nID ) const
{
    CIT_SupplyConsignMap it = supplyConsigns_.find( nID );
    if( it == supplyConsigns_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentManager::RegisterMedicalConsign( MOS_LogMedicalConsign& consign )
{
    bool bOut = medicalConsigns_.insert( std::make_pair( consign.GetID(), &consign ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::UnregisterMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentManager::UnregisterMedicalConsign( MOS_LogMedicalConsign& consign )
{
    int nOut = medicalConsigns_.erase( consign.GetID() );
    assert( nOut == 1 );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMedicalConsign* MOS_AgentManager::FindMedicalConsign( uint nID ) const
{
    CIT_MedicalConsignMap it = medicalConsigns_.find( nID );
    if( it == medicalConsigns_.end() )
        return 0;
    return it->second;
}
