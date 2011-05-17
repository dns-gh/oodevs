// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Team.h"
#include "IdManager.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::Team( Controller& controller, IdManager& idManager )
    : EntityImplementation< Team_ABC >( controller, idManager.GetNextId(), "" )
{
    name_ = tools::translate( "Preparation", "Army %1" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Team::Team( xml::xistream& xis, Controller& controller, IdManager& idManager )
    : EntityImplementation< Team_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
{
    RegisterSelf( *this );
    idManager.Lock( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::~Team()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Team::Rename
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void Team::Rename( const QString& name )
{
    name_ = name;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Team::SerializeAttributes
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Team::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", id_ )
        << xml::attribute( "name", name_.ascii() );
}

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void Team::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Team", "Info/Identifier" ), static_cast< const unsigned long >( id_ ) );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Team", "Info/Name" ), name_, *this, &Team::Rename );
}
