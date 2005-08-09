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

#include "Tester_Pch.h"
#include "KnowledgeGroup.h"
#include "Knowledges/PawnKnowledge.h"
#include "Team.h"
#include "Pawn.h"

using namespace TEST;

KnowledgeGroup::T_KnowledgeGroupMap KnowledgeGroup::knowledgeGroups_;

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::KnowledgeGroup
// Created: SBO 2005-05-12
//-----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( T_EntityId nSimId, DIN::DIN_Input& input )
    : nId_        ( nSimId )
    , knownPawns_ ()
    , pTeam_      ( 0 )
{
    uint32 nTeamId;
    input >> nTeamId;
    pTeam_ = Team::Find( nTeamId );
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

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::Initialize
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void KnowledgeGroup::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::Terminate
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void KnowledgeGroup::Terminate()
{
    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        delete it->second;
    knowledgeGroups_.clear();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::OnReceiveMsgUnitKnowledgeCreation
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void KnowledgeGroup::OnReceiveMsgUnitKnowledgeCreation( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
{
    std::cout << "knownledge creation: " << std::endl;
    if( knownPawns_.find( asnMsg.oid_connaissance ) != knownPawns_.end() )
        return;
    PawnKnowledge* pKnownPawn = new PawnKnowledge( asnMsg, *this );    
    bool bOut = knownPawns_.insert( std::make_pair( pKnownPawn->GetId(), pKnownPawn ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::OnReceiveMsgUnitKnowledgeUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void KnowledgeGroup::OnReceiveMsgUnitKnowledgeUpdate( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    // TODO
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
// Name: KnowledgeGroup::GetTP_Targets
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
T_IdVector& KnowledgeGroup::GetTP_Targets() const
{
    T_IdVector* pTargets = new T_IdVector();
    for( CIT_PawnKnowledgeMap it = knownPawns_.begin(); it != knownPawns_.end(); ++it )
        if( it->second->GetRealPawn() && it->second->GetRealPawn()->GetAutomat().GetTeam().IsAnEnemy( *pTeam_ ) == eTristate_True )
            pTargets->push_back( it->second->GetId() );
    return *pTargets;
}
