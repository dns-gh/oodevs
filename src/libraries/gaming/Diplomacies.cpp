// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Diplomacies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CommunicationHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Diplomacies constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Diplomacies::Diplomacies( Controller& controller, const Resolver_ABC< Team_ABC >& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , karma_( 'u' )
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
// Name: Diplomacies::UpdateData
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
template< typename T >
void Diplomacies::UpdateData( const T& message )
{
    Team_ABC* team1 = & resolver_.Get( message.oid_camp1 );
    Team_ABC* team2 = & resolver_.Get( message.oid_camp2 );
    if( & team1->Get< Diplomacies >() != this )
        diplomacies_[ & team1->Get< Diplomacies >() ] = message.diplomatie;
    if( & team2->Get< Diplomacies >() != this )
        diplomacies_[ & team2->Get< Diplomacies >() ] = message.diplomatie;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Diplomacies::DoUpdate( const ASN1T_MsgChangeDiplomatieAck& message )
{
    if( message.error_code == EnumChangeDiplomatieErrorCode::no_error )
        UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Diplomacies::DoUpdate( const ASN1T_MsgChangeDiplomatie& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::DoUpdate
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void Diplomacies::DoUpdate( const ASN1T_MsgSideCreation& message )
{
    switch( message.type )
    {
    case EnumDiplomatie::inconnu : karma_ = 'u'; break;
    case EnumDiplomatie::ami :     karma_ = 'f'; break;
    case EnumDiplomatie::ennemi :  karma_ = 'h'; break;
    case EnumDiplomatie::neutre :  karma_ = 'n'; break;
    }
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::GetKarma
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
char Diplomacies::GetKarma() const
{
    return karma_;
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::GetRelationship
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
ASN1T_EnumDiplomatie Diplomacies::GetRelationship( const kernel::Entity_ABC& rhs ) const
{
    const CommunicationHierarchies* h = rhs.Retrieve< CommunicationHierarchies >();
    const kernel::Entity_ABC* team = h ? & h->GetTop() : 0;
    const Diplomacies* o = team ? team->Retrieve< Diplomacies >() : 0;
    CIT_Diplomacies it = diplomacies_.find( o );
    if( it != diplomacies_.end() )
        return it->second;
    return EnumDiplomatie::inconnu;
}
