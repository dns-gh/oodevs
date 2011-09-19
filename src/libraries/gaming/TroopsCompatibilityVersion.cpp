// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TroopsCompatibilityVersion.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Automat_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TroopsCompatibilityVersion constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
TroopsCompatibilityVersion::TroopsCompatibilityVersion( Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver )
    : HierarchicExtension_ABC( automatResolver, formationResolver, teamResolver )
    , controller_( controller )
{
    for( int i = 0; i < eTroopHealthStateNbrStates; ++i )
        humans_[ i ].state_ = E_TroopHealthState( i );
}

// -----------------------------------------------------------------------------
// Name: TroopsCompatibilityVersion destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
TroopsCompatibilityVersion::~TroopsCompatibilityVersion()
{
    // NOTHING
}

namespace
{
    int Humans::* Rank( sword::EnumHumanRank rank )
    {
        if( rank == sword::officer )
            return &Humans::officers_;
        if( rank == sword::sub_officer )
            return &Humans::subOfficers_;
        return &Humans::troopers_;
    }
}

// -----------------------------------------------------------------------------
// Name: TroopsCompatibilityVersion::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void TroopsCompatibilityVersion::DoUpdate( const sword::UnitAttributes& message )
{
    if( !message.has_human_dotations() )
        return;
    Humans current[ kernel::eTroopHealthStateNbrStates ];
    for( int i = 0; i < eTroopHealthStateNbrStates; ++i )
        current[ i ].state_ = E_TroopHealthState( i );
    for( int i = 0; i < message.human_dotations().elem().size(); ++i )
    {
        const sword::HumanDotations_HumanDotation& elem = message.human_dotations().elem( i );
        sword::EnumHumanRank rank = elem.rank();
        sword::EnumHumanState state = elem.state();
        sword::EnumHumanLocation location = elem.location();
        int quantity = elem.quantity();

        current[ eTroopHealthStateTotal ].*Rank( rank ) += quantity;
        if( state == sword::healthy && location != sword::medical && !elem.mentally_wounded() && !elem.contaminated() )
            current[ eTroopHealthStateOperational ].*Rank( rank ) += quantity;
        if( state == sword::deadly )
            current[ eTroopHealthStateDead ].*Rank( rank ) += quantity;
        if( state != sword::healthy && state != sword::deadly )
            current[ eTroopHealthStateWounded ].*Rank( rank ) += quantity;
        if( elem.mentally_wounded() )
            current[ eTroopHealthStateMentalWounds ].*Rank( rank ) += quantity;
        if( elem.contaminated() )
            current[ eTroopHealthStateContaminated ].*Rank( rank ) += quantity;
        if( state != sword::deadly && location == sword::medical )
            current[ eTroopHealthStateInTreatment ].*Rank( rank ) += quantity;
        if( location == sword::maintenance )
            current[ eTroopHealthStateUsedForMaintenance ].*Rank( rank ) += quantity;
        if( state == sword::injured && location == sword::battlefield )
            current[ eTroopHealthStateWoundedNotEvacuated ].*Rank( rank ) += quantity;
    }
    Humans differences[ kernel::eTroopHealthStateNbrStates ];
    for( int i = 0; i < eTroopHealthStateNbrStates; ++i )
        differences[ i ] = humans_[ i ] - current[ i ];
    Update( differences );
}

// -----------------------------------------------------------------------------
// Name: TroopsCompatibilityVersion::Update
// Created: ABR 2011-08-30
// -----------------------------------------------------------------------------
void TroopsCompatibilityVersion::Update( const Humans differences[ kernel::eTroopHealthStateNbrStates ] )
{
    for( int i = 0; i < eTroopHealthStateNbrStates; ++i )
        humans_[ i ] -= differences[ i ];
    if( const kernel::Entity_ABC* superior = GetSuperior() )
        if( TroopsCompatibilityVersion* troops = const_cast< TroopsCompatibilityVersion* >( superior->Retrieve< TroopsCompatibilityVersion >() ) )
            troops->Update( differences );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: TroopsCompatibilityVersion::SetSuperior
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void TroopsCompatibilityVersion::SetSuperior( const kernel::Entity_ABC& superior )
{
    kernel::Entity_ABC* currentSuperior = const_cast< kernel::Entity_ABC* >( GetSuperior() );
    if( !currentSuperior || &superior != currentSuperior )
    {
        // create troops differences
        Humans differences[ kernel::eTroopHealthStateNbrStates ];
        for( int i = 0; i < kernel::eTroopHealthStateNbrStates; ++i )
            differences[ i ] = humans_[ i ];
        // add troops to new superior
        if( TroopsCompatibilityVersion* troops = const_cast< kernel::Entity_ABC& >( superior ).Retrieve< TroopsCompatibilityVersion >() )
            troops->Update( differences );
        // remove troops from previous superior
        if( currentSuperior )
            if( TroopsCompatibilityVersion* troops = currentSuperior->Retrieve< TroopsCompatibilityVersion >() )
            {
                for( int i = 0; i < kernel::eTroopHealthStateNbrStates; ++i )
                    differences[ i ] = -humans_[ i ];
                troops->Update( differences );
            }
    }
}

// -----------------------------------------------------------------------------
// Name: TroopsCompatibilityVersion::GetTotalHumans
// Created: SLG 2010-05-28
// -----------------------------------------------------------------------------
int TroopsCompatibilityVersion::GetTotalHumans() const
{
    int nbrHumans  = 0;
    for( unsigned int i = 0; i < unsigned int( kernel::eTroopHealthStateNbrStates ) ; ++i )
        nbrHumans += ( humans_[ i ].officers_ + humans_[ i ].subOfficers_ + humans_[ i ].troopers_ );
    return nbrHumans;
}
