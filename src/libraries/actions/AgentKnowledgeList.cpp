// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "AgentKnowledgeList.h"
#include "AgentKnowledge.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledgeList::AgentKnowledgeList( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledgeList::AgentKnowledgeList( const OrderParameter& parameter, const ASN1T_UnitKnowledgeList& asn, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new AgentKnowledge( OrderParameter( tools::translate( "Parameter", "Agent knowledge %1" ).arg( i + 1 ).ascii(), "agentknowledeg", false ), asn.elem[i], converter, owner, controller ) );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledgeList::AgentKnowledgeList( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &AgentKnowledgeList::ReadAgentKnowledge, resolver, converter, owner, controller );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledgeList::~AgentKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList::ReadAgentKnowledge
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledgeList::ReadAgentKnowledge( xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
{
    AddParameter( *new AgentKnowledge( xis, resolver, converter, owner, controller ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_UnitKnowledgeList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const AgentKnowledge& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_UnitKnowledgeList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledgeList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_unitKnowledgeList;
    ASN1T_UnitKnowledgeList*& list = asn.value.u.unitKnowledgeList = new ASN1T_UnitKnowledgeList();
    list->n = Count();
    if( IsSet() )
    {
        list->elem = new ASN1T_UnitKnowledge[list->n];
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList::Clean
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledgeList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.unitKnowledgeList )
        delete[] asn.value.u.unitKnowledgeList->elem;
    delete asn.value.u.unitKnowledgeList;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void AgentKnowledgeList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool AgentKnowledgeList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
