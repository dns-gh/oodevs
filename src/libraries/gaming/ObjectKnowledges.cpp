// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectKnowledges.h"
#include "clients_kernel/Controller.h"
#include "ObjectKnowledge.h"
#include "ObjectKnowledgeFactory.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledges::ObjectKnowledges( const Team_ABC& team, Controller& controller, ObjectKnowledgeFactory& factory )
    : team_( team )
    , controller_( controller )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledges::~ObjectKnowledges()
{
    controller_.Delete( *this );
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void ObjectKnowledges::DoUpdate( const kernel::InstanciationComplete& )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::DoUpdate( const ASN1T_MsgObjectKnowledgeCreation& message )
{
    if( ! Find( message.oid_connaissance ) )
        Register( message.oid_connaissance, * factory_.Create( team_, message ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    Get( message.oid_connaissance ).Update( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::DoUpdate( const ASN1T_MsgObjectKnowledgeDestruction& message )
{
    delete Find( message.oid_connaissance );
    Remove( message.oid_connaissance );
    controller_.Update( *this );
}
