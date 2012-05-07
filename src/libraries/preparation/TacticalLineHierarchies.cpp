// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TacticalLineHierarchies.h"
#include "TacticalLines.h"

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies constructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
TacticalLineHierarchies::TacticalLineHierarchies( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::SimpleHierarchies< kernel::TacticalHierarchies >( holder, superior )
    , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies destructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
TacticalLineHierarchies::~TacticalLineHierarchies()
{
    if( superior_ )
        if( TacticalLines* lines = superior_->Retrieve< TacticalLines >() )
            lines->Remove( GetEntity().GetId() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies::GetSymbol
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
std::string TacticalLineHierarchies::GetSymbol() const
{
    if( !superior_ )
        return std::string();
    std::string level = superior_->Get< TacticalHierarchies >().GetLevel();
    if( level.size() < 8 )
        return std::string();
    std::string result = "-- " + level.substr( 7 ) + " --"; // Strip "levels/" from symbol and construct line symbol from this.
    return result;
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLineHierarchies::GetLevel
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
std::string TacticalLineHierarchies::GetLevel() const
{
    if( !superior_ )
        return std::string();
    return superior_->Get< TacticalHierarchies >().GetLevel();
}