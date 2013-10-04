// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Troops.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Automat_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Troops constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Troops::Troops( Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver )
    : HierarchicExtension_ABC( automatResolver, formationResolver, teamResolver )
    , controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Troops destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Troops::~Troops()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Troops::DoUpdate
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
void Troops::DoUpdate( const sword::UnitAttributes& message )
{
    if( !message.has_human_dotations() )
        return;
    T_HumanStateVector differences( elements_ );
    for( auto it = differences.begin(); it != differences.end(); ++it )
        it->quantity_ = -it->quantity_;
    for( int i = 0; i < message.human_dotations().elem_size(); ++i )
        UpdateHumanState( differences, message.human_dotations().elem( i ) );
    Update( differences );
}

// -----------------------------------------------------------------------------
// Name: Troops::Update
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Troops::Update( const T_HumanStateVector& differences )
{
    for( auto it = differences.begin(); it != differences.end(); ++it )
        UpdateHumanState( elements_, *it );
    CleanHumanStates();
    if( kernel::Entity_ABC* superior = const_cast< kernel::Entity_ABC* >( GetSuperior() ) )
        if( Troops* troops = superior->Retrieve< Troops >() )
            troops->Update( differences );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Troops::SetSuperior
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
void Troops::SetSuperior( const kernel::Entity_ABC& superior )
{
    kernel::Entity_ABC* currentSuperior = const_cast< kernel::Entity_ABC* >( GetSuperior() );
    if( &superior == currentSuperior )
        return;
    // add troops to new superior
    if( Troops* troops = const_cast< kernel::Entity_ABC& >( superior ).Retrieve< Troops >() )
        troops->Update( elements_ );
    if( ! currentSuperior )
        return;
    // remove troops from previous superior
    if( Troops* troops = currentSuperior->Retrieve< Troops >() )
    {
        T_HumanStateVector differences( elements_ );
        for( auto it = differences.begin(); it != differences.end(); ++it )
            it->quantity_ = -it->quantity_;
        troops->Update( differences );
    }
}

// -----------------------------------------------------------------------------
// Name: Troops::GetTotalHumans
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
unsigned int Troops::GetTotalHumans() const
{
    unsigned int result = 0;
    for( std::size_t i = 0; i < elements_.size(); ++i )
        result += elements_[ i ].quantity_;
    return result;
}

// -----------------------------------------------------------------------------
// Name: Troops::GetTotalByRank
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
unsigned int Troops::GetTotalByRank( E_HumanRank rank ) const
{
    unsigned int result = 0;
    for( std::size_t i = 0; i < elements_.size(); ++i )
        if( elements_[ i ].rank_ == rank )
            result += elements_[ i ].quantity_;
    return result;
}

// -----------------------------------------------------------------------------
// Name: Troops::UpdateHumanState
// Created: ABR 2011-07-28
// -----------------------------------------------------------------------------
void Troops::UpdateHumanState( T_HumanStateVector& states, const HumanState& state ) const
{
    auto it = std::find( states.begin(), states.end(), state );
    if( it == states.end() )
        states.push_back( state );
    else
        it->quantity_ += state.quantity_;
}

// -----------------------------------------------------------------------------
// Name: Troops::CleanHumanStates
// Created: ABR 2011-08-29
// -----------------------------------------------------------------------------
void Troops::CleanHumanStates()
{
    for( auto it = elements_.begin(); it != elements_.end(); )
        if( it->quantity_ == 0 )
            it = elements_.erase( it );
        else
            ++it;
}
