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
#include "TER_DynamicData.h"
#include "TER_StaticData.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_FormatString.h"
#include <pathfind/TerrainPathfinder.h>

using namespace pathfind;

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread constructor
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
TER_PathFinderThread::TER_PathFinderThread( const TER_StaticData& staticData,
            unsigned int nMaxEndConnections, double rMinEndConnectionLength )
    : pathfinder_    ( new TerrainPathfinder( staticData ) )
    , staticPathfinder_( new TerrainPathfinder( staticData ) )
{
    pathfinder_->SetPickingDistances( 1000.f, 10000.f ); // minpicking, maxpicking
    pathfinder_->SetEndConnectionSetup( nMaxEndConnections, static_cast< float >( rMinEndConnectionLength * 1.1 ) ); // applying factor of 10%
    staticPathfinder_->SetPickingDistances( 1000.f, 10000.f ); // minpicking, maxpicking
    staticPathfinder_->SetEndConnectionSetup( nMaxEndConnections, static_cast< float >( rMinEndConnectionLength * 1.1 ) ); // applying factor of 10%
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread destructor
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
TER_PathFinderThread::~TER_PathFinderThread()
{
    // Ensure the retractation handlers are destroyed before the graph
    handlers_.clear();
}

namespace
{

geometry::Point2f MakePoint( const MT_Vector2D& v )
{
    return geometry::Point2f( static_cast< float >( v.rX_ ), static_cast< float >( v.rY_ ) );
}

RetractationPtr CreateDynamicData( TerrainPathfinder& pathfinder, const TER_DynamicData& data )
{
    const T_PointVector& points = data.GetPoints();
    std::vector< geometry::Point2f > geometryPoints;
    geometryPoints.reserve( points.size() );
    for( auto it = points.begin(); it != points.end(); ++it )
        geometryPoints.push_back( MakePoint( *it ) );
    auto handle = pathfinder.CreateRetractationHandle();
    pathfinder.AddDynamicData( geometryPoints, data.GetData(), *handle );
    return handle;
}

} // namespace


// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::ProcessDynamicData
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PathFinderThread::ProcessDynamicData()
{
    std::vector< Registration > registrations;
    {
        boost::mutex::scoped_lock locker( pendingMutex_ );
        registrations.swap( pending_ );
    }

    if( registrations.empty() )
        return;

    MT_Profiler profiler;
    profiler.Start();
    auto added = registrations.size();
    for( auto it = registrations.begin(); it != registrations.end(); ++it )
    {
        if( !it->remove )
        {
            RetractationPtr h = CreateDynamicData( *pathfinder_, *it->data );
            handlers_[it->data] = h;
        }
        else
        {
            handlers_.erase( it->data );
            --added;
        }
    }
    MT_LOG_INFO_MSG( MT_FormatString( "Dynamic data added: %d, removed: %d, %.2f ms",
        added, registrations.size() - added, profiler.Stop() ) );
}

boost::shared_ptr< TerrainPathfinder > TER_PathFinderThread::GetPathfinder( bool dynamic )
{
    return dynamic ? pathfinder_ : staticPathfinder_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::AddDynamicDataToRegister
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PathFinderThread::AddDynamicDataToRegister( const DynamicDataPtr& data )
{
    boost::mutex::scoped_lock locker( pendingMutex_ );
    Registration r;
    r.data = data;
    r.remove = false;
    pending_.push_back( r );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::AddDynamicDataToUnregister
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PathFinderThread::AddDynamicDataToUnregister( const DynamicDataPtr& data )
{
    boost::mutex::scoped_lock locker( pendingMutex_ );
    Registration r;
    r.data = data;
    r.remove = true;
    pending_.push_back( r );
}
