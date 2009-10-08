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
#include "clients_kernel/Karma.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CommunicationHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Diplomacies constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Diplomacies::Diplomacies( Controller& controller, const tools::Resolver_ABC< Team_ABC >& resolver )
    : controller_( controller )
    , resolver_( resolver )
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

namespace
{
    Karma ResolveDiplomacy( const ASN1T_EnumDiplomacy& diplomacy )
    {
        switch( diplomacy )
        {
        case EnumDiplomacy::ami:    return Karma::friend_;
        case EnumDiplomacy::ennemi: return Karma::enemy_;
        case EnumDiplomacy::neutre: return Karma::neutral_;
        }
        return kernel::Karma::unknown_;
    }
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::UpdateData
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
template< typename T >
void Diplomacies::UpdateData( const T& message )
{
    const Team_ABC& team1 = resolver_.Get( message.oid_camp1 );
    const Team_ABC& team2 = resolver_.Get( message.oid_camp2 );
    if( & team1.Get< Diplomacies_ABC >() != this )
        return;
    if( & team2.Get< Diplomacies_ABC >() != this )
        diplomacies_[ & team2.Get< Diplomacies_ABC >() ] = ResolveDiplomacy( message.diplomatie );
    controller_.Update( *(kernel::Diplomacies_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Diplomacies::DoUpdate( const ASN1T_MsgChangeDiplomacyAck& message )
{
    if( message.error_code == EnumChangeDiplomacyErrorCode::no_error )
        UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Diplomacies::DoUpdate( const ASN1T_MsgChangeDiplomacy& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::GetDiplomacy
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
const kernel::Karma& Diplomacies::GetDiplomacy( const kernel::Entity_ABC& rhs ) const
{
    const CommunicationHierarchies* h = rhs.Retrieve< CommunicationHierarchies >();
    const kernel::Entity_ABC* team = h ? & h->GetTop() : 0;
    const Diplomacies_ABC* o = team ? team->Retrieve< Diplomacies_ABC >() : 0;
    CIT_Diplomacies it = diplomacies_.find( o );
    if( it != diplomacies_.end() )
        return it->second;
    return kernel::Karma::unknown_;
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::DoUpdate
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void Diplomacies::DoUpdate( const ASN1T_MsgTeamCreation& message )
{
    switch( message.type )
    {
    case EnumDiplomacy::inconnu : karma_ = kernel::Karma::unknown_; break;
    case EnumDiplomacy::ami :     karma_ = kernel::Karma::friend_; break;
    case EnumDiplomacy::ennemi :  karma_ = kernel::Karma::enemy_; break;
    case EnumDiplomacy::neutre :  karma_ = kernel::Karma::neutral_; break;
    }
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::GetKarma
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
const kernel::Karma& Diplomacies::GetKarma() const
{
    return karma_;
}
