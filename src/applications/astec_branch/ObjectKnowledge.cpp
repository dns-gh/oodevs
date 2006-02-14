//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectKnowledge.cpp $
// $Author: Ape $
// $Modtime: 4/11/04 11:15 $
// $Revision: 3 $
// $Workfile: ObjectKnowledge.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "ObjectKnowledge.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& message, const Resolver_ABC< Object_ABC >& resolver )
    : resolver_( resolver )
    , id_      ( message.oid_connaissance ) 
    , type_    ( message.type )
{
    // $$$$ AGE 2006-02-14: Team !
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    // NOTHING
}
