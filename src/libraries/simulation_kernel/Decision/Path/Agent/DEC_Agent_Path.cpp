//*****************************************************************************
// 
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Agent_Path.cpp $
// $Author: Age $
// $Modtime: 16/06/05 15:23 $
// $Revision: 28 $
// $Workfile: DEC_Agent_Path.cpp $
// 
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Agent_Path.h"

#include "DEC_Agent_PathSection.h"
#include "DEC_Agent_PathClass.h"
#include "Decision/Path/DEC_PathType.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Decision/Path/DEC_PathFind_Manager.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Front.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Special.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Lima.h"
#include "Entities/Orders/MIL_LimaOrder.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Tools/MIL_Tools.h"

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::Initialize
// Created: JDY 03-04-10
//-----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( const MIL_AgentPion& queryMaker, const T_PointVector& points, const DEC_PathType& pathType )
    : DEC_PathResult           ()
    , queryMaker_              ( queryMaker )
    , bRefine_                 ( queryMaker.CanFly() && !queryMaker.IsAutonomous() )
    , fuseau_                  () //$$$ Debile
    , automateFuseau_          () //$$$ Debile
    , vDirDanger_              ( queryMaker.GetOrderManager().GetDirDanger() )
    , unitSpeeds_              ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_               ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , pathKnowledgeAgents_     ()
    , pathKnowledgeObjects_    ( )
    , rCostOutsideOfAllObjects_( 0. )
    , pathKnowledgePopulations_()
    , pathType_                ( pathType )
    , pathClass_               ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , bDecPointsInserted_      ( false )
    , pathPoints_              ()
{
    fuseau_         = queryMaker.GetOrderManager().GetFuseau();
    automateFuseau_ = queryMaker.GetAutomate().GetOrderManager().GetFuseau();
    
    pathPoints_.reserve( 1 + points.size() );
    pathPoints_.push_back( queryMaker_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    std::copy( points.begin(), points.end(), std::back_inserter( pathPoints_ ) );
    Initialize( pathPoints_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: LDC 2009-06-18
// -----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( const MIL_AgentPion& queryMaker, std::vector< boost::shared_ptr< MT_Vector2D > >& points , const DEC_PathType& pathType )
    : DEC_PathResult           ()
    , queryMaker_              ( queryMaker )
    , bRefine_                 ( queryMaker.CanFly() && !queryMaker.IsAutonomous() )
    , fuseau_                  () //$$$ Debile
    , automateFuseau_          () //$$$ Debile
    , vDirDanger_              ( queryMaker.GetOrderManager().GetDirDanger() )
    , unitSpeeds_              ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_               ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , pathKnowledgeAgents_     ()
    , pathKnowledgeObjects_    ( )
    , rCostOutsideOfAllObjects_( 0. )
    , pathKnowledgePopulations_()
    , pathType_                ( pathType )
    , pathClass_               ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , bDecPointsInserted_      ( false )
    , pathPoints_              ()
{
    fuseau_         = queryMaker.GetOrderManager().GetFuseau();
    automateFuseau_ = queryMaker.GetAutomate().GetOrderManager().GetFuseau();
    
    pathPoints_.reserve( 1 + points.size() );
    pathPoints_.push_back( queryMaker_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    for( std::vector< boost::shared_ptr< MT_Vector2D > >::const_iterator it = points.begin(); it != points.end(); ++it )
        pathPoints_.push_back( **it );
    Initialize( pathPoints_ );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( const MIL_AgentPion& queryMaker, const MT_Vector2D& vPosEnd, const DEC_PathType& pathType ) 
    : DEC_PathResult            ()
    , queryMaker_               ( queryMaker )
    , bRefine_                  ( queryMaker.CanFly() && !queryMaker.IsAutonomous() )
    , fuseau_                   () //$$$ Debile
    , automateFuseau_           () //$$$ Debile
    , vDirDanger_               ( queryMaker.GetOrderManager().GetDirDanger() )
    , unitSpeeds_               ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_                ( queryMaker.GetRole< moving::PHY_RoleAction_Moving >().GetMaxSlope() )
    , pathKnowledgeAgents_      ()
    , pathKnowledgeObjects_     ( )
    , rCostOutsideOfAllObjects_( 0. )
    , pathKnowledgePopulations_ ()
    , pathType_                 ( pathType )
    , pathClass_                ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , bDecPointsInserted_       ( false )
    , pathPoints_               ()
{
    fuseau_         = queryMaker.GetOrderManager().GetFuseau();
    automateFuseau_ = queryMaker.GetAutomate().GetOrderManager().GetFuseau();

    pathPoints_.reserve( 2 );
    pathPoints_.push_back( queryMaker_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    pathPoints_.push_back( vPosEnd );
    Initialize( pathPoints_ );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: NLD 2005-06-30
// -----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( const DEC_Agent_Path& rhs )
    : DEC_PathResult           ()    
    , queryMaker_              ( rhs.queryMaker_ )
    , bRefine_                 ( rhs.bRefine_ )
    , fuseau_                  () //$$$ Debile
    , automateFuseau_          () //$$$ Debile
    , vDirDanger_              ( rhs.vDirDanger_ )
 //   , unitSpeeds_               ( rhs.unitSpeeds_ ) $$$ TODO
    , unitSpeeds_              ( queryMaker_.GetRole< moving::PHY_RoleAction_Moving >() )
    , rMaxSlope_               ( rhs.rMaxSlope_ )
    , pathKnowledgeAgents_     ()
    , pathKnowledgeObjects_    ( )
    , rCostOutsideOfAllObjects_( 0. )
    , pathKnowledgePopulations_()
    , pathType_                ( rhs.pathType_ )
    , pathClass_               ( rhs.pathClass_ )
    , bDecPointsInserted_      ( false )
    , pathPoints_              ( rhs.pathPoints_ )
{
    fuseau_         = rhs.fuseau_;
    automateFuseau_ = rhs.automateFuseau_;

    Initialize( pathPoints_ );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Agent_Path::~DEC_Agent_Path()
{
    fuseau_        .Reset();
    automateFuseau_.Reset();
}

//=============================================================================
// INIT
//=============================================================================

 // -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::Initialize
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Agent_Path::Initialize( const T_PointVector& points )
{
    InitializePathKnowledges( points );

//    if( ! fuseau_.IsNull() && !fuseau_.IsInside( vStartPoint ) )
//    {
//        MT_Vector2D vEntryPoint;
//        fuseau_.ComputeEntryPoint( vStartPoint, vEntryPoint );
//        points.insert( points.begin(), vEntryPoint );
//    }

    assert( !points.empty() );
    const MT_Vector2D* pLastPoint = 0;
    for( CIT_PointVector itPoint = points.begin(); itPoint != points.end(); ++itPoint )
    {
        if( pLastPoint )
        {
            DEC_Agent_PathSection* pSection = new DEC_Agent_PathSection( *this, *pLastPoint, *itPoint );
            RegisterPathSection( *pSection );
        }
        pLastPoint = &*itPoint;
    }
}

// -----------------------------------------------------------------------------
// Name: IsObjectInsidePathPoint
// Created: NLD 2005-07-21
// -----------------------------------------------------------------------------
static
bool IsObjectInsidePathPoint( const DEC_Knowledge_Object& knowledge, const T_PointVector& pathPoints )
{
    const TER_Localisation& loc = knowledge.GetLocalisation();
    for( CIT_PointVector it = pathPoints.begin(); it != pathPoints.end(); ++it )
    {
        if( loc.IsInside( *it ) )
            return true;
    }
    return false;
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
            if ( knowledge.CanCollideWith( queryMaker_ ) && !IsObjectInsidePathPoint( knowledge, pathPoints ) ) //$$$ BOF
            {
                if ( pathKnowledgeObjects_.size() <= knowledge.GetType().GetID() )
                    pathKnowledgeObjects_.resize( knowledge.GetType().GetID() + 1 );
                assert( pathKnowledgeObjects_.size() > knowledge.GetType().GetID() );
                
                T_PathKnowledgeObjectVector& pathKnowledges = pathKnowledgeObjects_[ knowledge.GetType().GetID() ];
                pathKnowledges.push_back( DEC_Path_KnowledgeObject( pathClass_, knowledge ) );
                if( pathKnowledges.size() == 1 && pathKnowledges.front().GetCostOut() > 0 )
                    rCostOutsideOfAllObjects_ += pathKnowledges.front().GetCostOut();               
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

// =============================================================================
// DEC POINTS
// =============================================================================

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
    assert( itCurrent != resultList_.end() );

    const MT_Vector2D& vPosition = (*itCurrent)->GetPos();
    
    while ( itCurrent != resultList_.begin() && (*itCurrent)->GetPos() == vPosition )
        --itCurrent;
        
    return itCurrent;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAvant
// Created: NLD 2005-08-11
// -----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAvant( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent )
{
    MT_Float rDistanceLeft = spottedPathPoint.GetTypePoint() == DEC_Rep_PathPoint::eTypePointLima ?
                             queryMaker_.GetType().GetDistanceAvantLima() :
                             queryMaker_.GetType().GetDistanceAvantPoint( spottedPathPoint.GetTypeTerrain() );

    while( true )
    {
        const MT_Vector2D& vCurrentPos = (*itCurrent)->GetPos();

        if( itCurrent == resultList_.begin() )
        {
            DEC_Rep_PathPoint_Front* pNewPoint = new DEC_Rep_PathPoint_Front( vCurrentPos, spottedPathPoint );
            resultList_.insert( ++itCurrent, pNewPoint );
            break;
        }

        // remise en position des itérateurs
        IT_PathPointList itPrev = GetPreviousPathPointOnDifferentLocation( itCurrent );

        itCurrent = itPrev; 
        ++itCurrent; 
        assert( (*itCurrent)->GetPos() == vCurrentPos );
        
        // calcul de la distance "mangée" par le parcours de ce segment
        const MT_Vector2D& vPreviousPos = (*itPrev)->GetPos();
              MT_Vector2D  vTmp         = vPreviousPos - vCurrentPos;
        const MT_Float     vTmpMag      = vTmp.Magnitude();
        
        rDistanceLeft -= vTmpMag;
        if( rDistanceLeft == 0. )
        {
            // Positionnement du point avant au même endroit qu'un autre point
            DEC_Rep_PathPoint_Front* pNewPoint = new DEC_Rep_PathPoint_Front( vPreviousPos, spottedPathPoint );

            resultList_.insert( itPrev, pNewPoint );
            break;
        }
        else if( rDistanceLeft < 0. )
        {
            // Positionnement du point avant 
            vTmp /= vTmpMag;
            vTmp *= rDistanceLeft;
            vTmp += vPreviousPos;

            DEC_Rep_PathPoint_Front* pNewPoint = new DEC_Rep_PathPoint_Front( vTmp, spottedPathPoint );

            resultList_.insert( itCurrent, pNewPoint );
            break;
        }

        // tant pis, ça sera peut-être pour le prochain segment
        itCurrent = itPrev;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAvant
// Created: JVT 02-12-04
//----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAvant( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPointAvant )
{
    static MT_Float rDist = 2000.;
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
bool DEC_Agent_Path::InsertPoint( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPoint )
{
    static MT_Float rDist = 500.;
    if( rDistSinceLastPoint > rDist )
    {
        resultList_.insert( itCurrent, &spottedPathPoint );
        rDistSinceLastPoint = 0.;
        return true;
    }
    else 
    {
        delete &spottedPathPoint;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::Shit
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAndPointAvant( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPoint, MT_Float& rDistSinceLastPointAvant )
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
    MT_Float rDistSinceLastPointAvant = std::numeric_limits< MT_Float >::max();
    MT_Float rDistSinceLastPoint      = std::numeric_limits< MT_Float >::max();

    TerrainData nObjectTypesBefore;

    DEC_PathPoint* pPrevPoint = 0;

    for( IT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        DEC_PathPoint& current = **itPoint;

        if( pPrevPoint )
        {
            if( rDistSinceLastPointAvant != std::numeric_limits< MT_Float >::max() )
                rDistSinceLastPointAvant += pPrevPoint->GetPos().Distance( current.GetPos() );
            if( rDistSinceLastPoint != std::numeric_limits< MT_Float >::max() )
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
            pPrevPoint         = &current;
            nObjectTypesBefore = nObjectTypesToNextPoint;
            continue;
        }
        
        // Village
        if( IsPointAvantIn( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Urban() ) )
            InsertPointAndPointAvant( *new DEC_Rep_PathPoint_Special( (*itPoint)->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierVillage, TerrainData::Urban() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        else if( IsPointAvantOut( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Urban() ) )
            InsertPoint( *new DEC_Rep_PathPoint( (*itPoint)->GetPos(), DEC_Rep_PathPoint::eTypePointCCT, TerrainData::Urban() ), itPoint, rDistSinceLastPoint );

        // Forest
        else if( IsPointAvant( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Forest() ) )
            InsertPointAndPointAvant( *new DEC_Rep_PathPoint( (*itPoint)->GetPos(), DEC_Rep_PathPoint::eTypePointCCT, TerrainData::Forest() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Cross roads
        else if( current.GetObjectTypes().ContainsOne( TerrainData::Crossroad() ) )
            InsertPointAndPointAvant( *new DEC_Rep_PathPoint_Special( (*itPoint)->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierCarrefour, TerrainData::Crossroad() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Pont
        else if( IsPointAvantIn( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Bridge() ) 
                || ( !current.GetObjectTypes().ContainsOne( TerrainData::SmallRiver() ) && current.GetObjectTypes().ContainsOne( TerrainData::Bridge() ) && !nObjectTypesBefore.ContainsOne( TerrainData::Bridge() ) && !nObjectTypesToNextPoint.ContainsOne( TerrainData::Bridge() ) )
                )
            InsertPointAndPointAvant( *new DEC_Rep_PathPoint_Special( (*itPoint)->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierPont, TerrainData::Bridge() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

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
    DEC_PathPoint* pLastPoint = 0;
    for( IT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        DEC_PathPoint* pCurrentPoint = *itPoint;
        assert ( pCurrentPoint );

        if( pLastPoint && ( pCurrentPoint->GetPos() != pLastPoint->GetPos() ) )
        {
            MT_Line segment( pLastPoint->GetPos(), pCurrentPoint->GetPos() );
            MT_Vector2D posIntersect;
            if( lima.Intersect2D( segment, posIntersect ) )
            {   
                for( MIL_LimaOrder::CIT_LimaFunctions itFunction = lima.GetFunctions().begin(); itFunction != lima.GetFunctions().end(); ++itFunction )
                {
                    DEC_Rep_PathPoint* pPoint = new DEC_Rep_PathPoint_Lima( posIntersect, TerrainData(), lima.GetID(), **itFunction );
                    IT_PathPointList itTmp = resultList_.insert( itPoint, pPoint );
                    InsertPointAvant( *pPoint, itTmp );
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
}

// =============================================================================
// PATH CALCULATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::CleanAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void DEC_Agent_Path::CleanAfterComputation()
{
    DEC_Path_ABC::CleanAfterComputation(); 
    pathKnowledgeAgents_     .clear();
    pathKnowledgeObjects_    .clear();
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

    assert( resultList_.empty() );

    DEC_Path_ABC::Execute( pathfind );

#ifndef NDEBUG
    for( CIT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); )
    {
        DEC_PathPoint& point = **itPoint;

//        if( itPoint != resultList_.begin() )
//            assert( unitSpeeds_.IsPassable( point.GetObjectTypes() ) );
        ++itPoint;
        if( itPoint != resultList_.end() )
            assert( unitSpeeds_.GetMaxSpeed( point.GetObjectTypesToNextPoint() ) > 0 );
    }
#endif

    if( MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        double rComputationTime = profiler_.Stop();

        std::stringstream stream;
        if ( ! resultList_.empty() )
            stream << "[" << resultList_.front()->GetPos() << "] -> [" << resultList_.back()->GetPos() << "]";
        MT_LOG_MESSAGE_MSG( "DEC_Agent_Path::Compute: " << this << 
                            ", Thread : "  << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() <<
                            ", Time : " << rComputationTime << 
                            ", State : " << GetStateAsString() <<
                            ", Result : " << stream.str() );
    }
}


