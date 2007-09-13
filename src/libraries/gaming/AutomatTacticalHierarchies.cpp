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
#include "clients_kernel/Automat_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
AutomatTacticalHierarchies::AutomatTacticalHierarchies( Controller& controller, Entity_ABC& holder, Entity_ABC& superior
                                                       , const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const kernel::Resolver_ABC< kernel::Formation_ABC >& formationResolver )
    : MergingTacticalHierarchies( controller, holder, 0 )
    , controller_( controller )
    , automatResolver_( automatResolver )
    , formationResolver_( formationResolver )
    , superior_( superior )
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
// Created: NLD 2007-04-12
// -----------------------------------------------------------------------------
void AutomatTacticalHierarchies::DoUpdate( const ASN1T_MsgAutomatChangeSuperiorAck& message )
{
    if( !message.error_code )
    {
        if( message.oid_superior.t == T_MsgAutomatChangeSuperior_oid_superior_automate )
            ChangeSuperior( &automatResolver_.Get( message.oid_superior.u.automate ) );
        else if( message.oid_superior.t == T_MsgAutomatChangeSuperior_oid_superior_formation )
            ChangeSuperior( &formationResolver_.Get( message.oid_superior.u.formation ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies::DoUpdate
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AutomatTacticalHierarchies::DoUpdate( const InstanciationComplete& message )
{
    SetSuperior( &superior_ );
    kernel::MergingTacticalHierarchies::DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: AutomatTacticalHierarchies::UpdateSymbol
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void AutomatTacticalHierarchies::UpdateSymbol( bool up /*= true*/ )
{
    if( up )
        level_.clear();
    MergingTacticalHierarchies::UpdateSymbol( up );
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
