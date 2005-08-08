// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "PawnKnowledge.h"
#include "Pawn.h"
#include "KnowledgeGroup.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: PawnKnowledge constructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
PawnKnowledge::PawnKnowledge( const ASN1T_MsgUnitKnowledgeCreation& asnMsg, const KnowledgeGroup& knowledgeGroup )
    : owner_     ( knowledgeGroup )
    , nId_       ( asnMsg.oid_connaissance ) 
    , pRealPawn_ ( asnMsg.oid_unite_reelle == 0 ? 0 : Pawn::Find( asnMsg.oid_unite_reelle ) )
{
    // NOTHING
    std::cout << "group: " << knowledgeGroup.GetId() << " knows: " << asnMsg.oid_unite_reelle 
        << " from group: " << pRealPawn_->GetAutomat().GetKnowledgeGroup().GetId() << std::endl;
}

// -----------------------------------------------------------------------------
// Name: PawnKnowledge destructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
PawnKnowledge::~PawnKnowledge()
{
    // NOTHING
}
