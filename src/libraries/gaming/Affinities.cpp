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
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: Affinities constructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
Affinities::Affinities( kernel::Entity_ABC& entity, kernel::Controller& controller, TeamsModel& teams, kernel::PropertiesDictionary& dico )
    : entity_    ( entity )
    , controller_( controller )
    , teams_     ( teams )
    , dico_      ( dico )
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
        for( int i = 0; i < message.adhesions().adhesion_size(); ++i )
        {
            const sword::PartyAdhesion& adhesion = message.adhesions().adhesion( i );
            affinities_[ adhesion.party().id() ] = adhesion.value();
        }
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
    {
        kernel::Team_ABC& team = teams_.GetTeam( it->first );
        const QString baseName = tools::translate( "Affinities", "Affinities" );
        const QString key = baseName + QString( "/%1" ).arg( team.GetName() );
        if( !dico_.HasKey( key ) )
            dico_.Register( *this, key, it->second );
        else
            controller_.Update( kernel::DictionaryUpdated( entity_, baseName ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Affinities::FillParameterList
// Created: ABR 2011-01-31
// -----------------------------------------------------------------------------
void Affinities::FillParameterList( actions::parameters::ParameterList* parameterList ) const
{
    for( CIT_Affinities it = changingAffinities_.begin(); it != changingAffinities_.end(); ++it )
    {
        actions::parameters::ParameterList& list = parameterList->AddList( "Affinity" );
        list.AddIdentifier( "ID", it->first );
        list.AddNumeric( "Value", it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: Affinities::HasAffinities
// Created: LGY 2011-03-15
// -----------------------------------------------------------------------------
bool Affinities::HasAffinities() const
{
    return !affinities_.empty();
}

// -----------------------------------------------------------------------------
// Name: Affinities::Accept
// Created: LGY 2011-05-06
// -----------------------------------------------------------------------------
void Affinities::Accept( AffinitiesVisitor_ABC& visitor )
{
    changingAffinities_ = affinities_;
    BOOST_FOREACH( T_Affinities::value_type& affinity, changingAffinities_ )
    {
        kernel::Team_ABC& team = teams_.GetTeam( affinity.first );
        visitor.Visit( affinity.first, team.GetName().ascii(), affinity.second );
    }
}
