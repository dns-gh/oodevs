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

#include "MIL_pch.h"

#include "PHY_RolePion_Reinforcement.h"

#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Reinforcement, "PHY_RolePion_Reinforcement" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Reinforcement::PHY_RolePion_Reinforcement( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleInterface_Reinforcement( role )
    , pPion_                         ( &pion )
    , bHasChanged_                   ( true )
    , reinforcements_                ()
    , pPionReinforced_               ( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RolePion_Reinforcement::PHY_RolePion_Reinforcement()
    : PHY_RoleInterface_Reinforcement()
    , pPion_                         ( 0 )
    , pPionReinforced_               ( 0 )
    , reinforcements_                ()
    , bHasChanged_                   ( true )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Reinforcement::~PHY_RolePion_Reinforcement()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Reinforcement::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Reinforcement >( *this )
         & pPion_;
}


// =============================================================================
// TOOLS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CanReinforce
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::CanReinforce() const
{
    assert( pPion_ );
    return !pPion_->IsDead() && !GetRole< PHY_RolePion_Transported >().IsTransported();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::CanBeReinforced
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Reinforcement::CanBeReinforced() const
{
    assert( pPion_ );
    return !pPion_->IsDead() && !GetRole< PHY_RolePion_Transported >().IsTransported();
}

// =============================================================================
// OPERATIONS
// =============================================================================

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

// =============================================================================
// 
// =============================================================================

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
        GetRole< PHY_RolePion_Location >().Follow( *pPionReinforced_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::Clean
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::Clean()
{
    bHasChanged_ = false;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Reinforcement::SendFullState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Reinforcement::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg.GetAsnMsg().m.pion_renforcePresent = 1;
    msg.GetAsnMsg().pion_renforce          = pPionReinforced_ ? pPionReinforced_->GetID() : 0;

    msg.GetAsnMsg().m.pions_renforcantPresent = 1;
    msg.GetAsnMsg().pions_renforcant.n        = reinforcements_.size();
    if( !reinforcements_.empty() )
    {   
        ASN1T_OID* pAgents = new ASN1T_OID[ reinforcements_.size() ];
        uint i = 0;
        for( CIT_PionSet it = reinforcements_.begin(); it != reinforcements_.end(); ++it, ++i )
            pAgents[ i ] = (**it).GetID();

        msg.GetAsnMsg().pions_renforcant.elem = pAgents;
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

