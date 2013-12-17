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
#include "clients_kernel/Team_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationHierarchy constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
FormationHierarchy::FormationHierarchy( Controller& controller, Formation_ABC& entity, Entity_ABC* superior, kernel::SymbolFactory& factory,
        const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver )
    : MergingTacticalHierarchies( controller, entity, 0 )
    , formationResolver_( formationResolver )
    , teamResolver_( teamResolver )
    , superior_( superior )
    , level_   ( factory.CreateLevelSymbol( ENT_Tr::ConvertFromNatureLevel( entity.GetLevel() ) ) )
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

// -----------------------------------------------------------------------------
// Name: FormationHierarchy::UnregisterParent
// Created: MMC 2011-05-30
// -----------------------------------------------------------------------------
void FormationHierarchy::UnregisterParent()
{
    superior_ = 0;
    gui::MergingTacticalHierarchies::UnregisterParent();
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchy::DoUpdate
// Created: AHC 2013-01-14
// -----------------------------------------------------------------------------
void FormationHierarchy::DoUpdate( const sword::FormationChangeSuperior& message )
{
    if( message.superior().has_formation() )
        superior_ = &formationResolver_.Get( message.superior().formation().id() );
    else
        superior_ = &teamResolver_.Get( message.superior().party().id() );
    ChangeSuperior( superior_ );
}
