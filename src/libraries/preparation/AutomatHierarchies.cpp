// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AutomatHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "Tools.h"
#include <xeumeuleu/xml.h>

using namespace xml;

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies constructor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
AutomatHierarchies::AutomatHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::MergingTacticalHierarchies( controller, holder, 0 )
    , superior_( superior )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AutomatHierarchies destructor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
AutomatHierarchies::~AutomatHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::ChangeSuperior
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void AutomatHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    MergingTacticalHierarchies::ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::SerializeAttributes
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void AutomatHierarchies::SerializeAttributes( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        // $$$$ AGE 2007-04-05: quick ada fix. 
        const kernel::Entity_ABC* child = it->second;
        if( dynamic_cast< const kernel::Agent_ABC* >( child ) )
            xos << start( "unit" );
        else
            xos << start( "automat" );
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        xos << end();
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::DoUpdate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void AutomatHierarchies::DoUpdate( const kernel::InstanciationComplete& ic )
{
    SetSuperior( superior_ );
    MergingTacticalHierarchies::DoUpdate( ic );
}

// $$$$ AGE 2006-11-23: Tout ce bazar est dégueu, a tous les points de vue

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::UpdateSymbol
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void AutomatHierarchies::UpdateSymbol( bool up /*= true*/ )
{
    if( up )
        level_.clear();
    MergingTacticalHierarchies::UpdateSymbol( up );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::MergeSymbol
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void AutomatHierarchies::MergeSymbol( const kernel::Entity_ABC& entity )
{
    if( const TacticalHierarchies* hierarchies = entity.Retrieve< TacticalHierarchies >() )
    {
        const std::string childLevel = hierarchies->GetLevel();
        if( level_.empty() && !childLevel.empty() )
            level_ = Increase( childLevel );
        else if( !childLevel.empty() )
            level_ = Max( level_, Increase( childLevel ) );
    }
    MergingTacticalHierarchies::MergeSymbol( entity );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::Max
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string AutomatHierarchies::Max( const std::string& lhs, const std::string& rhs )
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
// Name: AutomatHierarchies::Increase
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string AutomatHierarchies::Increase( const std::string& value )
{
    const unsigned count = Count( value );
    const char level = Level( value );
    if( count < 3 || level == 'x' )
        return value + level;
    const char newChar = level == 'o' ? 'i' : 'x';
    return value.substr( 0, value.length() - count ) + newChar;
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::Level
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
char AutomatHierarchies::Level( const std::string& value )
{
    return *value.rbegin();
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::Count
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
unsigned AutomatHierarchies::Count( const std::string& value )
{
    char level = Level( value );
    unsigned count = 0;
    while( value[ value.size() - count - 1 ] == level )
        ++count;
    return count;
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::GetLevel
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string AutomatHierarchies::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::GetSuperior
// Created: SBO 2008-10-06
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* AutomatHierarchies::GetSuperior() const
{
    const kernel::Entity_ABC* superior = kernel::MergingTacticalHierarchies::GetSuperior();
    return superior ? superior : superior_;
}
