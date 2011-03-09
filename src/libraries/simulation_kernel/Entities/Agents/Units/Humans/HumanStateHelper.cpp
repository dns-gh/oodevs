// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HumanStateHelper.h"
#include "Entities/Agents/Units/Humans/Human_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"

// =============================================================================
// =============================================================================

// -----------------------------------------------------------------------------
// Name: HumanState::HumanState
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
HumanState::HumanState( const PHY_HumanRank& rank, const PHY_HumanWound& state, bool contaminated, bool psyop, unsigned int number )
    : rank_        ( rank )
    , state_       ( state )
    , contaminated_( contaminated )
    , psyop_       ( psyop )
    , number_      ( number )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HumanState::~HumanState
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
HumanState::~HumanState()
{
    // NOTHING
}

// =============================================================================
// =============================================================================

// -----------------------------------------------------------------------------
// Name: HumanStateHelper constructor
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
HumanStateHelper::HumanStateHelper()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HumanStateHelper destructor
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
HumanStateHelper::~HumanStateHelper()
{
    for( CIT_HumansStateVector it = humans_.begin(); it != humans_.end(); ++it )
        delete *it;
    humans_.clear();
}

// -----------------------------------------------------------------------------
// Name: HumanStateHelper::GetHumans
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
const HumanStateHelper::T_HumansStateVector& HumanStateHelper::GetHumans() const
{
    return humans_;
}

// -----------------------------------------------------------------------------
// Name: HumanStateHelper::AddHumanMergeIfNeeded
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
void HumanStateHelper::AddHumanMergeIfNeeded( const Human_ABC& human )
{
    bool merged = false;
    for( IT_HumansStateVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        HumanState& state = **it;
        if( state.rank_.GetID() == human.GetRank().GetID() && state.state_.GetID() == human.GetWound().GetID() && state.contaminated_ == human.IsContaminated() && state.psyop_ == human.IsMentalDiseased() )
        {
            ++state.number_;
            merged = true;
            break;
        }
    }
    if( !merged )
        humans_.push_back( new HumanState( human.GetRank(), human.GetWound(), human.IsContaminated(), human.IsMentalDiseased(), 1 ) );
}
