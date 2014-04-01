// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Agent_Path.h"
#include "DEC_Agent_PathSection.h"
#include "DEC_Agent_PathClass.h"
#include "DEC_Path_KnowledgeAgent.h"
#include "DEC_Path_KnowledgeObject.h"
#include "DEC_Path_KnowledgeObjectFlood.h"
#include "DEC_Path_KnowledgeObjectBurnSurface.h"
#include "DEC_Path_KnowledgePopulation.h"
#include "MIL_AgentServer.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_PathType.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_Rep_PathPoint_Front.h"
#include "Decision/DEC_Rep_PathPoint_Special.h"
#include "Decision/DEC_Rep_PathPoint_Lima.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "Entities/Agents/Roles/Terrain/PHY_RoleInterface_TerrainAnalysis.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Objects/FloodAttribute.h"
#include "Entities/Objects/FloodCapacity.h"
#include "Entities/Objects/BurnSurfaceCapacity.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Tools/MIL_Config.h"
#include "MT_Tools/MT_Logger.h"
#include "simulation_terrain/TER_Pathfinder_ABC.h"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

namespace
{
    double GetWeight( const MIL_Agent_ABC& queryMaker )
    {
        double weight = queryMaker.GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight();
        const auto& reinforcements = queryMaker.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
        for( auto it = reinforcements.begin(); it != reinforcements.end(); ++it )
            weight = std::max( weight, ( *it )->GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight() );
        return weight;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: JDY 03-04-10
//-----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( MIL_Agent_ABC& queryMaker, const T_PointVector& points, const DEC_PathType& pathType )
    : DEC_PathResult           ( pathType )
    , queryMaker_              ( queryMaker )
    , bRefine_                 ( queryMaker.GetType().GetUnitType().CanFly() && !queryMaker.IsAutonomous() )
    , vDirDanger_              ( queryMaker.GetOrderManager().GetDirDanger() )
    , unitSpeeds_              ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_               ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , rSlopeDeceleration_      ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetSlopeDeceleration() )
    , rCostOutsideOfAllObjects_( 0. )
    , pathClass_               ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , bDecPointsInserted_      ( false )
    , destroyed_               ( false )
    , unitMajorWeight_         ( GetWeight( queryMaker ) )
{
    queryMaker_.RegisterPath( *this );
    fuseau_= queryMaker.GetOrderManager().GetFuseau();
    automateFuseau_ = queryMaker.GetAutomate().GetOrderManager().GetFuseau();
    initialWaypoints_.reserve( 1 + points.size() );
    nextWaypoints_.reserve( points.size() );
    initialWaypoints_.push_back( queryMaker_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    std::copy( points.begin(), points.end(), std::back_inserter( initialWaypoints_ ) );
    std::copy( points.begin(), points.end(), std::back_inserter( nextWaypoints_ ) );
    Initialize( initialWaypoints_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: LDC 2009-06-18
// -----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( MIL_Agent_ABC& queryMaker, const std::vector< boost::shared_ptr< MT_Vector2D > >& points, const DEC_PathType& pathType )
    : DEC_PathResult           ( pathType )
    , queryMaker_              ( queryMaker )
    , bRefine_                 ( queryMaker.GetType().GetUnitType().CanFly() && !queryMaker.IsAutonomous() )
    , vDirDanger_              ( queryMaker.GetOrderManager().GetDirDanger() )
    , unitSpeeds_              ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_               ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , rSlopeDeceleration_      ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetSlopeDeceleration() )
    , rCostOutsideOfAllObjects_( 0. )
    , pathClass_               ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , bDecPointsInserted_      ( false )
    , destroyed_               ( false )
    , unitMajorWeight_         ( GetWeight( queryMaker ) )
{
    queryMaker_.RegisterPath( *this );
    fuseau_ = queryMaker.GetOrderManager().GetFuseau();
    automateFuseau_ = queryMaker.GetAutomate().GetOrderManager().GetFuseau();
    for( auto it = points.begin(); it != points.end(); ++it )
    {
        initialWaypoints_.push_back( **it );
        if( it != points.begin() )
            nextWaypoints_.push_back( **it );
    }
    Initialize( initialWaypoints_ );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( MIL_Agent_ABC& queryMaker, const MT_Vector2D& vPosStart, const MT_Vector2D& vPosEnd, const DEC_PathType& pathType )
    : DEC_PathResult            ( pathType )
    , queryMaker_               ( queryMaker )
    , bRefine_                  ( queryMaker.GetType().GetUnitType().CanFly() && !queryMaker.IsAutonomous() )
    , vDirDanger_               ( queryMaker.GetOrderManager().GetDirDanger() )
    , unitSpeeds_               ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_                ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , rSlopeDeceleration_       ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetSlopeDeceleration() )
    , rCostOutsideOfAllObjects_ ( 0. )
    , pathClass_                ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , bDecPointsInserted_       ( false )
    , destroyed_                ( false )
    , unitMajorWeight_          ( GetWeight( queryMaker ) )
{
    queryMaker_.RegisterPath( *this );
    fuseau_ = queryMaker.GetOrderManager().GetFuseau();
    automateFuseau_ = queryMaker.GetAutomate().GetOrderManager().GetFuseau();
    initialWaypoints_.push_back( vPosStart );
    initialWaypoints_.push_back( vPosEnd );
    nextWaypoints_.push_back( vPosEnd );
    Initialize( initialWaypoints_ );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Agent_Path::~DEC_Agent_Path()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::Destroy
// Created: MMC 2013-05-21
// -----------------------------------------------------------------------------
void DEC_Agent_Path::Destroy()
{
    if( destroyed_ )
        return;
    for( auto it = resultList_.begin(); it!= resultList_.end(); it++ )
        if( ( *it )->GetType() != DEC_PathPoint::eTypePointPath )
            ( *it )->RemoveFromDIA( *it );
    fuseau_.Reset();
    automateFuseau_.Reset();
    destroyed_ = true;
    queryMaker_.UnregisterPath( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::Initialize
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Agent_Path::Initialize( const T_PointVector& points )
{
    InitializePathKnowledges( points );
    if( points.empty() )
        MT_LOG_ERROR_MSG( "Initializing empty agent path" );
    const MT_Vector2D* pLastPoint = 0;
    for( auto itPoint = points.begin(); itPoint != points.end(); ++itPoint )
    {
        if( pLastPoint )
        {
            DEC_Agent_PathSection* pSection = new DEC_Agent_PathSection( *this, *pLastPoint, *itPoint );
            RegisterPathSection( *pSection );
        }
        pLastPoint = &( *itPoint );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetUnitMaxSlope
// Created: RPD 2009-11-23
// -----------------------------------------------------------------------------
double DEC_Agent_Path::GetUnitMajorWeight() const
{
    return unitMajorWeight_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InitializePathKnowledges
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_Agent_Path::InitializePathKnowledges( const T_PointVector& pathPoints )
{
    if( pathClass_.AvoidEnemies() )
    {
        auto bbKg = queryMaker_.GetKnowledgeGroup()->GetKnowledge();
        if( bbKg )
        {
            const T_KnowledgeAgentVector& enemies = bbKg->GetEnemies();
            for( auto itKnowledgeAgent = enemies.begin(); itKnowledgeAgent != enemies.end(); ++itKnowledgeAgent )
            {
                const DEC_Knowledge_Agent& knowledge = **itKnowledgeAgent;
                if( knowledge.IsValid() && fuseau_.IsInside( knowledge.GetPosition() ) )
                {
                    const double factor = pathClass_.GetEnemyCostOnContact();
                    if( factor > 0 )
                        pathKnowledgeAgents_.push_back( DEC_Path_KnowledgeAgent( knowledge.GetPosition(),
                            pathClass_.GetEnemyCostAtSecurityRange(), factor, knowledge.GetMaxRangeToFireOn( queryMaker_, 0 ) ) );
                }
            }
        }
    }

    // Objects
    if( pathClass_.AvoidObjects() )
    {
        T_KnowledgeObjectVector knowledgesObject;
        MIL_PathObjectFilter filter;
        if( DEC_BlackBoard_CanContainKnowledgeObject* container = queryMaker_.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
            container->GetObjectsAtInteractionHeight( knowledgesObject, queryMaker_, filter );

        T_PointVector firstPointVector;
        if( !pathPoints.empty() )
            firstPointVector.push_back( *pathPoints.begin() );
        for( auto itKnowledgeObject = knowledgesObject.begin(); itKnowledgeObject != knowledgesObject.end(); ++itKnowledgeObject )
        {
            const DEC_Knowledge_Object& knowledge = **itKnowledgeObject;
            if( knowledge.CanCollideWith( queryMaker_ ) )
            {
                if( knowledge.IsObjectInsidePathPoint( firstPointVector, &queryMaker_ ) )
                {
                    if( const MIL_Object_ABC* pObject = knowledge.GetObjectKnown() )
                    {
                        TerrainData data;
                        double rMaxSpeed = queryMaker_.GetRole< moving::PHY_RoleAction_Moving >().GetSpeed( data, *pObject );
                        if( rMaxSpeed == 0. || rMaxSpeed == std::numeric_limits< double >::max() )
                            continue;
                    }
                }

                if( pathKnowledgeObjects_.size() <= knowledge.GetType().GetID() )
                    pathKnowledgeObjects_.resize( knowledge.GetType().GetID() + 1 );

                T_PathKnowledgeObjectVector& pathKnowledges = pathKnowledgeObjects_[ knowledge.GetType().GetID() ];
                bool empty = pathKnowledges.empty();
                if( knowledge.GetType().GetCapacity< FloodCapacity >() )
                    pathKnowledges.push_back( boost::make_shared< DEC_Path_KnowledgeObjectFlood >( queryMaker_.GetType().GetUnitType().GetCrossingHeight(), knowledge ) );
                else if( knowledge.GetType().GetCapacity< BurnSurfaceCapacity >() )
                    pathKnowledges.push_back( boost::make_shared< DEC_Path_KnowledgeObjectBurnSurface >( knowledge ) );
                else if( ( pathClass_.GetObjectCost( knowledge.GetType() ) != 0 && knowledge.GetLocalisation().GetType() != TER_Localisation::eNone )
                     || knowledge.HasAgentMaxSpeedMultiplier() )
                    pathKnowledges.push_back( boost::make_shared< DEC_Path_KnowledgeObject >( knowledge, pathClass_.GetObjectCost( knowledge.GetType() ), pathClass_.GetThreshold() ) );
                if( empty && pathKnowledges.size() == 1 && pathKnowledges.front()->GetCostOut() > 0 )
                    rCostOutsideOfAllObjects_ += pathKnowledges.front()->GetCostOut();
            }
        }
    }

    // Populations
    if( pathClass_.HandlePopulations() )
    {
        auto bbKg = queryMaker_.GetKnowledgeGroup()->GetKnowledge();
        if( bbKg )
        {
            T_KnowledgePopulationVector knowledgesPopulation;
            bbKg->GetPopulations( knowledgesPopulation );
            pathKnowledgePopulations_.reserve( knowledgesPopulation.size() );
            for( auto it = knowledgesPopulation.begin(); it != knowledgesPopulation.end(); ++it )
                pathKnowledgePopulations_.push_back( boost::make_shared< DEC_Path_KnowledgePopulation >( boost::cref( **it ), boost::cref( pathClass_ ), !queryMaker_.GetType().IsTerrorist() ) );
        }

    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::IsPointAvantIn
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool DEC_Agent_Path::IsPointAvantIn( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    // Test : entrée dans environnement
    return !( nObjectTypesBefore.ContainsOne( nTypeTerrain ) ) && ( nObjectTypesToNextPoint.ContainsOne( nTypeTerrain ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::IsPointAvantOut
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool DEC_Agent_Path::IsPointAvantOut( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    // Test : sortie d'environnement
    return ( nObjectTypesBefore.ContainsOne( nTypeTerrain ) ) && !( nObjectTypesToNextPoint.ContainsOne(  nTypeTerrain ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::IsPointAvant
// Created: AGE 2005-02-04
// -----------------------------------------------------------------------------
bool DEC_Agent_Path::IsPointAvant( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    return IsPointAvantIn ( nObjectTypesBefore, nObjectTypesToNextPoint, nTypeTerrain )
        || IsPointAvantOut( nObjectTypesBefore, nObjectTypesToNextPoint, nTypeTerrain );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPreviousPathPointOnDifferentLocation
// Created: JVT 2005-07-08
// -----------------------------------------------------------------------------
inline
DEC_Agent_Path::T_PathPoints::iterator DEC_Agent_Path::GetPreviousPathPointOnDifferentLocation( T_PathPoints::iterator itCurrent )
{
    if( itCurrent == resultList_.end() )
        throw MASA_EXCEPTION( "Current path point is invalid" );
    const MT_Vector2D& vPosition = ( *itCurrent )->GetPos();
    while ( itCurrent != resultList_.begin() && ( *itCurrent )->GetPos() == vPosition )
        --itCurrent;
    return itCurrent;
}

#pragma warning( push )
#pragma warning( disable : 4127 ) // conditional expression is constant

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAvant
// Created: NLD 2005-08-11
// -----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAvant( const boost::shared_ptr< DEC_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent )
{
    double rDistanceLeft = spottedPathPoint->GetTypePoint() == DEC_Rep_PathPoint::eTypePointLima ?
                             queryMaker_.GetType().GetDistanceAvantLima() :
                             queryMaker_.GetType().GetDistanceAvantPoint( spottedPathPoint->GetTypeTerrain() );
    try
    {
        while( true )
        {
            const MT_Vector2D& vCurrentPos = ( *itCurrent )->GetPos();
            if( itCurrent == resultList_.begin() )
            {
                auto pNewPoint = boost::make_shared< DEC_Rep_PathPoint_Front >( vCurrentPos, spottedPathPoint );
                resultList_.insert( ++itCurrent, pNewPoint );
                break;
            }

            // remise en position des itérateurs
            auto itPrev = GetPreviousPathPointOnDifferentLocation( itCurrent );
            itCurrent = itPrev;
            ++itCurrent;
            if( ( *itCurrent )->GetPos() != vCurrentPos )
                throw MASA_EXCEPTION( "Current position is invalid" );

            // calcul de la distance "mangée" par le parcours de ce segment
            const MT_Vector2D& vPreviousPos = ( *itPrev )->GetPos();
            MT_Vector2D vTmp = vPreviousPos - vCurrentPos;
            const double vTmpMag = vTmp.Magnitude();

            rDistanceLeft -= vTmpMag;
            if( rDistanceLeft == 0. )
            {
                // Positionnement du point avant au même endroit qu'un autre point
                auto pNewPoint = boost::make_shared< DEC_Rep_PathPoint_Front >( vPreviousPos, spottedPathPoint );
                resultList_.insert( itPrev, pNewPoint );
                break;
            }
            else if( rDistanceLeft < 0. )
            {
                // Positionnement du point avant
                vTmp /= vTmpMag;
                vTmp *= rDistanceLeft;
                vTmp += vPreviousPos;
                auto pNewPoint = boost::make_shared< DEC_Rep_PathPoint_Front >( vTmp, spottedPathPoint );
                resultList_.insert( itCurrent, pNewPoint );
                break;
            }
            // tant pis, ça sera peut-être pour le prochain segment
            itCurrent = itPrev;
        }
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
}

#pragma warning( pop )

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAvant
// Created: JVT 02-12-04
//----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAvant( const boost::shared_ptr< DEC_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent, double& rDistSinceLastPointAvant )
{
    static const double rDist = 2000.;
    if( rDistSinceLastPointAvant > rDist )
    {
        InsertPointAvant( spottedPathPoint, itCurrent );
        rDistSinceLastPointAvant = 0.;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPoint
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
bool DEC_Agent_Path::InsertPoint( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent, double& rDistSinceLastPoint )
{
    static double rDist = 500.;
    if( rDistSinceLastPoint > rDist )
    {
        resultList_.insert( itCurrent, spottedPathPoint );
        rDistSinceLastPoint = 0.;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAndPointAvant
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAndPointAvant( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent, double& rDistSinceLastPoint, double& rDistSinceLastPointAvant )
{
    if( InsertPoint( spottedPathPoint, itCurrent, rDistSinceLastPoint ) )
        InsertPointAvant( spottedPathPoint, itCurrent, rDistSinceLastPointAvant );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAvants
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAvants()
{
    double rDistSinceLastPointAvant = std::numeric_limits< double >::max();
    double rDistSinceLastPoint = std::numeric_limits< double >::max();
    TerrainData nObjectTypesBefore;
    DEC_PathPoint* pPrevPoint = 0;
    for( auto itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        DEC_PathPoint& current = **itPoint;
        if( pPrevPoint )
        {
            if( rDistSinceLastPointAvant != std::numeric_limits< double >::max() )
                rDistSinceLastPointAvant += pPrevPoint->GetPos().Distance( current.GetPos() );
            if( rDistSinceLastPoint != std::numeric_limits< double >::max() )
                rDistSinceLastPoint += pPrevPoint->GetPos().Distance( current.GetPos() );
        }

        // On ne teste les points avants que sur les points du path find original
        if( current.GetType() != DEC_PathPoint::eTypePointPath )
        {
            pPrevPoint = &current;
            continue;
        }

        TerrainData nObjectTypesToNextPoint = current.GetObjectTypesToNextPoint();
        if( nObjectTypesBefore == TerrainData() )
        {
            pPrevPoint = &current;
            nObjectTypesBefore = nObjectTypesToNextPoint;
            continue;
        }

        // Village
        if( IsPointAvantIn( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Urban() ) )
            InsertPointAndPointAvant( boost::make_shared< DEC_Rep_PathPoint_Special >( ( *itPoint )->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierVillage, TerrainData::Urban() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Urban
        else if( IsPointAvantOut( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Urban() ) )
            InsertPoint( boost::make_shared< DEC_Rep_PathPoint >( ( *itPoint )->GetPos(), DEC_Rep_PathPoint::eTypePointCCT, TerrainData::Urban() ), itPoint, rDistSinceLastPoint );

        // Forest
        else if( IsPointAvant( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Forest() ) )
            InsertPointAndPointAvant( boost::make_shared< DEC_Rep_PathPoint >( ( *itPoint )->GetPos(), DEC_Rep_PathPoint::eTypePointCCT, TerrainData::Forest() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Cross roads
        else if( current.GetObjectTypes().ContainsOne( TerrainData::Crossroad() ) )
            InsertPointAndPointAvant( boost::make_shared< DEC_Rep_PathPoint_Special >( ( *itPoint )->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierCarrefour, TerrainData::Crossroad() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Pont
        else if( IsPointAvantIn( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Bridge() )
                || ( !current.GetObjectTypes().ContainsOne( TerrainData::SmallRiver() ) && current.GetObjectTypes().ContainsOne( TerrainData::Bridge() ) && !nObjectTypesBefore.ContainsOne( TerrainData::Bridge() ) && !nObjectTypesToNextPoint.ContainsOne( TerrainData::Bridge() ) )
                )
            InsertPointAndPointAvant( boost::make_shared< DEC_Rep_PathPoint_Special >( ( *itPoint )->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierPont, TerrainData::Bridge() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        nObjectTypesBefore = nObjectTypesToNextPoint;
        pPrevPoint = &current;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertLima
// Created: JVT 02-12-05
//-----------------------------------------------------------------------------
void DEC_Agent_Path::InsertLima( const MIL_LimaOrder& lima )
{
    boost::shared_ptr< DEC_PathPoint > pLastPoint;
    for( auto itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        boost::shared_ptr< DEC_PathPoint > pCurrentPoint = *itPoint;
        if( !pCurrentPoint )
        {
            MT_LOG_ERROR_MSG( "Current point is invalid" );
            break;
        }
        if( pLastPoint.get() && ( pCurrentPoint->GetPos() != pLastPoint->GetPos() ) )
        {
            MT_Line segment( pLastPoint->GetPos(), pCurrentPoint->GetPos() );
            MT_Vector2D posIntersect;
            if( lima.Intersect2D( segment, posIntersect ) )
            {
                for( auto itFunction = lima.GetFunctions().begin(); itFunction != lima.GetFunctions().end(); ++itFunction )
                {
                    auto pPoint = boost::make_shared< DEC_Rep_PathPoint_Lima >( posIntersect, TerrainData(), lima.GetID(), **itFunction );
                    auto itTmp = resultList_.insert( itPoint, pPoint );
                    InsertPointAvant( pPoint, itTmp );
                }
            }
        }
        pLastPoint = pCurrentPoint;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertLimas
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
void DEC_Agent_Path::InsertLimas()
{
    const T_LimaVector& limas = queryMaker_.GetOrderManager().GetLimas();
    for( auto it = limas.begin(); it != limas.end(); ++it )
        InsertLima( *it );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertDecPoints
// Created: JDY 03-03-04
//-----------------------------------------------------------------------------
void DEC_Agent_Path::InsertDecPoints()
{
    if( bDecPointsInserted_ )
        return;
    bDecPointsInserted_ = true;
    // Points avants
    if( queryMaker_.GetRole< PHY_RoleInterface_Location >().GetHeight() == 0 )
        InsertPointAvants();
    // Limas
    InsertLimas();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::CleanAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void DEC_Agent_Path::CleanAfterComputation()
{
    DEC_Path::CleanAfterComputation();
    pathKnowledgeObjects_.clear();
    pathKnowledgeAgents_.clear();
    pathKnowledgePopulations_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::IsPathForUnit
// Created: JSR 2013-03-11
// -----------------------------------------------------------------------------
bool DEC_Agent_Path::IsPathForUnit( MIL_Agent_ABC* pion ) const
{
    return &queryMaker_ == pion;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::Execute
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
void DEC_Agent_Path::Execute( TER_Pathfinder_ABC& pathfind )
{
    if( MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        MT_LOG_MESSAGE_MSG( "DEC_Agent_Path::Compute: " << this << " : computation begin" );
        MT_LOG_MESSAGE_MSG( "   Thread    : " << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() );
        MT_LOG_MESSAGE_MSG( "   Agent     : " << queryMaker_.GetID() );
        MT_LOG_MESSAGE_MSG( "   Path type : " << DEC_PathResult::GetPathType().GetName().c_str() );
        MT_LOG_MESSAGE_MSG( GetPathAsString() );
        profiler_.Start();
    }
    assert( resultList_.empty() );

    pathfind.SetId( queryMaker_.GetID() );
    DEC_Path::Execute( pathfind );
    DEC_PathResult::E_State nPathState = GetState();
    if( nPathState == DEC_Path_ABC::eImpossible )
    {
        const PHY_RoleAction_MovingUnderground* roleUnderground = queryMaker_.RetrieveRole< PHY_RoleAction_MovingUnderground >();
        if( roleUnderground && roleUnderground->IsUnderground() )
            queryMaker_.GetRole< moving::PHY_RoleAction_Moving >().SendRC( report::eRC_NotActivatedUndergroundNetwork );
        else
            queryMaker_.GetRole< moving::PHY_RoleAction_Moving >().SendRC( report::eRC_TerrainDifficile );
    }

#ifndef NDEBUG
    for( auto itPoint = resultList_.begin(); itPoint != resultList_.end(); )
    {
        DEC_PathPoint& point = **itPoint;

//        if( itPoint != resultList_.begin() && !unitSpeeds_.IsPassable( point.GetObjectTypes() ) )
//            throw MASA_EXCEPTION( "Unit max speed is not positive for a given object" );

        ++itPoint;
        if( itPoint != resultList_.end() && unitSpeeds_.GetMaxSpeed( point.GetObjectTypesToNextPoint() ) <= 0 )
            throw MASA_EXCEPTION( "Unit max speed is not positive for a given object" );
    }
#endif

    if( MIL_AgentServer::IsInitialized() && MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        double rComputationTime = profiler_.Stop();

        std::stringstream stream;
        if( ! resultList_.empty() )
            stream << "[" << resultList_.front()->GetPos() << "] -> [" << resultList_.back()->GetPos() << "]";
        MT_LOG_MESSAGE_MSG( "DEC_Agent_Path::Compute: " << this <<
                            ", Thread : "  << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() <<
                            ", Time : " << rComputationTime <<
                            ", State : " << GetStateAsString() <<
                            ", Result : " << stream.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::IsDestinationTrafficable
// $$$$ LDC Should be done in the Terrain heuristics.
// Created: LDC 2010-08-10
// -----------------------------------------------------------------------------
bool DEC_Agent_Path::IsDestinationTrafficable() const
{
    const PHY_RoleInterface_TerrainAnalysis& analysis = queryMaker_.GetRole< PHY_RoleInterface_TerrainAnalysis >();
    return analysis.CanMoveOnUrbanBlock( nextWaypoints_ ) &&
           analysis.CanMoveOnBurningCells( nextWaypoints_ ) &&
           analysis.CanMoveOnKnowledgeObject( nextWaypoints_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::NotifyPointReached
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
void DEC_Agent_Path::NotifyPointReached( const T_PathPoints::const_iterator& itCurrentPathPoint )
{
    const T_PointVector& computedWaypoints = GetComputedWaypoints();
    if( nextWaypoints_.size() > 1 && computedWaypoints.size() > 1 &&
        static_cast< float >( (*itCurrentPathPoint)->GetPos().rX_ ) == static_cast< float >( computedWaypoints.front().rX_ ) &&
        static_cast< float >( (*itCurrentPathPoint)->GetPos().rY_ ) == static_cast< float >( computedWaypoints.front().rY_ ) )
    {
        nextWaypoints_.erase( nextWaypoints_.begin() );
        RemoveComputedWaypoint();
    }
    DEC_PathResult::NotifyPointReached( itCurrentPathPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::IsWaypoint
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
bool DEC_Agent_Path::IsWaypoint( const MT_Vector2D& point ) const
{
    for( T_PointVector::const_iterator it = initialWaypoints_.begin(); it != initialWaypoints_.end(); ++it )
        if( *it == point )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::CancelPath
// Created: LDC 2012-03-23
// -----------------------------------------------------------------------------
void DEC_Agent_Path::CancelPath()
{
    queryMaker_.GetRole< moving::PHY_RoleAction_Moving >().SendRC( report::eRC_TerrainDifficile );
    Cancel();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathClass
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const DEC_Agent_PathClass& DEC_Agent_Path::GetPathClass() const
{
    return pathClass_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetUnitSpeeds
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
const PHY_Speeds& DEC_Agent_Path::GetUnitSpeeds() const
{
    return unitSpeeds_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetUnitMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
double DEC_Agent_Path::GetUnitMaxSlope() const
{
    return rMaxSlope_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetUnitSlopeDeceleration
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
double DEC_Agent_Path::GetUnitSlopeDeceleration() const
{
    return rSlopeDeceleration_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetAutomataFuseau
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
const MIL_Fuseau& DEC_Agent_Path::GetAutomataFuseau() const
{
    return automateFuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetFuseau
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
const MIL_Fuseau& DEC_Agent_Path::GetFuseau() const
{
    return fuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetDirDanger
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Agent_Path::GetDirDanger() const
{
    return vDirDanger_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathKnowledgeObjects
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
const DEC_Agent_Path::T_PathKnowledgeObjectByTypesVector& DEC_Agent_Path::GetPathKnowledgeObjects() const
{
    return pathKnowledgeObjects_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetCostOutsideOfAllObjects
// Created: NLD 2007-02-09
// -----------------------------------------------------------------------------
double DEC_Agent_Path::GetCostOutsideOfAllObjects() const
{
    return rCostOutsideOfAllObjects_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathKnowledgeAgents
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
const DEC_Agent_Path::T_PathKnowledgeAgentVector& DEC_Agent_Path::GetPathKnowledgeAgents() const
{
    return pathKnowledgeAgents_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathKnowledgePopulations
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
const DEC_Agent_Path::T_PathKnowledgePopulationVector& DEC_Agent_Path::GetPathKnowledgePopulations() const
{
    return pathKnowledgePopulations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::NeedRefine
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
bool DEC_Agent_Path::NeedRefine() const
{
    return bRefine_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::UseStrictClosest
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
bool DEC_Agent_Path::UseStrictClosest() const
{
    return ! automateFuseau_.IsNull();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetNextWaypoints
// Created: CMA 2012-02-20
// -----------------------------------------------------------------------------
const T_PointVector& DEC_Agent_Path::GetNextWaypoints() const
{
    return nextWaypoints_;
}
