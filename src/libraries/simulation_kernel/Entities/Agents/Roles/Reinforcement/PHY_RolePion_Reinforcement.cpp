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
#include "Entities/Agents/MIL_AgentPion.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/SpeedComputer_ABC.h"
#include "simulation_kernel/MoveComputer_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/ConsumptionModeChangeRequest_ABC.h"
#include "simulation_kernel/ObjectCollisionNotificationHandler_ABC.h"
#include "simulation_kernel/LocationActionNotificationHandler_ABC.h"
#include "MT_Tools/MT_ScipioException.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Reinforcement )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Reinforcement::PHY_RolePion_Reinforcement( MIL_AgentPion& pion )
    : owner_                ( pion )
    , bHasChanged_          ( true )
    , bExternalCanReinforce_( true )
    , reinforcements_       ()
    , pPionReinforced_      ( 0 )
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
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CanReinforce
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::CanReinforce() const
{
    return !owner_.IsDead() && bExternalCanReinforce_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CanBeReinforced
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::CanBeReinforced() const
{
    return !owner_.IsDead() && bExternalCanReinforce_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::IsReinforcedBy
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::IsReinforcedBy( MIL_AgentPion& pion ) const
{
    for ( CIT_PionSet itPion = reinforcements_.begin(); itPion != reinforcements_.end(); ++itPion )
    {
        const MIL_AgentPion& reinforcement = **itPion;
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
    if( pionToReinforce == owner_ )
        return false;

    // Detection des boucles
    if( IsReinforcedBy( pionToReinforce ) )
        return false;

    if( !CanReinforce() || !pionToReinforce.GetRole< PHY_RolePion_Reinforcement >().CanBeReinforced() )
        return false;

    // Annulation du renforcement en cours si il existe
    if( pPionReinforced_ )
        CancelReinforcement();

    pionToReinforce.GetRole< PHY_RolePion_Reinforcement >().NotifyReinforcementAdded( owner_ );
    pPionReinforced_ = &pionToReinforce;
    bHasChanged_ = true;
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
        pPionReinforced_->GetRole< PHY_RolePion_Reinforcement >().NotifyReinforcementRemoved( owner_ );
        pPionReinforced_ = 0;
        bHasChanged_ = true;
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
        if( bIsDead )
            CancelReinforcement();
        else
            owner_.Apply( &location::LocationActionNotificationHandler_ABC::Follow, *pPionReinforced_ );
    }

    if( HasChanged() )
        owner_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Clean
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::SendFullState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::SendFullState( client::UnitAttributes& msg ) const
{
    msg().mutable_reinforced_unit()->set_id( pPionReinforced_ ? pPionReinforced_->GetID() : 0 );
    if( !reinforcements_.empty() )
    {
        unsigned int i = 0;
        for( CIT_PionSet it = reinforcements_.begin(); it != reinforcements_.end(); ++it, ++i )
            msg().mutable_reinforcements()->add_elem()->set_id( (**it).GetID() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::SendChangedState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyReinforcementAdded
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyReinforcementAdded( MIL_AgentPion& reinforcement )
{
    if( ! reinforcements_.insert( &reinforcement ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyReinforcementRemoved
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyReinforcementRemoved( MIL_AgentPion& reinforcement )
{
    if( reinforcements_.erase( &reinforcement ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::IsReinforcing
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::IsReinforcing() const
{
    return pPionReinforced_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::IsReinforced
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::IsReinforced() const
{
    return !reinforcements_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::HasChanged
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::GetReinforcements
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
const PHY_RolePion_Reinforcement::T_PionSet& PHY_RolePion_Reinforcement::GetReinforcements() const
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
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = GetReinforcements();
    while( !reinforcements.empty() )
       (**reinforcements.begin()).GetRole< PHY_RoleInterface_Reinforcement >().CancelReinforcement();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Execute
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    for( CIT_PionSet itPion = reinforcements_.begin(); itPion != reinforcements_.end(); ++itPion )
    {
        MIL_AgentPion& reinforcement = **itPion;
        algorithm.ApplyOnReinforcement( reinforcement );
    }
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Execute
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::Execute(moving::MoveComputer_ABC& algorithm) const
{
    if( IsReinforcing() )
        algorithm.NotifyReinforcing();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Execute
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::ChangeConsumptionMode(dotation::ConsumptionModeChangeRequest_ABC& request)
{
    for ( CIT_PionSet itPion = reinforcements_.begin(); itPion != reinforcements_.end(); ++itPion )
    {
        MIL_AgentPion& reinforcement = **itPion;
        reinforcement.Apply(&ConsumptionChangeRequestHandler_ABC::ChangeConsumptionMode, request);
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyMovingOutsideObject
// Created: AHC 2009-10-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    const T_PionSet& reinforcements = GetReinforcements();
    for( CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        (**it).Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingInsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyMovingOutsideObject
// Created: AHC 2009-10-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    const T_PionSet& reinforcements = GetReinforcements();
    for( CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        (**it).Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingOutsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyPutInsideObject
// Created: AHC 2009-10-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyPutInsideObject( MIL_Object_ABC& object )
{
    const T_PionSet& reinforcements = GetReinforcements();
        for( CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
            (**it).Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyPutInsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyPutOutsideObject
// Created: AHC 2009-10-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyPutOutsideObject( MIL_Object_ABC& object )
{
    const T_PionSet& reinforcements = GetReinforcements();
        for( CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
            (**it).Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyPutOutsideObject, object );
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
