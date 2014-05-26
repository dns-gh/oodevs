// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Affinities.h"
#include "TeamsModel.h"
#include "AffinitiesVisitor_ABC.h"
#include "actions/ParameterList.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: Affinities constructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
Affinities::Affinities( kernel::Entity_ABC& entity, kernel::Controller& controller, TeamsModel& teams, gui::PropertiesDictionary& dico )
    : entity_    ( entity )
    , controller_( controller )
    , teams_     ( teams )
    , dico_      ( dico )
    , baseName_  ( tools::translate( "Affinities", "Affinities" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Affinities destructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
Affinities::~Affinities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Affinities::DoUpdate
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
void Affinities::DoUpdate( const sword::PopulationUpdate& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: Affinities::DoUpdate
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
void Affinities::DoUpdate( const sword::CrowdUpdate& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: Affinities::DoUpdate
// Created: LGY 2011-03-15
// -----------------------------------------------------------------------------
void Affinities::DoUpdate( const sword::UnitAttributes& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: Affinities::Update
// Created: LGY 2011-05-06
// -----------------------------------------------------------------------------
template< typename T >
void Affinities::Update( const T& message )
{
    if( message.has_adhesions() )
    {
        if( !affinities_.empty() )
        {
            for( auto it = affinities_.begin(); it != affinities_.end(); ++it )
            {
                kernel::Team_ABC& team = teams_.GetTeam( it->first );
                dico_.Remove(  baseName_ + QString( "/%1" ).arg( team.GetName() ) );
            }
            affinities_.clear();
        }
        for( int i = 0; i < message.adhesions().adhesion_size(); ++i )
        {
            const sword::PartyAdhesion& adhesion = message.adhesions().adhesion( i );
            affinities_[ adhesion.party().id() ] = adhesion.value();
            kernel::Team_ABC& team = teams_.GetTeam( adhesion.party().id() );
            auto it = affinities_.find( adhesion.party().id() );
            dico_.RegisterExtension( entity_, this, baseName_ + QString( "/%1" ).arg( team.GetName() ), it->second );
            controller_.Update( gui::DictionaryUpdated( entity_, baseName_ ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Affinities::FillParameterList
// Created: ABR 2011-01-31
// -----------------------------------------------------------------------------
void Affinities::FillParameterList( actions::parameters::ParameterList* parameterList ) const
{
    for( auto it = changingAffinities_.begin(); it != changingAffinities_.end(); ++it )
    {
        actions::parameters::ParameterList& list = parameterList->AddList( "Affinity" );
        list.AddIdentifier( "ID", it->first );
        list.AddNumeric( "Value", it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: Affinities::Accept
// Created: LGY 2011-05-06
// -----------------------------------------------------------------------------
bool Affinities::Accept( AffinitiesVisitor_ABC& visitor )
{
    changingAffinities_ = !affinities_.empty() ? affinities_ : InitializeAffinities();
    BOOST_FOREACH( T_Affinities::value_type& affinity, changingAffinities_ )
    {
        kernel::Team_ABC& team = teams_.GetTeam( affinity.first );
        visitor.Visit( affinity.first, team.GetName().toStdString(), affinity.second );
    }
    return !affinities_.empty();
}

// -----------------------------------------------------------------------------
// Name: Affinities::ConfigureOptionalAffinities
// Created: LGY 2011-05-06
// -----------------------------------------------------------------------------
Affinities::T_Affinities Affinities::InitializeAffinities()
{
    T_Affinities result;
    tools::Iterator< const kernel::Team_ABC& > it = teams_.Resolver< kernel::Team_ABC >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Team_ABC& team = it.NextElement();
        result[ team.GetId() ] = 0.f;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: Affinities::Clear
// Created: LGY 2011-05-06
// -----------------------------------------------------------------------------
void Affinities::Clear()
{
    changingAffinities_.clear();
}
