// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "KnowledgeGroup.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: JSR 2010-04-19
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( const kernel::OrderParameter& parameter, unsigned int id, const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& resolver, kernel::Controller& controller )
    : Entity< KnowledgeGroup_ABC >( parameter, &resolver.Get( id ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: JSR 2010-04-19
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( const kernel::OrderParameter& parameter, xml::xistream& xis, const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& resolver, kernel::Controller& controller )
    : Entity< KnowledgeGroup_ABC >( parameter, &resolver.Get( attribute< unsigned long >( xis, "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: JSR 2010-04-19
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( const kernel::OrderParameter& parameter, const kernel::KnowledgeGroup_ABC& group, kernel::Controller& controller )
    : Entity< KnowledgeGroup_ABC >( parameter, &group, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: JSR 2010-04-19
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::CommitTo
// Created: JSR 2010-04-19
// -----------------------------------------------------------------------------
void KnowledgeGroup::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() && GetValue())
        message.mutable_value()->mutable_knowledgegroup()->set_oid( GetValue()->GetId() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Accept
// Created: JSR 2010-04-19
// -----------------------------------------------------------------------------
void KnowledgeGroup::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

