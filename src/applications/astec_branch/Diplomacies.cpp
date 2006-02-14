// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Diplomacies.h"
#include "Team.h"

// -----------------------------------------------------------------------------
// Name: Diplomacies constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Diplomacies::Diplomacies(  const Resolver_ABC< Team >& resolver )
    : resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Diplomacies destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Diplomacies::~Diplomacies()
{

}

// -----------------------------------------------------------------------------
// Name: Diplomacies::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
template< typename T >
void Diplomacies::DoUpdate( const T& message )
{
    Team* team1 = & resolver_.Get( message.oid_camp1 );
    Team* team2 = & resolver_.Get( message.oid_camp2 );
    if( & team1->Get< Diplomacies >() != this )
        diplomacies_[ & team1->Get< Diplomacies >() ] = message.diplomatie;
    if( & team2->Get< Diplomacies >() != this )
        diplomacies_[ & team2->Get< Diplomacies >() ] = message.diplomatie;
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Diplomacies::Update( const ASN1T_MsgChangeDiplomatieAck& message )
{
    if( message.error_code == EnumChangeDiplomatieErrorCode::no_error )
        DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Diplomacies::Update( const ASN1T_MsgChangeDiplomatie& message )
{
    DoUpdate( message );
}
