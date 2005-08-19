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

#include "Tester_pch.h"
#include "KnowledgeGroup.h"
#include "Knowledges/PawnKnowledge.h"
#include "Team.h"
#include "Pawn.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::KnowledgeGroup
// Created: SBO 2005-05-12
//-----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( const EntityManager& entityManager, T_EntityId nSimId, DIN::DIN_Input& input )
    : nId_           ( nSimId )
    , knownPawns_    ()
    , pTeam_         ( 0 )
    , entityManager_ ( entityManager )
{
    uint32 nTeamId;
    input >> nTeamId;
    pTeam_ = entityManager_.FindTeam( nTeamId );
    assert( pTeam_ != 0 );
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
        if( it->second->GetRealPawn() && it->second->GetRealPawn()->GetAutomat().GetTeam().IsAnEnemy( *pTeam_ ) == eTristate_True )
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
        if( it->second->GetRealPawn() && it->second->GetRealPawn()->GetAutomat().GetTeam().IsAnEnemy( *pTeam_ ) == eTristate_True )
            return it->second->GetId();
    return 0;
}
