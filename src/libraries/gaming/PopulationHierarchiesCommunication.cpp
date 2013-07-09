// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationHierarchiesCommunication.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "protocol/Simulation.h"

// -----------------------------------------------------------------------------
// Name: PopulationHierarchiesCommunication constructor
// Created: JSR 2013-07-05
// -----------------------------------------------------------------------------
PopulationHierarchiesCommunication::PopulationHierarchiesCommunication( kernel::Controller& controller, kernel::Entity_ABC& holder, const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver )
    : EntityHierarchies< CommunicationHierarchies >( controller, holder, 0 )
    , groupResolver_( groupResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchiesCommunication destructor
// Created: JSR 2013-07-05
// -----------------------------------------------------------------------------
PopulationHierarchiesCommunication::~PopulationHierarchiesCommunication()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchiesCommunication::CanCommunicate
// Created: JSR 2013-07-05
// -----------------------------------------------------------------------------
bool PopulationHierarchiesCommunication::CanCommunicate() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchiesCommunication::IsJammed
// Created: JSR 2013-07-05
// -----------------------------------------------------------------------------
bool PopulationHierarchiesCommunication::IsJammed() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchiesCommunication::DoUpdate
// Created: JSR 2013-07-05
// -----------------------------------------------------------------------------
void PopulationHierarchiesCommunication::DoUpdate( const sword::CrowdCreation& message )
{
    SetSuperior( &groupResolver_.Get( message.knowledge_group().id() ) );
}
