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
#include "TER_StaticData.h"
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
TER_PathFinderThread::TER_PathFinderThread( const TER_StaticData& staticData, tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >& queue, bool bUseSameThread )
    : tools::thread::RequestProcessor_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >( queue )
    , pPathfinder_   ( 0 )
    , bUseSameThread_( bUseSameThread )
{
    pPathfinder_ = new TerrainPathfinder( staticData );
    pPathfinder_->SetPickingDistances( 1000.f, 10000.f ); // minpicking, maxpicking
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

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::Pick
// Created: BCI 2011-03-04
// -----------------------------------------------------------------------------
TerrainData TER_PathFinderThread::Pick( const MT_Vector2D& pos )
{
    TerrainRetractationHandle& handle = pPathfinder_->CreateRetractationHandle();
    TerrainData data = pPathfinder_->Pick( MakePoint( pos ), handle );
    delete &handle;
    return data;
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
