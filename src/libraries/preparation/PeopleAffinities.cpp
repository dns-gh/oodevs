// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "PeopleAffinities.h"
#include "Model.h"
#include "TeamsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Entity_ABC.h"
#include <tools/Iterator.h>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PeopleAffinities constructor
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
PeopleAffinities::PeopleAffinities( kernel::Controllers& controllers, Model& model, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : Affinities( model )
    , controllers_( controllers )
    , dictionary_ ( dictionary )
    , entity_( entity )
{
    InitializeAffinities();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities constructor
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
PeopleAffinities::PeopleAffinities( xml::xistream& xis, kernel::Controllers& controllers, Model& model, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : Affinities( xis, model )
    , controllers_( controllers )
    , dictionary_ ( dictionary )
    , entity_( entity )
{
    InitializeAffinities();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities destructor
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
PeopleAffinities::~PeopleAffinities()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::NotifyCreated
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void PeopleAffinities::NotifyCreated( const kernel::Team_ABC& team )
{
    AddTeam( team );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::NotifyUpdated
// Created: LGY 2011-04-20
// -----------------------------------------------------------------------------
void PeopleAffinities::NotifyUpdated( const kernel::Team_ABC& team )
{
    dictionary_.Remove( tools::translate( "Affinities", "Affinities/%1" ).arg( teams_[ team.GetId() ].c_str() ) );
    AddTeam( team );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::NotifyDeleted
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void PeopleAffinities::NotifyDeleted( const kernel::Team_ABC& team )
{
    affinities_.erase(team.GetId() );
    teams_.erase( team.GetId() );
    dictionary_.Remove( tools::translate( "Affinities", "Affinities/%1" ).arg( team.GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::Clear
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
void PeopleAffinities::Clear()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::Add
// Created: MCO 2013-05-17
// -----------------------------------------------------------------------------
void PeopleAffinities::Add( unsigned long team, float value )
{
    Affinities::Add( team, value );
    controllers_.controller_.Update( gui::DictionaryUpdated( entity_, tools::translate( "Affinities", "Affinities/%1" ).arg( teams_[ team ].c_str() ) ) );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::InitializeAffinities
// Created: LGY 2011-04-27
// -----------------------------------------------------------------------------
void PeopleAffinities::InitializeAffinities()
{
    auto it = model_.GetTeamResolver().CreateIterator();
    while( it.HasMoreElements() )
        AddTeam( it.NextElement() );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::AddTeam
// Created: LGY 2011-04-27
// -----------------------------------------------------------------------------
void PeopleAffinities::AddTeam( const kernel::Team_ABC& team )
{
    if( affinities_.find( team.GetId() ) == affinities_.end() )
        affinities_[ team.GetId() ] = 0.f;
    teams_[ team.GetId() ] = team.GetName();
    auto it = affinities_.find( team.GetId() );
    dictionary_.RegisterExtension( entity_, this, tools::translate( "Affinities", "Affinities/%1" ).arg( team.GetName() ), it->second, true );
}
