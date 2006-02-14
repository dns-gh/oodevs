// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectKnowledges.h"
#include "Controller.h"
#include "ObjectKnowledge.h"
#include "ObjectKnowledgeFactory.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledges::ObjectKnowledges( Controller& controller, ObjectKnowledgeFactory& factory )
    : controller_( controller )
    , factory_( factory )
{

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledges::~ObjectKnowledges()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::Update( const ASN1T_MsgObjectKnowledgeCreation& message )
{
    if( ! Find( message.oid_connaissance ) )
        Register( message.oid_connaissance, * factory_.Create( message ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::Update( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    Get( message.oid_connaissance ).Update( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::Update( const ASN1T_MsgObjectKnowledgeDestruction& message )
{
    delete Find( message.oid_connaissance );
    Remove( message.oid_connaissance );
    controller_.Update( *this );
}
