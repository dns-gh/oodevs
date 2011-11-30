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
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

namespace
{
    kernel::Karma MakeKarma( const sword::EnumDiplomacy& diplomacy )
    {
        switch( diplomacy )
        {
            case sword::friendly : return kernel::Karma::friend_;
            case sword::enemy  : return kernel::Karma::enemy_;
            case sword::neutral: return kernel::Karma::neutral_;
            case sword::unknown:
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
Team::Team( const sword::PartyCreation& message, Controllers& controllers )
    : EntityImplementation< Team_ABC >( controllers.controller_, message.party().id(), QString( message.name().c_str() ) )
    , controllers_( controllers )
    , karma_      ( MakeKarma( message.type() ) )
{
    CreateDictionary( controllers_.controller_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::~Team()
{
    Destroy();
    controllers_.Unregister( *this );
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

// -----------------------------------------------------------------------------
// Name: Team::OptionChanged
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
void Team::OptionChanged( const std::string& name, const kernel::OptionVariant& /*value*/ )
{
    if( name == "Color/Neutralized" || name == "Color/TacticallyDestroyed" || name == "Color/TotallyDestroyed" )
    {
        if( const kernel::TacticalHierarchies* pTactical = Retrieve< kernel::TacticalHierarchies >() )
            controllers_.controller_.Update( *pTactical );
        if( const kernel::CommunicationHierarchies* pCommunication = Retrieve< kernel::CommunicationHierarchies >() )
            controllers_.controller_.Update( *pCommunication );
    }
}
