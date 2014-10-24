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
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/BreakdownOriginType.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/EquipmentType.h"
#include <tools/Resolver_ABC.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: ComponentTypes constructor
// Created: SLI 2011-09-30
// -----------------------------------------------------------------------------
ComponentTypes::ComponentTypes( const tools::Resolver_ABC< kernel::AgentType, unsigned long >& agentTypes,
        const tools::Resolver_ABC< kernel::EquipmentType, unsigned long >& equipmentTypes,
        const tools::Resolver_ABC< kernel::BreakdownType, std::string >& breakdownsTypes )
    : agentTypes_( agentTypes )
    , equipmentTypes_( equipmentTypes )
    , breakdownsTypes_( breakdownsTypes )
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
        const unsigned long equiptId = composition.GetType().GetId();
        const kernel::EquipmentType* const equipPtr = equipmentTypes_.Find( equiptId );
        std::vector< unsigned long > breakdowns;
        if( equipPtr )
        {
            tools::Iterator< const kernel::BreakdownOriginType& > itBr = equipPtr->CreateBreakdownsIterator();
            while( itBr.HasMoreElements() )
            {
                const kernel::BreakdownOriginType& brO = itBr.NextElement();
                const kernel::BreakdownType* const br = breakdownsTypes_.Find( brO.GetName() );
                if( br )
                    breakdowns.push_back( br->GetId() );
            }

        }
        visitor.NotifyEquipment( equiptId, composition.GetType().GetName(), composition.GetCount(), composition.GetCrew(), breakdowns );
    }
}
