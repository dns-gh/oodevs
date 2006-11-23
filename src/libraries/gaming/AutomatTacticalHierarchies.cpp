// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AutomatTacticalHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Formation_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
AutomatTacticalHierarchies::AutomatTacticalHierarchies( Controller& controller, Entity_ABC& holder, const Resolver_ABC< Formation_ABC >& resolver, PropertiesDictionary& dictionary )
    : MergingTacticalHierarchies( controller, holder, 0 )
    , controller_( controller )
    , resolver_( resolver )
{
    // NOTHING
    // $$$$ AGE 2006-10-19: dico !
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies destructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
AutomatTacticalHierarchies::~AutomatTacticalHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies::DoUpdate
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AutomatTacticalHierarchies::DoUpdate( const ASN1T_MsgAutomateCreation& message )
{
    SetSuperior( &resolver_.Get( message.oid_formation ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies::UpdateSymbol
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void AutomatTacticalHierarchies::UpdateSymbol()
{
    level_.clear();
    MergingTacticalHierarchies::UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies::MergeSymbol
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void AutomatTacticalHierarchies::MergeSymbol( const kernel::Entity_ABC& entity )
{
    const std::string childLevel = entity.Get< TacticalHierarchies >().GetLevel();
    if( level_.empty() )
        level_ = Increase( childLevel );
    else
        level_ = Max( level_, Increase( childLevel ) );
    MergingTacticalHierarchies::MergeSymbol( entity );
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies::Max
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string AutomatTacticalHierarchies::Max( const std::string& lhs, const std::string& rhs )
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
// Name: AutomatTacticalHierarchies::Increase
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string AutomatTacticalHierarchies::Increase( const std::string& value )
{
    const unsigned count = Count( value );
    const char level = Level( value );
    if( count < 3 || level == 'x' )
        return value + level;
    const char newChar = level == 'o' ? 'i' : 'x';
    return value.substr( 0, value.length() - count ) + newChar;
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies::Level
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
char AutomatTacticalHierarchies::Level( const std::string& value )
{
    return *value.rbegin();
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies::Count
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
unsigned AutomatTacticalHierarchies::Count( const std::string& value )
{
    char level = Level( value );
    unsigned count = 0;
    while( value[ value.size() - count - 1 ] == level )
        ++count;
    return count;
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies::GetLevel
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string AutomatTacticalHierarchies::GetLevel() const
{
    return level_;
}
