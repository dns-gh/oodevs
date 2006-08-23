// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/KnowledgeGroup.cpp $
// $Author: Sbo $
// $Modtime: 21/07/05 17:04 $
// $Revision: 6 $
// $Workfile: KnowledgeGroup.cpp $
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "KnowledgeGroup.h"
#include "Knowledges/PawnKnowledge.h"
#include "Knowledges/PopulationKnowledge.h"
#include "Team.h"
#include "Pawn.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::KnowledgeGroup
// Created: SBO 2005-05-12
//-----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( const EntityManager& entityManager, Team& team, T_EntityId id )
    : nId_           ( id )
    , knownPawns_    ()
    , team_          ( team )
    , entityManager_ ( entityManager )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::~KnowledgeGroup
// Created: SBO 2005-05-12
//-----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    for( CIT_PawnKnowledgeMap it = knownPawns_.begin(); it != knownPawns_.end(); ++it )
        delete it->second;
    knownPawns_.clear();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::OnReceiveMsgUnitKnowledgeCreation
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void KnowledgeGroup::OnReceiveMsgUnitKnowledgeCreation( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
{
    if( knownPawns_.find( asnMsg.oid_connaissance ) != knownPawns_.end() )
        return;
    PawnKnowledge* pKnownPawn = new PawnKnowledge( entityManager_, asnMsg, *this );    
    bool bOut = knownPawns_.insert( std::make_pair( pKnownPawn->GetId(), pKnownPawn ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::OnReceiveMsgUnitKnowledgeUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void KnowledgeGroup::OnReceiveMsgUnitKnowledgeUpdate( const ASN1T_MsgUnitKnowledgeUpdate& /*asnMsg*/ )
{
    //$$$ TODO
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::OnReceiveMsgUnitKnowledgeDestruction
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void KnowledgeGroup::OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg )
{
    knownPawns_.erase( asnMsg.oid_connaissance );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::OnReceiveMsgPopulationKnowledgeCreation
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
void KnowledgeGroup::OnReceiveMsgPopulationKnowledgeCreation( const ASN1T_MsgPopulationKnowledgeCreation& asnMsg )
{
    if( knownPopulations_.find( asnMsg.oid_connaissance ) != knownPopulations_.end() )
        return;
    PopulationKnowledge* pKnownPopulation = new PopulationKnowledge( entityManager_, asnMsg, *this );
    bool bOut = knownPopulations_.insert( std::make_pair( pKnownPopulation->GetId(), pKnownPopulation ) ).second;
    assert( bOut );
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::OnReceiveMsgPopulationKnowledgeUpdate
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
void KnowledgeGroup::OnReceiveMsgPopulationKnowledgeUpdate( const ASN1T_MsgPopulationKnowledgeUpdate& /*asnMsg*/ )
{
    //$$$ TODO
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::OnReceiveMsgPopulationKnowledgeDestruction
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
void KnowledgeGroup::OnReceiveMsgPopulationKnowledgeDestruction( const ASN1T_MsgPopulationKnowledgeDestruction& asnMsg )
{
    knownPopulations_.erase( asnMsg.oid_connaissance );
}

// -----------------------------------------------------------------------------
// TEST PARAMETERS
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetTestParam_Knowledges
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
T_IdVector& KnowledgeGroup::GetTestParam_Knowledges( uint nNbr ) const
{
    // return the list of known enemies
    T_IdVector& targets = *new T_IdVector();
    for( CIT_PawnKnowledgeMap it = knownPawns_.begin(); nNbr > 0 && it != knownPawns_.end(); ++it )
        if( it->second->GetRealPawn() && it->second->GetRealPawn()->GetAutomat()->GetTeam().IsAnEnemy( team_ ) == eTristate_True )
        {
            targets.push_back( it->second->GetId() );
            --nNbr;
        }
    return targets;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetTestParam_Knowledge
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_EntityId KnowledgeGroup::GetTestParam_Knowledge() const
{
    // return the first enemy known
    for( CIT_PawnKnowledgeMap it = knownPawns_.begin(); it != knownPawns_.end(); ++it )
        if( it->second->GetRealPawn() && it->second->GetRealPawn()->GetAutomat()->GetTeam().IsAnEnemy( team_ ) == eTristate_True )
            return it->second->GetId();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetTestParam_KnowledgePopulation
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
T_EntityId KnowledgeGroup::GetTestParam_KnowledgePopulation() const
{
    if( knownPopulations_.size() > 0 )
        return knownPopulations_.begin()->first;
    return 0;
}
