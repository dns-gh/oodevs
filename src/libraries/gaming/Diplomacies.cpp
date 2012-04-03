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
#include "protocol/Protocol.h"

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
    Karma ResolveDiplomacy( const sword::EnumDiplomacy& diplomacy )
    {
        switch( diplomacy )
        {
        case sword::friendly:
            return Karma::friend_;
        case sword::enemy:
            return Karma::enemy_;
        case sword::neutral:
            return Karma::neutral_;
        default:
            return kernel::Karma::unknown_;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::UpdateData
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
template< typename T >
void Diplomacies::UpdateData( const T& message )
{
    const Team_ABC& team1 = resolver_.Get( message.party1().id() );
    const Team_ABC& team2 = resolver_.Get( message.party2().id() );
    if( & team1.Get< Diplomacies_ABC >() != this )
        return;
    if( & team2.Get< Diplomacies_ABC >() != this )
        diplomacies_[ & team2.Get< Diplomacies_ABC >() ] = ResolveDiplomacy( message.diplomacy() );
    controller_.Update( *(kernel::Diplomacies_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Diplomacies::DoUpdate( const sword::ChangeDiplomacyAck& message )
{
    if( message.error_code() == sword::ChangeDiplomacyAck::no_error_diplomacy )
        UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: Diplomacies::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Diplomacies::DoUpdate( const sword::ChangeDiplomacy& message )
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
void Diplomacies::DoUpdate( const sword::PartyCreation& message )
{
    switch( message.type() )
    {
    case sword::friendly :
        karma_ = kernel::Karma::friend_;
        break;
    case sword::enemy :
        karma_ = kernel::Karma::enemy_;
        break;
    case sword::neutral :
        karma_ = kernel::Karma::neutral_;
        break;
    default:
        karma_ = kernel::Karma::unknown_;
        break;
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
