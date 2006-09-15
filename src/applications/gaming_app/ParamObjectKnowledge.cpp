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
#include "gaming/ObjectKnowledge.h"
#include "gaming/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledge::ParamObjectKnowledge( QWidget* pParent, ASN1T_OID& id, const QString& label, const QString& menu, ObjectKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent )
    : EntityParameter< ObjectKnowledge >( pParent, id, label, menu )
    , converter_( converter )
    , agent_( static_cast< const Agent_ABC& >( agent ) ) // $$$$ AGE 2006-09-15: 
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
    const ObjectKnowledge* knowledge = converter_.Find( entity, agent_.GetTeam() );
    if( knowledge )
        EntityParameter< ObjectKnowledge >::NotifyContextMenu( *knowledge, menu );
}
