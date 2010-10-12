// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeUrbanFunctions.h"
#include "AlgorithmsFactories.h"
#include "OnComponentComputer_ABC.h"
#include "OnComponentFunctorComputerFactory_ABC.h"
#include "OnComponentFunctor_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_LinearInterpolation.h"
#include <urban/MaterialCompositionType.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/Architecture.h>
#include <geometry/Types.h>

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetCurrentPerceptionLevel
// Created: SLG 2010-02-01
// -----------------------------------------------------------------------------
float DEC_KnowledgeUrbanFunctions::GetCurrentRecceProgress( boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    if( pKnowledge.get() && pKnowledge->IsValid() )
        return pKnowledge->GetCurrentRecceProgress();
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetLivingEnemiesInBU
// Created: GGE 2010-08-16
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeUrbanFunctions::GetLivingEnemiesInBU( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    T_ConstKnowledgeAgentVector knowledges;

    const T_KnowledgeAgentVector& enemies = callerAgent.GetKnowledgeGroup().GetKnowledge().GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); it++ )
      if( (*it)->IsInUrbanBlock( pKnowledge->GetTerrainObjectKnown() ) )
        knowledges.push_back( *it );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetCurrentBarycenter
// Created: MGD 2010-02-19
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_KnowledgeUrbanFunctions::GetCurrentBarycenter( boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    boost::shared_ptr< MT_Vector2D > pos;
    if( pKnowledge.get() && pKnowledge->IsValid() )
    {
        geometry::Point2f temp = pKnowledge->GetBarycenter();
        pos.reset( new MT_Vector2D( temp.X(), temp.Y() ) );
    }
    return pos;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetBoundingBox
// Created: LGY 2010-10-11
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_KnowledgeUrbanFunctions::GetBoundingBox( boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( pKnowledge.get() && pKnowledge->IsValid() )
    {
        const geometry::Polygon2f* pBoundingBox = pKnowledge->GetTerrainObjectKnown().GetFootprint();
        if( pBoundingBox )
        {
            const geometry::Polygon2f::T_Vertices& points = pBoundingBox->Vertices();
            const geometry::Point2f barycenter = pBoundingBox->Barycenter();
            for( geometry::Polygon2f::CIT_Vertices it = points.begin(); it != points.end(); ++it )
            {
                const float distance = 10.f; // $$$$ _RC_ LGY 2010-10-11: delta hardcoded
                geometry::Vector2f vector( barycenter, *it );
                geometry::Point2f point = *it + vector.Normalize() * distance;
                result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( point.X(), point.Y() ) ) );
            }
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetPathfindCost
// Created: MGD 2010-03-18
// -----------------------------------------------------------------------------
float DEC_KnowledgeUrbanFunctions::GetPathfindCost( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    if( pKnowledge.get() && pKnowledge->IsValid() )
        return pKnowledge->GetPathfindCost( static_cast< float >( callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMajorComponentWeight() ) );
    return -1;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetRapForLocal
// Created: MGD 2010-05-05
// -----------------------------------------------------------------------------
float DEC_KnowledgeUrbanFunctions::GetRapForLocal( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    //@TODO MGD Add a rapFor computer, common with DEC_Knowledge_RapForLocal
    T_KnowledgeAgentVector dangerousEnemies_;

    double rTotalFightScoreEnemy  = 0;
    double rTotalFightScoreFriend = 0;

    const T_KnowledgeAgentVector& enemies = callerAgent.GetKnowledgeGroup().GetKnowledge().GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); it++ )
    {
        if( (*it)->IsInUrbanBlock( pKnowledge->GetTerrainObjectKnown() ) )
        {
            rTotalFightScoreEnemy += static_cast< float >( (*it)->GetDangerosity( callerAgent ) );
            dangerousEnemies_.push_back( *it );
        }
    }

    const T_KnowledgeAgentVector& allies = callerAgent.GetKnowledgeGroup().GetKnowledge().GetFriends();
    for( CIT_KnowledgeAgentVector it = allies.begin(); it != allies.end(); it++ )
    {
        if( (*it)->IsInUrbanBlock( pKnowledge->GetTerrainObjectKnown() ) )
        {
            double rTotalDangerosity = 0.;
            for( CIT_ConstKnowledgeAgentVector itAgentEnemy = dangerousEnemies_.begin(); itAgentEnemy != dangerousEnemies_.end(); ++itAgentEnemy )
                rTotalDangerosity += ( (*it)->GetDangerosity( **itAgentEnemy ) * (*it)->GetOperationalState() );
            rTotalFightScoreFriend += ( rTotalDangerosity / dangerousEnemies_.size() );
        }
    }

    double rRapForValue = 1.;
    if( rTotalFightScoreEnemy != 0. )
        rRapForValue = rTotalFightScoreFriend / rTotalFightScoreEnemy;

    // Add bonus if the pion is posted in this urbanbloc
    const UrbanObjectWrapper* urbanBlock = callerAgent.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    if( urbanBlock && &pKnowledge->GetTerrainObjectKnown() == &urbanBlock->GetObject() && callerAgent.GetRole< PHY_RoleInterface_Posture >().IsInstalled() )
        rRapForValue *= 1.2;
    rRapForValue = std::max( 0.2, std::min( 5., rRapForValue ) );
    return static_cast< float >( rRapForValue );
}

namespace
{
    class SensorFunctor : private boost::noncopyable
    {
    public:
         SensorFunctor( const MIL_Agent_ABC& perceiver, const MT_Vector2D& point, const MT_Vector2D& target )
            : perceiver_( perceiver )
            , point_    ( point )
            , target_   ( target )
            , energy_   ( 0 )
        {}
        ~SensorFunctor()
        {}
        void operator()( const PHY_Sensor& sensor )
        {
            const PHY_SensorTypeAgent* sensorTypeAgent = sensor.GetType().GetTypeAgent();
            if( sensorTypeAgent )
                energy_ = std::max( energy_, sensorTypeAgent->RayTrace( point_, target_ ) );
        }
        double GetEnergy() const
        {
            return energy_;
        }
    private:
        const MIL_Agent_ABC& perceiver_;
        const MT_Vector2D& point_;
        const MT_Vector2D& target_;
        double energy_;
    };

    class Functor : public OnComponentFunctor_ABC
    {
        public:
             Functor( const MIL_Agent_ABC& perceiver, const MT_Vector2D& point, const MT_Vector2D& target )
                : perceiver_( perceiver )
                , point_    ( point )
                , target_   ( target )
                , energy_   ( 0 )
            {}
            ~Functor()
            {}
        void operator()( PHY_ComposantePion& composante )
        {
            if( !composante.CanPerceive() )
                return;
            SensorFunctor dataFunctor( perceiver_, point_, target_ );
            composante.ApplyOnSensors( dataFunctor );
            energy_ = std::max( energy_, dataFunctor.GetEnergy() );
         }
        double GetEnergy() const
        {
            return energy_;
        }
    private:
        const MIL_Agent_ABC& perceiver_;
        const MT_Vector2D& point_;
        const MT_Vector2D& target_;
        double energy_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetPerception
// Created: LMT 2010-07-02
// -----------------------------------------------------------------------------
double DEC_KnowledgeUrbanFunctions::GetPerception( const MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > pPoint, boost::shared_ptr< MT_Vector2D > pTarget )
{
    Functor dataFunctor( callerAgent, *pPoint, *pTarget );
    std::auto_ptr< OnComponentComputer_ABC > dataComputer( callerAgent.GetAlgorithms().onComponentFunctorComputerFactory_->Create( dataFunctor ) );
    const_cast< MIL_AgentPion& >( callerAgent ).Execute( *dataComputer );
    return dataFunctor.GetEnergy();
}
