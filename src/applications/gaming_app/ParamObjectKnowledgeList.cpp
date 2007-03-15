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
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "gaming/ObjectKnowledge_ABC.h"
#include "gaming/ObjectKnowledgeConverter_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::ParamObjectKnowledgeList( QObject* parent, const QString& name, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent )
    : EntityListParameter< ObjectKnowledge_ABC >( parent, name )
    , converter_( converter )
    , agent_( agent )
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
// Name: ParamObjectKnowledgeList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_listKnowledgeObject;
    EntityListParameter< ObjectKnowledge_ABC >::CommitTo( (ASN1T_ListOID*&)asn.value.u.listKnowledgeObject );
    asn.null_value = asn.value.u.listKnowledgeObject->n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::Clean( ASN1T_MissionParameter& asn ) const
{
    EntityListParameter< ObjectKnowledge_ABC >::Clean( (ASN1T_ListOID*&)asn.value.u.listKnowledgeObject );
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
