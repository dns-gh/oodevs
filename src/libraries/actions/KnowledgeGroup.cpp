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
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Entity< KnowledgeGroup_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: JSR 2010-04-19
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< KnowledgeGroup_ABC >( parameter, resolver.FindKnowledgeGroup( id ), controller )
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
void KnowledgeGroup::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() && GetValue() ) // $$$$ _RC_ SBO 2010-09-03: what if not ?
       message.add_value()->mutable_knowledgegroup()->set_id( GetValue()->GetId() );
}
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::CommitTo
// Created: JSR 2010-04-19
// -----------------------------------------------------------------------------
void KnowledgeGroup::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() && GetValue() ) // $$$$ _RC_ SBO 2010-09-03: what if not ?
        message.mutable_knowledgegroup()->set_id( GetValue()->GetId() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Accept
// Created: JSR 2010-04-19
// -----------------------------------------------------------------------------
void KnowledgeGroup::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string KnowledgeGroup::SerializeType() const
{
    return "knowledgegroup";
}
