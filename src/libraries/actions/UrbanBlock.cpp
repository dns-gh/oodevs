// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "UrbanBlock.h"
#include "clients_kernel/UrbanKnowledgeConverter_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

namespace
{
    unsigned long ReadId( xml::xistream& xis )
    {
        unsigned long id;
        xis >> xml::attribute( "value", id );
        return id;
    }

    std::string ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const OrderParameter& parameter, Controller& controller )
: Knowledge_ABC< UrbanKnowledge_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const OrderParameter& parameter, unsigned long id, UrbanKnowledgeConverter_ABC& converter, const Entity_ABC& owner, Controller& controller )
: Knowledge_ABC< UrbanKnowledge_ABC >( parameter, converter.Find( id, owner ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( xml::xistream& xis, UrbanKnowledgeConverter_ABC& converter, const Entity_ABC& owner, Controller& controller )
: Knowledge_ABC< UrbanKnowledge_ABC >( OrderParameter( ReadName( xis ), "UrbanBlock", false ), converter.Find( ReadId( xis ), owner ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const OrderParameter& parameter, xml::xistream& xis, UrbanKnowledgeConverter_ABC& converter, const Entity_ABC& owner, Controller& controller )
: Knowledge_ABC< UrbanKnowledge_ABC >( parameter, converter.Find( ReadId( xis ), owner ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::~UrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::Accept
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void UrbanBlock::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void UrbanBlock::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.mutable_value()->Add()->mutable_urbanknowledge() );
}
// -----------------------------------------------------------------------------
// Name: UrbanBlock::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void UrbanBlock::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        CommitTo( *message.mutable_urbanknowledge() );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void UrbanBlock::CommitTo( sword::UrbanObjectKnowledgeId& message ) const
{
    Entity< UrbanKnowledge_ABC >::CommitTo< sword::UrbanObjectKnowledgeId >( message );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::ThrowInvalidKnowledge
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void UrbanBlock::ThrowInvalidKnowledge() const
{
    throw std::exception( tools::translate( "Parameter", "Invalid object knowledge." ).ascii() );
}
