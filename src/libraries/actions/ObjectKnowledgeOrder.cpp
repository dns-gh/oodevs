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

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder constructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
ObjectKnowledgeOrder::ObjectKnowledgeOrder( const OrderParameter& parameter, xml::xistream& xis, const EntityResolver_ABC& resolver,
                                            const ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner, Controller& controller )
    : ObjectKnowledge( parameter, controller )
    , converter_( converter )
    , owner_    ( owner )
    , pObject_  ( 0 )
{
    if( xis.has_attribute( "value" ) )
    {
        pObject_ = resolver.FindObject( xis.attribute< unsigned long >( "value", 0u ) );
        if( !pObject_ )
            throw std::runtime_error( "Unknown parameter : 'Invalid object id' " );
    }
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
    if( pObject_ != 0 )
    {
        const kernel::ObjectKnowledge_ABC* pKnowledge = converter_.Find( *pObject_, owner_ );
        if( pKnowledge )
            return pKnowledge->GetEntity()->GetId();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::Serialize
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
void ObjectKnowledgeOrder::Serialize( xml::xostream& xos ) const
{
    ObjectKnowledge::Serialize( xos );
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
        message.mutable_value()->Add()->mutable_objectknowledge()->set_id( id );
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
    if( pObject_ == 0 )
        return true;
    return converter_.Find( *pObject_, owner_ ) != 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeOrder::IsSet
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
bool ObjectKnowledgeOrder::IsSet() const
{
    return RetrieveId() != 0;
}
