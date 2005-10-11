// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-02-23 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PathFinderThread.cpp $
// $Author: Nld $
// $Modtime: 3/06/05 17:46 $
// $Revision: 5 $
// $Workfile: TER_PathFinderThread.cpp $
//
// *****************************************************************************

#include "ter_pch.h"
#include "TER_PathFinderThread.h"
#include "TER_NodeFunctor_ABC.h"
#include "TER_PathFindRequest_ABC.h"
#include "TER_DynamicData.h"
#include "pathfind/TerrainPathfinder.h"
#include "pathfind/TerrainRule_ABC.h"
#include "pathfind/Node.h"
#include "pathfind/SpatialContainerTraits.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT/MT_Archive/MT_FlatBinaryInputArchive.h"
#include "MT/MT_Archive/MT_FlatBinaryOutputArchive.h"
#include "MT/MT_Logger/MT_LogManager.h"
#include "MT/MT_Logger/MT_LogDefines.h"
#include "MT/MT_IO/MT_Dir.h"
#include "MT/MT_IO/MT_FormatString.h"

using namespace pathfind;

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread constructor
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
TER_PathFinderThread::TER_PathFinderThread( const std::string& strGraphArchive, const std::string& strNodeArchive, const std::string& strLinkArchive, tools::thread::MessageQueue_ABC< TER_PathFindRequest_ABC* >& queue )
    : tools::thread::RequestProcessor_ABC< TER_PathFindRequest_ABC* >( queue )
    , pPathfinder_( 0 )
{
    MT_FlatBinaryInputArchive graph, nodes, links;
    if( !graph.Open( strGraphArchive ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Cannot open file %s", strGraphArchive.c_str() ) );
    if( !nodes.Open( strNodeArchive ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Cannot open file %s", strNodeArchive.c_str() ) );
    if( !links.Open( strLinkArchive ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Cannot open file %s", strLinkArchive.c_str() ) );
    pPathfinder_ = new TerrainPathfinder( graph, nodes, links );
    pPathfinder_->SetGraphConfiguration( 10, 1000, 10000 ); // precision, minpicking, maxpicking
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
    boost::mutex::scoped_lock locker( dynamicDataMutex_ );

    if( !dynamicDataToRegister_.empty() )
    {
        MT_Profiler profiler;
        profiler.Start();

        for( CIT_DynamicDataVector it = dynamicDataToRegister_.begin(); it != dynamicDataToRegister_.end(); ++it )
        {
            TER_DynamicData* pData = *it;
            assert( pData );
            pData->RegisterDynamicData( *this );
        }

        MT_LOG_INFO_MSG( MT_FormatString( "Register %d dynamic data - %.2f ms", dynamicDataToRegister_.size(), profiler.Stop() ) );    
        dynamicDataToRegister_.clear();
    }

    if( !dynamicDataToUnregister_.empty() )
    {
        MT_Profiler profiler;
        profiler.Start();

        for( CIT_DynamicDataVector it = dynamicDataToUnregister_.begin(); it != dynamicDataToUnregister_.end(); ++it )
        {
            TER_DynamicData* pData = *it;
            assert( pData );
            pData->UnregisterDynamicData( *this );
        }

        MT_LOG_INFO_MSG( MT_FormatString( "Unregister %d dynamic data - %.2f ms", dynamicDataToUnregister_.size(), profiler.Stop() ) );    
        dynamicDataToUnregister_.clear();
    }
}   

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::Process
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
void TER_PathFinderThread::Process( TER_PathFindRequest_ABC* const& pRequest )
{
    try
    {
        ProcessDynamicData();
        
        if( pRequest )
            pRequest->Execute( *pPathfinder_ );
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
        return geometry::Point2f( float( v.rX_ ), float( v.rY_ ) );
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
        DataBuilder( TerrainData& data ) : data_( data ) {};
        void operator()( const Link< TerrainData >& link )
        {
            data_.Merge( link.GetUserData() );
        }
        const TerrainData& Result() const { return data_; } ;
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

    class NodeFinder
    {
    public:
        NodeFinder( TER_NodeFunctor_ABC& functor, const MT_Vector2D& vCenter, MT_Float rRadius )
            : functor_( functor )
            , center_( vCenter )
            , rSquareRadius_( rRadius * rRadius ) {};
        bool operator()( const Node< TerrainData >& node )
        {
            const MT_Vector2D nodePos( node.X(), node.Y() );
            if( center_.SquareDistance( nodePos ) < rSquareRadius_ )
                functor_.Visit( nodePos, BuildData( node ) );
            return true;
        }
    private:
        NodeFinder& operator=( const NodeFinder& );
        TER_NodeFunctor_ABC& functor_;
        MT_Vector2D center_;
        MT_Float rSquareRadius_;
    };
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::ApplyOnNodesWithinCircle
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
void TER_PathFinderThread::ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const
{
    NodeFinder finder( bestNodeFunction, vCenter, rRadius );
    pPathfinder_->ApplyOnNodesInCircle( MakePoint( vCenter ), float( rRadius ), finder );
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
    MT_MakeDir( strDirectory );
    Dump( strDirectory + "/" ); 
}

// -----------------------------------------------------------------------------
// Name: TER_PathFinderThread::Dump
// Created: AGE 2005-03-04
// -----------------------------------------------------------------------------
void TER_PathFinderThread::Dump( const std::string& strBaseArchiveName ) const
{
    MT_FlatBinaryOutputArchive graph, nodes, links;
    graph.EnableExceptions( true ); nodes.EnableExceptions( true ); links.EnableExceptions( true );
    graph.OpenOutputFile( strBaseArchiveName + "Graph.bin" );
    nodes.OpenOutputFile( strBaseArchiveName + "Nodes.bin" );
    links.OpenOutputFile( strBaseArchiveName + "Links.bin" );
    pPathfinder_->Dump( graph, nodes, links );
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
