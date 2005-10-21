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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Gtia.cpp $
// $Author: Ape $
// $Modtime: 24/11/04 10:03 $
// $Revision: 7 $
// $Workfile: MOS_Gtia.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_Gtia.h"

#include "MOS_Team.h"
#include "MOS_App.h"
#include "MOS_Agent.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_Population.h"
#include "MOS_PopulationKnowledge.h"
#include "MOS_AgentManager.h"

MOS_IDManager MOS_Gtia::idManager_( 0 );

// -----------------------------------------------------------------------------
// Name: MOS_Gtia constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_Gtia::MOS_Gtia( bool bGenerateId )
    : pTeam_    ( 0 )
    , nID_      ( bGenerateId ? idManager_.GetFreeIdentifier() : 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
MOS_Gtia::MOS_Gtia( uint32 nId )
    : pTeam_    ( 0 )
    , nID_      ( nId )
{
    idManager_.LockIdentifier( nID_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_Gtia::~MOS_Gtia()
{
    for( CIT_AgentKnowledgeMap it = agentKnowledges_.begin(); it != agentKnowledges_.end(); ++it )
    {
        pTeam_->RemoveAgentKnowledge( *(it->second) );
        delete it->second;
    }

    idManager_.ReleaseIdentifier( nID_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::ReadODB
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_Gtia::ReadODB( MOS_InputArchive& archive )
{
    archive.Section( "GroupeConnaissance" );
    archive.ReadAttribute( "id", nID_ );
    idManager_.LockIdentifier( nID_ );
    archive.ReadAttribute( "type", strType_ );
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::WriteODB
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_Gtia::WriteODB( MT_OutputArchive_ABC& archive )
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
// Name: MOS_Gtia::OnReceiveMsgUnitKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Gtia::OnReceiveMsgUnitKnowledgeCreation( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
{
    if( agentKnowledges_.find( asnMsg.oid_connaissance ) != agentKnowledges_.end() )
        return;

    MOS_AgentKnowledge* pAgentKnowledge = new MOS_AgentKnowledge( asnMsg, *this );
    agentKnowledges_.insert( std::make_pair( pAgentKnowledge->GetID(), pAgentKnowledge ) );
    pTeam_->RegisterAgentKnowledge( *pAgentKnowledge );

    //$$$$ Pas terrible, je tente plutot de limiter les appels au notifications de
    //$$$$ MOS_App dans le code reseau mais bon...
    MOS_App::GetApp().NotifyAgentKnowledgeCreated( *this, *pAgentKnowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::OnReceiveMsgUnitKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Gtia::OnReceiveMsgUnitKnowledgeUpdate( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    IT_AgentKnowledgeMap itAgentKnowledge = agentKnowledges_.find( asnMsg.oid_connaissance );
    assert( itAgentKnowledge != agentKnowledges_.end() );

    itAgentKnowledge->second->Update( asnMsg );

    //$$$$ Pas terrible, je tente plutot de limiter les appels au notifications de
    //$$$$ MOS_App dans le code reseau mais bon...
    MOS_App::GetApp().NotifyAgentKnowledgeUpdated( *this, *(itAgentKnowledge->second) );
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::OnReceiveMsgUnitKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Gtia::OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg )
{
    IT_AgentKnowledgeMap itAgentKnowledge = agentKnowledges_.find( asnMsg.oid_connaissance );
    assert( itAgentKnowledge != agentKnowledges_.end() );

    //$$$$ Pas terrible, je tente plutot de limiter les appels au notifications de
    //$$$$ MOS_App dans le code reseau mais bon...
    MOS_App::GetApp().NotifyAgentKnowledgeDeleted( *this, *(itAgentKnowledge->second) );

    pTeam_->RemoveAgentKnowledge( *(itAgentKnowledge->second) );
    delete itAgentKnowledge->second;
    agentKnowledges_.erase( itAgentKnowledge );
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::FindAgentKnowledge
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
MOS_AgentKnowledge* MOS_Gtia::FindAgentKnowledge( int nId )
{
    IT_AgentKnowledgeMap it = agentKnowledges_.find( nId );    
    if( it != agentKnowledges_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::FindKnowledgeOnAgent
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
MOS_AgentKnowledge* MOS_Gtia::FindKnowledgeOnAgent( const MOS_Agent& agent )
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
// Name: MOS_Gtia::OnReceiveMsgPopulationKnowledgeCreation
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void MOS_Gtia::OnReceiveMsgPopulationKnowledgeCreation( const ASN1T_MsgPopulationKnowledgeCreation& asnMsg )
{
    if( populationKnowledges_.find( asnMsg.oid_connaissance ) != populationKnowledges_.end() )
        return;

    MOS_PopulationKnowledge* pKnowledge = new MOS_PopulationKnowledge( asnMsg );
    populationKnowledges_.insert( std::make_pair( pKnowledge->GetID(), pKnowledge ) );
    MOS_App::GetApp().NotifyPopulationKnowledgeCreated( *this, *pKnowledge );
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::OnReceiveMsgPopulationKnowledgeUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void MOS_Gtia::OnReceiveMsgPopulationKnowledgeUpdate( const ASN1T_MsgPopulationKnowledgeUpdate& asnMsg )
{
    IT_PopulationKnowledgeMap it = populationKnowledges_.find( asnMsg.oid_connaissance );
    assert( it != populationKnowledges_.end() );
    it->second->Update( asnMsg );
    MOS_App::GetApp().NotifyPopulationKnowledgeUpdated( *this, *( it->second ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::OnReceiveMsgPopulationKnowledgeDestruction
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void MOS_Gtia::OnReceiveMsgPopulationKnowledgeDestruction( const ASN1T_MsgPopulationKnowledgeDestruction& asnMsg )
{
    IT_PopulationKnowledgeMap it = populationKnowledges_.find( asnMsg.oid_connaissance );
    assert( it != populationKnowledges_.end() );
    MOS_App::GetApp().NotifyPopulationKnowledgeDeleted( *this, *( it->second ) );
    delete it->second;
    populationKnowledges_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::FindPopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
MOS_PopulationKnowledge* MOS_Gtia::FindPopulationKnowledge( int nId )
{
    CIT_PopulationKnowledgeMap it = populationKnowledges_.find( nId );    
    if( it != populationKnowledges_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::FindKnowledgeOnPopulation
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
MOS_PopulationKnowledge* MOS_Gtia::FindKnowledgeOnPopulation( const MOS_Population& population )
{
    for( CIT_PopulationKnowledgeMap it = populationKnowledges_.begin(); it != populationKnowledges_.end(); ++it )
        if( &( it->second->GetPopulation() ) == &population )
            return it->second;
    return 0;
}