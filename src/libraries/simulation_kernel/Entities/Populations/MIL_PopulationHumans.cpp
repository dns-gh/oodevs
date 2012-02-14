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

namespace
{
    void SubstractUnsigned( unsigned int& result, unsigned int value )
    {
        if( result >= value )
            result -= value;
        else
            result = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::operator -=
// Created: JSR 2011-08-11
// -----------------------------------------------------------------------------
MIL_PopulationHumans& MIL_PopulationHumans::operator -=( const MIL_PopulationHumans& humans )
{
    SubstractUnsigned( healthy_, humans.healthy_ );
    SubstractUnsigned( wounded_, humans.wounded_ );
    SubstractUnsigned( contaminated_, humans.contaminated_ );
    SubstractUnsigned( dead_, humans.dead_ );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::operator *=
// Created: JSR 2011-08-11
// -----------------------------------------------------------------------------
MIL_PopulationHumans& MIL_PopulationHumans::operator *=( double ratio )
{
    healthy_ = static_cast< unsigned int >( healthy_ * ratio );
    wounded_ = static_cast< unsigned int >( wounded_ * ratio );
    contaminated_ = static_cast< unsigned int >( contaminated_ * ratio );
    dead_ = static_cast< unsigned int >( dead_ * ratio );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::Empty
// Created: JSR 2011-08-11
// -----------------------------------------------------------------------------
void MIL_PopulationHumans::Empty()
{
    healthy_ = 0;
    wounded_ = 0;
    contaminated_ = 0;
    dead_ = 0;
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
// Name: MIL_PopulationHumans::KillHumans
// Created: LGY 2012-02-13
// -----------------------------------------------------------------------------
void MIL_PopulationHumans::KillHumans( unsigned int human )
{
    unsigned int dead = human;
    if( healthy_ < dead )
        dead = healthy_;
    healthy_ -= dead;
    dead_ += dead;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::WoundHumans
// Created: LGY 2012-02-13
// -----------------------------------------------------------------------------
void MIL_PopulationHumans::WoundHumans( unsigned int human )
{
    unsigned int wounded = human;
    if( healthy_ < wounded )
        wounded = healthy_;
    healthy_ -= wounded;
    wounded_ += wounded;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::ApplyWounds
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationHumans::ApplyWounds( unsigned int wounds )
{
    if( wounds > healthy_ + contaminated_ )
        wounds = healthy_ + contaminated_;
    wounded_ += wounds;
    if( healthy_ >= wounds )
        healthy_ -= wounds;
    else
    {
        unsigned int woundsRemaining = wounds - healthy_;
        healthy_ = 0;
        contaminated_ -= woundsRemaining;
        wounded_ = wounds;
    }
    return wounds;
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

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::ReintegrateUrbanBlock
// Created: BCI 2011-03-21
// -----------------------------------------------------------------------------
void MIL_PopulationHumans::ReintegrateUrbanBlock()
{
    healthy_ = 0;
    wounded_ = 0;
    contaminated_ = 0;
    dead_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::ApplyContamination
// Created: LGY 2011-03-30
// -----------------------------------------------------------------------------
void MIL_PopulationHumans::ApplyContamination()
{
    contaminated_ += healthy_;
    healthy_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::ApplyIntoxication
// Created: LGY 2011-04-01
// -----------------------------------------------------------------------------
void MIL_PopulationHumans::ApplyIntoxication( double woundedPercentage, double deadPercentage )
{
    unsigned int healthy = static_cast< unsigned int >( healthy_ * ( 1.f - woundedPercentage - deadPercentage ) );
    unsigned int contaminated = static_cast< unsigned int >( contaminated_ * ( 1.f - woundedPercentage - deadPercentage ) );
    unsigned int wounded = static_cast< unsigned int >( wounded_ + ( healthy_ + contaminated_ ) * woundedPercentage );
    unsigned int dead = static_cast< unsigned int >( dead_ + ( healthy_ + contaminated_ ) * deadPercentage );
    unsigned int total = healthy + wounded + dead + contaminated;
    if( total != GetAllHumans() )
        wounded += GetAllHumans() - total;
    healthy_ = healthy;
    contaminated_ = contaminated;
    wounded_ = wounded;
    dead_ = dead;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::ApplyDecontamination
// Created: LGY 2011-11-09
// -----------------------------------------------------------------------------
bool MIL_PopulationHumans::ApplyDecontamination()
{
    if( contaminated_ > 0 )
    {
        --contaminated_;
        ++healthy_;
        return true;
    }
    return false;
}
