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

#include "Tester_Pch.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"
#include "Entities/Team.h"
#include "Entities/KnowledgeGroup.h"

using namespace TEST;

Automat::T_AutomatMap   Automat::automats_;

//-----------------------------------------------------------------------------
// Name: Automat::Automat
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Automat::Automat( const ASN1T_MsgAutomateCreation& asnMsg )
    : nId_              ( asnMsg.oid_automate )
    , pTeam_            ( Team::Find( asnMsg.oid_camp ) )
    , pKnowledgeGroup_  ( KnowledgeGroup::Find( asnMsg.oid_groupe_connaissance ) )
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
}