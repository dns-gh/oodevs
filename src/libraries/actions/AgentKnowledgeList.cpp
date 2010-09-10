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
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

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
AgentKnowledgeList::AgentKnowledgeList( const OrderParameter& parameter, const Common::UnitKnowledgeIdList& message, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new AgentKnowledge( OrderParameter( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i + 1 ).ascii(), "agentknowledge", false ), message.elem(i).id(), converter, owner, controller ) );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledgeList::AgentKnowledgeList( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Parameter< QString >( parameter )
{
    xis >> xml::list( "parameter", *this, &AgentKnowledgeList::ReadAgentKnowledge, resolver, converter, owner, controller );
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
void AgentKnowledgeList::ReadAgentKnowledge( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
{
    AddParameter( *new AgentKnowledge( xis, resolver, converter, owner, controller ) );
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( Common::UnitKnowledgeIdList& message ) : message_( &message ) {}
        virtual void Visit( const AgentKnowledge& param )
        {
            param.CommitTo( *message_->add_elem() );
        }
        Common::UnitKnowledgeIdList* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledgeList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
    {
        AsnSerializer serializer( *message.mutable_value()->mutable_unitknowledgelist() );
        Accept( serializer );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeList::Clean
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledgeList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_unitknowledgelist() )
        message.mutable_value()->clear_unitknowledgelist();
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
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( !it->second->IsSet() )
            return false;
    return !elements_.empty();
}
