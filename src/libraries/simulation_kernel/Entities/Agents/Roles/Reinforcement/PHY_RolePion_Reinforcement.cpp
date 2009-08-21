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
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Reinforcement, "PHY_RolePion_Reinforcement" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Reinforcement* role, const unsigned int /*version*/ )
{
    archive << role->pPion_;
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
    : pPion_                         ( &pion )
    , bHasChanged_                   ( true )
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
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CanReinforce
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::CanReinforce() const
{
    assert( pPion_ );
    return !pPion_->IsDead() && !pPion_->GetRole< PHY_RolePion_Transported >().IsTransported();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CanBeReinforced
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::CanBeReinforced() const
{
    assert( pPion_ );
    return !pPion_->IsDead() && !pPion_->GetRole< PHY_RolePion_Transported >().IsTransported();
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
    assert( pPion_ );
    if ( pionToReinforce == *pPion_ )
        return false;

    // Detection des boucles
    if( IsReinforcedBy( pionToReinforce ) )
        return false;

    if( !CanReinforce() || !pionToReinforce.GetRole< PHY_RolePion_Reinforcement >().CanBeReinforced() )
        return false;

    // Annulation du renforcement en cours si il existe
    if( pPionReinforced_ )
        CancelReinforcement();

    pionToReinforce.GetRole< PHY_RolePion_Reinforcement >().NotifyReinforcementAdded( *pPion_ );
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
        assert( pPion_ );
        
        pPionReinforced_->GetRole< PHY_RolePion_Reinforcement >().NotifyReinforcementRemoved( *pPion_ );
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
    assert( pPion_ );
    if( !pPionReinforced_ )
        return;

    if( bIsDead )
        CancelReinforcement();
    else
        pPion_->GetRole< PHY_RolePion_Location >().Follow( *pPionReinforced_ );
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
