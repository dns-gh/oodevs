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
#include "Diplomacy.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controller.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Diplomacies constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Diplomacies::Diplomacies( Controller& controller, const Resolver_ABC< Team_ABC >& resolver, const Team_ABC& team )
    : controller_( controller )
    , resolver_( resolver )
    , team_( team )
{
    // NOTHING
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
// Name: Diplomacies::GetDiplomacy
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
const Diplomacy& Diplomacies::GetDiplomacy( const Team_ABC& team )
{
    return diplomacies_[ &team.Get< Diplomacies >() ];
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::SetDiplomacy
// Created: SBO 2006-09-07
// -----------------------------------------------------------------------------
void Diplomacies::SetDiplomacy( const Team_ABC& team, const Diplomacy& diplomacy )
{
    diplomacies_[ &team.Get< Diplomacies >() ] = diplomacy;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::Serialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Diplomacies::Serialize( xml::xostream& xos ) const
{
    xos << start( "Diplomatie" );
    for( CIT_Diplomacies it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        xos << start( "Armee" )
                << attribute( "nom", it->first->team_.GetName() )
                << attribute( "relation", it->second.GetValue() )
            << end();
    }
    xos << end();
}
