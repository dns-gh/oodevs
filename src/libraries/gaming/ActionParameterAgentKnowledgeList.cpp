// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterAgentKnowledgeList.h"
#include "ActionParameterAgentKnowledge.h"
#include "ActionParameterVisitor_ABC.h"
#include "AgentKnowledge_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledgeList::ActionParameterAgentKnowledgeList( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledgeList::ActionParameterAgentKnowledgeList( const OrderParameter& parameter, const ASN1T_UnitKnowledgeList& asn, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterAgentKnowledge( tools::translate( "ActionParameter", "Agent knowledge %1" ).arg( i + 1 ), asn.elem[i], converter, owner ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledgeList::ActionParameterAgentKnowledgeList( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterAgentKnowledgeList::ReadAgentKnowledge, resolver, converter, owner );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledgeList destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledgeList::~ActionParameterAgentKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledgeList::ReadAgentKnowledge
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterAgentKnowledgeList::ReadAgentKnowledge( xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
{
    AddParameter( *new ActionParameterAgentKnowledge( xis, resolver, converter, owner ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_UnitKnowledgeList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterAgentKnowledge& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_UnitKnowledgeList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledgeList::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterAgentKnowledgeList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_unitKnowledgeList;
    ASN1T_UnitKnowledgeList*& list = asn.value.u.unitKnowledgeList = new ASN1T_UnitKnowledgeList();
    asn.null_value = ( list->n = Count() ) ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_UnitKnowledge[list->n];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledgeList::Clean
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterAgentKnowledgeList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.unitKnowledgeList )
        delete[] asn.value.u.unitKnowledgeList->elem;
    delete asn.value.u.unitKnowledgeList;
}
