// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ComponentTypes.h"
#include "ComponentTypeVisitor_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/ComponentType.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: ComponentTypes constructor
// Created: SLI 2011-09-30
// -----------------------------------------------------------------------------
ComponentTypes::ComponentTypes( const tools::Resolver_ABC< kernel::AgentType, unsigned long >& agentTypes )
    : agentTypes_( agentTypes )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ComponentTypes destructor
// Created: SLI 2011-09-30
// -----------------------------------------------------------------------------
ComponentTypes::~ComponentTypes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ComponentTypes::Apply
// Created: SLI 2011-09-30
// -----------------------------------------------------------------------------
void ComponentTypes::Apply( unsigned int agentTypeIdentifier, ComponentTypeVisitor_ABC& visitor ) const
{
    const kernel::AgentType& agentType = agentTypes_.Get( agentTypeIdentifier );
    for( tools::Iterator< const kernel::AgentComposition& > it = agentType.CreateIterator(); it.HasMoreElements(); )
    {
        const kernel::AgentComposition& composition = it.NextElement();
        visitor.NotifyEquipment( composition.GetType().GetId(), composition.GetType().GetName(), composition.GetCount() );
    }
}
