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

#include "Tester_pch.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"
#include "Entities/Team.h"
#include "Entities/KnowledgeGroup.h"
#include "Entities/EntityManager.h"
#include "Types/TypeManager.h"
#include "Types/Entities/AutomatType.h"
#include "Types/Entities/AutomatModel.h"
#include "Workspace.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: Automat::Automat
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Automat::Automat( const Workspace& workspace, const ASN1T_MsgAutomateCreation& asnMsg )
    : Testable_Entity  ( workspace )
    , nId_             ( asnMsg.oid_automate )
    , pTeam_           ( workspace.GetEntityManager().FindTeam( asnMsg.oid_camp ) )
    , pKnowledgeGroup_ ( workspace.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_connaissance ) )
    , pType_           ( workspace.GetTypeManager().FindAutomatType( asnMsg.type_automate ) )
{
    assert( pTeam_ );
    assert( pKnowledgeGroup_ );

    // create associated PC pawn
    Pawn& pawn = *new Pawn( workspace, asnMsg, *this );
    workspace.GetEntityManager().Register( pawn );
    pPc_ = &pawn;
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

// -----------------------------------------------------------------------------
// Name: Automat::ScheduleAllMissions
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void Automat::ScheduleAllMissions( Scheduler& scheduler, uint nIteration /* = 1 */ )
{
    assert( pType_ );
    pType_->GetModel().ScheduleAllMissions( *this, scheduler, nIteration );
}

// -----------------------------------------------------------------------------
// Name: Automat::ScheduleMission
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void Automat::ScheduleMission( Scheduler& scheduler, const std::string& strMissionName, uint nIteration /* = 1 */ )
{
    assert( pType_ );
    pType_->GetModel().ScheduleMission( *this, scheduler, strMissionName, nIteration );
}
