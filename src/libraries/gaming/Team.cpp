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
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
Team::Team( const ASN1T_MsgTeamCreation& asnMsg, Controller& controller )
    : EntityImplementation< Team_ABC >( controller, asnMsg.oid, asnMsg.nom )
{
    if( name_.isEmpty() )
        name_ = QString( tools::translate( "Team", "Army %1" ) ).arg( asnMsg.oid );
    
    switch( asnMsg.type )
    {
        case EnumDiplomacy::inconnu : karma_ = kernel::Karma::unknown_; break;
        case EnumDiplomacy::ami :     karma_ = kernel::Karma::friend_; break;
        case EnumDiplomacy::ennemi :  karma_ = kernel::Karma::enemy_; break;
        case EnumDiplomacy::neutre :  karma_ = kernel::Karma::neutral_; break;
    }

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

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Team::Register( Formation_ABC& /*formation*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Team::Remove( Formation_ABC& /*formation*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Team::Register( Population_ABC& /*population*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Team::Remove( Population_ABC& /*population*/ )
{
    //NOTHING
}


// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Team::Register( Object_ABC& /*object*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Team::Remove( Object_ABC& /*object*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Team::Register( KnowledgeGroup_ABC& /*knGroup*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Team::Remove( KnowledgeGroup_ABC& /*knGroup*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Team::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
