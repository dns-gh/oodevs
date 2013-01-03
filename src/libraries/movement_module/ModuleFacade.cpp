// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "ModuleFacade.h"
#include "Agent_Path.h"
#include "Agent_PathClass.h"
#include "PathType.h"
#include "MoveCommand.h"
#include "MagicMoveCommand.h"
#include "OrientateCommand.h"
#include "ComputeHeightCommand.h"
#include "FollowCommand.h"
#include "Rep_PathPoint_Front.h"
#include "simulation_kernel/Decision/DEC_Path_ABC.h" // $$$$ MCO : for enums
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <algorithm>

using namespace sword;
using namespace sword::movement;

namespace
{
    ModuleFacade* facade = 0; // $$$$ MCO : need a means to bind additional data to a hook
    DECLARE_HOOK( CancelPathFindJob, void, ( std::size_t path ) )

    DEFINE_HOOK( ComputeAgentFutureObjectCollision, 4, bool, ( const SWORD_Model* entity, const KnowledgeCache& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) )
    {
        assert( facade );
        PathWalker* pathWalker = facade->GetPathWalker( wrapper::View( entity )[ "identifier" ] );
        if( !pathWalker )
            return false;
        return pathWalker->ComputeFutureObjectCollision( entity, objectsToTest, rDistance, pObject, false, false );
    }
    DEFINE_HOOK( GetAgentFuturePosition, 3, MT_Vector2D, ( const SWORD_Model* entity, double rTime, bool bBoundOnPath ) )
    {
        assert( facade );
        PathWalker* pathWalker = facade->GetPathWalker( wrapper::View( entity )[ "identifier" ] );
        if( !pathWalker )
            return MT_Vector2D();
        return pathWalker->ExtrapolatePosition( entity, rTime, bBoundOnPath );
    }
    DEFINE_HOOK( IsAgentMovingOnPath, 2, bool, ( unsigned int entity, std::size_t path ) )
    {
        assert( facade );
        PathWalker* pathWalker = facade->GetPathWalker( entity );
        if( !pathWalker )
            return false;
        return pathWalker->IsMovingOn( facade->GetPath( path ).lock() );
    }
    DEFINE_HOOK( CreatePath, 3, std::size_t, ( const SWORD_Model* model, const MT_Vector2D& vPosEnd, int pathType ) )
    {
        assert( facade );
        const PathType* pPathType = PathType::Find( pathType );
        assert( pPathType );
        boost::shared_ptr< Agent_Path > path( new Agent_Path( *facade, model, vPosEnd, *pPathType ) );
        facade->RegisterPath( path );
        path->Initialize();
        return path->GetID();
    }
    DEFINE_HOOK( CreatePathList, 3, std::size_t, ( const SWORD_Model* model, std::vector< boost::shared_ptr< MT_Vector2D > >& points, int pathType ) )
    {
        const PathType* pPathType = PathType::Find( pathType );
        assert( pPathType );
        boost::shared_ptr< Agent_Path > path( new Agent_Path( *facade, model, points, *pPathType ) );
        facade->RegisterPath( path );
        path->Initialize();
        return path->GetID();
    }
    DEFINE_HOOK( PathGetLastPointOfPath, 1, boost::shared_ptr< MT_Vector2D >, ( std::size_t path ) )
    {
        return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( facade->GetPath( path ).lock()->GetResult().back()->GetPos() ) );
    }
    DEFINE_HOOK( ExecutePathfind, 2, void, ( std::size_t path, TER_Pathfinder_ABC& pathfind ) )
    {
        facade->GetPath( path ).lock()->Execute( pathfind );
    }
    DEFINE_HOOK( PathGetLength, 1, double, ( std::size_t path ) )
    {
        return facade->GetPath( path ).lock()->GetLength();
    }
    DEFINE_HOOK( PathGetState, 1, DEC_Path_ABC::E_State, ( std::size_t path ) )
    {
        return static_cast< DEC_Path_ABC::E_State >( facade->GetPath( path ).lock()->GetState() ); // $$$$ MCO : static_assert bijection
    }
    DEFINE_HOOK( AvoidEnemies, 1, bool, ( std::size_t path ) )
    {
        return facade->GetPath( path ).lock()->GetClass().AvoidEnemies();
    }
    DEFINE_HOOK( GetEnemyCostAtSecurityRange, 1, double, ( std::size_t path ) )
    {
        return facade->GetPath( path ).lock()->GetClass().GetEnemyCostAtSecurityRange();
    }
    DEFINE_HOOK( GetEnemyCostOnContact, 1, double, ( std::size_t path ) )
    {
        return facade->GetPath( path ).lock()->GetClass().GetEnemyCostOnContact();
    }
    DEFINE_HOOK( AvoidObjects, 1, bool, ( std::size_t path ) )
    {
        return facade->GetPath( path ).lock()->GetClass().AvoidObjects();
    }
    DEFINE_HOOK( GetFirstPoint, 3, void, ( std::size_t path, void(*callback)( const MT_Vector2D& point, void* userData ), void* userData ) )
    {
        const MT_Vector2D* first = facade->GetPath( path ).lock()->GetFirstPoint();
        if( first )
            callback( *first, userData );
    }
    DEFINE_HOOK( GetObjectCost, 2, double, ( std::size_t path, unsigned int type ) )
    {
        return facade->GetPath( path ).lock()->GetClass().GetObjectCost( type );
    }
    DEFINE_HOOK( GetThreshold, 1, double, ( std::size_t path ) )
    {
        return facade->GetPath( path ).lock()->GetClass().GetThreshold();
    }
    DEFINE_HOOK( HandlePopulations, 1, bool, ( std::size_t path ) )
    {
        return facade->GetPath( path ).lock()->GetClass().HandlePopulations();
    }
    DEFINE_HOOK( GetPopulationSecurityRange, 1, double, ( std::size_t path ) )
    {
        return facade->GetPath( path ).lock()->GetClass().GetPopulationSecurityRange();
    }
    DEFINE_HOOK( GetCostOutsideOfPopulation, 1, double, ( std::size_t path ) )
    {
        return facade->GetPath( path ).lock()->GetClass().GetCostOutsideOfPopulation();
    }
    DEFINE_HOOK( GetPopulationAttitudeCost, 2, double, ( std::size_t path, unsigned int type ) )
    {
        return facade->GetPath( path ).lock()->GetClass().GetPopulationAttitudeCost( type );
    }
    DEFINE_HOOK( GetPathPoints, 3, void, ( unsigned int entity, void(*callback)( std::size_t point, void* userData ), void* userData ) )
    {
        assert( facade );
        auto points = facade->GetPoints( entity );
        BOOST_FOREACH( std::size_t point, points )
            callback( point, userData );
    }
    DEFINE_HOOK( RemovePathPoint, 2, void, ( unsigned int entity, std::size_t point ) )
    {
        facade->RemovePathPoint( entity, point );
    }
    DEFINE_HOOK( GetPathTypePoint, 1, int, ( std::size_t point ) )
    {
        return facade->GetPoint( point ).GetTypePoint();
    }
    DEFINE_HOOK( GetPathDestPoint, 1, std::size_t, ( std::size_t point ) )
    {
        auto destPoint = facade->GetPoint( point ).GetDestPoint();
        facade->RegisterPoint( destPoint );
        return destPoint->GetID();
    }
    DEFINE_HOOK( GetPathTypeLimaPoint, 1, int, ( std::size_t point ) )
    {
        return facade->GetPoint( point ).GetTypeLima();
    }
    DEFINE_HOOK( GetPathLimaPoint, 1, unsigned int, ( std::size_t point ) )
    {
        return facade->GetPoint( point ).GetLimaID();
    }
    DEFINE_HOOK( GetPathDIAType, 1, const char*, ( std::size_t point ) )
    {
        return facade->GetPoint( point ).GetDIAType().c_str();
    }
    DEFINE_HOOK( GetPathPos, 1, const MT_Vector2D*, ( std::size_t point ) )
    {
        return &facade->GetPoint( point ).GetPos();
    }
    DEFINE_HOOK( InitializePathClass, 3, void, ( const char* xml, const unsigned int* first, size_t size ) )
    {
        xml::xistringstream xis( xml );
        xis >> xml::start( "pathfind" );
        std::vector< unsigned int > dangerousObjects( first, first + size );
        Agent_PathClass::Initialize( xis, dangerousObjects ); // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
    }
    DEFINE_HOOK( ComputeHeight, 1, double, ( const SWORD_Model* entity ) )
    {
        SWORD_UNUSED( entity );
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade constructor
// Created: SLI 2011-12-13
// -----------------------------------------------------------------------------
ModuleFacade::ModuleFacade( const wrapper::View& model )
    : Module( model )
{
    facade = this; // $$$$ MCO : TOUNDO
    PathType::Initialize();
    wrapper::RegisterCommand< MoveCommand >( "move", *this );
    wrapper::RegisterCommand< MagicMoveCommand >( "magic move" );
    wrapper::RegisterCommand< OrientateCommand >( "orientate" );
    wrapper::RegisterCommand< ComputeHeightCommand >( "compute height" );
    wrapper::RegisterCommand< FollowCommand >( "follow" );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::RegisterPathWalker
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
void ModuleFacade::RegisterPathWalker( unsigned int entity, PathWalker& walker )
{
    pathWalkers_[ entity ] = &walker;
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::UnregisterPathWalker
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
void ModuleFacade::UnregisterPathWalker( unsigned int entity )
{
    pathWalkers_.erase( entity );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::GetPathWalker
// Created: SLI 2012-12-13
// -----------------------------------------------------------------------------
PathWalker* ModuleFacade::GetPathWalker( unsigned int entity )
{
    auto it = pathWalkers_.find( entity );
    if( it == pathWalkers_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::RegisterPath
// Created: SLI 2012-12-13
// -----------------------------------------------------------------------------
void ModuleFacade::RegisterPath( boost::shared_ptr< Agent_Path > path )
{
    paths_[ path->GetID() ] = path;
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::UnregisterPath
// Created: SLI 2012-12-13
// -----------------------------------------------------------------------------
void ModuleFacade::UnregisterPath( std::size_t identifier )
{
    GET_HOOK( CancelPathFindJob )( identifier );
    paths_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::GetPath
// Created: SLI 2012-12-13
// -----------------------------------------------------------------------------
boost::weak_ptr< Agent_Path > ModuleFacade::GetPath( std::size_t identifier )
{
    auto it = paths_.find( identifier );
    if( it == paths_.end() )
        throw MASA_EXCEPTION( "Could not retrieve path '" + boost::lexical_cast< std::string >( identifier ) + "'" );
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::GetPoints
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
std::vector< std::size_t > ModuleFacade::GetPoints( unsigned int entity ) const
{
    auto it = pathPoints_.find( entity );
    if( it != pathPoints_.end() )
        return it->second;
    return std::vector< std::size_t >();
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::GetPoint
// Created: SLI 2012-12-13
// -----------------------------------------------------------------------------
const movement::PathPoint& ModuleFacade::GetPoint( std::size_t point ) const
{
    auto it = points_.find( point );
    if( it == points_.end() )
        throw MASA_EXCEPTION( "Could not retrieve decisionnal point" );
    return *it->second.lock();
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::AddPathPoint
// Created: SLI 2012-12-18
// -----------------------------------------------------------------------------
void ModuleFacade::AddPathPoint( unsigned int entity, const boost::weak_ptr< movement::PathPoint >& point )
{
    RegisterPoint( point );
    pathPoints_[ entity ].push_back( point.lock()->GetID() );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::RemovePathPoint
// Created: SLI 2012-12-18
// -----------------------------------------------------------------------------
void ModuleFacade::RemovePathPoint( unsigned int entity, std::size_t point )
{
    auto path = pathPoints_[ entity ];
    path.erase( std::remove( path.begin(), path.end(), point ), path.end() );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::RemovePathPoints
// Created: SLI 2012-12-18
// -----------------------------------------------------------------------------
void ModuleFacade::RemovePathPoints( unsigned int entity )
{
    pathPoints_.erase( entity );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::RegisterPoint
// Created: SLI 2012-12-18
// -----------------------------------------------------------------------------
void ModuleFacade::RegisterPoint( const boost::weak_ptr< movement::PathPoint >& point )
{
    points_[ point.lock()->GetID() ] = point;
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::UnregisterPoint
// Created: SLI 2012-12-18
// -----------------------------------------------------------------------------
void ModuleFacade::UnregisterPoint( const boost::weak_ptr< movement::PathPoint >& point )
{
    points_.erase( point.lock()->GetID() );
}
