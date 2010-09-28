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
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "ObjectKnowledgeFactory.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledges::ObjectKnowledges( const Entity_ABC& owner, Controller& controller, ObjectKnowledgeFactory& factory )
    : Creatable< ObjectKnowledges >( controller, this )
    , owner_( &owner )
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
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeCreation& message )
{
    if( ! Find( message.knowledge().id() ) )
        Register( message.knowledge().id(), * factory_.Create( *owner_, message ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message )
{
    factory_.Register( Get( message.knowledge().id() ), message.attributes() );
    Get( message.knowledge().id() ).Update( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeDestruction& message )
{
    delete Find( message.knowledge().id() );
    Remove( message.knowledge().id() );
    controller_.Update( *this );
}
