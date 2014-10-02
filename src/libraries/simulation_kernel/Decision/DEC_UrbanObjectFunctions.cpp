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
#include "MIL_AgentServer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Deployment/PHY_RoleInterface_Deployment.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Objects/MaterialAttribute.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeUrban.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/UrbanPhysicalCapacity.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetCurrentPerceptionLevel
// Created: SLG 2010-02-01
// -----------------------------------------------------------------------------
float DEC_UrbanObjectFunctions::GetCurrentRecceProgress( const MIL_AgentPion& pion, MIL_UrbanObject_ABC* pUrbanObject )
{
    if( pUrbanObject )
    {
        boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge = pion.GetArmy().GetKnowledge().GetKnowledgeUrbanContainer().GetKnowledgeUrban( *pUrbanObject );
        if( pKnowledge )
            return pKnowledge->GetCurrentRecceProgress();
    }
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetLivingEnemiesInBU
// Created: GGE 2010-08-16
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_UrbanObjectFunctions::GetLivingEnemiesInBU( const MIL_AgentPion& callerAgent, MIL_UrbanObject_ABC* pUrbanObject )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        const T_KnowledgeAgentVector& enemies = bbKg->GetEnemies();
        for( auto it = enemies.begin(); it != enemies.end(); it++ )
        {
            if( !(*it) )
                continue;
            if( pUrbanObject && ( *it )->IsInUrbanBlock( *pUrbanObject ) )
                knowledges.push_back( *it );
        }
    }
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetCurrentBarycenter
// Created: MGD 2010-02-19
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_UrbanObjectFunctions::GetCurrentBarycenter( MIL_UrbanObject_ABC* pUrbanObject )
{
    if( pUrbanObject )
        return boost::make_shared< MT_Vector2D >( pUrbanObject->GetLocalisation().ComputeBarycenter() );
    return boost::make_shared< MT_Vector2D >();
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetBarycenter
// Created: LGY 2011-11-17
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_UrbanObjectFunctions::GetBarycenter( MIL_UrbanObject_ABC* pUrbanObject )
{
    if( pUrbanObject )
    {
        TER_Localisation localisation = pUrbanObject->GetLocalisation();
        boost::shared_ptr< MT_Vector2D > pBaryventer( new MT_Vector2D( localisation.ComputeBarycenter() ) );
        if( localisation.IsInside( *pBaryventer ) )
            return pBaryventer;
        const T_PointVector& points = localisation.GetPoints();
        MT_Vector2D point( pBaryventer->rX_, pBaryventer->rY_ );
        double distance = std::numeric_limits< double >::max();
        for( auto it = points.begin(); it != points.end(); ++it )
        {
            double result = pBaryventer->Distance( *it );
            if( result < distance )
            {
                point = MT_Vector2D( it->rX_, it->rY_ );
                distance = result;
            }
        }
        return boost::make_shared< MT_Vector2D >( point );
    }
    return boost::make_shared< MT_Vector2D >();
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetBoundingBox
// Created: LGY 2010-10-11
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_UrbanObjectFunctions::GetBoundingBox( MIL_UrbanObject_ABC* pUrbanObject )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( pUrbanObject )
    {
        const T_PointVector& points = pUrbanObject->GetLocalisation().GetPoints();
        const MT_Vector2D barycenter = pUrbanObject->GetLocalisation().ComputeBarycenter();
        static const float distance = 10.f; // $$$$ _RC_ LGY 2010-10-11: delta hardcoded
        for( auto it = points.begin(); it != points.end(); ++it )
            result.push_back( boost::make_shared< MT_Vector2D >( *it + ( *it - barycenter ).Normalize() * distance ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetPathfindCost
// Created: MGD 2010-03-18
// -----------------------------------------------------------------------------
double DEC_UrbanObjectFunctions::GetPathfindCost( const MIL_AgentPion& callerAgent, MIL_UrbanObject_ABC* pUrbanObject )
{
    if( pUrbanObject )
    {
        if( const UrbanPhysicalCapacity* pPhysical = pUrbanObject->Retrieve< UrbanPhysicalCapacity >() )
        {
            if( callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight() > pPhysical->GetTrafficability() )
                return -1.;
            return pPhysical->GetOccupation();
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetRapForLocal
// Created: MGD 2010-05-05
// -----------------------------------------------------------------------------
float DEC_UrbanObjectFunctions::GetRapForLocal( const MIL_AgentPion& callerAgent, MIL_UrbanObject_ABC* pUrbanObject )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( !bbKg )
        return 1.;

    //@TODO MGD Add a rapFor computer, common with DEC_Knowledge_RapForLocal
    T_KnowledgeAgentVector dangerousEnemies_;

    double rTotalFightScoreEnemy  = 0;
    double rTotalFightScoreFriend = 0;

    const PHY_MaterialCompositionType* material = 0;
    if( const MaterialAttribute* materialAttribute = pUrbanObject ? pUrbanObject->RetrieveAttribute< MaterialAttribute >() : 0 )
        material = &materialAttribute->GetMaterial();

    const T_KnowledgeAgentVector& enemies = bbKg->GetEnemies();
    for( auto it = enemies.begin(); it != enemies.end(); it++ )
    {
        if( !(*it) )
            continue;
        if( material )
        {
            rTotalFightScoreEnemy += static_cast< float >( ( *it )->GetDangerosity( callerAgent, false, material ) );
            dangerousEnemies_.push_back( *it );
        }
    }

    if( !dangerousEnemies_.empty() )
    {
        const T_KnowledgeAgentVector& allies = bbKg->GetFriends();
        for( auto it = allies.begin(); it != allies.end(); it++ )
        {
            if( !(*it) )
                continue;
            double rTotalDangerosity = 0.;
            for( auto itAgentEnemy = dangerousEnemies_.begin(); itAgentEnemy != dangerousEnemies_.end(); ++itAgentEnemy )
                rTotalDangerosity += ( ( *it )->GetDangerosity( **itAgentEnemy, true, ( *itAgentEnemy )->GetUrbanMaterial() ) * ( *it )->GetOperationalState() );
            rTotalFightScoreFriend += ( rTotalDangerosity / dangerousEnemies_.size() );
        }
    }

    double rRapForValue = 1.;
    if( rTotalFightScoreEnemy != 0. )
        rRapForValue = rTotalFightScoreFriend / rTotalFightScoreEnemy;

    // Add bonus if the pion is posted in this urbanbloc
    const MIL_UrbanObject_ABC* urbanBlock = callerAgent.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    if( pUrbanObject && urbanBlock && pUrbanObject == urbanBlock && callerAgent.GetRole< PHY_RoleInterface_Deployment >().IsDeployed() )  // $$$$ _RC_ LGY 2011-02-24: == sur les ID
        rRapForValue *= 1.2;
    rRapForValue = std::max( 0.2, std::min( 5., rRapForValue ) );
    return static_cast< float >( rRapForValue );
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetStateUrbanBlock
// Created: DDA 2011-03-30
// -----------------------------------------------------------------------------
float DEC_UrbanObjectFunctions::GetStateUrbanBlock( MIL_UrbanObject_ABC* pUrbanObject )
{
    if( pUrbanObject )
    {
        StructuralCapacity* pCapacity = pUrbanObject->Retrieve< StructuralCapacity >();
        if( pCapacity )
            return pCapacity->GetStructuralState();
    }
    return -1.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::SetUrbanBlockState
// Created: MCO 2014-07-09
// -----------------------------------------------------------------------------
void DEC_UrbanObjectFunctions::SetUrbanBlockState( MIL_UrbanObject_ABC* pUrbanObject, float state )
{
    if( !pUrbanObject )
        return;
    if( StructuralCapacity* pCapacity = pUrbanObject->Retrieve< StructuralCapacity >() )
        pCapacity->SetStructuralState( state );
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetPolygonFromUrbanBlock
// Created: EVH 2011-03-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_UrbanObjectFunctions::GetPolygonFromUrbanBlock( const MIL_UrbanObject_ABC* pUrbanObject )
{
    if( !pUrbanObject )
        throw MASA_EXCEPTION( "invalid parameter." );
    boost::shared_ptr< TER_Localisation > location( new TER_Localisation( pUrbanObject->GetLocalisation() ) );
    return location;
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::DestroyUrbanBlock
// Created: EVH 2011-04-04
// -----------------------------------------------------------------------------
void DEC_UrbanObjectFunctions::DestroyUrbanBlock(  MIL_AgentPion& callerAgent, MIL_UrbanObject_ABC* pUrbanObject, const PHY_DotationCategory* category )
{
    if( !pUrbanObject )
        throw MASA_EXCEPTION( "invalid parameter." );
    StructuralCapacity* capacity = const_cast< StructuralCapacity* >( pUrbanObject->Retrieve< StructuralCapacity >() );
    if( capacity )
    {
        if( category->CanBeUsedForIndirectFire() )
        {
            const PHY_RoleInterface_Location& location = callerAgent.Get< PHY_RoleInterface_Location >();
            TER_Localisation localisation( location.GetPosition(), category->GetRadius() );
            capacity->ApplyIndirectFire( *pUrbanObject, localisation, *category, &callerAgent.GetArmy() );
        }
        else
            capacity->ApplyIndirectFire( *pUrbanObject, pUrbanObject->GetLocalisation(), *category, &callerAgent.GetArmy() );
        callerAgent.Get< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( *category, 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetType
// Created: LMT 2011-04-05
// -----------------------------------------------------------------------------
std::string DEC_UrbanObjectFunctions::GetType( const MIL_UrbanObject_ABC* pUrbanObject )
{
    if( !pUrbanObject )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pUrbanObject->GetType().GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_UrbanObjectFunctions::GetUrbanBlockInZone
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
T_UrbanObjectVector DEC_UrbanObjectFunctions::GetUrbanBlockInZone( TER_Localisation* pLocalisation )
{
    T_UrbanObjectVector result;
    if( pLocalisation )
    {
        const T_UrbanObjectVector& blocks = MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlocks();
        for( auto it = blocks.begin(); it != blocks.end(); ++it )
            if( ( *it ) && pLocalisation->Contains( ( *it )->GetLocalisation() ) )
                result.push_back( *it );
    }
    return result;
}
