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
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Terrain/PHY_RoleInterface_TerrainAnalysis.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Objects/FloodAttribute.h"
#include "Entities/Objects/FloodCapacity.h"
#include "Entities/Objects/BurnSurfaceCapacity.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_BurningCells.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Logger.h"

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: JDY 03-04-10
//-----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( const MIL_Agent_ABC& queryMaker, const T_PointVector& points, const DEC_PathType& pathType )
    : DEC_PathResult           ()
    , queryMaker_              ( queryMaker )
    , bRefine_                 ( queryMaker.GetType().GetUnitType().CanFly() && !queryMaker.IsAutonomous() )
    , vDirDanger_              ( queryMaker.GetOrderManager().GetDirDanger() )
    , unitSpeeds_              ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_               ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , rCostOutsideOfAllObjects_( 0. )
    , pathType_                ( pathType )
    , pathClass_               ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , bDecPointsInserted_      ( false )
{
    fuseau_= queryMaker.GetOrderManager().GetFuseau();
    automateFuseau_ = queryMaker.GetAutomate().GetOrderManager().GetFuseau();
    initialPathPoints_.reserve( 1 + points.size() );
    initialPathPoints_.push_back( queryMaker_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    std::copy( points.begin(), points.end(), std::back_inserter( initialPathPoints_ ) );
    Initialize( initialPathPoints_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: LDC 2009-06-18
// -----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( const MIL_Agent_ABC& queryMaker, std::vector< boost::shared_ptr< MT_Vector2D > >& points, const DEC_PathType& pathType )
    : DEC_PathResult           ()
    , queryMaker_              ( queryMaker )
    , bRefine_                 ( queryMaker.GetType().GetUnitType().CanFly() && !queryMaker.IsAutonomous() )
    , vDirDanger_              ( queryMaker.GetOrderManager().GetDirDanger() )
    , unitSpeeds_              ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_               ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , rCostOutsideOfAllObjects_( 0. )
    , pathType_                ( pathType )
    , pathClass_               ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , bDecPointsInserted_      ( false )
{
    fuseau_ = queryMaker.GetOrderManager().GetFuseau();
    automateFuseau_ = queryMaker.GetAutomate().GetOrderManager().GetFuseau();
    initialPathPoints_.reserve( 1 + points.size() );
    initialPathPoints_.push_back( queryMaker_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    for( std::vector< boost::shared_ptr< MT_Vector2D > >::const_iterator it = points.begin(); it != points.end(); ++it )
        initialPathPoints_.push_back( **it );
    Initialize( initialPathPoints_ );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( const MIL_Agent_ABC& queryMaker, const MT_Vector2D& vPosEnd, const DEC_PathType& pathType )
    : DEC_PathResult            ()
    , queryMaker_               ( queryMaker )
    , bRefine_                  ( queryMaker.GetType().GetUnitType().CanFly() && !queryMaker.IsAutonomous() )
    , vDirDanger_               ( queryMaker.GetOrderManager().GetDirDanger() )
    , unitSpeeds_               ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_                ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , rCostOutsideOfAllObjects_ ( 0. )
    , pathType_                 ( pathType )
    , pathClass_                ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , bDecPointsInserted_       ( false )
{
    fuseau_ = queryMaker.GetOrderManager().GetFuseau();
    automateFuseau_ = queryMaker.GetAutomate().GetOrderManager().GetFuseau();
    initialPathPoints_.reserve( 2 );
    initialPathPoints_.push_back( queryMaker_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    initialPathPoints_.push_back( vPosEnd );
    Initialize( initialPathPoints_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::DEC_Agent_Path
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( const MIL_Agent_ABC& queryMaker, const MT_Vector2D& vPosEnd, const DEC_PathType& pathType, bool loaded )
    : DEC_PathResult           ()
    , queryMaker_              ( queryMaker )
    , bRefine_                 ( queryMaker.GetType().GetUnitType().CanFly() && !queryMaker.IsAutonomous() )
    , vDirDanger_              ( queryMaker.GetOrderManager().GetDirDanger() )
    , unitSpeeds_              ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >(), loaded )
    , rMaxSlope_               ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , pathKnowledgeObjects_    ( )
    , rCostOutsideOfAllObjects_( 0. )
    , pathType_                ( pathType )
    , pathClass_               ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , bDecPointsInserted_      ( false )
{
    fuseau_ = queryMaker.GetOrderManager().GetFuseau();
    automateFuseau_ = queryMaker.GetAutomate().GetOrderManager().GetFuseau();
    initialPathPoints_.reserve( 2 );
    initialPathPoints_.push_back( queryMaker_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    initialPathPoints_.push_back( vPosEnd );
    Initialize( initialPathPoints_ );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: NLD 2005-06-30
// -----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( const DEC_Agent_Path& rhs )
    : DEC_PathResult           ()
    , queryMaker_              ( rhs.queryMaker_ )
    , bRefine_                 ( rhs.bRefine_ )
    , vDirDanger_              ( rhs.vDirDanger_ )
 //   , unitSpeeds_               ( rhs.unitSpeeds_ ) $$$ TODO
    , unitSpeeds_              ( queryMaker_.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_               ( rhs.rMaxSlope_ )
    , rCostOutsideOfAllObjects_( 0. )
    , pathType_                ( rhs.pathType_ )
    , pathClass_               ( rhs.pathClass_ )
    , bDecPointsInserted_      ( false )
    , initialPathPoints_       ( rhs.initialPathPoints_ )
{
    fuseau_ = rhs.fuseau_;
    automateFuseau_ = rhs.automateFuseau_;
    Initialize( initialPathPoints_ );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Agent_Path::~DEC_Agent_Path()
{
    for( CIT_PathKnowledgeObjectByTypesVector itTypes = pathKnowledgeObjects_.begin(); itTypes != pathKnowledgeObjects_.end(); ++itTypes )
        for( CIT_PathKnowledgeObjectVector it = itTypes->begin(); it != itTypes->end(); ++it )
            delete *it;
    for( IT_PathPointList it = resultList_.begin(); it!= resultList_.end(); it++ )
        if( ( *it )->GetType() != DEC_PathPoint::eTypePointPath )
            ( *it )->RemoveFromDIA( *it );
    fuseau_.Reset();
    automateFuseau_.Reset();
}

 // -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::Initialize
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Agent_Path::Initialize( const T_PointVector& points )
{
    InitializePathKnowledges( points );
    if( points.empty() )
        throw std::runtime_error( "List of points is empty" );
    const MT_Vector2D* pLastPoint = 0;
    for( CIT_PointVector itPoint = points.begin(); itPoint != points.end(); ++itPoint )
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
    return queryMaker_.GetRole< PHY_RoleInterface_Composantes >().GetMajorComponentWeight();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InitializePathKnowledges
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_Agent_Path::InitializePathKnowledges( const T_PointVector& pathPoints )
{
    if( pathClass_.AvoidEnemies() )
    {
        const T_KnowledgeAgentVector& enemies = queryMaker_.GetKnowledgeGroup().GetKnowledge().GetEnemies();
        for( CIT_KnowledgeAgentVector itKnowledgeAgent = enemies.begin(); itKnowledgeAgent != enemies.end(); ++itKnowledgeAgent )
        {
            const DEC_Knowledge_Agent& knowledge = **itKnowledgeAgent;
            if( knowledge.IsValid() && fuseau_.IsInside( knowledge.GetPosition() ) )
                pathKnowledgeAgents_.push_back( DEC_Path_KnowledgeAgent( pathClass_, knowledge, queryMaker_ ) );
        }
    }

    // Objects
    if( pathClass_.AvoidObjects() )
    {
        T_KnowledgeObjectVector knowledgesObject;
        queryMaker_.GetArmy().GetKnowledge().GetObjects( knowledgesObject );
        for( CIT_KnowledgeObjectVector itKnowledgeObject = knowledgesObject.begin(); itKnowledgeObject != knowledgesObject.end(); ++itKnowledgeObject )
        {
            const DEC_Knowledge_Object& knowledge = **itKnowledgeObject;
            if( knowledge.CanCollideWith( queryMaker_ ) )
            {
                if( pathKnowledgeObjects_.size() <= knowledge.GetType().GetID() )
                    pathKnowledgeObjects_.resize( knowledge.GetType().GetID() + 1 );
                if( pathKnowledgeObjects_.size() <= knowledge.GetType().GetID() )
                    throw std::runtime_error( "Size of path knowledge objects list is invalid" );

                T_PathKnowledgeObjectVector& pathKnowledges = pathKnowledgeObjects_[ knowledge.GetType().GetID() ];
                if( knowledge.GetType().GetCapacity< FloodCapacity >() )
                    pathKnowledges.push_back( new DEC_Path_KnowledgeObjectFlood( queryMaker_.GetType().GetUnitType().GetCrossingHeight(), knowledge ) );
                else if( knowledge.GetType().GetCapacity< BurnSurfaceCapacity >() )
                    pathKnowledges.push_back( new DEC_Path_KnowledgeObjectBurnSurface( knowledge ) );
                else
                    pathKnowledges.push_back( new DEC_Path_KnowledgeObject( pathClass_, knowledge ) );
                if( pathKnowledges.size() == 1 && pathKnowledges.front()->GetCostOut() > 0 )
                    rCostOutsideOfAllObjects_ += pathKnowledges.front()->GetCostOut();
            }
        }
    }

    // Populations
    if( pathClass_.HandlePopulations() )
    {
        T_KnowledgePopulationVector knowledgesPopulation;
        queryMaker_.GetKnowledgeGroup().GetKnowledge().GetPopulations( knowledgesPopulation );
        pathKnowledgePopulations_.reserve( knowledgesPopulation.size() );
        for( CIT_KnowledgePopulationVector it = knowledgesPopulation.begin(); it != knowledgesPopulation.end(); ++it )
            pathKnowledgePopulations_.push_back( DEC_Path_KnowledgePopulation( pathClass_, **it, !queryMaker_.GetType().IsTerrorist() ) );
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
DEC_Agent_Path::IT_PathPointList DEC_Agent_Path::GetPreviousPathPointOnDifferentLocation( IT_PathPointList itCurrent )
{
    if( itCurrent == resultList_.end() )
        throw std::runtime_error( "Current path point is invalid" );
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
void DEC_Agent_Path::InsertPointAvant( const boost::shared_ptr< DEC_PathPoint > spottedPathPoint, IT_PathPointList itCurrent )
{
    double rDistanceLeft = spottedPathPoint->GetTypePoint() == DEC_Rep_PathPoint::eTypePointLima ?
                             queryMaker_.GetType().GetDistanceAvantLima() :
                             queryMaker_.GetType().GetDistanceAvantPoint( spottedPathPoint->GetTypeTerrain() );
    while( true )
    {
        const MT_Vector2D& vCurrentPos = ( *itCurrent )->GetPos();
        if( itCurrent == resultList_.begin() )
        {
            boost::shared_ptr< DEC_PathPoint > pNewPoint( new DEC_Rep_PathPoint_Front( vCurrentPos, spottedPathPoint ) );
            resultList_.insert( ++itCurrent, pNewPoint );
            break;
        }

        // remise en position des itérateurs
        IT_PathPointList itPrev = GetPreviousPathPointOnDifferentLocation( itCurrent );
        itCurrent = itPrev;
        ++itCurrent;
        if( ( *itCurrent )->GetPos() != vCurrentPos )
            throw std::runtime_error( "Current position is invalid" );

        // calcul de la distance "mangée" par le parcours de ce segment
        const MT_Vector2D& vPreviousPos = ( *itPrev )->GetPos();
        MT_Vector2D vTmp = vPreviousPos - vCurrentPos;
        const double vTmpMag = vTmp.Magnitude();

        rDistanceLeft -= vTmpMag;
        if( rDistanceLeft == 0. )
        {
            // Positionnement du point avant au même endroit qu'un autre point
            boost::shared_ptr< DEC_PathPoint > pNewPoint( new DEC_Rep_PathPoint_Front( vPreviousPos, spottedPathPoint ) );
            resultList_.insert( itPrev, pNewPoint );
            break;
        }
        else if( rDistanceLeft < 0. )
        {
            // Positionnement du point avant
            vTmp /= vTmpMag;
            vTmp *= rDistanceLeft;
            vTmp += vPreviousPos;
            boost::shared_ptr< DEC_PathPoint > pNewPoint( new DEC_Rep_PathPoint_Front( vTmp, spottedPathPoint ) );
            resultList_.insert( itCurrent, pNewPoint );
            break;
        }
        // tant pis, ça sera peut-être pour le prochain segment
        itCurrent = itPrev;
    }
}

#pragma warning( pop )

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAvant
// Created: JVT 02-12-04
//----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAvant( const boost::shared_ptr< DEC_PathPoint > spottedPathPoint, IT_PathPointList itCurrent, double& rDistSinceLastPointAvant )
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
bool DEC_Agent_Path::InsertPoint( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, IT_PathPointList itCurrent, double& rDistSinceLastPoint )
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
void DEC_Agent_Path::InsertPointAndPointAvant( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, IT_PathPointList itCurrent, double& rDistSinceLastPoint, double& rDistSinceLastPointAvant )
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
    for( IT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
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
            InsertPointAndPointAvant( boost::shared_ptr< DEC_PathPoint >( new DEC_Rep_PathPoint_Special( ( *itPoint )->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierVillage, TerrainData::Urban() ) ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Urban
        else if( IsPointAvantOut( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Urban() ) )
            InsertPoint( boost::shared_ptr< DEC_PathPoint >( new DEC_Rep_PathPoint( ( *itPoint )->GetPos(), DEC_Rep_PathPoint::eTypePointCCT, TerrainData::Urban() ) ), itPoint, rDistSinceLastPoint );

        // Forest
        else if( IsPointAvant( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Forest() ) )
            InsertPointAndPointAvant( boost::shared_ptr< DEC_PathPoint >( new DEC_Rep_PathPoint( ( *itPoint )->GetPos(), DEC_Rep_PathPoint::eTypePointCCT, TerrainData::Forest() ) ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Cross roads
        else if( current.GetObjectTypes().ContainsOne( TerrainData::Crossroad() ) )
            InsertPointAndPointAvant( boost::shared_ptr< DEC_PathPoint >( new DEC_Rep_PathPoint_Special( ( *itPoint )->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierCarrefour, TerrainData::Crossroad() ) ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Pont
        else if( IsPointAvantIn( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Bridge() )
                || ( !current.GetObjectTypes().ContainsOne( TerrainData::SmallRiver() ) && current.GetObjectTypes().ContainsOne( TerrainData::Bridge() ) && !nObjectTypesBefore.ContainsOne( TerrainData::Bridge() ) && !nObjectTypesToNextPoint.ContainsOne( TerrainData::Bridge() ) )
                )
            InsertPointAndPointAvant( boost::shared_ptr< DEC_PathPoint >( new DEC_Rep_PathPoint_Special( ( *itPoint )->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierPont, TerrainData::Bridge() ) ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

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
    for( IT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        boost::shared_ptr< DEC_PathPoint > pCurrentPoint = *itPoint;
        if( !pCurrentPoint )
            throw std::runtime_error( "Current point is invalid" );
        if( pLastPoint.get() && ( pCurrentPoint->GetPos() != pLastPoint->GetPos() ) )
        {
            MT_Line segment( pLastPoint->GetPos(), pCurrentPoint->GetPos() );
            MT_Vector2D posIntersect;
            if( lima.Intersect2D( segment, posIntersect ) )
            {
                for( MIL_LimaOrder::CIT_LimaFunctions itFunction = lima.GetFunctions().begin(); itFunction != lima.GetFunctions().end(); ++itFunction )
                {
                    boost::shared_ptr< DEC_PathPoint > pPoint( new DEC_Rep_PathPoint_Lima( posIntersect, TerrainData(), lima.GetID(), **itFunction ) );
                    IT_PathPointList itTmp = resultList_.insert( itPoint, pPoint );
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
    for( CIT_LimaVector it = limas.begin(); it != limas.end(); ++it )
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

    CIT_PathPointList itTempResult = resultList_.begin();
    for( CIT_PointVector itPoint = initialPathPoints_.begin(); itPoint != initialPathPoints_.end(); ++itPoint )
    {
        MT_Vector2D point( *itPoint );
        for( CIT_PathPointList itResult = itTempResult; itResult != resultList_.end(); ++itResult )
        {
            DEC_PathPoint& result = **itResult;
            if( point.SquareDistance( result.GetPos() ) < precision_ )
            {
                followingPathPoints_.push_back( std::make_pair( point, itResult ) );
                itTempResult = ++itResult;
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::CleanAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void DEC_Agent_Path::CleanAfterComputation()
{
    DEC_Path_ABC::CleanAfterComputation();
    for( CIT_PathKnowledgeObjectByTypesVector itTypes = pathKnowledgeObjects_.begin(); itTypes != pathKnowledgeObjects_.end(); ++itTypes )
        for( CIT_PathKnowledgeObjectVector it = itTypes->begin(); it != itTypes->end(); ++it )
            delete *it;
    pathKnowledgeObjects_.clear();
    pathKnowledgeAgents_.clear();
    pathKnowledgePopulations_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::Execute
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
void DEC_Agent_Path::Execute( TerrainPathfinder& pathfind )
{
    if( MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        MT_LOG_MESSAGE_MSG( "DEC_Agent_Path::Compute: " << this << " : computation begin" );
        MT_LOG_MESSAGE_MSG( "   Thread    : " << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() );
        MT_LOG_MESSAGE_MSG( "   Agent     : " << queryMaker_.GetID() );
        MT_LOG_MESSAGE_MSG( "   Path type : " << pathType_.GetName().c_str() );
        MT_LOG_MESSAGE_MSG( GetPathAsString() );
        profiler_.Start();
    }
    if( !resultList_.empty() )
        throw std::runtime_error( "List of path points is not empty before running pathfind" );

    if( !IsDestinationTrafficable() )
    {
        queryMaker_.GetRole< moving::PHY_RoleAction_Moving >().SendRC( MIL_Report::eReport_DifficultTerrain );
        Cancel();
    }
    DEC_Path_ABC::Execute( pathfind );
    DEC_PathResult::E_State nPathState = GetState();
    if( nPathState == DEC_Path_ABC::eImpossible )
        queryMaker_.GetRole< moving::PHY_RoleAction_Moving >().SendRC( MIL_Report::eReport_DifficultTerrain );

#ifndef NDEBUG
    for( CIT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); )
    {
        DEC_PathPoint& point = **itPoint;

//        if( itPoint != resultList_.begin() && !unitSpeeds_.IsPassable( point.GetObjectTypes() ) )
//            throw std::runtime_error( "Unit max speed is not positive for a given object" );

        ++itPoint;
        if( itPoint != resultList_.end() && unitSpeeds_.GetMaxSpeed( point.GetObjectTypesToNextPoint() ) <= 0 )
            throw std::runtime_error( "Unit max speed is not positive for a given object" );
    }
#endif

    if( MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
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
    return analysis.CanMoveOnUrbanBlock( initialPathPoints_ ) &&
           analysis.CanMoveOnBurningCells( initialPathPoints_ ) &&
           analysis.CanMoveOnKnowledgeObject( initialPathPoints_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPointOnPathCloseTo
// Created: CMA 2011-11-10
// -----------------------------------------------------------------------------
MT_Vector2D DEC_Agent_Path::GetPointOnPathCloseTo( const MT_Vector2D& posToTest, const MT_Vector2D& lastJoiningPoint, bool forceNextPoint )
{
    if( followingPathPoints_.size() > 1 )
        return DEC_PathResult::GetPointOnPathCloseTo( posToTest, followingPathPoints_, lastJoiningPoint, forceNextPoint );
    else if( followingPathPoints_.size() == 1 )
        return followingPathPoints_.front().first;
    return posToTest;
}
