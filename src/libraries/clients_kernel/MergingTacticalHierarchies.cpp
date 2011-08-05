// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MergingTacticalHierarchies.h"
#include "SymbolHierarchy_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies constructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
MergingTacticalHierarchies::MergingTacticalHierarchies( Controller& controller, Entity_ABC& entity, Entity_ABC* superior )
     : EntityHierarchies< TacticalHierarchies >( controller, entity, superior )
     , controller_( controller )
     , entity_    ( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies destructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
MergingTacticalHierarchies::~MergingTacticalHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::GetSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
std::string MergingTacticalHierarchies::GetSymbol() const
{
    return entity_.Get< SymbolHierarchy_ABC >().GetValue();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::UpdateSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::UpdateSymbol( bool up /* = true*/ )
{
    if( up )
        entity_.Get< SymbolHierarchy_ABC >().PrepareForMerge();
    else
    {
        tools::Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const TacticalHierarchies* child = it.NextElement().Retrieve< TacticalHierarchies >();
            if( child )
                const_cast< TacticalHierarchies* >( child )->UpdateSymbol( false );
        }
    }
    const std::string oldSymbol = GetSymbol();
    const std::string oldLevel = GetLevel();
    tools::Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
        MergeSymbol( it.NextElement() );
    if( GetSymbol() != oldSymbol || GetLevel() != oldLevel )
        controller_.Update( *static_cast< Symbol_ABC* >( this ) );
    if( up )
        if( TacticalHierarchies* superior = SuperiorHierarchy() )
            superior->UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::RegisterSubordinate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::RegisterSubordinate( kernel::Entity_ABC& entity )
{
    EntityHierarchies< TacticalHierarchies >::RegisterSubordinate( entity );
    MergeSymbol( entity );
    controller_.Update( *static_cast< Symbol_ABC* >( this ) );
    if( TacticalHierarchies* superior = SuperiorHierarchy() )
        superior->UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::UnregisterSubordinate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::UnregisterSubordinate( const kernel::Entity_ABC& entity )
{
    EntityHierarchies< TacticalHierarchies >::UnregisterSubordinate( entity );
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::MergeSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::MergeSymbol( const kernel::Entity_ABC& entity )
{
    if( const TacticalHierarchies* hierarchies = entity.Retrieve< TacticalHierarchies >() )
        entity_.Get< SymbolHierarchy_ABC >().MergeSymbol( hierarchies->GetSymbol() );
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::MaxLevel
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string MergingTacticalHierarchies::MaxLevel( const std::string& lhs, const std::string& rhs )
{
    const char ll = Level( lhs );
    const char rl = Level( rhs );
    if( ll == 'o' && rl != 'o' )
        return rhs;
    if( ll == 'i' && rl == 'x' )
        return rhs;
    if( rl == 'o' && ll != 'o' )
        return lhs;
    if( rl == 'i' && ll == 'x' )
        return lhs;
    const unsigned lc = Count( lhs );
    const unsigned rc = Count( rhs );
    if( lc < rc )
        return rhs;
    return lhs;
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::IncreaseLevel
// Created: LDC 2011-02-23
// -----------------------------------------------------------------------------
std::string MergingTacticalHierarchies::IncreaseLevel( const std::string& value )
{
    if( value.empty() )
        return std::string( "o" );
    const unsigned count = Count( value );
    const char level = Level( value );
    if( count < 3 || level == 'x' )
        return value + level;
    const char newChar = level == 'o' ? 'i' : 'x';
    return value.substr( 0, value.length() - count ) + newChar;
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::Level
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
char MergingTacticalHierarchies::Level( const std::string& value )
{
    if( value.empty() )
        return 'o';
    return *value.rbegin();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::Count
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
unsigned MergingTacticalHierarchies::Count( const std::string& value )
{
    if( value.empty() )
        return 0;
    char level = Level( value );
    unsigned count = 0;
    while( value[ value.size() - count - 1 ] == level )
        ++count;
    return count;
}