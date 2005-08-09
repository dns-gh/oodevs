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
#include "ObjectKnowledge.h"
#include "Entities/Object.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& asnMsg, const Team& team )
    : owner_       ( team )
    , nId_         ( asnMsg.oid_connaissance ) 
    , pRealObject_ ( asnMsg.oid_objet_reel == 0 ? 0 : Object::Find( asnMsg.oid_objet_reel ) )
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