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
#include "NetworkNotificationHandler_ABC.h"
#include "Checkpoints/SerializationTools.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "Entities/Objects/UndergroundAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
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
    : pion_           ( pion )
    , transferTime_   ( 0 )
    , speed_          ( 0 )
    , preparingToHide_( false )
    , bHasChanged_    ( false )
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
       & currentNetwork_
       & pCurrentLocation_
       & pDestination_
       & transferTime_
       & speed_
       & bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::Update
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::Update( bool /*bIsDead*/ )
{
    if( bHasChanged_ )
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
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
bool PHY_RoleAction_MovingUnderground::InitializeUndergroundMoving( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( speed_ == 0 || !IsUnderground() )
        return false;
    if( !pKnowledge || !pKnowledge->IsValid() )
        return false;
    const UndergroundAttribute* attr = pKnowledge->RetrieveAttribute< UndergroundAttribute >();
    if( !attr || attr->Network() != currentNetwork_ )
        return false;
    transferTime_ = EstimatedUndergroundTime( pKnowledge );
    pDestination_ = pKnowledge;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::Run
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
bool PHY_RoleAction_MovingUnderground::Run()
{
    if( transferTime_ == 0 )
        return false;
    if( !pDestination_ || !pDestination_->IsValid() )
    {
        transferTime_ = 0;
        pDestination_.reset();
        return false;
    }
    transferTime_ -= MIL_Singletons::GetTime().GetTickDuration();
    if( transferTime_ <= 0 )
    {
        bHasChanged_ = true;
        MT_Vector2D destination = pDestination_->GetLocalisation().ComputeBarycenter();
        pion_.GetRole< PHY_RolePion_Location >().MagicMove( destination );
        pion_.GetRole< PHY_RolePion_UrbanLocation >().MagicMove( destination );
        transferTime_ = 0;
        pCurrentLocation_ = pDestination_;
        pDestination_.reset();
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::CanExitFromCurrentLocation
// Created: JSR 2011-07-20
// -----------------------------------------------------------------------------
bool PHY_RoleAction_MovingUnderground::CanExitFromCurrentLocation() const
{
    if( pCurrentLocation_ && pCurrentLocation_->IsValid() )
        if( MIL_Object_ABC* object = pCurrentLocation_->GetObjectKnown() )
            if( const UndergroundAttribute* pAttribute = object->RetrieveAttribute< UndergroundAttribute >() )
                return pAttribute->IsActivated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::PreparingToHide
// Created: JSR 2011-10-20
// -----------------------------------------------------------------------------
bool PHY_RoleAction_MovingUnderground::PreparingToHide() const
{
    return preparingToHide_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::IsUnderground
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
bool PHY_RoleAction_MovingUnderground::IsUnderground() const
{
    return !currentNetwork_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::EstimatedUndergroundTime
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
double PHY_RoleAction_MovingUnderground::EstimatedUndergroundTime( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge ) const
{
    if( speed_ == 0 )
        return -1.f;
    return pion_.GetRole< PHY_RolePion_Location >().GetPosition().Distance( pKnowledge->GetLocalisation().ComputeBarycenter() ) / speed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::HideInUndergroundNetwork
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
bool PHY_RoleAction_MovingUnderground::HideInUndergroundNetwork( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( IsUnderground() || !pKnowledge || !pKnowledge->IsValid() )
        return false;
    const MIL_Object_ABC* object = pKnowledge->GetObjectKnown();
    if( !object )
        return false;
    const UndergroundAttribute* attr = object->RetrieveAttribute< UndergroundAttribute >();
    if( !attr || !attr->IsActivated() )
        return false;
    bHasChanged_ = true;
    preparingToHide_ = true;
    unsigned int duration = MIL_Singletons::GetTime().GetTickDuration();
    speed_ = duration == 0 ? 0 : pion_.GetRole< moving::PHY_RoleAction_Moving >().GetSpeedWithReinforcement( TerrainData(), *object ) / duration;
    preparingToHide_ = false;
    pCurrentLocation_ = pKnowledge;
    currentNetwork_ = attr->Network();
    pDestination_.reset();
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_MovingUnderground::GetOutFromUndergroundNetwork
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_RoleAction_MovingUnderground::GetOutFromUndergroundNetwork()
{
    if( IsUnderground() )
    {
        currentNetwork_.clear();
        bHasChanged_ = true;
        transferTime_ = 0;
        speed_ = 0;
        pCurrentLocation_.reset();
        pDestination_.reset();
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
