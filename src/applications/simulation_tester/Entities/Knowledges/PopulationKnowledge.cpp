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

#include "Tester_pch.h"
#include "PopulationKnowledge.h"
#include "Entities/Population.h"
#include "Entities/EntityManager.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge constructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const EntityManager& entityManager, const ASN1T_MsgPopulationKnowledgeCreation& asnMsg, const KnowledgeGroup& knowledgeGroup )
    : owner_     ( knowledgeGroup )
    , nId_       ( asnMsg.oid_connaissance ) 
    , pRealPopulation_ ( asnMsg.oid_population_reelle == 0 ? 0 : entityManager.FindPopulation( asnMsg.oid_population_reelle ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge destructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
PopulationKnowledge::~PopulationKnowledge()
{
    // NOTHING
}
