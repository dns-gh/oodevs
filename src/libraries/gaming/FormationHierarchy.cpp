// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FormationHierarchy.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/SymbolFactory.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationHierarchy constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
FormationHierarchy::FormationHierarchy( Controller& controller, Formation_ABC& entity, Entity_ABC* superior, kernel::SymbolFactory& factory )
    : MergingTacticalHierarchies( controller, entity, 0 )
    , superior_( superior )
    , level_   ( factory.CreateLevelSymbol( entity.GetLevel() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchy destructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
FormationHierarchy::~FormationHierarchy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchy::GetLevel
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string FormationHierarchy::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchy::DoUpdate
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void FormationHierarchy::DoUpdate( const kernel::InstanciationComplete& ic )
{
    SetSuperior( superior_ );
    MergingTacticalHierarchies::DoUpdate( ic );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchy::GetSuperior
// Created: SBO 2008-10-06
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* FormationHierarchy::GetSuperior() const
{
    const kernel::Entity_ABC* superior = MergingTacticalHierarchies::GetSuperior();
    return superior ? superior : superior_;
}