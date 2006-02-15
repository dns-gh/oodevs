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
// $Archive: /MVW_v10/Build/SDK/Light2/src/KnowledgeGroup.cpp $
// $Author: Ape $
// $Modtime: 24/11/04 10:03 $
// $Revision: 7 $
// $Workfile: KnowledgeGroup.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "KnowledgeGroup.h"

#include "Team.h"
#include "App.h"
#include "Agent.h"
#include "AgentKnowledge.h"
#include "Population.h"
#include "PopulationKnowledge.h"
#include "AgentManager.h"

IDManager KnowledgeGroup::idManager_( 0 );

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( bool bGenerateId )
    : pTeam_    ( 0 )
    , nID_      ( bGenerateId ? idManager_.GetFreeIdentifier() : 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( uint32 nId )
    : pTeam_    ( 0 )
    , nID_      ( nId )
{
    idManager_.LockIdentifier( nID_ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    for( CIT_AgentKnowledgeMap it = agentKnowledges_.begin(); it != agentKnowledges_.end(); ++it )
    {
        pTeam_->RemoveAgentKnowledge( *(it->second) );
        delete it->second;
    }

    idManager_.ReleaseIdentifier( nID_ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::ReadODB
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void KnowledgeGroup::ReadODB( InputArchive& archive )
{
    archive.Section( "GroupeConnaissance" );
    archive.ReadAttribute( "id", nID_ );
    idManager_.LockIdentifier( nID_ );
    archive.ReadAttribute( "type", strType_ );
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::WriteODB
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void KnowledgeGroup::WriteODB( MT_OutputArchive_ABC& archive )
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
// Name: KnowledgeGroup::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
bool KnowledgeGroup::Update( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
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
// Name: KnowledgeGroup::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    IT_AgentKnowledgeMap itAgentKnowledge = agentKnowledges_.find( asnMsg.oid_connaissance );
    assert( itAgentKnowledge != agentKnowledges_.end() );

    itAgentKnowledge->second->Update( asnMsg );

    //$$$$ Pas terrible, je tente plutot de limiter les appels au notifications de
    //$$$$ App dans le code reseau mais bon...
//    App::GetApp().NotifyAgentKnowledgeUpdated( *this, *(itAgentKnowledge->second) );
}


// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg )
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
// Name: KnowledgeGroup::UpdatePopulationKnowledg
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
template< typename T >
void KnowledgeGroup::UpdatePopulationKnowledge( const T& message )
{
    populationKnowledges_[ message.oid_connaissance_population ]->Update( message );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgPopulationFluxKnowledgeDestruction& asnMsg )
{
    UpdatePopulationKnowledge( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::FindAgentKnowledge
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
AgentKnowledge* KnowledgeGroup::FindAgentKnowledge( int nId )
{
    IT_AgentKnowledgeMap it = agentKnowledges_.find( nId );    
    if( it != agentKnowledges_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::FindKnowledgeOnAgent
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
AgentKnowledge* KnowledgeGroup::FindKnowledgeOnAgent( const Agent& agent )
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
// Name: KnowledgeGroup::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
bool KnowledgeGroup::Update( const ASN1T_MsgPopulationKnowledgeCreation& asnMsg )
{
    if( populationKnowledges_.find( asnMsg.oid_connaissance ) != populationKnowledges_.end() )
        return false;

    PopulationKnowledge* pKnowledge = new PopulationKnowledge( asnMsg );
    populationKnowledges_.insert( std::make_pair( pKnowledge->GetID(), pKnowledge ) );
//    App::GetApp().NotifyPopulationKnowledgeCreated( *this, *pKnowledge );
    return true;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgPopulationKnowledgeUpdate& asnMsg )
{
    IT_PopulationKnowledgeMap it = populationKnowledges_.find( asnMsg.oid_connaissance );
    assert( it != populationKnowledges_.end() );
    it->second->Update( asnMsg );
//    App::GetApp().NotifyPopulationKnowledgeUpdated( *this, *( it->second ) );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgPopulationKnowledgeDestruction& asnMsg )
{
    IT_PopulationKnowledgeMap it = populationKnowledges_.find( asnMsg.oid_connaissance );
    assert( it != populationKnowledges_.end() );
//    App::GetApp().NotifyPopulationKnowledgeDeleted( *this, *( it->second ) );
    delete it->second;
    populationKnowledges_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::FindPopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge* KnowledgeGroup::FindPopulationKnowledge( int nId )
{
    CIT_PopulationKnowledgeMap it = populationKnowledges_.find( nId );    
    if( it != populationKnowledges_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::FindKnowledgeOnPopulation
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
PopulationKnowledge* KnowledgeGroup::FindKnowledgeOnPopulation( const Population& population )
{
    for( CIT_PopulationKnowledgeMap it = populationKnowledges_.begin(); it != populationKnowledges_.end(); ++it )
        if( &( it->second->GetPopulation() ) == &population )
            return it->second;
    return 0;
}