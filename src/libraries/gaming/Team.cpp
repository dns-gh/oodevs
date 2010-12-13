// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Team.h"
#include "Tools.h"
#include "clients_kernel/PropertiesDictionary.h"

using namespace kernel;

namespace
{
    kernel::Karma MakeKarma( const sword::EnumDiplomacy& diplomacy )
    {
        switch( diplomacy )
        {
            case sword::friend_diplo : return kernel::Karma::friend_;
            case sword::enemy_diplo  : return kernel::Karma::enemy_;
            case sword::neutral_diplo: return kernel::Karma::neutral_;
            case sword::unknown_diplo:
            default: return kernel::Karma::unknown_;
        }
    }

    QString MakeName( const std::string& name )
    {
        return name.empty() ? QString( tools::translate( "Team", "Army %1" ) ).arg( name.c_str() ) : name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
Team::Team( const sword::PartyCreation& message, Controller& controller )
    : EntityImplementation< Team_ABC >( controller, message.party().id(), QString( message.name().c_str() ) )
    , karma_( MakeKarma( message.type() ) )
{
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::~Team()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void Team::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Team& self = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Team", "Info/Identifier" ), self.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Team", "Info/Name" ), self.name_ );
}

// -----------------------------------------------------------------------------
// Name: Team::GetKarma
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
const kernel::Karma& Team::GetKarma() const
{
    return karma_;
}
