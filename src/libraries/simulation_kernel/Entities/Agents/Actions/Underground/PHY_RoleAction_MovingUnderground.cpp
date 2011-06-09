// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_MovingUnderground.h"
#include "DetectionComputer_ABC.h"
#include "MIL_Singletons.h"
#include "MIL_Time_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RoleAction_MovingUnderground )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RoleAction_MovingUnderground* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RoleAction_MovingUnderground* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RoleAction_MovingUnderground( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground constructor
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
PHY_RoleAction_MovingUnderground::PHY_RoleAction_MovingUnderground( MIL_Agent_ABC& pion )
    : pion_        ( pion )
    , transferTime_( 0 )
    , bHasChanged_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground destructor
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
PHY_RoleAction_MovingUnderground::~PHY_RoleAction_MovingUnderground()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::serialize
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_MovingUnderground::serialize( Archive& ar, const unsigned int )
{
    ar & boost::serialization::base_object< tools::Role_ABC >( *this )
       & transferTime_
       & firstPosition_
       & secondPosition_
       & bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::Update
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::Clean
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::Execute
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::Execute( detection::DetectionComputer_ABC& algorithm ) const
{
    if( algorithm.GetTarget() == pion_ )
        algorithm.SetUnderground( IsUnderground() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::InitializeUndergroundMoving
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::InitializeUndergroundMoving( const MIL_Object_ABC& firstObject, const MIL_Object_ABC& secondObject )
{
    bool isUnderground = IsUnderground();
    transferTime_ = EstimatedUndergroundTime( firstObject, secondObject );
    firstPosition_ = firstObject.GetLocalisation().ComputeBarycenter();
    secondPosition_ = secondObject.GetLocalisation().ComputeBarycenter();
    bHasChanged_ = ( isUnderground != IsUnderground() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::Run
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
bool PHY_RoleAction_MovingUnderground::Run()
{
    if( transferTime_ == 0 )
        return false;
    transferTime_ -= MIL_Singletons::GetTime().GetTickDuration();
    if( transferTime_ <= 0 )
    {
        bHasChanged_ = true;
        if( !secondPosition_.IsZero() )
        {
            pion_.GetRole< PHY_RolePion_Location >().MagicMove( secondPosition_ );
            pion_.GetRole< PHY_RolePion_UrbanLocation >().MagicMove( secondPosition_ );
        }
        Reset();
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::IsUnderground
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
bool PHY_RoleAction_MovingUnderground::IsUnderground() const
{
    return transferTime_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::EstimatedUndergroundTime
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
double PHY_RoleAction_MovingUnderground::EstimatedUndergroundTime( const MIL_Object_ABC& firstObject, const MIL_Object_ABC& secondObject ) const
{
    double speed = pion_.GetRole< moving::PHY_RoleAction_Moving >().GetSpeedWithReinforcement( TerrainData(), firstObject );
    if( speed == 0 )
        return -1.f;
    double dist = firstObject.GetLocalisation().ComputeBarycenter().Distance( secondObject.GetLocalisation().ComputeBarycenter() );
    return dist / speed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::HideInUndergroundNetwork
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::HideInUndergroundNetwork( const MIL_Object_ABC& exit )
{
    if( !IsUnderground() )
        bHasChanged_ = true;
    transferTime_ = std::numeric_limits< double >::max();
    firstPosition_ = exit.GetLocalisation().ComputeBarycenter();
    secondPosition_.Reset();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::GetOutFromUndergroundNetwork
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::GetOutFromUndergroundNetwork()
{
    if( IsUnderground() )
    {
        if( !firstPosition_.IsZero() )
        {
            pion_.GetRole< PHY_RolePion_Location >().MagicMove( firstPosition_ );
            pion_.GetRole< PHY_RolePion_UrbanLocation >().MagicMove( firstPosition_ );
        }
        bHasChanged_ = true;
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::SendChangedState
// Created: JSR 2011-06-09
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::SendFullState
// Created: JSR 2011-06-09
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::SendFullState( client::UnitAttributes& msg ) const
{
    msg().set_underground( IsUnderground() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::Reset
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::Reset()
{
    transferTime_ = 0;
    firstPosition_.Reset();
    secondPosition_.Reset();
}

