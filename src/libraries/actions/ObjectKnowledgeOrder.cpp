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
// Name: ObjectKnowledgeOrder::CheckKnowledgeValidity
// Created: LGY 2011-07-08
// -----------------------------------------------------------------------------
bool ObjectKnowledgeOrder::CheckKnowledgeValidity() const
{
    return converter_.Find( object_, owner_ ) ? true : false;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::CommitTo
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
void ObjectKnowledgeOrder::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( false );
    const kernel::ObjectKnowledge_ABC* pKnowledge = converter_.Find( object_, owner_ );
    if( pKnowledge )
    {
        sword::ObjectKnowledgeId& id = *message.mutable_value()->Add()->mutable_objectknowledge();
        id.set_id( pKnowledge->GetId() );
    }
}
