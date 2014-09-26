// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_Reinforcement.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Reinforcement.h"
#include "SpeedComputer_ABC.h"
#include "MoveComputer_ABC.h"
#include "PostureComputer_ABC.h"
#include "NetworkNotificationHandler_ABC.h"
#include "ConsumptionModeChangeRequest_ABC.h"
#include "ObjectCollisionNotificationHandler_ABC.h"
#include "LocationActionNotificationHandler_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "protocol/ClientSenders.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/range/algorithm_ext/erase.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Reinforcement )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RolePion_Reinforcement::PHY_RolePion_Reinforcement()
    : owner_                 ( 0 )
    , bReinforcedChanged_    ( false )
    , bReinforcementsChanged_( false )
    , bExternalCanReinforce_ ( true )
    , pPionReinforced_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Reinforcement::PHY_RolePion_Reinforcement( MIL_AgentPion& pion )
    : owner_                 ( &pion )
    , bReinforcedChanged_    ( false )
    , bReinforcementsChanged_( false )
    , bExternalCanReinforce_ ( true )
    , pPionReinforced_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Reinforcement::~PHY_RolePion_Reinforcement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Reinforcement::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Reinforcement >( *this );
    file & owner_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CanReinforce
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::CanReinforce() const
{
    return !owner_->IsDead() && bExternalCanReinforce_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CanBeReinforced
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::CanBeReinforced() const
{
    return !owner_->IsDead() && bExternalCanReinforce_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::IsReinforcedBy
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::IsReinforcedBy( MIL_AgentPion& pion ) const
{
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
    {
        const MIL_AgentPion& reinforcement = **it;
        if( pion == reinforcement || reinforcement.GetRole< PHY_RolePion_Reinforcement >().IsReinforcedBy( pion ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Reinforce
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::Reinforce( MIL_AgentPion& pionToReinforce )
{
    if( pionToReinforce == *owner_ )
        return false;

    // Detection des boucles
    if( IsReinforcedBy( pionToReinforce ) )
        return false;

    if( !CanReinforce() || !pionToReinforce.GetRole< PHY_RolePion_Reinforcement >().CanBeReinforced() )
        return false;

    // Annulation du renforcement en cours si il existe
    if( pPionReinforced_ )
        CancelReinforcement();

    pionToReinforce.GetRole< PHY_RolePion_Reinforcement >().NotifyReinforcementAdded( *owner_ );
    pPionReinforced_ = &pionToReinforce;
    bReinforcedChanged_ = true;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CancelReinforcement
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::CancelReinforcement()
{
    if( pPionReinforced_ )
    {
        pPionReinforced_->GetRole< PHY_RolePion_Reinforcement >().NotifyReinforcementRemoved( *owner_ );
        pPionReinforced_ = 0;
        bReinforcedChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::Update( bool bIsDead )
{
    if( pPionReinforced_ )
    {
        if( bIsDead || pPionReinforced_->IsMarkedForDestruction() )
            CancelReinforcement();
        else
            owner_->Apply( &location::LocationActionNotificationHandler_ABC::Follow, *pPionReinforced_ );
    }
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); )
    {
        if( ( *it )->IsMarkedForDestruction() )
            it = reinforcements_.erase( it );
        else
            ++it;
    }
    if( HasChanged() )
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Clean
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::Clean()
{
    bReinforcedChanged_ = false;
    bReinforcementsChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::SendFullState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::SendFullState( client::UnitAttributes& msg ) const
{
    msg().mutable_reinforced_unit()->set_id( pPionReinforced_ ? pPionReinforced_->GetID() : 0 );
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
        msg().mutable_reinforcements()->add_elem()->set_id( (*it)->GetID() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::SendChangedState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bReinforcedChanged_ )
        msg().mutable_reinforced_unit()->set_id( pPionReinforced_ ? pPionReinforced_->GetID() : 0 );
    if( bReinforcementsChanged_ )
    {
        msg().mutable_reinforcements();
        for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
            msg().mutable_reinforcements()->add_elem()->set_id( (*it)->GetID() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyReinforcementAdded
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyReinforcementAdded( MIL_AgentPion& reinforcement )
{
    reinforcements_.push_back( &reinforcement );
    bReinforcementsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyReinforcementRemoved
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyReinforcementRemoved( MIL_AgentPion& reinforcement )
{
    boost::remove_erase( reinforcements_, &reinforcement );
    bReinforcementsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::HasChanged
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::HasChanged() const
{
    return bReinforcedChanged_ || bReinforcementsChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::GetReinforcements
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
const PHY_RolePion_Reinforcement::T_Pions& PHY_RolePion_Reinforcement::GetReinforcements() const
{
    return reinforcements_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::LoadForTransport
// Created: AHC 2009-09-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::LoadForTransport( const MIL_Agent_ABC& /*transporter*/, bool /*bTransportOnlyLoadable*/, bool& /*bTransportedByAnother*/ )
{
    CancelReinforcement();
    const T_Pions reinforcements = reinforcements_;
    for( auto it = reinforcements.begin(); it != reinforcements.end(); ++it )
       (*it)->GetRole< PHY_RoleInterface_Reinforcement >().CancelReinforcement();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Execute
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
        algorithm.ApplyOnReinforcement( **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Execute
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::Execute( moving::MoveComputer_ABC& algorithm ) const
{
    if( pPionReinforced_ )
        algorithm.NotifyReinforcing();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Execute
// Created: MCO 2013-06-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
        algorithm.ApplyOnReinforcement( **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Execute
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::ChangeConsumptionMode( dotation::ConsumptionModeChangeRequest_ABC& request )
{
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
        (*it)->Apply( &ConsumptionChangeRequestHandler_ABC::ChangeConsumptionMode, request );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyMovingInsideObject
// Created: AHC 2009-10-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyMovingInsideObject( MIL_Object_ABC& object, const MT_Vector2D& startPos, const MT_Vector2D& endPos )
{
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
        (*it)->Apply( &terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingInsideObject, object, startPos, endPos );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyMovingOutsideObject
// Created: AHC 2009-10-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
        (*it)->Apply( &terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingOutsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyPutInsideObject
// Created: AHC 2009-10-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyPutInsideObject( MIL_Object_ABC& object )
{
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
        (*it)->Apply( &terrain::ObjectCollisionNotificationHandler_ABC::NotifyPutInsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyPutOutsideObject
// Created: AHC 2009-10-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyPutOutsideObject( MIL_Object_ABC& object )
{
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
        (*it)->Apply( &terrain::ObjectCollisionNotificationHandler_ABC::NotifyPutOutsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyIsLoaded
// Created: MGD 2009-10-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyIsLoadedForTransport()
{
    bExternalCanReinforce_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyIsUnLoaded
// Created: MGD 2009-10-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyIsUnLoadedForTransport()
{
    bExternalCanReinforce_ = true;
}
