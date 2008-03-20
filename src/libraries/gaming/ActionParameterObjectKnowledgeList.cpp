// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterObjectKnowledgeList.h"
#include "ActionParameterObjectKnowledge.h"
#include "ActionParameterVisitor_ABC.h"
#include "ObjectKnowledge_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterObjectKnowledgeList::ActionParameterObjectKnowledgeList( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterObjectKnowledgeList::ActionParameterObjectKnowledgeList( const OrderParameter& parameter, const ASN1T_ObjectKnowledgeList& asn, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterObjectKnowledge( OrderParameter( tools::translate( "ActionParameter", "Object knowledge %1" ).arg( i + 1 ).ascii(), "objectknowledge", false ), asn.elem[i], converter, owner ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterObjectKnowledgeList::ActionParameterObjectKnowledgeList( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterObjectKnowledgeList::ReadObjectKnowledge, resolver, converter, owner );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledgeList destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterObjectKnowledgeList::~ActionParameterObjectKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledgeList::ReadObjectKnowledge
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterObjectKnowledgeList::ReadObjectKnowledge( xml::xistream& xis, const Resolver_ABC< Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
{
    AddParameter( *new ActionParameterObjectKnowledge( xis, resolver, converter, owner ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_ObjectKnowledgeList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterObjectKnowledge& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_ObjectKnowledgeList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledgeList::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterObjectKnowledgeList::CommitTo( ASN1T_MissionParameter& asn ) const
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
// Name: ActionParameterObjectKnowledgeList::Clean
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterObjectKnowledgeList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.objectKnowledgeList )
        delete[] asn.value.u.objectKnowledgeList->elem;
    delete asn.value.u.objectKnowledgeList;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledgeList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameterObjectKnowledgeList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    ActionParameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledgeList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool ActionParameterObjectKnowledgeList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
