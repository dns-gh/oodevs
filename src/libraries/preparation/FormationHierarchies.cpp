// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationHierarchies.h"
#include "AutomatDecisions.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/SymbolFactory.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: FormationHierarchies constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationHierarchies::FormationHierarchies( kernel::Controller& controller, kernel::Formation_ABC& holder, kernel::Entity_ABC* superior, kernel::SymbolFactory& factory )
    : kernel::MergingTacticalHierarchies( controller, holder, 0 )
    , superior_( superior )
    , level_   ( factory.CreateLevelSymbol( holder.GetLevel() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies destructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationHierarchies::~FormationHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::GetLevel
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string FormationHierarchies::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::ChangeSuperior
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void FormationHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    kernel::MergingTacticalHierarchies::ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::UnregisterParent
// Created: MMC 2011-05-30
// -----------------------------------------------------------------------------
void FormationHierarchies::UnregisterParent()
{
    superior_ = 0;
    MergingTacticalHierarchies::UnregisterParent();
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::SerializeAttributes
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void FormationHierarchies::SerializeAttributes( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( it->second->Retrieve< AutomatDecisions >() ? "automat" : "formation" ); // $$$$ SBO 2006-09-22: bof bof
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::SerializeLogistics
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void FormationHierarchies::SerializeLogistics( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Interface().Apply( & Serializable_ABC::SerializeLogistics, xos );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::DoUpdate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void FormationHierarchies::DoUpdate( const kernel::InstanciationComplete& ic )
{
    SetSuperior( superior_ );
    kernel::MergingTacticalHierarchies::DoUpdate( ic );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::GetSuperior
// Created: SBO 2008-10-06
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* FormationHierarchies::GetSuperior() const
{
    const kernel::Entity_ABC* superior = kernel::MergingTacticalHierarchies::GetSuperior();
    return superior ? superior : superior_;
}
