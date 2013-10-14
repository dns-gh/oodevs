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
#include "clients_kernel/TacticalHierarchies.h"
#include "protocol/Protocol.h"

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder constructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
ObjectKnowledgeOrder::ObjectKnowledgeOrder( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver,
                                            const kernel::ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller )
    : ObjectKnowledge( parameter, controller )
    , resolver_( resolver )
    , converter_( converter )
    , owner_( owner )
    , objectId_( xis.attribute< unsigned long >( "value", 0 ) )
{
    if( objectId_ != 0 && resolver.FindObject( objectId_ ) == 0 )
        throw MASA_EXCEPTION( "Unknown parameter : 'Invalid object id' " );
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
// Name: ObjectKnowledgeOrder::RetrieveId
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
unsigned long ObjectKnowledgeOrder::RetrieveId() const
{
    const kernel::Object_ABC* object = resolver_.FindObject( objectId_ );
    if( object )
    {
        const kernel::ObjectKnowledge_ABC* pKnowledge = converter_.Find( *object, owner_ );
        if( pKnowledge )
            return pKnowledge->GetEntityId();
        else
        {
            const kernel::Hierarchies* hierarchies = owner_.Retrieve< kernel::TacticalHierarchies >();
            if( hierarchies )
            {
                pKnowledge = converter_.Find( *object, hierarchies->GetTop() );
                if( pKnowledge )
                    return pKnowledge->GetEntityId();
            }
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::Serialize
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
void ObjectKnowledgeOrder::Serialize( xml::xostream& xos ) const
{
    Parameter< const kernel::ObjectKnowledge_ABC* >::Serialize( xos );
    unsigned long id = RetrieveId();
    if( id != 0 )
        xos << xml::attribute( "value", id );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::CommitTo
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
void ObjectKnowledgeOrder::CommitTo( sword::MissionParameter& message ) const
{
    unsigned long id = RetrieveId();
    message.set_null_value( id == 0 );
    if( id != 0 )
        message.add_value()->mutable_objectknowledge()->set_id( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::CommitTo
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
void ObjectKnowledgeOrder::CommitTo( sword::MissionParameter_Value& message ) const
{
    unsigned long id = RetrieveId();
    if( id != 0 )
        message.mutable_objectknowledge()->set_id( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::CheckKnowledgeValidity
// Created: LGY 2011-07-08
// -----------------------------------------------------------------------------
bool ObjectKnowledgeOrder::CheckKnowledgeValidity() const
{
    if( !resolver_.FindObject( objectId_ ) )
        return true;
    return IsSet();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::IsSet
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
bool ObjectKnowledgeOrder::IsSet() const
{
    return RetrieveId() != 0;
}
