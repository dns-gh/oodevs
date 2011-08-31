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
    for( IT_HumanStateVector it = differences.begin(); it != differences.end(); ++it )
        it->quantity_ = -it->quantity_;
    for( int i = 0; i < message.human_dotations().elem_size(); ++i )
    {
        HumanState current( message.human_dotations().elem( i ) );
        if( HumanState* difference = FindHumanState( differences, current ) )
            difference->quantity_ += current.quantity_;
        else
            differences.push_back( current );
    }
    Update( differences );
}

// -----------------------------------------------------------------------------
// Name: Troops::Update
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Troops::Update( const T_HumanStateVector& differences )
{
    for( CIT_HumanStateVector it = differences.begin(); it != differences.end(); ++it )
    {
        if( it->quantity_ == 0 )
            continue;
        HumanState* humanState = FindHumanState( elements_, *it );
        if( !humanState )
            elements_.push_back( HumanState( *it ) );
        else
            humanState->quantity_ += it->quantity_;
    }
    RemoveHumanStates();
    if( const kernel::Entity_ABC* superior = GetSuperior() )
        if( Troops* troops = const_cast< Troops* >( superior->Retrieve< Troops >() ) )
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
    if( !currentSuperior || &superior != currentSuperior )
    {
        // create troops differences
        T_HumanStateVector differences( elements_ );
        // add troops to new superior
        if( Troops* troops = const_cast< kernel::Entity_ABC& >( superior ).Retrieve< Troops >() )
            troops->Update( differences );
        // remove troops from previous superior
        if( currentSuperior )
            if( Troops* troops = currentSuperior->Retrieve< Troops >() )
            {
                for( IT_HumanStateVector it = differences.begin(); it != differences.end(); ++it )
                    it->quantity_ = -it->quantity_;
                troops->Update( differences );
            }
    }
}

// -----------------------------------------------------------------------------
// Name: Troops::GetTotalHumans
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
unsigned int Troops::GetTotalHumans() const
{
    unsigned int result = 0;
    for( unsigned i = 0; i < elements_.size(); ++i )
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
    for( unsigned i = 0; i < elements_.size(); ++i )
        if( elements_[ i ].rank_ == rank )
            result += elements_[ i ].quantity_;
    return result;
}

// -----------------------------------------------------------------------------
// Name: Troops::FindHumanState
// Created: ABR 2011-07-28
// -----------------------------------------------------------------------------
HumanState* Troops::FindHumanState( T_HumanStateVector& container, const HumanState& state ) const
{
    for( CIT_HumanStateVector it = container.begin(); it != container.end(); ++it )
        if( *it == state )
        {
            unsigned int i = 0;
            for( ; i < state.injuries_.size(); ++i )
                if( state.injuries_[ i ].first != it->injuries_[ i ].first || state.injuries_[ i ].second != it->injuries_[ i ].second )
                    break;
            if( i == state.injuries_.size() )
                return const_cast< HumanState* >( &( *it ) );
        }
        return 0;
}

// -----------------------------------------------------------------------------
// Name: Troops::RemoveHumanStates
// Created: ABR 2011-08-29
// -----------------------------------------------------------------------------
void Troops::RemoveHumanStates()
{
    for( IT_HumanStateVector it = elements_.begin(); it != elements_.end(); )
        if( it->quantity_ == 0 )
            it = elements_.erase( it );
        else
            ++it;
}
