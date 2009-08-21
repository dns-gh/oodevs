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

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Population.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_Knowledge_PopulationCollision.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Population, "PHY_RolePion_Population" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Population* role, const unsigned int /*version*/ )
{
    archive << role->pPion_;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Population* role, const unsigned int /*version*/ )
{
	MIL_Agent_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_Population( *pion );
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Population::PHY_RolePion_Population( MIL_Agent_ABC& pion )
    : pPion_                                ( &pion )
    , bHasChanged_                          ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Population::~PHY_RolePion_Population()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Population::serialize( Archive& file, const uint )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::ModifyMaxSpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Population::ModifyMaxSpeed( MT_Float rSpeed ) const
{
    assert( pPion_ );

    T_KnowledgePopulationCollisionVector collisions;
    pPion_->GetKnowledge().GetPopulationsColliding( collisions );

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

    T_KnowledgePopulationCollisionVector collisions;
    pPion_->GetKnowledge().GetPopulationsColliding( collisions );

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
    assert( pPion_ );

    T_KnowledgePopulationCollisionVector populationsColliding;
    pPion_->GetKnowledge().GetPopulationsColliding( populationsColliding );
    MT_Float rPopulationDensity = 0.;
    for( CIT_KnowledgePopulationCollisionVector it = populationsColliding.begin(); it != populationsColliding.end(); ++it )
        rPopulationDensity = std::max( rPopulationDensity, (**it).GetMaxPopulationDensity() );

    return rPopulationDensity;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Population::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Population::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Population::HasChanged() const
{
    return bHasChanged_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::IsInvulnerable
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
bool PHY_RolePion_Population::IsInvulnerable() const
{
    return false;
}
