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
