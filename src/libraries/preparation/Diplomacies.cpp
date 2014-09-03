// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Diplomacies.h"
#include "TeamKarmas.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Diplomacies constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Diplomacies::Diplomacies( Controller& controller, const tools::Resolver_ABC< Team_ABC >& resolver,
                          const Team_ABC& team, gui::PropertiesDictionary& dictionary, TeamKarmas& karmas )
    : controller_( controller )
    , resolver_  ( resolver )
    , team_      ( team )
    , karma_     ( &karmas.GetDefault() )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies constructor
// Created: SBO 2008-12-10
// -----------------------------------------------------------------------------
Diplomacies::Diplomacies( xml::xistream& xis, Controller& controller, const tools::Resolver_ABC< Team_ABC >& resolver,
                          const Team_ABC& team, gui::PropertiesDictionary& dictionary, TeamKarmas& karmas )
    : controller_( controller )
    , resolver_  ( resolver )
    , team_      ( team )
    , karma_     ( 0 )
{
    karma_ = &karmas.Get( xis.attribute< std::string >( "type" ).c_str() );
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Diplomacies::~Diplomacies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::CreateDictionary
// Created: SBO 2008-12-10
// -----------------------------------------------------------------------------
void Diplomacies::CreateDictionary( gui::PropertiesDictionary& dictionary )
{
    dictionary.Register( static_cast< const Entity_ABC& >( team_ ), tools::translate( "preparation::Team", "Info/Karma" ), karma_, *this, &Diplomacies::SetKarma, false );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::GetDiplomacy
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
const Karma& Diplomacies::GetDiplomacy( const Entity_ABC& entity ) const
{
    // $$$$ SBO 2008-12-09: look into hierarchy
    CIT_Diplomacies it = diplomacies_.find( &entity.Get< Diplomacies_ABC >() );
    if( it != diplomacies_.end() )
        return it->second;
    return Karma::unknown_;
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::SetDiplomacy
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
void Diplomacies::SetDiplomacy( const Team_ABC& team, const Karma& diplomacy )
{
    diplomacies_[ &team.Get< Diplomacies_ABC >() ] = diplomacy;
    controller_.Update( *static_cast< const Diplomacies_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::Serialize
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Diplomacies::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", team_.GetId() );
    tools::Iterator< const Team_ABC& > it = resolver_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Team_ABC& team = it.NextElement();
        if( &team == &team_ )
            continue;
        xos << xml::start( "relationship" )
                << xml::attribute( "party", team.GetId() );
        CIT_Diplomacies it = diplomacies_.find( &team.Get< Diplomacies_ABC >() );
        if( it != diplomacies_.end() )
            xos << xml::attribute( "diplomacy", it->second.GetId() );
        else
            xos << xml::attribute( "diplomacy", Karma::neutral_.GetId() );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::Load
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Diplomacies::Load( xml::xistream& xis )
{
    xis >> xml::list( "relationship", *this, &Diplomacies::ReadRelationship );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::ReadRelationship
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Diplomacies::ReadRelationship( xml::xistream& xis )
{
    int id;
    std::string diplomacy;
    xis >> xml::attribute( "party", id )
        >> xml::attribute( "diplomacy", diplomacy );
    SetDiplomacy( resolver_.Get( id ), Karma::ResolveId( diplomacy.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::GetKarma
// Created: SBO 2008-12-10
// -----------------------------------------------------------------------------
const Karma& Diplomacies::GetKarma() const
{
    return *karma_;
}

// -----------------------------------------------------------------------------
// Name: Team::SetKarma
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
void Diplomacies::SetKarma( const TeamKarma& karma )
{
    karma_ = karma;
    // $$$$ SBO 2008-12-10: use notifications somehow
    Team_ABC& that = const_cast< Team_ABC& >( team_ );
    that.Get< TacticalHierarchies >().UpdateSymbolDownward();
    controller_.Update( *static_cast< const Diplomacies_ABC* >( this ) );
    controller_.Update( team_ );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::SerializeAttributes
// Created: SBO 2008-12-10
// -----------------------------------------------------------------------------
void Diplomacies::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "type", karma_->GetId() );
}
