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
#include "Network/NET_ASN_Messages.h"

#include "simulation_kernel/SpeedComputer_ABC.h"
#include "simulation_kernel/MoveComputer_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/ConsumptionModeChangeRequest_ABC.h"
#include "simulation_kernel/ObjectCollisionNotificationHandler_ABC.h"
#include "simulation_kernel/LocationActionNotificationHandler_ABC.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Reinforcement, "PHY_RolePion_Reinforcement" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Reinforcement* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Reinforcement* role, const unsigned int /*version*/ )
{
	MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_Reinforcement( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Reinforcement::PHY_RolePion_Reinforcement( MIL_AgentPion& pion )
    : pion_                          ( pion )
    , bHasChanged_                   ( true )
    , bExternalCanReinforce_         ( true )
    , reinforcements_                ()
    , pPionReinforced_               ( 0 )
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
void PHY_RolePion_Reinforcement::serialize( Archive& file, const uint )
{
	file & boost::serialization::base_object< PHY_RoleInterface_Reinforcement >( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CanReinforce
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::CanReinforce() const
{
    return !pion_.IsDead() && bExternalCanReinforce_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CanBeReinforced
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::CanBeReinforced() const
{
    return !pion_.IsDead() && bExternalCanReinforce_;
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
        if ( pion == reinforcement || reinforcement.GetRole< PHY_RolePion_Reinforcement >().IsReinforcedBy( pion ) )
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
    if ( pionToReinforce == pion_ )
        return false;

    // Detection des boucles
    if( IsReinforcedBy( pionToReinforce ) )
        return false;

    if( !CanReinforce() || !pionToReinforce.GetRole< PHY_RolePion_Reinforcement >().CanBeReinforced() )
        return false;

    // Annulation du renforcement en cours si il existe
    if( pPionReinforced_ )
        CancelReinforcement();

    pionToReinforce.GetRole< PHY_RolePion_Reinforcement >().NotifyReinforcementAdded( pion_ );
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
        pPionReinforced_->GetRole< PHY_RolePion_Reinforcement >().NotifyReinforcementRemoved( pion_ );
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
    if( !pPionReinforced_ )
        return;

    if( bIsDead )
        CancelReinforcement();
    else
        pion_.Apply( &location::LocationActionNotificationHandler_ABC::Follow, *pPionReinforced_ );

    if( HasChanged() )
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
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
void PHY_RolePion_Reinforcement::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg().m.pion_renforcePresent = 1;
    msg().pion_renforce          = pPionReinforced_ ? pPionReinforced_->GetID() : 0;

    msg().m.pions_renforcantPresent = 1;
    msg().pions_renforcant.n        = reinforcements_.size();
    if( !reinforcements_.empty() )
    {   
        ASN1T_OID* pAgents = new ASN1T_OID[ reinforcements_.size() ];
        uint i = 0;
        for( CIT_PionSet it = reinforcements_.begin(); it != reinforcements_.end(); ++it, ++i )
            pAgents[ i ] = (**it).GetID();

        msg().pions_renforcant.elem = pAgents;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::SendChangedState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
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
    bool bOut = reinforcements_.insert( &reinforcement ).second;
    assert( bOut );
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::NotifyReinforcementRemoved
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::NotifyReinforcementRemoved( MIL_AgentPion& reinforcement )
{
    int nOut = reinforcements_.erase( &reinforcement );
    assert( nOut == 1 );
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
void PHY_RolePion_Reinforcement::LoadForTransport   ( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable )
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
