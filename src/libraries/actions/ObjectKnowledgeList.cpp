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
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "xeumeuleu/xml.h"

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
ObjectKnowledgeList::ObjectKnowledgeList( const OrderParameter& parameter, const ASN1T_ObjectKnowledgeList& asn, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ObjectKnowledge( OrderParameter( tools::translate( "Parameter", "Object knowledge %1" ).arg( i + 1 ).ascii(), "objectknowledge", false ), asn.elem[i], converter, owner, controller ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledgeList::ObjectKnowledgeList( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
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
void ObjectKnowledgeList::ReadObjectKnowledge( xml::xistream& xis, const Resolver_ABC< Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
{
    AddParameter( *new ObjectKnowledge( xis, resolver, converter, owner, controller ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_ObjectKnowledgeList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ObjectKnowledge& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_ObjectKnowledgeList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledgeList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_objectKnowledgeList;
    ASN1T_ObjectKnowledgeList*& list = asn.value.u.objectKnowledgeList = new ASN1T_ObjectKnowledgeList();
    list->n = Count();
    if( IsSet() )
    {
        list->elem = new ASN1T_ObjectKnowledge[list->n];
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeList::Clean
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledgeList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.objectKnowledgeList )
        delete[] asn.value.u.objectKnowledgeList->elem;
    delete asn.value.u.objectKnowledgeList;
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
