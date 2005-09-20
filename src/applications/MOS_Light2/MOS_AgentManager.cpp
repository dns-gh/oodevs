//*****************************************************************************
// 
// This file is part of MASA DIN library.
// 
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
// 
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
// 
//*****************************************************************************
// 
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentManager.cpp $
// $Author: Age $
// $Modtime: 14/04/05 12:41 $
// $Revision: 13 $
// $Workfile: MOS_AgentManager.cpp $
// 
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_AgentManager.h"

#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_AgentModel.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"
#include "MOS_TypePion.h"
#include "MOS_TypeAutomate.h"
#include "MOS_LogMaintenanceConsign.h"
#include "MOS_LogMedicalConsign.h"
#include "MOS_LogSupplyConsign.h"
#include "MOS_TypeComposante.h"

#include "MT_Tools/MT_ScipioException.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentManager.inl"
#endif


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
    DeleteAllAgents();
    DeleteAllTeams();

    MT_DELETEOWNED( vAvailableModels_ );
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
    MOS_InputArchive scipioArchive;

    scipioArchive.Open( "scipio.xml" );
    scipioArchive.Section( "Scipio" );
    scipioArchive.Section( "Donnees" );

    InitializeModels         ( scipioArchive );
    InitializeTypesComposante( scipioArchive );
    InitializeTypesPion      ( scipioArchive );
    InitializeTypesAutomate  ( scipioArchive );

    scipioArchive.EndSection(); // Donnees
    scipioArchive.EndSection(); // Scipio
    scipioArchive.Close();
}



// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::InitializeTypesPion
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentManager::InitializeTypesPion( MOS_InputArchive& scipioArchive )
{
    std::string strTypesPionFile;
    scipioArchive.ReadField( "Pions", strTypesPionFile );

    MOS_InputArchive archive;
    archive.Open( strTypesPionFile );

    archive.BeginList( "Pions" );
    while( archive.NextListElement() )
    {
        archive.Section( "Unite" );
        std::string strName;
        archive.ReadAttribute( "nom", strName );
        MOS_TypePion* pTypePion = new MOS_TypePion( strName, archive );
        bool bOut = typesPion_.insert( std::make_pair( pTypePion->GetID(), pTypePion ) ).second;
        if( !bOut )
        {
            std::stringstream strOutputMsg;
            strOutputMsg << "Cet id de type de pion est utilisé plusieurs fois: " << pTypePion->GetID();
            MT_LOG_INFO( strOutputMsg.str().c_str(), eDefault, 0 );
            delete pTypePion;
        }

        archive.EndSection(); // Unite
    }
    archive.EndList(); // Pions

    archive.Close();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::InitializeTypesAutomate
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentManager::InitializeTypesAutomate( MOS_InputArchive& scipioArchive )
{
    std::string strTypesAutomateFile;
    scipioArchive.ReadField( "Automates", strTypesAutomateFile );

    MOS_InputArchive archive;
    archive.Open( strTypesAutomateFile );
    archive.BeginList( "Automates" );
    while( archive.NextListElement() )
    {
        archive.Section( "Unite" );
        std::string strName;
        archive.ReadAttribute( "nom", strName );

        MOS_TypeAutomate* pTypeAutomate = new MOS_TypeAutomate( strName, archive );
        bool bOut = typesAutomate_.insert( std::make_pair( pTypeAutomate->GetID(), pTypeAutomate ) ).second;
        if( !bOut )
        {
            std::stringstream strOutputMsg;
            strOutputMsg << "Cet id de type d'automate est utilisé plusieurs fois: " << pTypeAutomate->GetID();
            MT_LOG_INFO( strOutputMsg.str().c_str(), eDefault, 0 );
            delete pTypeAutomate;
        }

        archive.EndSection(); // Unite
    }
    archive.EndList(); // Automates

    archive.Close();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::InitializeModels
// Created: AGE 2012-12-22
// -----------------------------------------------------------------------------
void MOS_AgentManager::ReadModelList( MOS_InputArchive& modelArchive, bool bAutomata )
{
    modelArchive.Section( "Modele" );
    std::string strModelName;
    modelArchive.ReadAttribute( "nom", strModelName );
    IT_ModelVector found = std::find_if( vAvailableModels_.begin(), vAvailableModels_.end(), MOS_AgentModel::Cmp( strModelName ) );
    if( found != vAvailableModels_.end() )
        throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, MT_FormatString( "Le modèle %s existe déjà.", strModelName.c_str() ).c_str() );

    MOS_AgentModel* pModel = new MOS_AgentModel( bAutomata, strModelName );
    vAvailableModels_.push_back( pModel );
    pModel->Initialize( modelArchive );
    modelArchive.EndSection(); // Modele
};

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::InitializeModels
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
void MOS_AgentManager::InitializeModels( MOS_InputArchive& scipioArchive )
{    
    // Modeles
    std::string strDecFile;
    scipioArchive.ReadField( "Decisionnel", strDecFile );
    MOS_InputArchive decArchive;
    decArchive.Open( strDecFile );
    decArchive.Section( "Decisionnel" );
    decArchive.Section( "DirectIA" );
    std::string strModelFile;
    decArchive.ReadField( "Modeles", strModelFile );
    decArchive.EndSection(); // Decisionnel
    decArchive.EndSection(); // DirectIA
    decArchive.Close();

    std::string strModelsDir;
    MT_ExtractFilePath( strDecFile, strModelsDir );    

    MOS_InputArchive modelArchive;
    modelArchive.Open( strModelsDir + strModelFile );
    modelArchive.Section( "Modeles" );
    modelArchive.BeginList( "Pions" );

    while( modelArchive.NextListElement() )
        ReadModelList( modelArchive, false );
    modelArchive.EndList();     // Pions

    modelArchive.BeginList( "Automates" );
    while( modelArchive.NextListElement() )
        ReadModelList( modelArchive, true );

    modelArchive.EndList();     // Automates
    modelArchive.EndSection(); // Modeles
    modelArchive.Close();    
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::InitializeTypesComposantes
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
void MOS_AgentManager::InitializeTypesComposante( MOS_InputArchive& scipioArchive )
{
    std::string strTypesComposantesFile;
    scipioArchive.ReadField( "Composantes", strTypesComposantesFile );

    MOS_InputArchive archive;
    archive.Open( strTypesComposantesFile );
    archive.BeginList( "Composantes" );
    
    while( archive.NextListElement() )
    {
        archive.Section( "Composante" );

        std::string strName;
        archive.ReadAttribute( "nom", strName );

        MOS_TypeComposante* pTypeComposante = new MOS_TypeComposante( strName, archive );
        bool bOut = typesComposante_.insert( std::make_pair( pTypeComposante->GetID(), pTypeComposante ) ).second;
        if( !bOut )
        {
            std::stringstream strOutputMsg;
            strOutputMsg << "Cet id de type de composante est utilisé plusieurs fois: " << pTypeComposante->GetID();
            MT_LOG_INFO( strOutputMsg.str().c_str(), eDefault, 0 );
            delete pTypeComposante;
        }

        archive.EndSection(); // Composante
    }
    archive.EndList(); // Composantes

    archive.Close();
}

//=============================================================================
// TEAMS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterTeam
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentManager::RegisterTeam( MOS_Team& team )
{
    team.SetIdx( teamMap_.size() );
    bool bOut = teamMap_.insert( std::make_pair( team.GetID(), &team ) ).second;
    assert( bOut );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::DeleteAllTeams
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentManager::DeleteAllTeams()
{
    for( IT_TeamMap itTeam = teamMap_.begin(); itTeam != teamMap_.end(); ++itTeam )
        delete itTeam->second;
    teamMap_.clear();
}

//=============================================================================
// AGENTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::AddAgent
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_AgentManager::AddAgent( MOS_Agent& agent )
{
    bool bOut = agentMap_.insert( std::make_pair( agent.GetAgentID(), &agent ) ).second;
    assert( bOut );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RemoveAgent
/** @param  agent 
*/
// Created: APE 2004-08-27
// -----------------------------------------------------------------------------
void MOS_AgentManager::RemoveAgent( MOS_Agent& agent )
{
    IT_AgentMap it = agentMap_.find( agent.GetAgentID() );
    assert( it != agentMap_.end() );
    agentMap_.erase( it );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::DeleteAllAgents
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
void MOS_AgentManager::DeleteAllAgents()
{
    for( IT_AgentMap it = agentMap_.begin(); it != agentMap_.end(); ++it )
        delete it->second;

    agentMap_.clear();
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


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindModel
/** @param  strModelName 
    @return 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
MOS_AgentModel* MOS_AgentManager::FindModel( const std::string& strModelName ) const
{
    for( CIT_ModelVector it = vAvailableModels_.begin(); it != vAvailableModels_.end(); ++it )
        if( (*it)->GetName() == strModelName )
            return (*it);

    return 0;
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
// Name: MOS_AgentManager::FindTypeAutomate
// Created: AGE 2005-04-14
// -----------------------------------------------------------------------------
const MOS_TypeAutomate* MOS_AgentManager::FindTypeAutomate( const std::string& strName ) const
{
    for( CIT_TypeAutomateMap it = typesAutomate_.begin(); it != typesAutomate_.end(); ++it )
    {
        const MOS_TypeAutomate& typeAutomate = *it->second;
        if( typeAutomate.GetName() == strName )
            return &typeAutomate;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindTypeComposante
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
const MOS_TypeComposante* MOS_AgentManager::FindTypeComposante( const std::string& strName ) const
{
    for( CIT_TypeComposanteMap it = typesComposante_.begin(); it != typesComposante_.end(); ++it )
    {
        const MOS_TypeComposante& typeComposante = *it->second;
        if( typeComposante.GetName() == strName )
            return &typeComposante;
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
// Name: MOS_AgentManager::FindTypeComposante
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
const MOS_TypeComposante* MOS_AgentManager::FindTypeComposante( uint nID ) const
{
    CIT_TypeComposanteMap it = typesComposante_.find( nID );
    if( it == typesComposante_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::ReadODB
/** @param  archive 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_AgentManager::ReadODB( MOS_InputArchive& archive )
{
    // Clear the teams that were created during initialization.
    teamMap_.clear();

    archive.BeginList( "Armees" );
    while( archive.NextListElement() )
    {
        std::auto_ptr<MOS_Team> spTeam( new MOS_Team() );
        spTeam->ReadODB( archive, true );
        this->RegisterTeam( *(spTeam.release()));
    }
    archive.EndList();

    archive.BeginList( "Automates" );
    while( archive.NextListElement())
    {
        std::auto_ptr<MOS_Agent> spAgent( new MOS_Agent() );
        spAgent->ReadODB( archive, true );
        this->AddAgent( *(spAgent.release()));
    }
    archive.EndList();

    archive.BeginList( "Pions" );
    while( archive.NextListElement())
    {
        std::auto_ptr<MOS_Agent> spAgent( new MOS_Agent() );
        spAgent->ReadODB( archive, false );
        this->AddAgent( *(spAgent.release()));
    }
    archive.EndList();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::WriteODB
/** @param  archive 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_AgentManager::WriteODB( MT_XXmlOutputArchive& archive )
{
    archive.BeginList( "Armees", teamMap_.size() );
    for( IT_TeamMap itTeam = teamMap_.begin(); itTeam != teamMap_.end(); ++itTeam )
    {
        (*itTeam).second->WriteODB( archive );
    }
    archive.EndList();

    uint nNbrAutomates = 0;
    IT_AgentMap it;
    for( it = agentMap_.begin(); it != agentMap_.end(); ++it )
        if( (*it).second->IsAutomate() )
            ++nNbrAutomates;

    archive.BeginList( "Automates", nNbrAutomates );
    for( it = agentMap_.begin(); it != agentMap_.end(); ++it )
        if( (*it).second->IsAutomate() )
            (*it).second->WriteODB( archive );
    archive.EndList();

    archive.BeginList( "Pions", agentMap_.size() - nNbrAutomates );
    for( it = agentMap_.begin(); it != agentMap_.end(); ++it )
        if( ! (*it).second->IsAutomate() )
            (*it).second->WriteODB( archive );
    archive.EndList();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::AddConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
template< typename T >
void MOS_AgentManager::AddConsign( std::map< ASN1T_OID, T* >& cont, T& consign )
{
    cont[ consign.GetID() ] = &consign;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::DeleteConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
template< typename T >
void MOS_AgentManager::DeleteConsign( std::map< ASN1T_OID, T* >& cont, ASN1T_OID nConsign )
{
    std::map< ASN1T_OID, T* >::iterator itConsign = cont.find( nConsign );
    if( itConsign != cont.end() )
    {
        delete itConsign->second;
        cont.erase( itConsign );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
template< typename T >
T* MOS_AgentManager::FindConsign( std::map< ASN1T_OID, T* >& cont, ASN1T_OID nConsign )
{
    std::map< ASN1T_OID, T* >::iterator itConsign = cont.find( nConsign );
    if( itConsign != cont.end() )
        return itConsign->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentManager::RegisterConsign( MOS_LogMaintenanceConsign& consign )
{
    AddConsign( maintenanceConsigns_, consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentManager::RegisterConsign( MOS_LogMedicalConsign& consign )
{
    AddConsign( medicalConsigns_, consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::RegisterConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentManager::RegisterConsign( MOS_LogSupplyConsign& consign )
{
    AddConsign( supplyConsigns_, consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindMaintenanceConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_LogMaintenanceConsign* MOS_AgentManager::FindMaintenanceConsign( ASN1T_OID nConsignId )
{
    return FindConsign( maintenanceConsigns_, nConsignId );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindMedicalConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_LogMedicalConsign* MOS_AgentManager::FindMedicalConsign( ASN1T_OID nConsignId )
{
    return FindConsign( medicalConsigns_, nConsignId );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindSupplyConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_LogSupplyConsign* MOS_AgentManager::FindSupplyConsign( ASN1T_OID nConsignId )
{
    return FindConsign( supplyConsigns_, nConsignId );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::DeleteMaintenanceConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentManager::DeleteMaintenanceConsign( ASN1T_OID nConsignId )
{
    DeleteConsign( maintenanceConsigns_, nConsignId );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::DeleteMedicalConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentManager::DeleteMedicalConsign( ASN1T_OID nConsignId )
{
    DeleteConsign( medicalConsigns_, nConsignId );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::DeleteSupplyConsign
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentManager::DeleteSupplyConsign( ASN1T_OID nConsignId )
{
    DeleteConsign( supplyConsigns_, nConsignId );
}

