// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Agent_Path.h"
#include "Agent_PathSection.h"
#include "Agent_PathClass.h"
#include "PathPoint.h"
#include "PathType.h"
#include "Rep_PathPoint_Front.h"
#include "Rep_PathPoint_Special.h"
#include "Rep_PathPoint_Lima.h"
#include "simulation_kernel/Entities/Orders/MIL_Report.h" // $$$$ MCO : for enums
#include "MT_Tools/MT_Line.h"
#include <tools/Exception.h>
#include "wrapper/Event.h"
#include "wrapper/View.h"
#include "wrapper/Hook.h"
#include <module_api/Log.h>
#include <boost/foreach.hpp>
#include <sstream>

using namespace sword;
using namespace sword::movement;

DECLARE_HOOK( GetDistanceAvantLima, double, ( const SWORD_Model* entity ) )
DECLARE_HOOK( GetDistanceAvantPoint, double, ( const SWORD_Model* entity, const TerrainData& data ) )
DECLARE_HOOK( GetLimaOrders, void, ( const MIL_LimaOrder& lima, void(*visitor)( unsigned int limaID, int functionID, void* userData ), void* userData ) )
DECLARE_HOOK( GetLimas, void, ( const SWORD_Model* entity, void(*visitor)( const MIL_LimaOrder& lima, void* userData ), void* userData ) ) // queryMaker_.GetOrderManager().GetLimas()
DECLARE_HOOK( GetPathfindCurrentThread, int, () )
DECLARE_HOOK( IsDestinationTrafficable, bool, ( const SWORD_Model* entity, const MT_Vector2D* first, size_t size ) )
DECLARE_HOOK( IsNullAutomateFuseau, bool, ( const SWORD_Model* entity ) )
DECLARE_HOOK( LimaIntersect2D, bool, ( const MIL_LimaOrder& lima, const MT_Line& line, MT_Vector2D& intersection ) )
DECLARE_HOOK( UsePathDebug, bool, () )
DECLARE_HOOK( StartComputePathfind, void, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
DECLARE_HOOK( VisitKnowledgeObjects, void, ( const SWORD_Model* entity, bool(*visitor)( const DEC_Knowledge_Object* object, void* userData ), void* userData ) )
DECLARE_HOOK( CanMoveOn, bool, ( const SWORD_Model* entity, const MT_Vector2D& point ) )

//-----------------------------------------------------------------------------
// Name: Agent_Path constructor
// Created: JDY 03-04-10
//-----------------------------------------------------------------------------
Agent_Path::Agent_Path( const wrapper::View& entity, const T_PointVector& points, const PathType& pathType )
    : PathResult( pathType )
    , entity_                  ( entity )
    , unitSpeeds_              ( entity )
    , pathClass_               ( Agent_PathClass::GetPathClass( pathType, entity ) )
    , bDecPointsInserted_      ( false )
{
    initialWaypoints_.reserve( 1 + points.size() );
    nextWaypoints_.reserve( points.size() );
    initialWaypoints_.push_back( MT_Vector2D( entity[ "movement/position/x" ], entity[ "movement/position/y" ] ) );
    std::copy( points.begin(), points.end(), std::back_inserter( initialWaypoints_ ) );
    std::copy( points.begin(), points.end(), std::back_inserter( nextWaypoints_ ) );
    Initialize( initialWaypoints_ );
}

// -----------------------------------------------------------------------------
// Name: Agent_Path constructor
// Created: LDC 2009-06-18
// -----------------------------------------------------------------------------
Agent_Path::Agent_Path( const wrapper::View& entity, std::vector< boost::shared_ptr< MT_Vector2D > >& points, const PathType& pathType )
    : PathResult               ( pathType )
    , entity_                  ( entity )
    , unitSpeeds_              ( entity )
    , pathClass_               ( Agent_PathClass::GetPathClass( pathType, entity ) )
    , bDecPointsInserted_      ( false )
{
    initialWaypoints_.reserve( 1 + points.size() );
    nextWaypoints_.reserve( points.size() );
    initialWaypoints_.push_back( MT_Vector2D( entity[ "movement/position/x" ], entity[ "movement/position/y" ] ) );
    for( std::vector< boost::shared_ptr< MT_Vector2D > >::const_iterator it = points.begin(); it != points.end(); ++it )
    {
        initialWaypoints_.push_back( **it );
        nextWaypoints_.push_back( **it );
    }
    Initialize( initialWaypoints_ );
}

//-----------------------------------------------------------------------------
// Name: Agent_Path constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
Agent_Path::Agent_Path( const wrapper::View& entity, const MT_Vector2D& vPosEnd, const PathType& pathType )
    : PathResult         ( pathType )
    , entity_            ( entity )
    , unitSpeeds_        ( entity )
    , pathClass_         ( Agent_PathClass::GetPathClass( pathType, entity ) )
    , bDecPointsInserted_( false )
{
    initialWaypoints_.reserve( 2 );
    nextWaypoints_.reserve( 1 );
    initialWaypoints_.push_back( MT_Vector2D( entity[ "movement/position/x" ], entity[ "movement/position/y" ] ) );
    initialWaypoints_.push_back( vPosEnd );
    nextWaypoints_.push_back( vPosEnd );
    Initialize( initialWaypoints_ );
}

//-----------------------------------------------------------------------------
// Name: Agent_Path destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
Agent_Path::~Agent_Path()
{
    for( IT_PathPointList it = resultList_.begin(); it!= resultList_.end(); it++ )
        if( ( *it )->GetType() != PathPoint::eTypePointPath )
            ( *it )->RemoveFromDIA( *it );
}

 // -----------------------------------------------------------------------------
// Name: Agent_Path::Initialize
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void Agent_Path::Initialize( const T_PointVector& points )
{
    if( points.empty() )
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Initializing empty agent path" );
    const MT_Vector2D* pLastPoint = 0;
    for( CIT_PointVector itPoint = points.begin(); itPoint != points.end(); ++itPoint )
    {
        if( pLastPoint )
        {
            Agent_PathSection* pSection = new Agent_PathSection( entity_, *this, *pLastPoint, *itPoint );
            RegisterPathSection( *pSection );
        }
        pLastPoint = &( *itPoint );
    }
}

//-----------------------------------------------------------------------------
// Name: Agent_Path::IsPointAvantIn
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool Agent_Path::IsPointAvantIn( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    // Test : entrée dans environnement
    return !( nObjectTypesBefore.ContainsOne( nTypeTerrain ) ) && ( nObjectTypesToNextPoint.ContainsOne( nTypeTerrain ) );
}

//-----------------------------------------------------------------------------
// Name: Agent_Path::IsPointAvantOut
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool Agent_Path::IsPointAvantOut( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    // Test : sortie d'environnement
    return ( nObjectTypesBefore.ContainsOne( nTypeTerrain ) ) && !( nObjectTypesToNextPoint.ContainsOne(  nTypeTerrain ) );
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::IsPointAvant
// Created: AGE 2005-02-04
// -----------------------------------------------------------------------------
bool Agent_Path::IsPointAvant( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    return IsPointAvantIn ( nObjectTypesBefore, nObjectTypesToNextPoint, nTypeTerrain )
        || IsPointAvantOut( nObjectTypesBefore, nObjectTypesToNextPoint, nTypeTerrain );
}

//-----------------------------------------------------------------------------
// Name: Agent_Path::GetPreviousWaypointOnDifferentLocation
// Created: JVT 2005-07-08
// -----------------------------------------------------------------------------
Agent_Path::IT_PathPointList Agent_Path::GetPreviousWaypointOnDifferentLocation( IT_PathPointList itCurrent )
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
// Name: Agent_Path::InsertPointAvant
// Created: NLD 2005-08-11
// -----------------------------------------------------------------------------
void Agent_Path::InsertPointAvant( const boost::shared_ptr< PathPoint > spottedWaypoint, IT_PathPointList itCurrent )
{
    double rDistanceLeft = spottedWaypoint->GetTypePoint() == Rep_PathPoint::eTypePointLima ?
                             GET_HOOK( GetDistanceAvantLima )( entity_ ) :
                             GET_HOOK( GetDistanceAvantPoint )( entity_, spottedWaypoint->GetTypeTerrain() ); // $$$$ MCO : refactor to just one hook
    try
    {
        while( true )
        {
            const MT_Vector2D& vCurrentPos = ( *itCurrent )->GetPos();
            if( itCurrent == resultList_.begin() )
            {
                boost::shared_ptr< PathPoint > pNewPoint( new Rep_PathPoint_Front( vCurrentPos, spottedWaypoint ) );
                resultList_.insert( ++itCurrent, pNewPoint );
                break;
            }

            // remise en position des itérateurs
            IT_PathPointList itPrev = GetPreviousWaypointOnDifferentLocation( itCurrent );
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
                boost::shared_ptr< PathPoint > pNewPoint( new Rep_PathPoint_Front( vPreviousPos, spottedWaypoint ) );
                resultList_.insert( itPrev, pNewPoint );
                break;
            }
            else if( rDistanceLeft < 0. )
            {
                // Positionnement du point avant
                vTmp /= vTmpMag;
                vTmp *= rDistanceLeft;
                vTmp += vPreviousPos;
                boost::shared_ptr< PathPoint > pNewPoint( new Rep_PathPoint_Front( vTmp, spottedWaypoint ) );
                resultList_.insert( itCurrent, pNewPoint );
                break;
            }
            // tant pis, ça sera peut-être pour le prochain segment
            itCurrent = itPrev;
        }
    }
    catch( const std::exception& e )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, tools::GetExceptionMsg( e ).c_str() );
    }
}

#pragma warning( pop )

//-----------------------------------------------------------------------------
// Name: Agent_Path::InsertPointAvant
// Created: JVT 02-12-04
//----------------------------------------------------------------------------
void Agent_Path::InsertPointAvant( const boost::shared_ptr< PathPoint > spottedWaypoint, IT_PathPointList itCurrent, double& rDistSinceLastPointAvant )
{
    static const double rDist = 2000.;
    if( rDistSinceLastPointAvant > rDist )
    {
        InsertPointAvant( spottedWaypoint, itCurrent );
        rDistSinceLastPointAvant = 0.;
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::InsertPoint
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
bool Agent_Path::InsertPoint( boost::shared_ptr< PathPoint > spottedWaypoint, IT_PathPointList itCurrent, double& rDistSinceLastPoint )
{
    static double rDist = 500.;
    if( rDistSinceLastPoint > rDist )
    {
        resultList_.insert( itCurrent, spottedWaypoint );
        rDistSinceLastPoint = 0.;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::InsertPointAndPointAvant
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void Agent_Path::InsertPointAndPointAvant( boost::shared_ptr< PathPoint > spottedWaypoint, IT_PathPointList itCurrent, double& rDistSinceLastPoint, double& rDistSinceLastPointAvant )
{
    if( InsertPoint( spottedWaypoint, itCurrent, rDistSinceLastPoint ) )
        InsertPointAvant( spottedWaypoint, itCurrent, rDistSinceLastPointAvant );
}

//-----------------------------------------------------------------------------
// Name: Agent_Path::InsertPointAvants
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
void Agent_Path::InsertPointAvants()
{
    double rDistSinceLastPointAvant = std::numeric_limits< double >::max();
    double rDistSinceLastPoint = std::numeric_limits< double >::max();
    TerrainData nObjectTypesBefore;
    PathPoint* pPrevPoint = 0;
    for( IT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        PathPoint& current = **itPoint;
        if( pPrevPoint )
        {
            if( rDistSinceLastPointAvant != std::numeric_limits< double >::max() )
                rDistSinceLastPointAvant += pPrevPoint->GetPos().Distance( current.GetPos() );
            if( rDistSinceLastPoint != std::numeric_limits< double >::max() )
                rDistSinceLastPoint += pPrevPoint->GetPos().Distance( current.GetPos() );
        }

        // On ne teste les points avants que sur les points du path find original
        if( current.GetType() != PathPoint::eTypePointPath )
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
            InsertPointAndPointAvant( boost::shared_ptr< PathPoint >( new Rep_PathPoint_Special( ( *itPoint )->GetPos(), Rep_PathPoint_Special::eTypePointParticulierVillage, TerrainData::Urban() ) ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Urban
        else if( IsPointAvantOut( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Urban() ) )
            InsertPoint( boost::shared_ptr< PathPoint >( new Rep_PathPoint( ( *itPoint )->GetPos(), Rep_PathPoint::eTypePointCCT, TerrainData::Urban() ) ), itPoint, rDistSinceLastPoint );

        // Forest
        else if( IsPointAvant( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Forest() ) )
            InsertPointAndPointAvant( boost::shared_ptr< PathPoint >( new Rep_PathPoint( ( *itPoint )->GetPos(), Rep_PathPoint::eTypePointCCT, TerrainData::Forest() ) ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Cross roads
        else if( current.GetObjectTypes().ContainsOne( TerrainData::Crossroad() ) )
            InsertPointAndPointAvant( boost::shared_ptr< PathPoint >( new Rep_PathPoint_Special( ( *itPoint )->GetPos(), Rep_PathPoint_Special::eTypePointParticulierCarrefour, TerrainData::Crossroad() ) ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Pont
        else if( IsPointAvantIn( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Bridge() )
                || ( !current.GetObjectTypes().ContainsOne( TerrainData::SmallRiver() ) && current.GetObjectTypes().ContainsOne( TerrainData::Bridge() ) && !nObjectTypesBefore.ContainsOne( TerrainData::Bridge() ) && !nObjectTypesToNextPoint.ContainsOne( TerrainData::Bridge() ) )
                )
            InsertPointAndPointAvant( boost::shared_ptr< PathPoint >( new Rep_PathPoint_Special( ( *itPoint )->GetPos(), Rep_PathPoint_Special::eTypePointParticulierPont, TerrainData::Bridge() ) ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        nObjectTypesBefore = nObjectTypesToNextPoint;
        pPrevPoint = &current;
    }
}

namespace
{
    struct Lima
    {
        Agent_Path* path;
        MT_Vector2D intersection;
        PathResult::IT_PathPointList itPoint;
    };

    void VisitLimaOrders( unsigned int limaID, int functionID, void* userData )
    {
        Lima* l = (Lima*)userData;
        l->path->InsertLima( limaID, functionID, l->intersection, l->itPoint );
    }
}

//-----------------------------------------------------------------------------
// Name: Agent_Path::InsertLimaFunction
// Created: JVT 02-12-05
//-----------------------------------------------------------------------------
void Agent_Path::InsertLima( unsigned int limaID, int functionID, const MT_Vector2D& posIntersect, IT_PathPointList itPoint )
{
    boost::shared_ptr< PathPoint > pPoint( new Rep_PathPoint_Lima( posIntersect, TerrainData(), limaID, functionID ) );
    IT_PathPointList itTmp = resultList_.insert( itPoint, pPoint );
    InsertPointAvant( pPoint, itTmp );
}

//-----------------------------------------------------------------------------
// Name: Agent_Path::InsertLima
// Created: JVT 02-12-05
//-----------------------------------------------------------------------------
void Agent_Path::InsertLima( const MIL_LimaOrder& lima )
{
    boost::shared_ptr< PathPoint > pLastPoint;
    for( IT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        boost::shared_ptr< PathPoint > pCurrentPoint = *itPoint;
        if( !pCurrentPoint )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Current point is invalid" );
            break;
        }
        if( pLastPoint.get() && ( pCurrentPoint->GetPos() != pLastPoint->GetPos() ) )
        {
            MT_Line segment( pLastPoint->GetPos(), pCurrentPoint->GetPos() );
            MT_Vector2D posIntersect;
            if( GET_HOOK( LimaIntersect2D )( lima, segment, posIntersect ) ) // $$$$ MCO : not sure MT_Line is safe to be passed through a hook
            {
                Lima l;
                l.path = this;
                l.intersection = posIntersect;
                l.itPoint = itPoint;
                GET_HOOK( GetLimaOrders )( lima, &VisitLimaOrders, &l );
            }
        }
        pLastPoint = pCurrentPoint;
    }
}

namespace
{
    void VisitLimas( const MIL_LimaOrder& lima, void* userData )
    {
        ((Agent_Path*)userData)->InsertLima( lima );
    }
}

//-----------------------------------------------------------------------------
// Name: Agent_Path::InsertLimas
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
void Agent_Path::InsertLimas()
{
    GET_HOOK( GetLimas )( entity_, &VisitLimas, this );
}

//-----------------------------------------------------------------------------
// Name: Agent_Path::InsertDecPoints
// Created: JDY 03-03-04
//-----------------------------------------------------------------------------
void Agent_Path::InsertDecPoints()
{
    if( bDecPointsInserted_ )
        return;
    bDecPointsInserted_ = true;
    // Points avants
    if( entity_[ "movement/height" ] == 0 )
        InsertPointAvants();
    // Limas
    InsertLimas();
}

std::ostream& operator<<( std::ostream& out, const MT_Vector2D& vect )
{
    return out << '[' << vect.rX_ << ", " << vect.rY_ << ']';
}

namespace
{
    void SendEvent( const wrapper::View& entity, MIL_Report::E_DecisionalReport code )
    {
        wrapper::Event event( "movement report" );
        event[ "entity/data" ] = entity[ "data" ];
        event[ "code" ] = code;
        event.Post();
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::Execute
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
void Agent_Path::Execute( TER_Pathfinder_ABC& pathfind )
{
    if( GET_HOOK( UsePathDebug )() ) // $$$$ MCO : this is a "static data" configuration value
    {
        std::stringstream s;
        s << "Agent_Path::Compute: " << this << " : computation begin"
          << ", Thread : " << GET_HOOK( GetPathfindCurrentThread )()
          << ", Agent : " << static_cast< unsigned int >( entity_[ "identifier" ] )
          << ", Path type : " << PathResult::GetPathType().GetName()
          << ", " << GetPathAsString().c_str();
        ::SWORD_Log( SWORD_LOG_LEVEL_INFO, s.str().c_str() );
        profiler_.Start();
    }
    assert( resultList_.empty() );

    Path_ABC::Execute( pathfind );
    PathResult::E_State nPathState = GetState();
    if( nPathState == Path_ABC::eImpossible )
        SendEvent( entity_,
            entity_[ "is-underground" ] // $$$$ MCO 2012-07-09: module should not be aware of the "is-underground" feature
                ? MIL_Report::eRC_NotActivatedUndergroundNetwork
                : MIL_Report::eRC_TerrainDifficile );

#ifndef NDEBUG
    for( CIT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); )
    {
        PathPoint& point = **itPoint;

//        if( itPoint != resultList_.begin() && !unitSpeeds_.IsPassable( point.GetObjectTypes() ) )
//            throw MASA_EXCEPTION( "Unit max speed is not positive for a given object" );

        ++itPoint;
        if( itPoint != resultList_.end() && unitSpeeds_.GetMaxSpeed( point.GetObjectTypesToNextPoint() ) <= 0 )
            throw MASA_EXCEPTION( "Unit max speed is not positive for a given object" );
    }
#endif

    if( GET_HOOK( UsePathDebug )() )
    {
        double rComputationTime = profiler_.Stop();
        std::stringstream s;
        s << "Agent_Path::Compute: " << this
          << ", Thread : "  << GET_HOOK( GetPathfindCurrentThread )()
          << ", Time : " << rComputationTime
          << ", State : " << GetStateAsString()
          << ", Result : ";
        if( ! resultList_.empty() )
            s << resultList_.front()->GetPos() << " -> " << resultList_.back()->GetPos();
        ::SWORD_Log( SWORD_LOG_LEVEL_INFO, s.str().c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::NotifyPointReached
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
void Agent_Path::NotifyPointReached( const CIT_PathPointList& itCurrentPathPoint )
{
    const T_PointVector& computedWaypoints = GetComputedWaypoints();
    if( nextWaypoints_.size() > 1 && computedWaypoints.size() > 1 &&
        static_cast< float >( (*itCurrentPathPoint)->GetPos().rX_ ) == static_cast< float >( computedWaypoints.front().rX_ ) &&
        static_cast< float >( (*itCurrentPathPoint)->GetPos().rY_ ) == static_cast< float >( computedWaypoints.front().rY_ ) )
    {
        nextWaypoints_.erase( nextWaypoints_.begin() );
        RemoveComputedWaypoint();
    }
    PathResult::NotifyPointReached( itCurrentPathPoint );
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::IsWaypoint
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
bool Agent_Path::IsWaypoint( const MT_Vector2D& point ) const
{
    for( T_PointVector::const_iterator it = initialWaypoints_.begin(); it != initialWaypoints_.end(); ++it )
        if( *it == point )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::ComputePath
// Created: LDC 2012-03-23
// -----------------------------------------------------------------------------
void Agent_Path::ComputePath( const boost::shared_ptr< Path_ABC >& pPath )
{
    if( ! GET_HOOK( IsDestinationTrafficable )( entity_, nextWaypoints_.empty() ? 0 : &nextWaypoints_[0], nextWaypoints_.size() ) )
    {
        SendEvent( entity_, MIL_Report::eRC_TerrainDifficile );
        Cancel();
    }
    else
        GET_HOOK( StartComputePathfind )( entity_, pPath );
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::NeedRefine
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
bool Agent_Path::NeedRefine() const
{
    return entity_[ "can-fly" ] && ! entity_[ "is-autonomous" ];
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::UseStrictClosest
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
bool Agent_Path::UseStrictClosest() const
{
    return ! GET_HOOK( IsNullAutomateFuseau )( entity_ );
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::GetFirstPoint
// Created: MCO 2012-05-24
// -----------------------------------------------------------------------------
const MT_Vector2D* Agent_Path::GetFirstPoint() const
{
    const PathResult::T_PathPointList& path = GetResult( false );
    return path.empty() ? 0 : &path.front()->GetPos();
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::GetClass
// Created: MCO 2012-05-24
// -----------------------------------------------------------------------------
const Agent_PathClass& Agent_Path::GetClass() const
{
    return pathClass_;
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::GetPathClass
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const Agent_PathClass& Agent_Path::GetPathClass() const
{
    return pathClass_;
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::GetUnitSpeeds
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
const Speeds& Agent_Path::GetUnitSpeeds() const
{
    return unitSpeeds_;
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::GetNextWaypoints
// Created: CMA 2012-02-20
// -----------------------------------------------------------------------------
const T_PointVector& Agent_Path::GetNextWaypoints() const
{
    return nextWaypoints_;
}
