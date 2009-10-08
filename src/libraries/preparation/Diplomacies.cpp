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
#include "Tools.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Diplomacies constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Diplomacies::Diplomacies( Controller& controller, const tools::Resolver_ABC< Team_ABC >& resolver, const Team_ABC& team, PropertiesDictionary& dico, TeamKarmas& karmas )
    : controller_( controller )
    , resolver_( resolver )
    , team_( team )
    , karma_( &karmas.GetDefault() )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies constructor
// Created: SBO 2008-12-10
// -----------------------------------------------------------------------------
Diplomacies::Diplomacies( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::Team_ABC >& resolver, const kernel::Team_ABC& team, kernel::PropertiesDictionary& dico, TeamKarmas& karmas )
    : controller_( controller )
    , resolver_( resolver )
    , team_( team )
    , karma_( 0 )
{
    std::string karma;
    xis >> xml::attribute( "type", karma );
    karma_ = &karmas.Get( karma.c_str() );
    CreateDictionary( dico );
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
void Diplomacies::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( (const Entity_ABC&)team_, tools::translate( "Team", "Info/Karma" ), karma_, *this, &Diplomacies::SetKarma );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::GetDiplomacy
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
const kernel::Karma& Diplomacies::GetDiplomacy( const kernel::Entity_ABC& entity ) const
{
    // $$$$ SBO 2008-12-09: look into hierarchy
    return const_cast< T_Diplomacies& >( diplomacies_ )[ &entity.Get< Diplomacies_ABC >() ];
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::SetDiplomacy
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
void Diplomacies::SetDiplomacy( const Team_ABC& team, const kernel::Karma& diplomacy )
{
    diplomacies_[ &team.Get< Diplomacies_ABC >() ] = diplomacy;
    controller_.Update( *(const Diplomacies_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::Serialize
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Diplomacies::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", int( team_.GetId() ) );
    tools::Iterator< const Team_ABC& > it = resolver_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Team_ABC& team = it.NextElement();
        if( &team == &team_ )
            continue;
        xos << xml::start( "relationship" )
                << xml::attribute( "side", int( team.GetId() ) );
        CIT_Diplomacies it = diplomacies_.find( &team.Get< Diplomacies_ABC >() );
        if( it != diplomacies_.end() )
            xos << xml::attribute( "diplomacy", it->second.GetId() );
        else
            xos << xml::attribute( "diplomacy", kernel::Karma::neutral_.GetId() );
        xos << xml::end();
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
    xis >> xml::attribute( "side", id )
        >> xml::attribute( "diplomacy", diplomacy );
    SetDiplomacy( resolver_.Get( id ), kernel::Karma::ResolveId( diplomacy.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::GetKarma
// Created: SBO 2008-12-10
// -----------------------------------------------------------------------------
const kernel::Karma& Diplomacies::GetKarma() const
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
    that.Get< kernel::TacticalHierarchies >().UpdateSymbol( false );
    that.Get< kernel::IntelligenceHierarchies >().UpdateSymbol( false );
    controller_.Update( *(const Diplomacies_ABC*)this );
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
