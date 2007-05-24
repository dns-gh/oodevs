// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamObjectKnowledge.h"
#include "gaming/ObjectKnowledge_ABC.h"
#include "gaming/ObjectKnowledgeConverter_ABC.h"
#include "gaming/ActionParameterObjectKnowledge.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledge::ParamObjectKnowledge( QObject* parent, const OrderParameter& parameter, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent )
    : EntityParameter< ObjectKnowledge_ABC >( parent, parameter )
    , parameter_( &parameter )
    , converter_( converter )
    , agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ParamObjectKnowledge::ParamObjectKnowledge( QObject* parent, const QString& name, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, const ObjectKnowledge_ABC& potential )
    : EntityParameter< ObjectKnowledge_ABC >( parent, name, potential )
    , parameter_( 0 )
    , converter_( converter )
    , agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledge::~ParamObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( agent_.Get< CommunicationHierarchies >().GetTop() );
    const ObjectKnowledge_ABC* knowledge = converter_.Find( entity, team );
    if( knowledge )
        EntityParameter< ObjectKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_knowledgeObject;
    EntityParameter< ObjectKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.knowledgeObject );
    asn.null_value = asn.value.u.knowledgeObject ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::CommitTo( ASN1T_KnowledgeObject& asn ) const
{
    EntityParameter< ObjectKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::CommitTo( Action_ABC& action ) const
{
    if( !parameter_ )
        throw std::runtime_error( "OrderParameter not set" ); // $$$$ SBO 2007-04-25: 
    std::auto_ptr< ActionParameterEntity< ObjectKnowledge_ABC > > param( new ActionParameterObjectKnowledge( *parameter_ ) );
    EntityParameter< ObjectKnowledge_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::CommitTo( ActionParameter_ABC& param ) const
{
    std::auto_ptr< ActionParameterEntity< ObjectKnowledge_ABC > > parameter( new ActionParameterObjectKnowledge( GetName() ) );
    EntityParameter< ObjectKnowledge_ABC >::CommitTo( *parameter );
    param.AddParameter( *parameter.release() );
}
