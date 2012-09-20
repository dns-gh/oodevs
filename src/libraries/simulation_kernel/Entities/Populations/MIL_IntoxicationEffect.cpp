// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_IntoxicationEffect.h"
#include "MIL_PopulationHumans.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_IntoxicationEffect constructor
// Created: LGY 2011-03-31
// -----------------------------------------------------------------------------
MIL_IntoxicationEffect::MIL_IntoxicationEffect( MIL_PopulationHumans& humans, double delay, unsigned int time )
    : delay_ ( delay )
    , humans_( &humans )
    , time_  ( time )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_IntoxicationEffect constructor
// Created: JSR 2012-03-07
// -----------------------------------------------------------------------------
MIL_IntoxicationEffect::MIL_IntoxicationEffect()
    : delay_( 0 )
    , humans_( 0 )
    , time_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_IntoxicationEffect destructor
// Created: LGY 2011-03-31
// -----------------------------------------------------------------------------
MIL_IntoxicationEffect::~MIL_IntoxicationEffect()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_IntoxicationEffect::Update
// Created: LGY 2011-03-31
// -----------------------------------------------------------------------------
void MIL_IntoxicationEffect::Update( unsigned int time )
{
    assert( humans_ );
    if( time - time_ >= delay_ )
    {
        BOOST_FOREACH( const T_Effect& effect, effects_ )
            humans_->ApplyIntoxication( effect.first, effect.second );
        time_ = time;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_IntoxicationEffect::Add
// Created: LGY 2011-04-01
// -----------------------------------------------------------------------------
void MIL_IntoxicationEffect::Add( double woundedPercentage, double deadPercentage )
{
    assert( humans_ );
    effects_.push_back( std::make_pair( woundedPercentage, deadPercentage ) );
    humans_->ApplyIntoxication( woundedPercentage, deadPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_IntoxicationEffect::load
// Created: LDC 2012-03-07
// -----------------------------------------------------------------------------
void MIL_IntoxicationEffect::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> delay_;
    ar >> humans_;
    ar >> time_;
    ar >> effects_;
}

// -----------------------------------------------------------------------------
// Name: MIL_IntoxicationEffect::save
// Created: LDC 2012-03-07
// -----------------------------------------------------------------------------
void MIL_IntoxicationEffect::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << delay_;
    ar << humans_;
    ar << time_;
    ar << effects_;
}
