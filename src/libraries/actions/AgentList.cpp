// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "AgentList.h"
#include "Agent.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: AgentList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
AgentList::AgentList( const kernel::OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
AgentList::AgentList( const OrderParameter& parameter, const ASN1T_UnitList& asn, const Resolver_ABC< Agent_ABC >& resolver )
    : Parameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new Agent( OrderParameter( tools::translate( "Parameter", "Agent %1" ).arg( i + 1 ).ascii(), "agent", false ), asn.elem[i], resolver ) );
}

// -----------------------------------------------------------------------------
// Name: AgentList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
AgentList::AgentList( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver )
    : Parameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &AgentList::ReadAgent, resolver );
}

// -----------------------------------------------------------------------------
// Name: AgentList destructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
AgentList::~AgentList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentList::ReadAgent
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void AgentList::ReadAgent( xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver )
{
    AddParameter( *new Agent( xis, resolver ) );
}

namespace
{
    struct StringSerializer : public ParameterVisitor_ABC
    {
        explicit StringSerializer( std::string& content ) : content_( &content ) {}
        virtual void Visit( const Agent& param )
        {
            if( !content_->empty() )
                *content_ += ",";
            param.CommitTo( *content_ );
        }
        std::string* content_;
    };
}

// -----------------------------------------------------------------------------
// Name: AgentList::CommitTo
// Created: AGE 2007-10-03
// -----------------------------------------------------------------------------
void AgentList::CommitTo( std::string& content ) const
{
    StringSerializer serializer( content );
    Accept( serializer );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_UnitList& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const Agent& param )
        {
            param.CommitTo( asn_->elem[current_++] );
        }

        ASN1T_UnitList* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: AgentList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void AgentList::CommitTo( ASN1T_MissionParameter& asn ) const
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
// Name: AgentList::Clean
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void AgentList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.unitList )
        delete[] asn.value.u.unitList->elem;
    delete asn.value.u.unitList;
}

// -----------------------------------------------------------------------------
// Name: AgentList::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void AgentList::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: AgentList::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool AgentList::IsSet() const
{
    return !elements_.empty(); // $$$$ SBO 2008-03-19: each element must be set as well...
}
