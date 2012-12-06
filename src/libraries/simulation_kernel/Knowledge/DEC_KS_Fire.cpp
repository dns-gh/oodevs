// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Fire.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 16:32 $
// $Revision: 6 $
// $Workfile: DEC_KS_Fire.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KS_Fire.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "Entities/Agents/Perceptions/PHY_ZURBPerceptionComputer.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/MIL_AgentPion.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_Fire )

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Fire::DEC_KS_Fire( DEC_KnowledgeBlackBoard_AgentPion& blackBoard )
    : DEC_KnowledgeSource_ABC ( blackBoard, 1 )
    , pBlackBoard_            ( &blackBoard )
    , pionsAttacking_         ()
    , populationsAttacking_   ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Fire::DEC_KS_Fire()
    : DEC_KnowledgeSource_ABC ()
    , pBlackBoard_            ( 0 )
    , pionsAttacking_         ()
    , populationsAttacking_   ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Fire::~DEC_KS_Fire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::Prepare
// Created: HBD 2009-12-30
// -----------------------------------------------------------------------------
void DEC_KS_Fire::Prepare()
{
    // Nothing
}
// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::Clean
// Created: HBD 2009-12-30
// -----------------------------------------------------------------------------
void DEC_KS_Fire::Clean()
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Fire::Talk( int /*currentTimeStep*/ )
{
    assert( pBlackBoard_ );

    const MIL_Agent_ABC& agentInteracting = pBlackBoard_->GetPion();

    for( CIT_PionSet itAttacker = pionsAttacking_.begin(); itAttacker != pionsAttacking_.end(); ++itAttacker )
    {
        MIL_AgentPion& attacker = **itAttacker;

        // Si le pion qui attaque est furtif
        if( attacker.GetRole< PHY_RolePion_Posture>().IsStealth() )
            continue;

        // On ne gére la connaissance que lorsque le tireur est à distance inférieure de la distance max de détection
        if( agentInteracting.GetRole< PHY_RoleInterface_Perceiver >().GetMaxAgentPerceptionDistance() <= agentInteracting.GetRole< PHY_RoleInterface_Location >().GetPosition().Distance( attacker.GetRole< PHY_RoleInterface_Location >().GetPosition() ) )
            continue;

        // Si la cible est dans une zone urbaine
        if( agentInteracting.GetRole< PHY_RoleInterface_UrbanLocation >().IsInCity() )
        {
            // On vérifie que le tireur voit toujours sa cible (afin d'éviter les tirs a travers les blocs urbains)
            PHY_ZURBPerceptionComputer computer( attacker, 0, 0 );
            if( computer.ComputePerception( agentInteracting ) == PHY_PerceptionLevel::notSeen_ )
                continue;
        }

        DEC_Knowledge_AgentPerception* pKnowledge = pBlackBoard_->GetKnowledgeAgentPerceptionContainer().GetKnowledgeAgentPerception( attacker );
        if( !pKnowledge )
            pKnowledge = &pBlackBoard_->GetKnowledgeAgentPerceptionContainer().CreateKnowledgeAgentPerception( agentInteracting, attacker );
        pKnowledge->Update( PHY_PerceptionLevel::recognized_, false );
        pKnowledge->NotifyAttacker();
    }
    pionsAttacking_.clear();

    // Population
    for( CIT_PopulationSet itAttacker = populationsAttacking_.begin(); itAttacker != populationsAttacking_.end(); ++itAttacker )
    {
        DEC_Knowledge_PopulationPerception* pKnowledge = pBlackBoard_->GetKnowledgePopulationPerceptionContainer().GetKnowledgePopulationPerception( **itAttacker );
        if( pKnowledge )
            pKnowledge->NotifyAttacker();
    }
    populationsAttacking_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::IsAttacked
// Created: JSR 2012-04-12
// -----------------------------------------------------------------------------
bool DEC_KS_Fire::IsAttacked() const
{
    return !pionsAttacking_.empty() || !populationsAttacking_.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::NotifyHitByDirectFire
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
void DEC_KS_Fire::NotifyAttackedBy( MIL_AgentPion& attacker )
{
    pionsAttacking_.insert( &attacker );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::NotifyAttackedBy
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void DEC_KS_Fire::NotifyAttackedBy( MIL_Population& attacker )
{
    populationsAttacking_.insert( &attacker );
}

