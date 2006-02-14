// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Gtia.cpp $
// $Author: Ape $
// $Modtime: 24/11/04 10:03 $
// $Revision: 7 $
// $Workfile: Gtia.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Gtia.h"

#include "Team.h"
#include "App.h"
#include "Agent.h"
#include "AgentKnowledge.h"
#include "Population.h"
#include "PopulationKnowledge.h"
#include "AgentManager.h"

IDManager Gtia::idManager_( 0 );

// -----------------------------------------------------------------------------
// Name: Gtia constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Gtia::Gtia( bool bGenerateId )
    : pTeam_    ( 0 )
    , nID_      ( bGenerateId ? idManager_.GetFreeIdentifier() : 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Gtia constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
Gtia::Gtia( uint32 nId )
    : pTeam_    ( 0 )
    , nID_      ( nId )
{
    idManager_.LockIdentifier( nID_ );
}

// -----------------------------------------------------------------------------
// Name: Gtia destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Gtia::~Gtia()
{
    for( CIT_AgentKnowledgeMap it = agentKnowledges_.begin(); it != agentKnowledges_.end(); ++it )
    {
        pTeam_->RemoveAgentKnowledge( *(it->second) );
        delete it->second;
    }

    idManager_.ReleaseIdentifier( nID_ );
}

// -----------------------------------------------------------------------------
// Name: Gtia::ReadODB
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void Gtia::ReadODB( InputArchive& archive )
{
    archive.Section( "GroupeConnaissance" );
    archive.ReadAttribute( "id", nID_ );
    idManager_.LockIdentifier( nID_ );
    archive.ReadAttribute( "type", strType_ );
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: Gtia::WriteODB
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void Gtia::WriteODB( MT_OutputArchive_ABC& archive )
{
    archive.Section( "GroupeConnaissance" );
    archive.WriteAttribute( "id", nID_ );
    archive.WriteAttribute( "type", strType_ );
    archive.EndSection();
}


// =============================================================================
// NETWORK EVENTS
// =============================================================================

// =============================================================================
// AGENT KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
bool Gtia::Update( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
{
    if( agentKnowledges_.find( asnMsg.oid_connaissance ) != agentKnowledges_.end() )
        return false;

    AgentKnowledge* pAgentKnowledge = new AgentKnowledge( asnMsg, *this );
    agentKnowledges_.insert( std::make_pair( pAgentKnowledge->GetID(), pAgentKnowledge ) );
    pTeam_->RegisterAgentKnowledge( *pAgentKnowledge );

    //$$$$ Pas terrible, je tente plutot de limiter les appels au notifications de
    //$$$$ App dans le code reseau mais bon...
//    App::GetApp().NotifyAgentKnowledgeCreated( *this, *pAgentKnowledge );
    return true;
}


// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void Gtia::Update( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    IT_AgentKnowledgeMap itAgentKnowledge = agentKnowledges_.find( asnMsg.oid_connaissance );
    assert( itAgentKnowledge != agentKnowledges_.end() );

    itAgentKnowledge->second->Update( asnMsg );

    //$$$$ Pas terrible, je tente plutot de limiter les appels au notifications de
    //$$$$ App dans le code reseau mais bon...
//    App::GetApp().NotifyAgentKnowledgeUpdated( *this, *(itAgentKnowledge->second) );
}


// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void Gtia::Update( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg )
{
    IT_AgentKnowledgeMap itAgentKnowledge = agentKnowledges_.find( asnMsg.oid_connaissance );
    assert( itAgentKnowledge != agentKnowledges_.end() );

    //$$$$ Pas terrible, je tente plutot de limiter les appels au notifications de
    //$$$$ App dans le code reseau mais bon...
//    App::GetApp().NotifyAgentKnowledgeDeleted( *this, *(itAgentKnowledge->second) );

    pTeam_->RemoveAgentKnowledge( *(itAgentKnowledge->second) );
    delete itAgentKnowledge->second;
    agentKnowledges_.erase( itAgentKnowledge );
}

// -----------------------------------------------------------------------------
// Name: Gtia::UpdatePopulationKnowledg
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
template< typename T >
void Gtia::UpdatePopulationKnowledge( const T& message )
{
    populationKnowledges_[ message.oid_connaissance_population ]->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Gtia::Update( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Gtia::Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Gtia::Update( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Gtia::Update( const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Gtia::Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Gtia::Update( const ASN1T_MsgPopulationFluxKnowledgeDestruction& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Gtia::FindAgentKnowledge
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
AgentKnowledge* Gtia::FindAgentKnowledge( int nId )
{
    IT_AgentKnowledgeMap it = agentKnowledges_.find( nId );    
    if( it != agentKnowledges_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Gtia::FindKnowledgeOnAgent
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
AgentKnowledge* Gtia::FindKnowledgeOnAgent( const Agent& agent )
{
    for( IT_AgentKnowledgeMap it = agentKnowledges_.begin(); it != agentKnowledges_.end(); ++it )
        if( &(it->second->GetRealAgent()) == &agent )
            return it->second;
    return 0;
}

// =============================================================================
// POPULATION KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
bool Gtia::Update( const ASN1T_MsgPopulationKnowledgeCreation& asnMsg )
{
    if( populationKnowledges_.find( asnMsg.oid_connaissance ) != populationKnowledges_.end() )
        return false;

    PopulationKnowledge* pKnowledge = new PopulationKnowledge( asnMsg );
    populationKnowledges_.insert( std::make_pair( pKnowledge->GetID(), pKnowledge ) );
//    App::GetApp().NotifyPopulationKnowledgeCreated( *this, *pKnowledge );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void Gtia::Update( const ASN1T_MsgPopulationKnowledgeUpdate& asnMsg )
{
    IT_PopulationKnowledgeMap it = populationKnowledges_.find( asnMsg.oid_connaissance );
    assert( it != populationKnowledges_.end() );
    it->second->Update( asnMsg );
//    App::GetApp().NotifyPopulationKnowledgeUpdated( *this, *( it->second ) );
}

// -----------------------------------------------------------------------------
// Name: Gtia::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void Gtia::Update( const ASN1T_MsgPopulationKnowledgeDestruction& asnMsg )
{
    IT_PopulationKnowledgeMap it = populationKnowledges_.find( asnMsg.oid_connaissance );
    assert( it != populationKnowledges_.end() );
//    App::GetApp().NotifyPopulationKnowledgeDeleted( *this, *( it->second ) );
    delete it->second;
    populationKnowledges_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: Gtia::FindPopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge* Gtia::FindPopulationKnowledge( int nId )
{
    CIT_PopulationKnowledgeMap it = populationKnowledges_.find( nId );    
    if( it != populationKnowledges_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Gtia::FindKnowledgeOnPopulation
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
PopulationKnowledge* Gtia::FindKnowledgeOnPopulation( const Population& population )
{
    for( CIT_PopulationKnowledgeMap it = populationKnowledges_.begin(); it != populationKnowledges_.end(); ++it )
        if( &( it->second->GetPopulation() ) == &population )
            return it->second;
    return 0;
}