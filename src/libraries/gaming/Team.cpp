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

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
Team::Team( const ASN1T_MsgSideCreation& asnMsg, Controller& controller )
    : EntityImplementation< Team_ABC >( controller, asnMsg.oid, asnMsg.nom )
    , karma_( asnMsg.type )
{
    if( name_.isEmpty() )
        name_ = QString( tools::translate( "Team", "Army %1" ) ).arg( asnMsg.oid );
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
// Name: Team::IsFriend
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
bool Team::IsFriend() const
{
    return karma_ == EnumDiplomatie::ami;
}
    
// -----------------------------------------------------------------------------
// Name: Team::IsEnemy
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
bool Team::IsEnemy() const
{
    return karma_ == EnumDiplomatie::ennemi;
}
    
// -----------------------------------------------------------------------------
// Name: Team::IsNeutral
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
bool Team::IsNeutral() const
{
    return karma_ == EnumDiplomatie::neutre;
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