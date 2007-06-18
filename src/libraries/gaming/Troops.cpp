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
Troops::Troops( Controller& controller, const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const kernel::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const kernel::Resolver_ABC< kernel::Team_ABC >& teamResolver )
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
    unsigned Humans::* Rank( ASN1T_EnumHumanRank rank )
    {
        if( rank == EnumHumanRank::officier )
            return &Humans::officers_;
        if( rank == EnumHumanRank::sous_officer )
            return &Humans::subOfficers_;
        return &Humans::troopers_;
    }
}

// -----------------------------------------------------------------------------
// Name: Troops::AddDifference
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Troops::AddDifference( T_Differences& differences, kernel::E_TroopHealthState state, ASN1T_EnumHumanRank rank, int value )
{
    differences[ std::make_pair( state, rank ) ] = value - humans_[state].*Rank( rank );
}

// -----------------------------------------------------------------------------
// Name: Troops::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Troops::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( ! message.m.dotation_eff_personnelPresent  )
        return;

    T_Differences differences;
    uint nSize = message.dotation_eff_personnel.n;
    while( nSize > 0 )
    {
        const ASN1T_HumanDotations& dot = message.dotation_eff_personnel.elem[ --nSize ];
        AddDifference( differences, eTroopHealthStateTotal             , dot.rang, dot.nb_total );
        AddDifference( differences, eTroopHealthStateOperational       , dot.rang, dot.nb_operationnels );
        AddDifference( differences, eTroopHealthStateDead              , dot.rang, dot.nb_morts );
        AddDifference( differences, eTroopHealthStateWounded           , dot.rang, dot.nb_blesses );
        AddDifference( differences, eTroopHealthStateMentalWounds      , dot.rang, dot.nb_blesses_mentaux );
        AddDifference( differences, eTroopHealthStateContaminated      , dot.rang, dot.nb_contamines_nbc );
        AddDifference( differences, eTroopHealthStateInTreatment       , dot.rang, dot.nb_dans_chaine_sante );
        AddDifference( differences, eTroopHealthStateUsedForMaintenance, dot.rang, dot.nb_utilises_pour_maintenance );
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
    for( unsigned int i = 0; i < (unsigned int)kernel::eTroopHealthStateNbrStates; ++i )
    {
        differences[std::make_pair( (kernel::E_TroopHealthState)i, EnumHumanRank::officier )]     = int( humans_[i].officers_ );
        differences[std::make_pair( (kernel::E_TroopHealthState)i, EnumHumanRank::sous_officer )] = int( humans_[i].subOfficers_ );
        differences[std::make_pair( (kernel::E_TroopHealthState)i, EnumHumanRank::mdr )]          = int( humans_[i].troopers_ );
    }
    if( Troops* troops = const_cast< Troops* >( superior.Retrieve< Troops >() ) )
        troops->Update( differences );

    if( const kernel::Entity_ABC* superior = GetSuperior() )
        if( Troops* troops = const_cast< Troops* >( superior->Retrieve< Troops >() ) )
        {
            differences.clear();
            for( unsigned int i = 0; i < (unsigned int)kernel::eTroopHealthStateNbrStates; ++i )
            {
                differences[std::make_pair( (kernel::E_TroopHealthState)i, EnumHumanRank::officier )]     = -int( humans_[i].officers_ );
                differences[std::make_pair( (kernel::E_TroopHealthState)i, EnumHumanRank::sous_officer )] = -int( humans_[i].subOfficers_ );
                differences[std::make_pair( (kernel::E_TroopHealthState)i, EnumHumanRank::mdr )]          = -int( humans_[i].troopers_ );
            }
            troops->Update( differences );
        }
}
