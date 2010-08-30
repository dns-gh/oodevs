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
#include "clients_kernel/PropertiesDictionary.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation::Formation( const Common::MsgFormationCreation& message, Controller& controller, const tools::Resolver_ABC< HierarchyLevel_ABC >& resolver  )
    : EntityImplementation< Formation_ABC >( controller, message.formation().id(), QString( message.name().c_str() ) )
    , level_( resolver.Get( message.level() ) )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( level_.GetName() ).arg( message.formation().id() );
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
