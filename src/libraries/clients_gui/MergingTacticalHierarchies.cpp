// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MergingTacticalHierarchies.h"
#include "PropertiesDictionary.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies constructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
MergingTacticalHierarchies::MergingTacticalHierarchies( Controller& controller, Entity_ABC& entity, Entity_ABC* superior )
     : EntityHierarchies< TacticalHierarchies >( controller, entity, superior )
     , controller_( controller )
     , entity_( entity )
     , symbolCanBeUpdated_( false )
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
// Name: MergingTacticalHierarchies::GetStaticSymbol
// Created: LDC 2013-04-19
// -----------------------------------------------------------------------------
std::string MergingTacticalHierarchies::GetStaticSymbol() const
{
    return entity_.Get< SymbolHierarchy_ABC >().GetStaticValue();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::GetMoveSymbol
// Created: LDC 2013-04-19
// -----------------------------------------------------------------------------
std::string MergingTacticalHierarchies::GetMoveSymbol() const
{
    return entity_.Get< SymbolHierarchy_ABC >().GetMoveValue();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::UpdateSymbolUpward
// Created: LDC 2012-04-12
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::UpdateSymbolUpward()
{
    if( !symbolCanBeUpdated_ )
        return;
    entity_.Get< SymbolHierarchy_ABC >().PrepareForMerge();
    UpdateSymbol();
    if( TacticalHierarchies* superior = SuperiorHierarchy() )
        superior->UpdateSymbolUpward();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::UpdateSymbolDownward
// Created: LDC 2012-04-12
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::UpdateSymbolDownward()
{
    tools::Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const TacticalHierarchies* child = it.NextElement().Retrieve< TacticalHierarchies >();
        if( child )
            const_cast< TacticalHierarchies* >( child )->UpdateSymbolDownward();
    }
    if( !symbolCanBeUpdated_ )
        return;
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::UpdateSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::UpdateSymbol()
{
    const std::string oldSymbol = GetSymbol();
    const std::string oldLevel = GetLevel();
    tools::Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
    std::map< std::string, int > symbolCount;
    std::string highestCount;
    int max = 0;
    while( it.HasMoreElements() )
    {
        if( const TacticalHierarchies* hierarchies = it.NextElement().Retrieve< TacticalHierarchies >() )
        {
            std::string symbolName = hierarchies->GetSymbol();
            if( symbolCount.find( symbolName ) == symbolCount.end() )
                symbolCount[ symbolName ] = 0;
            int& count = symbolCount[ symbolName ];
            ++count;
            if( max < count )
            {
                max = count;
                highestCount = symbolName;
            }
        }
    }
    UpdateLevel();
    entity_.Get< SymbolHierarchy_ABC >().ResetSymbol( highestCount );
    if( TacticalHierarchies* superior = SuperiorHierarchy() )
    {
        if( const kernel::Diplomacies_ABC* diplomacy = superior->GetTop().Retrieve< kernel::Diplomacies_ABC >() )
            entity_.Get< SymbolHierarchy_ABC >().UpdateKarma( diplomacy->GetKarma() );
    }
    if( GetSymbol() != oldSymbol || GetLevel() != oldLevel )
        controller_.Update( *static_cast< Symbol_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::UpdateLevel
// Created: LDC 2012-04-12
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::UpdateLevel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::RegisterSubordinate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::RegisterSubordinate( kernel::Entity_ABC& entity )
{
    EntityHierarchies< TacticalHierarchies >::RegisterSubordinate( entity );
    std::string currentSymbol = GetSymbol();
    std::string currentLevel = GetLevel();
    UpdateSymbol();
    controller_.Update( *static_cast< Symbol_ABC* >( this ) );
    if( currentSymbol != GetSymbol() || currentLevel != GetLevel() )
        if( TacticalHierarchies* superior = SuperiorHierarchy() )
            superior->UpdateSymbolUpward();
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
// Name: MergingTacticalHierarchies::DoUpdate
// Created: LDC 2012-02-13
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::DoUpdate( const kernel::InstanciationComplete& ic )
{
    EntityHierarchies< TacticalHierarchies >::DoUpdate( ic );
    symbolCanBeUpdated_ = true;
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
// Name: MergingTacticalHierarchies::DecreaseLevel
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
std::string MergingTacticalHierarchies::DecreaseLevel( const std::string& value )
{
    E_NatureLevel parentLevel = ENT_Tr::ConvertToNatureLevel( value.substr( 7 ) );
    if( parentLevel > eNatureLevel_b )
        parentLevel = static_cast< E_NatureLevel >( parentLevel - 1 );
    return "levels/" + ENT_Tr::ConvertFromNatureLevel( parentLevel );
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