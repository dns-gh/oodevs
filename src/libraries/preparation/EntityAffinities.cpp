// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "EntityAffinities.h"
#include "Model.h"
#include "TeamsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/tools.h"
#include "tools/Iterator.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: EntityAffinities constructor
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
EntityAffinities::EntityAffinities( kernel::Controllers& controllers, Model& model, const kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dictionary )
    : controllers_( controllers )
    , model_      ( model )
    , entity_ ( entity )
    , dictionary_ ( dictionary )
{
    InitializeAffinities();
    UpdateDictionary();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityAffinities constructor
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
EntityAffinities::EntityAffinities( xml::xistream& xis, kernel::Controllers& controllers, Model& model, const kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dictionary )
    : controllers_( controllers )
    , model_      ( model )
    , entity_ ( entity )
    , dictionary_ ( dictionary )
{
    xis >> xml::optional
        >> xml::start( "adhesions" )
            >> xml::list( "adhesion", *this, &EntityAffinities::ReadAffinity )
        >> xml::end;
    if( affinities_.empty() )
        InitializeAffinities();
    UpdateDictionary();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityAffinities destructor
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
EntityAffinities::~EntityAffinities()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityAffinities::ReadAffinity
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void EntityAffinities::ReadAffinity( xml::xistream& xis )
{
    affinities_[ xis.attribute< unsigned long >( "party" ) ] = xis.attribute< float >( "value" );
}

// -----------------------------------------------------------------------------
// Name: EntityAffinities::UpdateDictionary
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void EntityAffinities::UpdateDictionary()
{
    for( T_Affinities::iterator it = affinities_.begin(); it != affinities_.end(); ++it )
        if( const kernel::Team_ABC* team = model_.teams_.Find( it->first ) )
        {
            dictionary_.Register( entity_, tools::translate( "PopulationAffinities", "Affinities/%1" ).arg( team->GetName() ), it->second );
            knownTeams_[ it->first ] = team->GetName();
        }
}

// -----------------------------------------------------------------------------
// Name: EntityAffinities::SerializeAttributes
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void EntityAffinities::SerializeAttributes( xml::xostream& xos ) const
{
    if ( !VerifyAffinitiesContent() )
        throw std::runtime_error( __FUNCTION__ ": affinities list differs from team list" );
    xos << xml::start( "adhesions" );
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
    {
        xos << xml::start( "adhesion" )
                << xml::attribute( "party", it->first )
                << xml::attribute( "value", it->second )
            << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: EntityAffinities::NotifyCreated
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void EntityAffinities::NotifyCreated( const kernel::Team_ABC& team )
{
    if( !dictionary_.HasKey( team.GetName() ) )
    {
        dictionary_.Register( entity_, tools::translate( "PopulationAffinities", "Affinities/%1" ).arg( team.GetName() ), affinities_[ team.GetId() ] );
        knownTeams_[ team.GetId() ] = team.GetName();
    }
}

// -----------------------------------------------------------------------------
// Name: EntityAffinities::NotifyUpdated
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void EntityAffinities::NotifyUpdated( const kernel::Team_ABC& team )
{
    dictionary_.Remove( tools::translate( "PopulationAffinities", "Affinities/%1" ).arg( knownTeams_[ team.GetId() ].c_str() ) );
    dictionary_.Register( entity_, tools::translate( "PopulationAffinities", "Affinities/%1" ).arg( team.GetName() ), affinities_[ team.GetId() ] );
    knownTeams_[ team.GetId() ] = team.GetName();
}

// -----------------------------------------------------------------------------
// Name: EntityAffinities::NotifyDeleted
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void EntityAffinities::NotifyDeleted( const kernel::Team_ABC& team )
{
    affinities_.erase( team.GetId() );
    knownTeams_.erase( team.GetId() );
    dictionary_.Remove( tools::translate( "PopulationAffinities", "Affinities/%1" ).arg( team.GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: EntityAffinities::InitializeAffinities
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void EntityAffinities::InitializeAffinities()
{
    tools::Iterator< const kernel::Team_ABC& > it = model_.teams_.CreateIterator();
    while( it.HasMoreElements() )
        affinities_[ it.NextElement().GetId() ] = 0.f;
}

// -----------------------------------------------------------------------------
// Name: EntityAffinities::VerifyAffinitiesContent
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
bool EntityAffinities::VerifyAffinitiesContent() const
{
    if( affinities_.size() != model_.teams_.Count() || affinities_.size() != knownTeams_.size() )
        return false;
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
        if( !model_.teams_.Find( it->first ) )
            return false;
    return true;
}
