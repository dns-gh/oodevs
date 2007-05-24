// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAgentKnowledgeList.h"
#include "ParamAgentKnowledge.h"
#include "ParamVisitor_ABC.h"
#include "gaming/AgentKnowledge_ABC.h"
#include "gaming/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledgeList::ParamAgentKnowledgeList( QObject* parent, const OrderParameter& parameter, kernel::ActionController& controller, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityListParameter< AgentKnowledge_ABC >( parent, parameter, controller )
    , parameter_( parameter )
    , converter_( converter )
    , agent_    ( agent )
    , count_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledgeList::~ParamAgentKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::AddToMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tr( "Add agent knowledge" ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< AgentKnowledge_ABC >* ParamAgentKnowledgeList::CreateElement( const AgentKnowledge_ABC& potential )
{
    return new ParamAgentKnowledge( this, tr( "Agent knowledge %1" ).arg( ++count_ ), converter_, agent_, potential );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::NotifyContextMenu( const Agent_ABC& entity, ContextMenu& menu )
{
    const AgentKnowledge_ABC* knowledge = converter_.Find( entity, agent_ );
    if( knowledge )
        EntityListParameter< AgentKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

namespace
{
    struct AsnSerializer : public ParamVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_ListKnowledgeAgent& list ) : list_( list ), index_( 0 ) {}
        virtual void Visit( const Param_ABC& param )
        {
            if( index_ < list_.n )
                static_cast< const ParamAgentKnowledge& >( param ).CommitTo( list_.elem[index_++] );
        }

        ASN1T_ListKnowledgeAgent& list_;
        unsigned int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_ListKnowledgeAgent*& list = asn.value.u.listKnowledgeAgent = new ASN1T_ListKnowledgeAgent();
    asn.value.t = T_MissionParameter_value_listKnowledgeAgent;
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_KnowledgeAgent[ list->n ];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listKnowledgeAgent )
        delete[] asn.value.u.listKnowledgeAgent->elem;
    delete asn.value.u.listKnowledgeAgent;
}

namespace
{
    struct ActionSerializer : public ParamVisitor_ABC
    {
        explicit ActionSerializer( ActionParameter_ABC& parent ) : parent_( parent ) {}
        virtual void Visit( const Param_ABC& param )
        {
            static_cast< const ParamAgentKnowledge& >( param ).CommitTo( parent_ );
        }

        ActionParameter_ABC& parent_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::CommitTo( Action_ABC& action ) const
{
//    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterAgentKnowledegList( parameter_ ) );
//    ActionSerializer serializer( *param );
//    Accept( serializer );
//    action.AddParameter( *param.release() );
}
