// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterAgentList.h"
#include "ActionParameterAgent.h"
#include "ActionParameterVisitor_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgentList::ActionParameterAgentList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgentList::ActionParameterAgentList( const OrderParameter& parameter, const ASN1T_ListAgent& asn, const Resolver_ABC< Agent_ABC >& resolver )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterAgent( tools::translate( "ActionParameter", "Agent %1" ).arg( i + 1 ), asn.elem[i], resolver ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgentList::ActionParameterAgentList( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterAgentList::ReadAgent, resolver );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList destructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgentList::~ActionParameterAgentList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList::ReadAgent
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAgentList::ReadAgent( xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver )
{
    AddParameter( *new ActionParameterAgent( xis, resolver ) );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_ListAgent& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterAgent& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_ListAgent* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAgentList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_listAgent;
    ASN1T_ListAgent*& list = asn.value.u.listAgent = new ASN1T_ListAgent();
    asn.null_value = ( list->n = Count() ) ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Agent[list->n];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList::Clean
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAgentList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listAgent )
        delete[] asn.value.u.listAgent->elem;
    delete asn.value.u.listAgent;
}
