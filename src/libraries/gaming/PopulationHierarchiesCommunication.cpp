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

// -----------------------------------------------------------------------------
// Name: PopulationHierarchiesCommunication constructor
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
PopulationHierarchiesCommunication::PopulationHierarchiesCommunication( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::KnowledgeGroup_ABC* knowledgeGroup )
    : gui::EntityHierarchies< CommunicationHierarchies >( controller, holder, knowledgeGroup )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchiesCommunication destructor
// Created: JSR 2013-07-1
// -----------------------------------------------------------------------------
PopulationHierarchiesCommunication::~PopulationHierarchiesCommunication()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchiesCommunication::CanCommunicate
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
bool PopulationHierarchiesCommunication::CanCommunicate() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchiesCommunication::CanReceive
// Created: LDC 2013-09-27
// -----------------------------------------------------------------------------
bool PopulationHierarchiesCommunication::CanReceive() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchiesCommunication::IsJammed
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
bool PopulationHierarchiesCommunication::IsJammed() const
{
    return false;
}
