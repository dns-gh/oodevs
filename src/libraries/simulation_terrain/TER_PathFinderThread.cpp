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
#include "TER_PathfindRequest.h"
#include "TER_DynamicData.h"
#include "TER_StaticData.h"
#include "MT_Tools/MT_Profiler.h"
#include "TER_Pathfinder_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_FormatString.h"
#include <pathfind/PathfindFileDumper.h>
#include <pathfind/TerrainPathfinder.h>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

using namespace pathfind;
namespace bii = boost::interprocess::ipcdetail;

namespace
{
    std::set< size_t > ParseFilter( const std::string& filter )
    {
        std::set< size_t > reply;
        boost::tokenizer< boost::escaped_list_separator< char > > tokens( filter );
        for( auto it = tokens.begin(); it != tokens.end(); ++it )
            reply.insert( boost::lexical_cast< size_t >( *it ) );
        return reply;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread constructor
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
TER_PathFinderThread::TER_PathFinderThread( const TER_StaticData& staticData,
            unsigned int nMaxEndConnections, double rMinEndConnectionLength,
            const tools::Path& dump, const std::string& filter )
    : dump_          ( dump )
    , filter_        ( ParseFilter( filter ) )
    , pathfinder_    ( new TerrainPathfinder( staticData ) )
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
    std::vector< DynamicDataPtr > toRegister, toUnregister;
    {
        boost::mutex::scoped_lock locker( dynamicDataMutex_ );
        toRegister  .swap( dynamicDataToRegister_   );
        toUnregister.swap( dynamicDataToUnregister_ );
    }

    if( !toRegister.empty() )
    {
        MT_Profiler profiler;
        profiler.Start();

        for( auto it = toRegister.begin(); it != toRegister.end(); ++it )
        {
            RetractationPtr h = CreateDynamicData( *pathfinder_, **it );
            handlers_[*it] = h;
        }
        MT_LOG_INFO_MSG( MT_FormatString( "Register %d dynamic data - %.2f ms",
                    toRegister.size(), profiler.Stop() ) );
    }

    if( !toUnregister.empty() )
    {
        MT_Profiler profiler;
        profiler.Start();

        for( auto it = toUnregister.begin(); it != toUnregister.end(); ++it )
            handlers_.erase( *it );
        MT_LOG_INFO_MSG( MT_FormatString( "Unregister %d dynamic data - %.2f ms",
                    toUnregister.size(), profiler.Stop() ) );
    }
}

namespace
{
    struct PathfinderProxy : public TER_Pathfinder_ABC
                           , public boost::noncopyable
    {
        PathfinderProxy( const tools::Path& dump, const std::set< size_t >& filter,
                         TerrainPathfinder& root )
            : dump_  ( dump )
            , filter_( filter )
            , root_  ( root )
            , id_    ( 0 )
        {
            // NOTHING
        }
        virtual void SetId( size_t id )
        {
            id_ = id;
        }
        virtual void SetChoiceRatio( float ratio )
        {
            root_.SetChoiceRatio( ratio );
        }
        virtual void SetConfiguration( unsigned nRefining, unsigned int nSubdivisions )
        {
            root_.SetConfiguration( nRefining, nSubdivisions );
        }
        virtual PathResultPtr ComputePath( const geometry::Point2f& from,
                                  const geometry::Point2f& to,
                                  TerrainRule_ABC& rule )
        {
            const bool dump = !dump_.IsEmpty() &&
                ( filter_.empty() || filter_.count( id_ ) );
            if( dump )
            {
                PathfindFileDumper dumper( GetFilename(), rule );
                return root_.ComputePath( from, to, dumper );
            }
            return root_.ComputePath( from, to, rule );
        }
    private:
        tools::Path GetFilename() const
        {
            std::stringstream name;
            name << "pathfind_"
                 << id_
                 << "_"
                 << bii::atomic_inc32( &s_idx_ );
            return dump_ / name.str().c_str();
        }
    private:
        static boost::uint32_t    s_idx_;
        const tools::Path&        dump_;
        const std::set< size_t >& filter_;
        TerrainPathfinder&        root_;
        size_t                    id_;
    };

    boost::uint32_t PathfinderProxy::s_idx_ = 0;
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::Process
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
double TER_PathFinderThread::Process( TER_PathfindRequest& rq, unsigned int deadline )
{
    try
    {
        ProcessDynamicData();
        PathfinderProxy proxy( dump_, filter_,
            rq.IgnoreDynamicObjects() ? *staticPathfinder_ : *pathfinder_ );
        return rq.FindPath( proxy, deadline );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception caught in pathfinder thread : " << tools::GetExceptionMsg( e ) );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Unknown exception caught in pathfinder thread" );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::AddDynamicDataToRegister
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PathFinderThread::AddDynamicDataToRegister( const DynamicDataPtr& data )
{
    boost::mutex::scoped_lock locker( dynamicDataMutex_ );
    dynamicDataToRegister_.push_back( data );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::AddDynamicDataToUnregister
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PathFinderThread::AddDynamicDataToUnregister( const DynamicDataPtr& data )
{
    boost::mutex::scoped_lock locker( dynamicDataMutex_ );
    dynamicDataToUnregister_.push_back( data );
}
