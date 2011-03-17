// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationHumans.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans constructor
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
MIL_PopulationHumans::MIL_PopulationHumans()
    : healthy_     ( 0 )
    , wounded_     ( 0 )
    , contaminated_( 0 )
    , dead_        ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans constructor
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
MIL_PopulationHumans::MIL_PopulationHumans( xml::xistream& xis )
    : healthy_     ( xis.attribute< unsigned int >( "healthy" ) )
    , wounded_     ( xis.attribute< unsigned int >( "wounded" ) )
    , contaminated_( xis.attribute< unsigned int >( "contaminated" ) )
    , dead_        ( xis.attribute< unsigned int >( "dead" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans constructor
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
MIL_PopulationHumans::MIL_PopulationHumans( unsigned int healthy, unsigned int contaminated, unsigned int wounded, unsigned int dead )
    : healthy_     ( healthy )
    , wounded_     ( wounded )
    , contaminated_( contaminated )
    , dead_        ( dead )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans constructor
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
MIL_PopulationHumans::MIL_PopulationHumans( const MIL_PopulationHumans& from )
    : healthy_     ( from.healthy_ )
    , wounded_     ( from.wounded_ )
    , contaminated_( from.contaminated_)
    , dead_        ( from.dead_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans destructor
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
MIL_PopulationHumans::~MIL_PopulationHumans()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::operator +=
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
MIL_PopulationHumans& MIL_PopulationHumans::operator +=( const MIL_PopulationHumans& humans )
{
    healthy_ += humans.healthy_;
    wounded_ += humans.wounded_;
    contaminated_ += humans.contaminated_;
    dead_ += humans.dead_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::GetHealthyHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationHumans::GetHealthyHumans() const
{
    return healthy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::GetWoundedHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationHumans::GetWoundedHumans() const
{
    return wounded_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::GetContaminatedHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationHumans::GetContaminatedHumans() const
{
    return contaminated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::GetDeadHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationHumans::GetDeadHumans() const
{
    return dead_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::IsEmpty
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
bool MIL_PopulationHumans::IsEmpty() const
{
    return healthy_ == 0 && wounded_ == 0 && contaminated_ == 0 && dead_ == 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::GetAllHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationHumans::GetAllHumans() const
{
    return healthy_ + wounded_ + contaminated_ + dead_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::GetTotalLivingHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationHumans::GetTotalLivingHumans() const
{
    return healthy_ + wounded_ + contaminated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::GetLivingRatio
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
double MIL_PopulationHumans::GetLivingRatio() const
{
    return static_cast< double >( healthy_ + wounded_ + contaminated_ ) / ( healthy_ + wounded_ + contaminated_ + dead_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::KillAll
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
void MIL_PopulationHumans::KillAll()
{
    dead_ += healthy_ + wounded_ + contaminated_;
    healthy_ = 0;
    wounded_ = 0;
    contaminated_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::ApplyNumberOfDead
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationHumans::ApplyNumberOfDead( unsigned int dead )
{
    if( dead > healthy_ + contaminated_ + wounded_ )
        dead = healthy_ + contaminated_ + wounded_;
    dead_ += dead;
    if( healthy_ >= dead )
        healthy_ -= dead;
    else
    {
        unsigned int deadRemaining = dead - healthy_;
        healthy_ = 0;
        if( contaminated_ >= deadRemaining )
            contaminated_ -= deadRemaining;
        else
        {
            deadRemaining -= contaminated_;
            contaminated_ = 0;
            wounded_ = std::max( 0u, wounded_ - deadRemaining );
        }        
    }
    return dead;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::PullFrom
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
void MIL_PopulationHumans::PullFrom( unsigned int number, MIL_PopulationHumans& humans )
{
    healthy_ = std::min( number, humans.healthy_ );
    humans.healthy_ -= healthy_;
    number -= healthy_;

    contaminated_ = std::min( number, humans.contaminated_ );
    humans.contaminated_ -= contaminated_;
    number -= contaminated_;

    wounded_ = std::min( number, humans.wounded_ );
    humans.wounded_ -= wounded_;
    number -= wounded_;

    dead_ = std::min( number, humans.dead_ );
    humans.dead_ -= dead_;
}
