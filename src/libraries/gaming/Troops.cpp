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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Troops constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Troops::Troops( Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver )
    : HierarchicExtension_ABC( automatResolver, formationResolver, teamResolver )
    , controller_( controller )
{
    for( int i = 0; i < eTroopHealthStateNbrStates; ++i )
        humans_[ i ].state_ = E_TroopHealthState( i );
}

// -----------------------------------------------------------------------------
// Name: Troops destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Troops::~Troops()
{
    // NOTHING
}

namespace
{
    unsigned Humans::* Rank( sword::EnumHumanRank rank )
    {
        if( rank == sword::officer )
            return &Humans::officers_;
        if( rank == sword::sub_officer )
            return &Humans::subOfficers_;
        return &Humans::troopers_;
    }
}

// -----------------------------------------------------------------------------
// Name: Troops::AddDifference
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Troops::AddDifference( T_Differences& differences, kernel::E_TroopHealthState state, sword::EnumHumanRank rank, int value )
{
    differences[ std::make_pair( state, rank ) ] = value - humans_[state].*Rank( rank );
}

// -----------------------------------------------------------------------------
// Name: Troops::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Troops::DoUpdate( const sword::UnitAttributes& message )
{
    if( ! message.has_human_dotations() )
        return;
    T_Differences differences;
    uint nSize = message.human_dotations().elem_size();
    while( nSize > 0 )
    {
        const sword::HumanDotations_HumanDotation& dot = message.human_dotations().elem( --nSize );
        AddDifference( differences, eTroopHealthStateTotal             , dot.rank(), dot.total() );
        AddDifference( differences, eTroopHealthStateOperational       , dot.rank(), dot.operational() );
        AddDifference( differences, eTroopHealthStateDead              , dot.rank(), dot.dead() );
        AddDifference( differences, eTroopHealthStateWounded           , dot.rank(), dot.wounded() );
        AddDifference( differences, eTroopHealthStateMentalWounds      , dot.rank(), dot.mentally_wounded() );
        AddDifference( differences, eTroopHealthStateContaminated      , dot.rank(), dot.contaminated() );
        AddDifference( differences, eTroopHealthStateInTreatment       , dot.rank(), dot.healing() );
        AddDifference( differences, eTroopHealthStateUsedForMaintenance, dot.rank(), dot.maintenance() );
    }
    Update( differences );
}

// -----------------------------------------------------------------------------
// Name: Troops::Update
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Troops::Update( const T_Differences& differences )
{
    for( T_Differences::const_iterator it = differences.begin(); it != differences.end(); ++it )
        humans_[it->first.first].*Rank( it->first.second ) += it->second;

    if( const kernel::Entity_ABC* superior = GetSuperior() )
        if( Troops* troops = const_cast< Troops* >( superior->Retrieve< Troops >() ) )
            troops->Update( differences );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Troops::SetSuperior
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void Troops::SetSuperior( const kernel::Entity_ABC& superior )
{
    T_Differences differences;
    for( unsigned int i = 0; i < unsigned int( kernel::eTroopHealthStateNbrStates ); ++i )
    {
        differences[std::make_pair( (kernel::E_TroopHealthState)i, sword::officer )]     = int( humans_[i].officers_ );
        differences[std::make_pair( (kernel::E_TroopHealthState)i, sword::sub_officer )] = int( humans_[i].subOfficers_ );
        differences[std::make_pair( (kernel::E_TroopHealthState)i, sword::trooper )]          = int( humans_[i].troopers_ );
    }
    if( Troops* troops = const_cast< Troops* >( superior.Retrieve< Troops >() ) )
        troops->Update( differences );

    if( const kernel::Entity_ABC* superior = GetSuperior() )
        if( Troops* troops = const_cast< Troops* >( superior->Retrieve< Troops >() ) )
        {
            differences.clear();
            for( unsigned int i = 0; i < unsigned int( kernel::eTroopHealthStateNbrStates ); ++i )
            {
                differences[std::make_pair( (kernel::E_TroopHealthState)i, sword::officer )]     = -int( humans_[i].officers_ );
                differences[std::make_pair( (kernel::E_TroopHealthState)i, sword::sub_officer )] = -int( humans_[i].subOfficers_ );
                differences[std::make_pair( (kernel::E_TroopHealthState)i, sword::trooper )]          = -int( humans_[i].troopers_ );
            }
            troops->Update( differences );
        }
}

// -----------------------------------------------------------------------------
// Name: Troops::Troops::GetTotalHumans
// Created: SLG 2010-05-28
// -----------------------------------------------------------------------------
int Troops::GetTotalHumans() const
{
    int nbrHumans  = 0;
    for( unsigned int i = 0; i < unsigned int( kernel::eTroopHealthStateNbrStates ) ; ++i )
    {
        nbrHumans += ( humans_[ i ].officers_ + humans_[ i ].subOfficers_ + humans_[ i ].troopers_ );
    }
    return nbrHumans;
}