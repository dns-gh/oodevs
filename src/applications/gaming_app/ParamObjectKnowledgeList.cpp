// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamObjectKnowledgeList.h"
#include "ParamObjectKnowledge.h"
#include "ParamVisitor_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "gaming/ActionParameterObjectKnowledgeList.h"
#include "gaming/ObjectKnowledge_ABC.h"
#include "gaming/ObjectKnowledgeConverter_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::ParamObjectKnowledgeList( QObject* parent, const OrderParameter& parameter, kernel::ActionController& controller, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent )
    : EntityListParameter< ObjectKnowledge_ABC >( parent, parameter, controller )
    , parameter_( parameter )
    , converter_( converter )
    , agent_( agent )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::~ParamObjectKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::AddToMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tr( "Add object knowledge" ), menu );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter< ObjectKnowledge_ABC >* ParamObjectKnowledgeList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< ObjectKnowledge_ABC >* ParamObjectKnowledgeList::CreateElement( const ObjectKnowledge_ABC& potential )
{
    return new ParamObjectKnowledge( this, tr( "Object knowledge %1" ).arg( ++count_ ), converter_, agent_, potential );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( agent_.Get< CommunicationHierarchies >().GetTop() );
    const ObjectKnowledge_ABC* knowledge = converter_.Find( entity, team );
    if( knowledge )
        EntityListParameter< ObjectKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

namespace
{
    struct AsnSerializer : public ParamVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_ListKnowledgeObject& list ) : list_( list ), index_( 0 ) {}
        virtual void Visit( const Param_ABC& param )
        {
            if( index_ < list_.n )
                static_cast< const ParamObjectKnowledge& >( param ).CommitTo( list_.elem[index_++] );
        }

        ASN1T_ListKnowledgeObject& list_;
        unsigned int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_ListKnowledgeObject*& list = asn.value.u.listKnowledgeObject = new ASN1T_ListKnowledgeObject();
    asn.value.t = T_MissionParameter_value_listKnowledgeObject;
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_KnowledgeObject[ list->n ];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listKnowledgeObject )
        delete[] asn.value.u.listKnowledgeObject->elem;
    delete asn.value.u.listKnowledgeObject;
}

namespace
{
    struct ActionSerializer : public ParamVisitor_ABC
    {
        explicit ActionSerializer( ActionParameter_ABC& parent ) : parent_( parent ) {}
        virtual void Visit( const Param_ABC& param )
        {
            static_cast< const ParamObjectKnowledge& >( param ).CommitTo( parent_ );
        }

        ActionParameter_ABC& parent_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterObjectKnowledgeList( parameter_ ) );
    ActionSerializer serializer( *param );
    Accept( serializer );
    action.AddParameter( *param.release() );
}
