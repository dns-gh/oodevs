// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

#include "simulation_tester_pch.h"
#include "ObjectKnowledge.h"
#include "Entities/Object.h"
#include "Entities/EntityManager.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const EntityManager& entityManager, const MsgObjectKnowledgeCreation& asnMsg, const Team& team )
    : owner_       ( team )
    , nId_         ( asnMsg.oid_connaissance() ) 
    , pRealObject_ ( asnMsg.oid_objet_reel() == 0 ? 0 : entityManager.FindObject( asnMsg.oid_objet_reel() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    // NOTHING
}