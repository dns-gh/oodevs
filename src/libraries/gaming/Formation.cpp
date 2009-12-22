// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Formation.h"
#include "Tools.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation::Formation( const ASN1T_MsgFormationCreation& message, Controller& controller, const tools::Resolver_ABC< HierarchyLevel_ABC >& resolver  )
    : EntityImplementation< Formation_ABC >( controller, message.oid, message.nom )
    , level_( resolver.Get( message.niveau ) )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( level_.GetName() ).arg( message.oid );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
const kernel::HierarchyLevel_ABC& Formation::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Formation::GetParent
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
kernel::Formation_ABC* Formation::GetParent() const
{
    throw std::exception( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetTeam
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
kernel::Team_ABC& Formation::GetTeam() const
{
    throw std::exception( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetFormations
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
const tools::Resolver< kernel::Formation_ABC >& Formation::GetFormations() const
{
    throw std::exception( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetAutomates
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
const tools::Resolver< kernel::Automat_ABC >& Formation::GetAutomates() const
{
    throw std::exception( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Formation::CreateDictionary
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Formation::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Formation& self = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Formation", "Info/Identifier" ), self.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Formation", "Info/Name" ), self.name_ );
}

// -----------------------------------------------------------------------------
// Name: Formation::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Register( kernel::Formation_ABC& /*formation*/ )
{
    throw std::exception( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Formation::Remove
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Remove( kernel::Formation_ABC& /*formation*/ )
{
    throw std::exception( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Register( kernel::Automat_ABC& /*automat*/ )
{
    throw std::exception( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Formation::Remove
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Remove( kernel::Automat_ABC& /*automat*/ )
{
     throw std::exception( __FUNCTION__ " not implemented" );
}
