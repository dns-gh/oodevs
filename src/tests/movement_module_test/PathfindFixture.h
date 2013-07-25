// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PATHFIND_FIXTURE_H
#define PATHFIND_FIXTURE_H

#include "ModuleFixture.h"
#include "movement_module/PathWalker.h" // $$$$ _RC_ SLI 2012-03-01: for E_ReturnCode enum
#include "simulation_kernel/Entities/Orders/MIL_Report.h" // $$$$ MCO : for enums
#include <core/UserData.h>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/make_shared.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

namespace bp = boost::phoenix;

class TER_Pathfinder_ABC
{};

namespace sword
{
namespace movement
{
    typedef std::pair< geometry::Point2f, TerrainData > T_Point; // $$$$ _RC_ SLI 2012-03-08: Point2d instead???
    typedef std::vector< T_Point > T_Points;

    struct PathfindFixture : public sword::movement::ModuleFixture
    {
        PathfindFixture()
            : identifier( 12u )
            , handler   ( reinterpret_cast< const void* >( 0xABCD ) )
            , pathId    ( 47u )
            , entity    ( model[ "entities" ][ identifier ] )
        {
            const std::string xml( "<pathfind>"
                                   "  <unit-rules>"
                                        + MakeRules() +
                                   "  </unit-rules>"
                                   "</pathfind>" );
            const size_t dangerousObject = 0;
            InitializePathClass( xml.c_str(), dangerousObject, dangerousObject );
        }
        bool AddPoints( const T_Points& points, PathfindHandlePathPoint handler, void* handlerData )
        {
            const bool pathfound = true;
            for( size_t i = 0; i < points.size(); ++i )
                handler( points[ i ].first, points[ i ].second, ( i + 1 ) < points.size() ? points[ i + 1 ].second : TerrainData(), handlerData );
            return pathfound;
        }
        std::string MakeRules()
        {
            const std::vector< std::string > types = boost::assign::list_of( "movement" )
                                                                           ( "recon" )
                                                                           ( "infiltration" )
                                                                           ( "assault" )
                                                                           ( "retreat" )
                                                                           ( "backup" )
                                                                           ( "minesweep" )
                                                                           ( "logistic" )
                                                                           ( "criminal" )
                                                                           ( "nbc" );
            std::string result;
            BOOST_FOREACH( const std::string& type, types )
                result += "<rule autonomous='false' flying='false' type='" + type + "'/>"
                       +  "<rule autonomous='true' flying='false' type='" + type + "'/>"
                       +  "<rule autonomous='false' flying='true' type='" + type + "'/>"
                       +  "<rule autonomous='true' flying='true' type='" + type + "'/>";
            return result;
        }
        void UpdatePosition( const geometry::Point2f& position )
        {
            entity[ "movement/position/x" ] = position.X();
            entity[ "movement/position/y" ] = position.Y();
            entity[ "movement/has-been-teleported" ] = false;
        }
        void ConfigureImpossiblePathfind()
        {
            MOCK_EXPECT( UsePathDebug ).returns( false );
            MOCK_EXPECT( IsDestinationTrafficable ).returns( true );
            MOCK_EXPECT( GetMaxPathFindComputationDuration ).returns( std::numeric_limits< unsigned int >::max() );
            MOCK_EXPECT( IsNullAutomateFuseau ).returns( true );
            MOCK_EXPECT( ComputePathfind ).once().returns( false );
            MOCK_EXPECT( StartComputePathfind ).once().calls( boost::bind( &ExecutePathfind, _1, boost::ref( pathfind ) ) );
        }
        std::size_t CreatePathParameter( const geometry::Point2f& objective )
        {
            entity[ "identifier" ] = identifier; // $$$$ _RC_ SLI 2012-03-09: smell?
            entity[ "data" ] = "data";
            entity[ "can-fly" ] = false;
            entity[ "is-autonomous" ] = false;
            entity[ "is-underground" ] = false; // $$$$ MCO 2012-07-09: module should not be aware of this "is-underground" feature
            entity[ "danger/x" ] = 1;
            entity[ "danger/y" ] = 0;
            entity[ "is-deployed" ] = false;
            entity[ "movement/intention" ] = false;
            entity[ "movement/height" ] = 0;
            entity[ "movement/direction/x" ] = 0;
            entity[ "movement/direction/y" ] = 0;
            const MT_Vector2D vPosEnd( objective.X(), objective.Y() );
            const int movementPathType = 0;
            const double maxSpeed = 50;
            const double baseSpeed = 10;
            const double terrainSpeed = 20;
            MOCK_EXPECT( GetTheoricMaxSpeed ).once().returns( maxSpeed );
            MOCK_EXPECT( GetSpeed ).once().returns( baseSpeed );
            MOCK_EXPECT( GetSpeed ).exactly( TerrainData::nAreaTypes + TerrainData::nBorderTypes + TerrainData::nLinearTypes ).returns( terrainSpeed );
            MOCK_EXPECT( NotifyPathCreation ).once().returns( pathId );
            MOCK_EXPECT( GetPathHandler ).once().with( pathId ).returns( handler );
            MOCK_EXPECT( InitializePath ).once();
            MOCK_EXPECT( IsNullAutomateFuseau ).returns( true );
            return CreatePath( core::Convert( &entity ), vPosEnd, movementPathType );
        }
        std::size_t CreateSimplePath()
        {
            const T_Points points = boost::assign::map_list_of( geometry::Point2f( 0, 0 ), TerrainData() )
                                                              ( geometry::Point2f( 0, 10 ), TerrainData() );
            UpdatePosition( points.front().first );
            return CreatePathParameter( points.back().first );
        }
        TER_Pathfinder_ABC pathfind;
        const unsigned int identifier;
        const void* handler;
        const std::size_t pathId;
        core::Model& entity;
    };
}
}

#endif // PATHFIND_FIXTURE_H
