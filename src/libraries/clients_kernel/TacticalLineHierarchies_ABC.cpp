// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "TacticalLineHierarchies_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies_ABC constructor
// Created: MMC 2012-05-14
// -----------------------------------------------------------------------------
TacticalLineHierarchies_ABC::TacticalLineHierarchies_ABC( Entity_ABC& holder, Entity_ABC* superior )
    : SimpleHierarchies< TacticalHierarchies >( holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies_ABC destructor
// Created: MMC 2012-05-14
// -----------------------------------------------------------------------------
TacticalLineHierarchies_ABC::~TacticalLineHierarchies_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies_ABC::GetSymbol
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
std::string TacticalLineHierarchies_ABC::GetSymbol() const
{
    if( !GetSuperior() )
        return std::string();
    std::string level = GetSuperior()->Get< TacticalHierarchies >().GetLevel();
    if( level.size() < 8 )
        return std::string();
    std::string result = "-- " + level.substr( 7 ) + " --"; // Strip "levels/" from symbol and construct line symbol from this.
    return result;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies_ABC::GetLevel
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
std::string TacticalLineHierarchies_ABC::GetLevel() const
{
    if( !GetSuperior() )
        return std::string();
    return GetSuperior()->Get< TacticalHierarchies >().GetLevel();
}
