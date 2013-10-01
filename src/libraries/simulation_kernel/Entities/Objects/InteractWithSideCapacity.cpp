// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InteractWithSideCapacity.h"
#include "MIL_Object_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( InteractWithSideCapacity )

// -----------------------------------------------------------------------------
// Name: InteractWithSideCapacity constructor
// Created: CMA 2012-05-10
// -----------------------------------------------------------------------------
InteractWithSideCapacity::InteractWithSideCapacity( xml::xistream& xis )
    : friendSide_ ( xis.attribute< bool >( "friend", false ) )
    , enemySide_  ( xis.attribute< bool >( "enemy", false ) )
    , neutralSide_( xis.attribute< bool >( "neutral", false ) )
    , civilian_   ( xis.attribute< bool >( "civilian", false ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractWithSideCapacity constructor
// Created: CMA 2012-05-10
// -----------------------------------------------------------------------------
InteractWithSideCapacity::InteractWithSideCapacity()
    : friendSide_ ( false )
    , enemySide_  ( false )
    , neutralSide_( false )
    , civilian_   ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractWithSideCapacity destructor
// Created: CMA 2012-05-10
// -----------------------------------------------------------------------------
InteractWithSideCapacity::~InteractWithSideCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractWithSideCapacity::IsPossible
// Created: CMA 2012-05-10
// -----------------------------------------------------------------------------
bool InteractWithSideCapacity::IsPossible( const MIL_Army_ABC& army1, const MIL_Agent_ABC& agent ) const
{
    return ( IsPossible( army1, agent.GetArmy() ) || ( agent.IsCivilian() && civilian_ ) );
}

// -----------------------------------------------------------------------------
// Name: InteractWithSideCapacity::IsPossible
// Created: LGY 2013-01-04
// -----------------------------------------------------------------------------
bool InteractWithSideCapacity::IsPossible( const MIL_Army_ABC& army1, const MIL_Army_ABC& army2 ) const
{
    return ( army1.IsAFriend( army2 ) == eTristate_True && friendSide_  ) ||
           ( army1.IsAnEnemy( army2 ) == eTristate_True && enemySide_   ) ||
           ( army1.IsNeutral( army2 ) == eTristate_True && neutralSide_ );
}

// -----------------------------------------------------------------------------
// Name: InteractWithSideCapacity::serialize
// Created: CMA 2012-05-10
// -----------------------------------------------------------------------------
template< typename Archive > void InteractWithSideCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this ) & friendSide_ & enemySide_ & neutralSide_ & civilian_;
}

// -----------------------------------------------------------------------------
// Name: InteractWithSideCapacity::Register
// Created: CMA 2012-05-10
// -----------------------------------------------------------------------------
void InteractWithSideCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: InteractWithSideCapacity::Instanciate
// Created: CMA 2012-05-10
// -----------------------------------------------------------------------------
void InteractWithSideCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    InteractWithSideCapacity* capacity = new InteractWithSideCapacity( *this );
    object.AddCapacity( capacity );
}
