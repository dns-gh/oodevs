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
ActionParameterAgentList::ActionParameterAgentList( const OrderParameter& parameter, const ASN1T_UnitList& asn, const Resolver_ABC< Agent_ABC >& resolver )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterAgent( OrderParameter( tools::translate( "ActionParameter", "Agent %1" ).arg( i + 1 ).ascii(), "agent", false ), asn.elem[i], resolver ) );
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
    struct StringSerializer : public ActionParameterVisitor_ABC
    {
        explicit StringSerializer( std::string& content ) : content_( &content ) {}
        virtual void Visit( const ActionParameterAgent& param )
        {
            if( !content_->empty() )
                *content_ += ",";
            param.CommitTo( *content_ );
        }
        std::string* content_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList::CommitTo
// Created: AGE 2007-10-03
// -----------------------------------------------------------------------------
void ActionParameterAgentList::CommitTo( std::string& content ) const
{
    StringSerializer serializer( content );
    Accept( serializer );
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_UnitList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterAgent& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_UnitList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAgentList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_unitList;
    ASN1T_UnitList*& list = asn.value.u.unitList = new ASN1T_UnitList();
    list->n = Count();
    if( IsSet() )
    {
        list->elem = new ASN1T_Unit[list->n];
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList::Clean
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAgentList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.unitList )
        delete[] asn.value.u.unitList->elem;
    delete asn.value.u.unitList;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameterAgentList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    ActionParameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool ActionParameterAgentList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
