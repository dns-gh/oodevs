// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ObjectKnowledgeOrder.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
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
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder constructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
ObjectKnowledgeOrder::ObjectKnowledgeOrder( const OrderParameter& parameter, xml::xistream& xis, const EntityResolver_ABC& resolver,
                                            const ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner, Controller& controller )
    : ObjectKnowledge( parameter, 0, controller )
    , converter_( converter )
    , owner_    ( owner )
    , object_   ( resolver.GetObject( ReadId( xis ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder destructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
ObjectKnowledgeOrder::~ObjectKnowledgeOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::CommitTo
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
void ObjectKnowledgeOrder::CommitTo( sword::MissionParameter& message ) const
{
    const kernel::ObjectKnowledge_ABC* pKnowledge = converter_.Find( object_, owner_ );
    if( pKnowledge )
    {
        message.set_null_value( false );
        sword::ObjectKnowledgeId& id = *message.mutable_value()->Add()->mutable_objectknowledge();
        id.set_id( pKnowledge->GetEntity()->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::CommitTo
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
void ObjectKnowledgeOrder::CommitTo( sword::MissionParameter_Value& message ) const
{
    const kernel::ObjectKnowledge_ABC* pKnowledge = converter_.Find( object_, owner_ );
    if( pKnowledge )
        message.mutable_objectknowledge()->set_id( pKnowledge->GetEntity()->GetId() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::CheckKnowledgeValidity
// Created: LGY 2011-07-08
// -----------------------------------------------------------------------------
bool ObjectKnowledgeOrder::CheckKnowledgeValidity() const
{
    return converter_.Find( object_, owner_ ) ? true : false;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::IsSet
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
bool ObjectKnowledgeOrder::IsSet() const
{
    return CheckKnowledgeValidity();
}
