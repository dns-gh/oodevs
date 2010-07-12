// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ObjectKnowledgeList.h"
#include "ObjectKnowledge.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace xml;
using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledgeList::ObjectKnowledgeList( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledgeList::ObjectKnowledgeList( const OrderParameter& parameter, const Common::MsgObjectKnowledgeList& message, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new ObjectKnowledge( OrderParameter( tools::translate( "Parameter", "Object knowledge %1" ).arg( i + 1 ).ascii(), "objectknowledge", false ), message.elem().Get(i).oid(), converter, owner, controller ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledgeList::ObjectKnowledgeList( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ObjectKnowledgeList::ReadObjectKnowledge, resolver, converter, owner, controller );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledgeList::~ObjectKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList::ReadObjectKnowledge
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledgeList::ReadObjectKnowledge( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
{
    AddParameter( *new ObjectKnowledge( xis, resolver, converter, owner, controller ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( Common::MsgObjectKnowledgeList& message ) : message_( &message ) {}
        virtual void Visit( const ObjectKnowledge& param )
        {
            param.CommitTo( *message_->add_elem() );
        }
        Common::MsgObjectKnowledgeList* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledgeList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    Common::MsgObjectKnowledgeList* list = message.mutable_value()->mutable_objectknowledgelist();
    if( IsSet() )
    {
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList::Clean
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledgeList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_objectknowledgelist() )
        message.mutable_value()->clear_objectknowledgelist();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ObjectKnowledgeList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool ObjectKnowledgeList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
