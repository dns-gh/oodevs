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
#include "clients_kernel/Agent_ABC.h"
#include "gaming/ObjectKnowledge.h"
#include "gaming/ObjectKnowledgeConverter_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::ParamObjectKnowledgeList( QWidget* pParent, ASN1T_ListKnowledgeObject& asn, const QString& label, const QString& menu, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent )
    : EntityListParameter< ObjectKnowledge >( pParent, asn.n, asn.elem, label, menu )
    , converter_( converter )
    , agent_( static_cast< const Agent_ABC& >( agent ) ) // $$$$ AGE 2006-09-15: 
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
// Name: ParamObjectKnowledgeList::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    const ObjectKnowledge* knowledge = converter_.Find( entity, agent_.GetTeam() );
    if( knowledge )
        EntityListParameter< ObjectKnowledge >::NotifyContextMenu( *knowledge, menu );
}
