// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Automat.cpp $
// $Author: Sbo $
// $Modtime: 21/07/05 17:02 $
// $Revision: 6 $
// $Workfile: Automat.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"
#include "Entities/Team.h"
#include "Entities/KnowledgeGroup.h"

using namespace TIC;

Automat::T_AutomatMap   Automat::automats_;
Automat::T_EntityIdSet  Automat::disaggregAutomats_;

//-----------------------------------------------------------------------------
// Name: Automat::Automat
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Automat::Automat( const ASN1T_MsgAutomateCreation& asnMsg )
    : nId_              ( asnMsg.oid_automate )
    , pTeam_            ( Team::Find( asnMsg.oid_camp ) )
    , pKnowledgeGroup_  ( KnowledgeGroup::Find( asnMsg.oid_groupe_connaissance ) )
    , bConfigDisaggreg_ ( disaggregAutomats_.find( nId_ ) != disaggregAutomats_.end() )
{
    assert( pTeam_ );
    assert( pKnowledgeGroup_ );

    // create associated PC pawn
    Pawn* pPawn = new Pawn( asnMsg, *this );
    Pawn::Register( *pPawn );
    pPc_ = pPawn;
}

//-----------------------------------------------------------------------------
// Name: Automat::~Automat
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Automat::~Automat()
{
    childPawns_.clear();
}

//-----------------------------------------------------------------------------
// Name: Automat::Initialize
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void Automat::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: Automat::Terminate
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void Automat::Terminate()
{
    for( CIT_AutomatMap it = automats_.begin(); it != automats_.end(); ++it )
        delete it->second;
    automats_.clear();
    disaggregAutomats_.clear();
}

//-----------------------------------------------------------------------------
// Name: Automat::AttachPawn
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
void Automat::AttachPawn( Pawn& pawn )
{
    childPawns_.insert( &pawn );
}

//-----------------------------------------------------------------------------
// Name: Automat::DetachPawn
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
void Automat::DetachPawn( Pawn& pawn )
{
	childPawns_.erase( &pawn );
}

//-----------------------------------------------------------------------------
// Name: Automat::OnKnowledgeGroupChanged
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
void Automat::OnKnowledgeGroupChanged( KnowledgeGroup& knowledgeGroup )
{
    pKnowledgeGroup_ = &knowledgeGroup;
    /*
    // check if disaggregation status must be changed
    if( pKnowledgeGroup_->MustBeDisaggregated() != knowledgeGroup.MustBeDisaggregated() )
    {
        pKnowledgeGroup_ = &knowledgeGroup;
        if( pKnowledgeGroup_->MustBeDisaggregated() )
            for( CIT_PawnSet it = childPawns_.begin(); it != childPawns_.end(); ++it )
                ( *it )->Disaggregate();
        else
            for( CIT_PawnSet it = childPawns_.begin(); it != childPawns_.end(); ++it )
                // if no disaggregation specified in in config file
                // and pawn is not in a disaggregation area, then aggregate pawn
                if( !( *it )->MustBeDisaggregated() && !( *it )->IsInDisaggregatedArea() )
                    ( *it )->Aggregate();
    }
    // if disaggregation status has not changed, just update automat's knowledge group
    else
        pKnowledgeGroup_ = &knowledgeGroup;
    */
}