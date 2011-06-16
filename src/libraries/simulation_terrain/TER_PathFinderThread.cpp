// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PathFinderThread.h"
#include "TER_NodeFunctor_ABC.h"
#include "TER_PathFindRequest_ABC.h"
#include "TER_DynamicData.h"
#include "simulation_kernel/MIL_AgentServer.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Profiler.h"
#include "simulation_kernel/Decision/DEC_PathFind_Manager.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_FormatString.h"
#include <pathfind/TerrainPathfinder.h>
#include <pathfind/TerrainRule_ABC.h>
#include <pathfind/TerrainHeuristic.h>
#include <spatialcontainer/Node.h>
#include <spatialcontainer/SpatialContainerTraits.h>
#include <boost/filesystem/convenience.hpp>

using namespace spatialcontainer;
using namespace pathfind;

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread constructor
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
TER_PathFinderThread::TER_PathFinderThread( const std::string& strGraphArchive, const std::string& strNodeArchive, const std::string& strLinkArchive, tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >& queue, bool bUseSameThread )
    : tools::thread::RequestProcessor_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >( queue )
    , pPathfinder_   ( 0 )
    , bUseSameThread_( bUseSameThread )
{
    pPathfinder_ = new TerrainPathfinder( strGraphArchive, strNodeArchive, strLinkArchive );
    pPathfinder_->SetGraphConfiguration( 10, 1000, 10000 ); // precision, minpicking, maxpicking
    if( !bUseSameThread )
        Start();
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread destructor
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
TER_PathFinderThread::~TER_PathFinderThread()
{
    Terminate();
    delete pPathfinder_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::ProcessDynamicData
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PathFinderThread::ProcessDynamicData()
{
    T_DynamicDataVector tmpDynamicDataToRegister;
    T_DynamicDataVector tmpDynamicDataToUnregister;
    {
        boost::mutex::scoped_lock locker( dynamicDataMutex_ );
        tmpDynamicDataToRegister  .swap( dynamicDataToRegister_   );
        tmpDynamicDataToUnregister.swap( dynamicDataToUnregister_ );
    }
    if( !tmpDynamicDataToRegister.empty() )
    {
        MT_Profiler profiler;
        profiler.Start();

        for( CIT_DynamicDataVector it = tmpDynamicDataToRegister.begin(); it != tmpDynamicDataToRegister.end(); ++it )
        {
            TER_DynamicData* pData = *it;
            assert( pData );
            pData->RegisterDynamicData( *this );
        }
        MT_LOG_INFO_MSG( MT_FormatString( "Register %d dynamic data - %.2f ms", tmpDynamicDataToRegister.size(), profiler.Stop() ) );
        tmpDynamicDataToRegister.clear();
    }
    if( !tmpDynamicDataToUnregister.empty() )
    {
        MT_Profiler profiler;
        profiler.Start();

        for( CIT_DynamicDataVector it = tmpDynamicDataToUnregister.begin(); it != tmpDynamicDataToUnregister.end(); ++it )
        {
            TER_DynamicData* pData = *it;
            assert( pData );
            pData->UnregisterDynamicData( *this );
        }
        MT_LOG_INFO_MSG( MT_FormatString( "Unregister %d dynamic data - %.2f ms", tmpDynamicDataToUnregister.size(), profiler.Stop() ) );
        tmpDynamicDataToUnregister.clear();
    }
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::Process
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
void TER_PathFinderThread::Process( const boost::shared_ptr< TER_PathFindRequest_ABC >& pRequest )
{
    try
    {
        ProcessDynamicData();
        if( pRequest.get() )
        {
            pRequest->Execute( *pPathfinder_ );
            MIL_AgentServer::GetWorkspace().GetPathFindManager().CleanPathAfterComputation( pRequest );
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception caught in pathfinder thread : " << e.what() );
        assert( false );
    }
    catch( ... )
    {
        _clearfp();
        MT_LOG_ERROR_MSG( "Unknown exception caught in pathfinder thread" );
        assert( false );
    }
}

namespace
{
    inline
    geometry::Point2f MakePoint( const MT_Vector2D& v )
    {
        return geometry::Point2f( static_cast< float >( v.rX_ ), static_cast< float >( v.rY_ ) );
    };
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::CreateLineTree
// Created: AGE 2005-10-07
// -----------------------------------------------------------------------------
TerrainRetractationHandle& TER_PathFinderThread::CreateLineTree( const T_PointVector& points, const TerrainData& terrainData )
{
    std::vector< geometry::Point2f > geometryPoints;
    geometryPoints.reserve( points.size() );
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        geometryPoints.push_back( MakePoint( *it ) );
    return pPathfinder_->CreateDynamicData( geometryPoints.begin(), geometryPoints.end(), terrainData );
}

namespace
{
    class DataBuilder
    {
    public:
        DataBuilder( TerrainData& data )
            : data_( data )
        {
            // NOTHING
        }
        void operator()( const Link< TerrainData >& link )
        {
            data_.Merge( link.GetUserData() );
        }
        const TerrainData& Result() const
        {
            return data_;
        }
    private:
        DataBuilder& operator=( const DataBuilder& );
        TerrainData& data_;
    };

    TerrainData BuildData( const Node< TerrainData >& node )
    {
        TerrainData result;
        DataBuilder builder( result );
        node.ApplyOnLinks( builder );
        return builder.Result();
    };

    class NodeCircleFinder
    {
    public:
        NodeCircleFinder( TER_NodeFunctor_ABC& functor, const MT_Vector2D& vCenter, double rRadius )
            : functor_      ( functor )
            , center_       ( vCenter )
            , rSquareRadius_( rRadius * rRadius )
        {
            // NOTHING
        }
        bool operator()( const Node< TerrainData >& node )
        {
            const MT_Vector2D nodePos( node.X(), node.Y() );
            if( center_.SquareDistance( nodePos ) < rSquareRadius_ )
                functor_.Visit( nodePos, BuildData( node ) );
            return true;
        }
    private:
        NodeCircleFinder& operator=( const NodeCircleFinder& );
        TER_NodeFunctor_ABC& functor_;
        MT_Vector2D center_;
        double rSquareRadius_;
    };
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::ApplyOnNodesWithinCircle
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
void TER_PathFinderThread::ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, double rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const
{
    NodeCircleFinder finder( bestNodeFunction, vCenter, rRadius );
    pPathfinder_->ApplyOnNodesInCircle( MakePoint( vCenter ), static_cast< float >( rRadius ), finder );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::FindCrossroadsWithinCircle
// Created: RPD 2009-08-18
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > TER_PathFinderThread::FindCrossroadsWithinCircle( const MT_Vector2D& center, float radius )
{
    std::vector< spatialcontainer::Node< TerrainData >* > result = pPathfinder_->FindCrossroadsWithinCircle( MakePoint( center ), radius );
    std::vector< boost::shared_ptr< MT_Vector2D > > points;
    for( std::vector< spatialcontainer::Node< TerrainData >* >::const_iterator it = result.begin(); it != result.end(); ++it )
        points.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( static_cast< double >( (*it)->X() ), static_cast< double >( (*it)->Y() ) ) ) );
    return points;
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::FindSafetyPositionsWithinCircle
// Created: LDC 2010-10-28
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > TER_PathFinderThread::FindSafetyPositionsWithinCircle( const MT_Vector2D& center, float radius, float safetyDistance )
{
    std::vector< geometry::Point2f > result = pPathfinder_->FindSafetyPositionsWithinCircle( MakePoint( center ), radius, safetyDistance );
    std::vector< boost::shared_ptr< MT_Vector2D > > points;
    for( std::vector< geometry::Point2f >::const_iterator it = result.begin(); it != result.end(); ++it )
        points.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( static_cast< double >( it->X() ), static_cast< double >( it->Y() ) ) ) );
    return points;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > > TER_PathFinderThread::FindAllPositions
// Created: BCI 2011-04-01
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > TER_PathFinderThread::FindAllPositions( const MT_Vector2D& center, float radius )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    std::vector< spatialcontainer::Node< TerrainData >* > nodes = pPathfinder_->FindNodesWithinCircle( MakePoint( center ), radius );
    for( std::vector< spatialcontainer::Node< TerrainData >* >::const_iterator iterator = nodes.begin(); iterator != nodes.end(); ++iterator )
    {
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( (**iterator).X(), (**iterator).Y() ) ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: std::vector< TerrainData > TER_PathFinderThread::FindTerrainDataWithinCircle
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
TerrainData TER_PathFinderThread::FindTerrainDataWithinCircle( const MT_Vector2D& center, float radius )
{
    TerrainData result;
    std::vector< spatialcontainer::Node< TerrainData >* > nodes = pPathfinder_->FindNodesWithinCircle( MakePoint( center ), radius );
    for( std::vector< spatialcontainer::Node< TerrainData >* >::const_iterator iterator = nodes.begin(); iterator != nodes.end(); ++iterator )
    {
        result.Merge( TerrainHeuristic::BuildData( **iterator ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::Pick
// Created: BCI 2011-03-04
// -----------------------------------------------------------------------------
TerrainData TER_PathFinderThread::Pick( const MT_Vector2D& pos )
{
    return pPathfinder_->Pick( MakePoint( pos ) );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::ProcessInSimulationThread
// Created: JSR 2010-06-16
// -----------------------------------------------------------------------------
void TER_PathFinderThread::ProcessInSimulationThread( const boost::shared_ptr< TER_PathFindRequest_ABC >& pRequest )
{
    if( !bUseSameThread_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Pathfind : Process in same thread than simulation not expected." );
    Process( pRequest );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::Dump
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
void TER_PathFinderThread::Dump() const
{
    static unsigned int nDump = 0;
    std::stringstream str;
    str << nDump++;
    const std::string strDirectory = str.str();
    boost::filesystem::create_directories( strDirectory );
    Dump( strDirectory + "/" );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::Dump
// Created: AGE 2005-03-04
// -----------------------------------------------------------------------------
void TER_PathFinderThread::Dump( const std::string& strBaseArchiveName ) const
{
    pPathfinder_->Dump( strBaseArchiveName + "Graph.bin",
                        strBaseArchiveName + "Nodes.bin",
                        strBaseArchiveName + "Links.bin" );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::AddDynamicDataToRegister
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PathFinderThread::AddDynamicDataToRegister( TER_DynamicData& data )
{
    boost::mutex::scoped_lock locker( dynamicDataMutex_ );
    dynamicDataToRegister_.push_back( &data );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::AddDynamicDataToUnregister
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PathFinderThread::AddDynamicDataToUnregister( TER_DynamicData& data )
{
    boost::mutex::scoped_lock locker( dynamicDataMutex_ );
    dynamicDataToUnregister_.push_back( &data );
}
