// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentAffinities.h"
#include "Model.h"
#include "TeamsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/DictionaryUpdated.h"
#include <tools/Iterator.h>
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: AgentAffinities constructor
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
AgentAffinities::AgentAffinities( kernel::Entity_ABC& agent, kernel::Controllers& controllers, Model& model,
                                  gui::PropertiesDictionary& dictionary, const QString& propertyName )
    : Affinities( model )
    , agent_       ( agent )
    , controllers_ ( controllers )
    , dictionary_  ( dictionary )
    , propertyName_( propertyName )
{
    InitializeAffinities();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities constructor
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
AgentAffinities::AgentAffinities( xml::xistream& xis, kernel::Entity_ABC& agent, kernel::Controllers& controllers, Model& model,
                                  gui::PropertiesDictionary& dictionary, const QString& propertyName )
    : Affinities( xis, model )
    , agent_       ( agent )
    , controllers_ ( controllers )
    , dictionary_  ( dictionary )
    , propertyName_( propertyName )
{
    InitializeAffinities();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities destructor
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
AgentAffinities::~AgentAffinities()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::Add
// Created: LGY 2011-04-20
// -----------------------------------------------------------------------------
void AgentAffinities::Add( unsigned long team, float affinity )
{
    affinities_[ team ] = affinity;
    auto it = affinities_.find( team );
    dictionary_.Register( agent_, propertyName_ + "/" + teams_[ team ].c_str(), it->second );
    controllers_.controller_.Update( gui::DictionaryUpdated( agent_, propertyName_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::Clear
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
void AgentAffinities::Clear()
{
    BOOST_FOREACH( const T_Affinities::value_type& value, affinities_ )
        dictionary_.Remove( propertyName_ + "/" + teams_[ value.first ].c_str() );
    affinities_.clear();
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::NotifyCreated
// Created: LGY 2011-04-20
// -----------------------------------------------------------------------------
void AgentAffinities::NotifyCreated( const kernel::Team_ABC& team )
{
    if( !affinities_.empty() && affinities_.find( team.GetId() ) == affinities_.end() )
        affinities_[ team.GetId() ] = 0.f;
    AddTeam( team );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::NotifyUpdated
// Created: LGY 2011-04-20
// -----------------------------------------------------------------------------
void AgentAffinities::NotifyUpdated( const kernel::Team_ABC& team )
{
    AddTeam( team );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::NotifyDeleted
// Created: LGY 2011-04-20
// -----------------------------------------------------------------------------
void AgentAffinities::NotifyDeleted( const kernel::Team_ABC& team )
{
    teams_.erase( team.GetId() );
    affinities_.erase( team.GetId() );
    dictionary_.Remove( propertyName_ + "/" + team.GetName() );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::InitializeAffinities
// Created: LGY 2011-04-20
// -----------------------------------------------------------------------------
void AgentAffinities::InitializeAffinities()
{
    auto it = model_.GetTeamResolver().CreateIterator();
    while( it.HasMoreElements() )
        AddTeam( it.NextElement() );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::AddTeam
// Created: LGY 2011-04-27
// -----------------------------------------------------------------------------
void AgentAffinities::AddTeam( const kernel::Team_ABC& team )
{
    dictionary_.Remove( propertyName_ + "/" + teams_[ team.GetId() ].c_str() );
    teams_[ team.GetId() ] = team.GetName();
    auto it = affinities_.find( team.GetId() );
    if( it != affinities_.end() )
        dictionary_.Register( agent_, propertyName_ + "/" + teams_[ team.GetId() ].c_str(), it->second );
}
