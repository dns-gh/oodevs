// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ObjectKnowledge.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const OrderParameter& parameter, Controller& controller )
    : Knowledge_ABC< ObjectKnowledge_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const OrderParameter& parameter, unsigned long id, const ObjectKnowledgeConverter_ABC& converter,
                                  const Entity_ABC& owner, Controller& controller, const kernel::EntityResolver_ABC& entities )
    : Knowledge_ABC< ObjectKnowledge_ABC >( parameter, converter.Find( entities.GetObject( id ), owner ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const OrderParameter& parameter, const ObjectKnowledge_ABC* knowledge, Controller& controller )
    : Knowledge_ABC< ObjectKnowledge_ABC >( parameter, knowledge, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Accept
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        Knowledge_ABC< ObjectKnowledge_ABC >::CommitTo( *message.mutable_value()->Add()->mutable_objectknowledge() );
}
// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        Knowledge_ABC< ObjectKnowledge_ABC >::CommitTo( *message.mutable_objectknowledge() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::CommitTo( sword::ObjectKnowledgeId& message ) const
{
    Knowledge_ABC< ObjectKnowledge_ABC >::CommitTo< sword::ObjectKnowledgeId >( message );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string ObjectKnowledge::SerializeType() const
{
    return "objectknowledge";
}
