// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RolePion_Population.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 18:15 $
// $Revision: 9 $
// $Workfile: PHY_RolePion_Population.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RolePion_Population.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_KS_AgentQuerier.h"
#include "Knowledge/DEC_Knowledge_PopulationCollision.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Population, "PHY_RolePion_Population" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Population::PHY_RolePion_Population( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleInterface_Population          ( role )
    , pPion_                                ( &pion )
    , bHasChanged_                          ( true )
    , bSlowDownEnabled_                     ( true )
    , bReloadingDurationModificationEnabled_( true )
    , bInvulnerabilityEnabled_              ( false )
{
}
 
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePion_Population::PHY_RolePion_Population()
    : PHY_RoleInterface_Population          ()
    , pPion_                                ( 0 )
    , bHasChanged_                          ( true )
    , bSlowDownEnabled_                     ( true )
    , bReloadingDurationModificationEnabled_( true )
    , bInvulnerabilityEnabled_              ( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Population::~PHY_RolePion_Population()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Population::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Population >( *this )
         & pPion_
         & bSlowDownEnabled_
         & bReloadingDurationModificationEnabled_
         & bInvulnerabilityEnabled_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::ModifyMaxSpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Population::ModifyMaxSpeed( MT_Float rSpeed ) const
{
    assert( pPion_ );
    if( !bSlowDownEnabled_ )
        return rSpeed;

    T_KnowledgePopulationCollisionVector collisions;
    pPion_->GetKSQuerier().GetPopulationsColliding( collisions );

    MT_Float rMaxSpeed = rSpeed;
    for( CIT_KnowledgePopulationCollisionVector it = collisions.begin(); it != collisions.end(); ++it )
    {
        const DEC_Knowledge_PopulationCollision& population = **it;
        rMaxSpeed = std::min( rMaxSpeed, population.GetPionMaxSpeed() );
    }

    return rMaxSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::ModifyReloadingDuration
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Population::ModifyReloadingDuration( MT_Float rDuration ) const
{
    assert( pPion_ );
    if( !bReloadingDurationModificationEnabled_ )
        return rDuration;

    T_KnowledgePopulationCollisionVector collisions;
    pPion_->GetKSQuerier().GetPopulationsColliding( collisions );

    MT_Float rFactor = 1.;
    for( CIT_KnowledgePopulationCollisionVector it = collisions.begin(); it != collisions.end(); ++it )
    {
        const DEC_Knowledge_PopulationCollision& population = **it;
        rFactor = std::max( rFactor, population.GetPionReloadingTimeFactor() );
    }

    return rDuration * rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::GetCollidingPopulationDensity
// Created: NLD 2006-01-19
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Population::GetCollidingPopulationDensity() const
{
    T_KnowledgePopulationCollisionVector populationsColliding;
    pPion_->GetKSQuerier().GetPopulationsColliding( populationsColliding );
    MT_Float rPopulationDensity = 0.;
    for( CIT_KnowledgePopulationCollisionVector it = populationsColliding.begin(); it != populationsColliding.end(); ++it )
        rPopulationDensity = std::max( rPopulationDensity, (**it).GetMaxPopulationDensity() );

    return rPopulationDensity;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
//void PHY_RolePion_Population::SendFullState( NET_ASN_MsgUnitAttributes& /*msg*/ ) const
//{
//    // NOTHING
//}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
//void PHY_RolePion_Population::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
//{
//    if( bHasChanged_ )
//        SendFullState( msg );
//}
