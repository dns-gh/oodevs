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
#include "protocol/Protocol.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.h>

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
AgentList::AgentList( const OrderParameter& parameter, const Common::MsgUnitList& message, const Resolver_ABC< Agent_ABC >& resolver, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new Agent( OrderParameter( tools::translate( "Parameter", "Agent %1" ).arg( i + 1 ).ascii(), "agent", false ), message.elem(i).oid(),  resolver, controller ) );
}

// -----------------------------------------------------------------------------
// Name: AgentList constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
AgentList::AgentList( const OrderParameter& parameter, xml::xistream& xis, const tools::Resolver_ABC< Agent_ABC >& resolver, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &AgentList::ReadAgent, resolver, controller );
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
void AgentList::ReadAgent( xml::xistream& xis, const tools::Resolver_ABC< Agent_ABC >& resolver, kernel::Controller& controller )
{
    AddParameter( *new Agent( xis, resolver, controller ) );
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
        explicit AsnSerializer( Common::MsgUnitList& message ) : message_( &message ) {}
        virtual void Visit( const Agent& param )
        {
            param.CommitTo( *message_->add_elem() );
        }
        Common::MsgUnitList* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: AgentList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void AgentList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value ( !IsSet() );
    Common::MsgUnitList* list = message.mutable_value()->mutable_unitlist();
    if( IsSet() )
    {
        AsnSerializer serializer( *list );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentList::Clean
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void AgentList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_unitlist() )
        message.mutable_value()->clear_unitlist();
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
