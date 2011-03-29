// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_UrbanObjectFunctions.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeUrban.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include <urban/Architecture.h>

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetCurrentPerceptionLevel
// Created: SLG 2010-02-01
// -----------------------------------------------------------------------------
float DEC_UrbanObjectFunctions::GetCurrentRecceProgress( const MIL_AgentPion& pion, UrbanObjectWrapper* pUrbanObject )
{
    if( pUrbanObject )
    {
        boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge = pion.GetArmy().GetKnowledge().GetKnowledgeUrbanContainer().GetKnowledgeUrban( *pUrbanObject );
        if( pKnowledge.get() )
            return pKnowledge->GetCurrentRecceProgress();
    }
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetLivingEnemiesInBU
// Created: GGE 2010-08-16
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_UrbanObjectFunctions::GetLivingEnemiesInBU( const MIL_AgentPion& callerAgent, UrbanObjectWrapper* pUrbanObject )
{
    T_ConstKnowledgeAgentVector knowledges;
    const T_KnowledgeAgentVector& enemies = callerAgent.GetKnowledgeGroup().GetKnowledge().GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); it++ )
        if( pUrbanObject && ( *it )->IsInUrbanBlock( *pUrbanObject ) )
            knowledges.push_back( *it );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetCurrentBarycenter
// Created: MGD 2010-02-19
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_UrbanObjectFunctions::GetCurrentBarycenter( UrbanObjectWrapper* pUrbanObject )
{
    if( pUrbanObject )
        return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( pUrbanObject->GetLocalisation().ComputeBarycenter() ) );
    return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D() );
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetBoundingBox
// Created: LGY 2010-10-11
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_UrbanObjectFunctions::GetBoundingBox( UrbanObjectWrapper* pUrbanObject )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( pUrbanObject )
    {
        const T_PointVector& points = pUrbanObject->GetLocalisation().GetPoints();
        const MT_Vector2D barycenter = pUrbanObject->GetLocalisation().ComputeBarycenter();
        static const float distance = 10.f; // $$$$ _RC_ LGY 2010-10-11: delta hardcoded
        for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
            result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( *it + ( *it - barycenter ).Normalize() * distance ) ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetPathfindCost
// Created: MGD 2010-03-18
// -----------------------------------------------------------------------------
float DEC_UrbanObjectFunctions::GetPathfindCost( const MIL_AgentPion& callerAgent, UrbanObjectWrapper* pUrbanObject )
{
    if( pUrbanObject )
        if( const urban::Architecture* architecture = pUrbanObject->GetArchitecture() )
            return architecture->GetPathfindCost( static_cast< float >( callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMajorComponentWeight() ) );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetRapForLocal
// Created: MGD 2010-05-05
// -----------------------------------------------------------------------------
float DEC_UrbanObjectFunctions::GetRapForLocal( const MIL_AgentPion& callerAgent, UrbanObjectWrapper* pUrbanObject )
{
    //@TODO MGD Add a rapFor computer, common with DEC_Knowledge_RapForLocal
    T_KnowledgeAgentVector dangerousEnemies_;

    double rTotalFightScoreEnemy  = 0;
    double rTotalFightScoreFriend = 0;

    const T_KnowledgeAgentVector& enemies = callerAgent.GetKnowledgeGroup().GetKnowledge().GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); it++ )
        if( pUrbanObject && ( *it )->IsInUrbanBlock( *pUrbanObject ) )
        {
            rTotalFightScoreEnemy += static_cast< float >( ( *it )->GetDangerosity( callerAgent ) );
            dangerousEnemies_.push_back( *it );
        }

    const T_KnowledgeAgentVector& allies = callerAgent.GetKnowledgeGroup().GetKnowledge().GetFriends();
    for( CIT_KnowledgeAgentVector it = allies.begin(); it != allies.end(); it++ )
        if( pUrbanObject && ( *it )->IsInUrbanBlock( *pUrbanObject ) )
        {
            double rTotalDangerosity = 0.;
            for( CIT_ConstKnowledgeAgentVector itAgentEnemy = dangerousEnemies_.begin(); itAgentEnemy != dangerousEnemies_.end(); ++itAgentEnemy )
                rTotalDangerosity += ( ( *it )->GetDangerosity( **itAgentEnemy ) * ( *it )->GetOperationalState() );
            rTotalFightScoreFriend += ( rTotalDangerosity / dangerousEnemies_.size() );
        }

    double rRapForValue = 1.;
    if( rTotalFightScoreEnemy != 0. )
        rRapForValue = rTotalFightScoreFriend / rTotalFightScoreEnemy;

    // Add bonus if the pion is posted in this urbanbloc
    const UrbanObjectWrapper* urbanBlock = callerAgent.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    if( pUrbanObject && urbanBlock && pUrbanObject == urbanBlock && callerAgent.GetRole< PHY_RoleInterface_Posture >().IsInstalled() )  // $$$$ _RC_ LGY 2011-02-24: == sur les ID
        rRapForValue *= 1.2;
    rRapForValue = std::max( 0.2, std::min( 5., rRapForValue ) );
    return static_cast< float >( rRapForValue );
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::DestroyUrbanBlock
// Created: LGY 2011-03-24
// -----------------------------------------------------------------------------
void DEC_UrbanObjectFunctions::DestroyUrbanBlock( UrbanObjectWrapper* pUrbanObject, double percentage )
{
    if( pUrbanObject )
    {
        StructuralCapacity* pCapacity = pUrbanObject->Retrieve< StructuralCapacity >();
        if( pCapacity )
            pCapacity->Build( -percentage );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetPolygonFromUrbanBlock
// Created: EVH 2011-03-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_UrbanObjectFunctions::GetPolygonFromUrbanBlock( const UrbanObjectWrapper* pUrbanObject )
{
    boost::shared_ptr< TER_Localisation > location( new TER_Localisation( pUrbanObject->GetLocalisation() ) );
    return location;
}
