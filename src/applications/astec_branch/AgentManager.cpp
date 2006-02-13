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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentManager.cpp $
// $Author: Age $
// $Modtime: 14/04/05 12:41 $
// $Revision: 13 $
// $Workfile: AgentManager.cpp $
// 
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "astec_pch.h"
#include "AgentManager.h"

#include "Agent.h"
#include "App.h"
#include "ObjectKnowledge.h"
#include "AgentModel.h"
#include "Gtia.h"
#include "Team.h"
#include "TypePion.h"
#include "TypeAutomate.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "TypeComposante.h"
#include "TypePopulation.h"
#include "Population.h"

#include "MT_Tools/MT_ScipioException.h"

#   include "AgentManager.inl"

//-----------------------------------------------------------------------------
// Name: AgentManager constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
AgentManager::AgentManager()
{
}


//-----------------------------------------------------------------------------
// Name: AgentManager destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
AgentManager::~AgentManager()
{
    DeleteAllConflicts();
    MT_DELETEOWNED( vAvailableModels_ );
}


//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentManager::Initialize
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void AgentManager::Initialize()
{
    InputArchive scipioArchive;

    scipioArchive.Open( "scipio.xml" );
    scipioArchive.Section( "Scipio" );
    scipioArchive.Section( "Donnees" );

    InitializeModels         ( scipioArchive );
    InitializeTypesComposante( scipioArchive );
    InitializeTypesPion      ( scipioArchive );
    InitializeTypesAutomate  ( scipioArchive );
	InitializeTypesPopulation( scipioArchive );

    scipioArchive.EndSection(); // Donnees
    scipioArchive.EndSection(); // Scipio
    scipioArchive.Close();
}



// -----------------------------------------------------------------------------
// Name: AgentManager::InitializeTypesPion
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentManager::InitializeTypesPion( InputArchive& scipioArchive )
{
    std::string strTypesPionFile;
    scipioArchive.ReadField( "Pions", strTypesPionFile );

    InputArchive archive;
    archive.Open( strTypesPionFile );

    archive.BeginList( "Pions" );
    while( archive.NextListElement() )
    {
        archive.Section( "Unite" );
        std::string strName;
        archive.ReadAttribute( "nom", strName );
        TypePion* pTypePion = new TypePion( strName, archive );
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
// Name: AgentManager::InitializeTypesAutomate
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentManager::InitializeTypesAutomate( InputArchive& scipioArchive )
{
    std::string strTypesAutomateFile;
    scipioArchive.ReadField( "Automates", strTypesAutomateFile );

    InputArchive archive;
    archive.Open( strTypesAutomateFile );
    archive.BeginList( "Automates" );
    while( archive.NextListElement() )
    {
        archive.Section( "Unite" );
        std::string strName;
        archive.ReadAttribute( "nom", strName );

        TypeAutomate* pTypeAutomate = new TypeAutomate( strName, archive );
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
// Name: AgentManager::InitializeModels
// Created: AGE 2012-12-22
// -----------------------------------------------------------------------------
void AgentManager::ReadModelList( InputArchive& modelArchive, bool bAutomata , bool popu )
{
    modelArchive.Section( "Modele" );
    std::string strModelName;
    modelArchive.ReadAttribute( "nom", strModelName );
    IT_ModelVector found = std::find_if( vAvailableModels_.begin(), vAvailableModels_.end(), AgentModel::Cmp( strModelName ) );
    if( found != vAvailableModels_.end() )
        throw MT_ScipioException( "DEC_Workspace::Initialize", __FILE__, __LINE__, MT_FormatString( "Le modèle %s existe déjà.", strModelName.c_str() ).c_str() );

    AgentModel* pModel = new AgentModel( bAutomata, strModelName );
    vAvailableModels_.push_back( pModel );
    pModel->Initialize( modelArchive, popu );
    modelArchive.EndSection(); // Modele
};

// -----------------------------------------------------------------------------
// Name: AgentManager::InitializeModels
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
void AgentManager::InitializeModels( InputArchive& scipioArchive )
{    
    // Modeles
    std::string strDecFile;
    scipioArchive.ReadField( "Decisionnel", strDecFile );
    InputArchive decArchive;
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

    InputArchive modelArchive;
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

    modelArchive.BeginList( "Populations" );
    while( modelArchive.NextListElement() )
        ReadModelList( modelArchive, false, true );
    modelArchive.EndList();     // Populations

    modelArchive.EndSection(); // Modeles
    modelArchive.Close();    
}


// -----------------------------------------------------------------------------
// Name: AgentManager::InitializeTypesComposantes
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
void AgentManager::InitializeTypesComposante( InputArchive& scipioArchive )
{
    std::string strTypesComposantesFile;
    scipioArchive.ReadField( "Composantes", strTypesComposantesFile );

    InputArchive archive;
    archive.Open( strTypesComposantesFile );
    archive.BeginList( "Composantes" );
    
    while( archive.NextListElement() )
    {
        archive.Section( "Composante" );

        std::string strName;
        archive.ReadAttribute( "nom", strName );

        TypeComposante* pTypeComposante = new TypeComposante( strName, archive );
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

// -----------------------------------------------------------------------------
// Name: AgentManager::InitializeTypesPopulation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentManager::InitializeTypesPopulation( InputArchive& scipioArchive )
{
    std::string strTypesPopFile;
    scipioArchive.ReadField( "Populations", strTypesPopFile );

    InputArchive archive;
    archive.Open( strTypesPopFile );
    archive.BeginList( "Populations" );
    
    while( archive.NextListElement() )
    {
		archive.Section( "Population" );
        std::string strName;
        archive.ReadAttribute( "nom", strName );

        TypePopulation* pTypePopulation = new TypePopulation( strName, archive );
		bool bOut = typesPopulation_.insert( std::make_pair( pTypePopulation->GetID(), pTypePopulation ) ).second;
        if( !bOut )
        {
            std::stringstream strOutputMsg;
            strOutputMsg << "Cet id de type de population est utilisé plusieurs fois: " << pTypePopulation->GetID();
            MT_LOG_INFO( strOutputMsg.str().c_str(), eDefault, 0 );
            delete pTypePopulation;
        }

        archive.EndSection(); // Population
    }
    archive.EndList(); // Populations

    archive.Close();
}

//=============================================================================
// TEAMS
//=============================================================================


// -----------------------------------------------------------------------------
// Name: AgentManager::DeleteAllConflicts
// Created: SBO 2006-01-17
// -----------------------------------------------------------------------------
void AgentManager::DeleteAllConflicts()
{
    conflictMap_.clear();
}

// -----------------------------------------------------------------------------
// Name: AgentManager::FindModel
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
AgentModel* AgentManager::FindModel( const std::string& strModelName ) const
{
    for( CIT_ModelVector it = vAvailableModels_.begin(); it != vAvailableModels_.end(); ++it )
        if( (*it)->GetName() == strModelName )
            return (*it);

    return 0;
}


// -----------------------------------------------------------------------------
// Name: AgentManager::FindTypePion
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
const TypePion* AgentManager::FindTypePion( const std::string& strName ) const
{
    for( CIT_TypePionMap it = typesPion_.begin(); it != typesPion_.end(); ++it )
    {
        const TypePion& typePion = *it->second;
        if( typePion.GetName() == strName )
            return &typePion;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::FindTypeAutomate
// Created: AGE 2005-04-14
// -----------------------------------------------------------------------------
const TypeAutomate* AgentManager::FindTypeAutomate( const std::string& strName ) const
{
    for( CIT_TypeAutomateMap it = typesAutomate_.begin(); it != typesAutomate_.end(); ++it )
    {
        const TypeAutomate& typeAutomate = *it->second;
        if( typeAutomate.GetName() == strName )
            return &typeAutomate;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::FindTypeComposante
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
const TypeComposante* AgentManager::FindTypeComposante( const std::string& strName ) const
{
    for( CIT_TypeComposanteMap it = typesComposante_.begin(); it != typesComposante_.end(); ++it )
    {
        const TypeComposante& typeComposante = *it->second;
        if( typeComposante.GetName() == strName )
            return &typeComposante;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::FindTypePopulation
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
const TypePopulation* AgentManager::FindTypePopulation( const std::string& strName ) const
{
    for( CIT_TypePopulationMap it = typesPopulation_.begin(); it != typesPopulation_.end(); ++it )
    {
        const TypePopulation& typePopulation = *it->second;
        if( typePopulation.GetName() == strName )
            return &typePopulation;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::FindTypePion
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
const TypePion* AgentManager::FindTypePion( uint nID ) const
{
    CIT_TypePionMap it = typesPion_.find( nID );
    if( it == typesPion_.end() )
        return 0;
    return it->second;
}


// -----------------------------------------------------------------------------
// Name: AgentManager::FindTypeAutomate
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
const TypeAutomate* AgentManager::FindTypeAutomate( uint nID ) const
{
    CIT_TypeAutomateMap it = typesAutomate_.find( nID );
    if( it == typesAutomate_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::FindTypeComposante
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
const TypeComposante* AgentManager::FindTypeComposante( uint nID ) const
{
    CIT_TypeComposanteMap it = typesComposante_.find( nID );
    if( it == typesComposante_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::FindTypePopulation
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
const TypePopulation* AgentManager::FindTypePopulation( uint nID ) const
{
    CIT_TypePopulationMap it = typesPopulation_.find( nID );
    if( it == typesPopulation_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::ReadODB
/** @param  archive 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void AgentManager::ReadODB( InputArchive& archive )
{
    // Clear the teams that were created during initialization.
    // $$$$ AGE 2006-02-13: 
//    archive.BeginList( "Armees" );
//    while( archive.NextListElement() )
//    {
//        std::auto_ptr<Team> spTeam( new Team() );
//        spTeam->ReadODB( archive, true );
//        this->RegisterTeam( *(spTeam.release()));
//    }
//    archive.EndList();
//
//    archive.BeginList( "Automates" );
//    while( archive.NextListElement())
//    {
//        std::auto_ptr<Agent> spAgent( new Agent() );
//        spAgent->ReadODB( archive, true );
//        this->AddAgent( *(spAgent.release()));
//    }
//    archive.EndList();
//
//    archive.BeginList( "Pions" );
//    while( archive.NextListElement())
//    {
//        std::auto_ptr<Agent> spAgent( new Agent() );
//        spAgent->ReadODB( archive, false );
//        this->AddAgent( *(spAgent.release()));
//    }
//    archive.EndList();
//
//    archive.BeginList( "Populations");
//    while( archive.NextListElement())
//    {
//        std::auto_ptr<Population> spAgent( new Population() );
//        spAgent->ReadODB( archive );
//        this->AddPopulation( *(spAgent.release()));
//    }
//    archive.EndList();
}


// -----------------------------------------------------------------------------
// Name: AgentManager::WriteODB
/** @param  archive 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void AgentManager::WriteODB( MT_XXmlOutputArchive& archive )
{
    // $$$$ AGE 2006-02-13: 
//    archive.BeginList( "Armees", teamMap_.size() );
//    for( IT_TeamMap itTeam = teamMap_.begin(); itTeam != teamMap_.end(); ++itTeam )
//    {
//        (*itTeam).second->WriteODB( archive );
//    }
//    archive.EndList();
//
//    uint nNbrAutomates = 0;
//    IT_AgentMap it;
//    for( it = agentMap_.begin(); it != agentMap_.end(); ++it )
//        if( (*it).second->IsAutomate() )
//            ++nNbrAutomates;
//
//    archive.BeginList( "Automates", nNbrAutomates );
//    for( it = agentMap_.begin(); it != agentMap_.end(); ++it )
//        if( (*it).second->IsAutomate() )
//            (*it).second->WriteODB( archive );
//    archive.EndList();
//
//    archive.BeginList( "Pions", agentMap_.size() - nNbrAutomates );
//    for( it = agentMap_.begin(); it != agentMap_.end(); ++it )
//        if( ! (*it).second->IsAutomate() )
//            (*it).second->WriteODB( archive );
//    archive.EndList();
//
//    archive.BeginList( "Populations", populationMap_.size() );
//    for( IT_PopulationMap itPop = populationMap_.begin(); itPop != populationMap_.end(); ++itPop )
//        (*itPop).second->WriteODB( archive );
//    archive.EndList();
}

