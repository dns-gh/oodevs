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
#include "OrientateCommand.h"
#include "ComputeHeightCommand.h"
#include "Rep_PathPoint_Front.h"
#include "simulation_kernel/Decision/DEC_Path_ABC.h" // $$$$ MCO : for enums
#include "wrapper/Command.h"
#include "wrapper/Hook.h"
#include "wrapper/View.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <algorithm>

using namespace sword;
using namespace sword::movement;

namespace
{
    ModuleFacade* facade = 0; // $$$$ MCO : need a means to bind additional data to a hook

    DEFINE_HOOK( ComputeAgentFutureObjectCollision, bool, ( const SWORD_Model* entity, const KnowledgeCache& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) )
    {
        assert( facade );
        return facade->ComputeFutureObjectCollision( entity, objectsToTest, rDistance, pObject );
    }
    DEFINE_HOOK( GetAgentFuturePosition, MT_Vector2D, ( const SWORD_Model* entity, double rTime, bool bBoundOnPath ) )
    {
        assert( facade );
        return facade->GetFuturePosition( entity, rTime, bBoundOnPath );
    }
    DEFINE_HOOK( IsAgentMovingOnPath, bool, ( unsigned int entity, boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        assert( facade );
        return facade->IsMovingOnPath( entity, path );
    }
    DEFINE_HOOK( CreatePath, boost::shared_ptr< sword::movement::Path_ABC >, ( const SWORD_Model* model, const MT_Vector2D& vPosEnd, int pathType ) )
    {
        const PathType* pPathType = PathType::Find( pathType );
        assert( pPathType );
        return boost::shared_ptr< Path_ABC >( new Agent_Path( model, vPosEnd, *pPathType ) );
    }
    DEFINE_HOOK( CreatePathList, boost::shared_ptr< sword::movement::Path_ABC >, ( const SWORD_Model* model, std::vector< boost::shared_ptr< MT_Vector2D > >& points, int pathType ) )
    {
        const PathType* pPathType = PathType::Find( pathType );
        assert( pPathType );
        return boost::shared_ptr< Path_ABC >( new Agent_Path( model, points, *pPathType ) );
    }
    DEFINE_HOOK( PathGetLastPointOfPath, boost::shared_ptr< MT_Vector2D >, ( boost::shared_ptr< sword::movement::Path_ABC > pPath ) )
    {
        const PathResult* path = dynamic_cast< const PathResult* > ( pPath.get() );
        if( !path || path->GetResult().empty() || !path->GetResult().back() )
            return boost::shared_ptr< MT_Vector2D >();
        return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( path->GetResult().back()->GetPos() ) );
    }
    DEFINE_HOOK( ExecutePathfind, void, ( boost::shared_ptr< sword::movement::Path_ABC > path, TerrainPathfinder& pathfind ) )
    {
        path->Execute( pathfind );
    }
    DEFINE_HOOK( ComputePath, void, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        path->ComputePath( path );
    }
    DEFINE_HOOK( CleanPathAfterComputation, void, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        path->CleanAfterComputation();
    }
    DEFINE_HOOK( PathGetLength, double, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        return path->GetLength();
    }
    DEFINE_HOOK( PathGetState, DEC_Path_ABC::E_State, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        return static_cast< DEC_Path_ABC::E_State >( path->GetState() ); // $$$$ MCO : static_assert bijection
    }
    DEFINE_HOOK( AvoidEnemies, bool, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        return path->GetClass().AvoidEnemies();
    }
    DEFINE_HOOK( GetEnemyCostAtSecurityRange, double, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        return path->GetClass().GetEnemyCostAtSecurityRange();
    }
    DEFINE_HOOK( GetEnemyCostOnContact, double, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        return path->GetClass().GetEnemyCostOnContact();
    }
    DEFINE_HOOK( AvoidObjects, bool, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        return path->GetClass().AvoidObjects();
    }
    DEFINE_HOOK( GetFirstPoint, void, ( boost::shared_ptr< sword::movement::Path_ABC > path, void(*callback)( const MT_Vector2D& point, void* userData ), void* userData ) )
    {
        const MT_Vector2D* first = path->GetFirstPoint();
        if( first )
            callback( *first, userData );
    }
    DEFINE_HOOK( GetObjectCost, double, ( boost::shared_ptr< sword::movement::Path_ABC > path, unsigned int type ) )
    {
        return path->GetClass().GetObjectCost( type );
    }
    DEFINE_HOOK( GetThreshold, double, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        return path->GetClass().GetThreshold();
    }
    DEFINE_HOOK( HandlePopulations, bool, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        return path->GetClass().HandlePopulations();
    }
    DEFINE_HOOK( GetPopulationSecurityRange, double, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        return path->GetClass().GetPopulationSecurityRange();
    }
    DEFINE_HOOK( GetCostOutsideOfPopulation, double, ( boost::shared_ptr< sword::movement::Path_ABC > path ) )
    {
        return path->GetClass().GetCostOutsideOfPopulation();
    }
    DEFINE_HOOK( GetPopulationAttitudeCost, double, ( boost::shared_ptr< sword::movement::Path_ABC > path, unsigned int type ) )
    {
        return path->GetClass().GetPopulationAttitudeCost( type );
    }
    DEFINE_HOOK( GetPathPoints, void, ( unsigned int entity, void(*callback)( boost::shared_ptr< sword::movement::PathPoint > point, void* userData ), void* userData ) )
    {
        assert( facade );
        facade->GetPoints( entity, callback, userData );
    }
    DEFINE_HOOK( AddPathPoint, void, ( unsigned int entity, boost::shared_ptr< sword::movement::PathPoint > point ) )
    {
        assert( facade );
        facade->AddPoints( entity, point );
    }
    DEFINE_HOOK( RemovePathPoint, void, ( unsigned int entity, boost::shared_ptr< sword::movement::PathPoint > point ) )
    {
        assert( facade );
        facade->RemovePoints( entity, point );
    }
    DEFINE_HOOK( GetPathTypePoint, int, ( boost::shared_ptr< sword::movement::PathPoint > pPoint ) )
    {
        return pPoint->GetTypePoint();
    }
    DEFINE_HOOK( GetPathDestPoint, boost::shared_ptr< sword::movement::PathPoint >, ( boost::shared_ptr< sword::movement::PathPoint > pPoint ) )
    {
        return pPoint->GetDestPoint();
    }
    DEFINE_HOOK( GetPathTypeLimaPoint, int, ( boost::shared_ptr< sword::movement::PathPoint > pPoint ) )
    {
        return pPoint->GetTypeLima();
    }
    DEFINE_HOOK( GetPathLimaPoint, unsigned int, ( boost::shared_ptr< sword::movement::PathPoint > pPoint ) )
    {
        return pPoint->GetLimaID();
    }
    DEFINE_HOOK( GetPathDIAType, const char*, ( boost::shared_ptr< sword::movement::PathPoint > point ) )
    {
        return point->GetDIAType().c_str();
    }
    DEFINE_HOOK( GetPathPos, const MT_Vector2D&, ( boost::shared_ptr< sword::movement::PathPoint > point ) )
    {
        return point->GetPos();
    }
    DEFINE_HOOK( InitializePathClass, void, ( const char* xml, const unsigned int* first, size_t size ) )
    {
        xml::xistringstream xis( xml );
        xis >> xml::start( "pathfind" );
        std::vector< unsigned int > dangerousObjects( first, first + size );
        Agent_PathClass::Initialize( xis, dangerousObjects ); // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
    }
    DEFINE_HOOK( ComputeHeight, double, ( const SWORD_Model* entity ) )
    {
        SWORD_UNUSED( entity );
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade constructor
// Created: SLI 2011-12-13
// -----------------------------------------------------------------------------
ModuleFacade::ModuleFacade()
{
    facade = this; // $$$$ MCO : TOUNDO
    MT_Profiler::Initialize();
    PathType::Initialize();
    wrapper::RegisterCommand< MoveCommand >( "move", *this );
    wrapper::RegisterCommand< OrientateCommand >( "orientate", *this );
    wrapper::RegisterCommand< ComputeHeightCommand >( "compute height", *this );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::Register
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
void ModuleFacade::Register( unsigned int entity, PathWalker& walker )
{
    paths_[ entity ] = &walker;
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::Unregister
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
void ModuleFacade::Unregister( unsigned int entity )
{
    paths_.erase( entity );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::ComputeFutureObjectCollision
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
bool ModuleFacade::ComputeFutureObjectCollision( const wrapper::View& entity, const KnowledgeCache& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) const
{
    std::map< unsigned int, PathWalker* >::const_iterator it = paths_.find( entity[ "identifier" ] );
    if( it == paths_.end() )
        return false;
    return it->second->ComputeFutureObjectCollision( entity, objectsToTest, rDistance, pObject, false, false );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::GetFuturePosition
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
MT_Vector2D ModuleFacade::GetFuturePosition( const wrapper::View& entity, double rTime, bool bBoundOnPath ) const
{
    std::map< unsigned int, PathWalker* >::const_iterator it = paths_.find( entity );
    if( it == paths_.end() )
        return MT_Vector2D();
    return it->second->ExtrapolatePosition( entity, rTime, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::IsMovingOnPath
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
bool ModuleFacade::IsMovingOnPath( unsigned int entity, boost::shared_ptr< Path_ABC > path ) const
{
    if( ! path )
        return false;
    std::map< unsigned int, PathWalker* >::const_iterator it = paths_.find( entity );
    if( it == paths_.end() )
        return false;
    return it->second->IsMovingOn( *path );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::GetPoints
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
void ModuleFacade::GetPoints( unsigned int entity, void(*callback)( boost::shared_ptr< movement::PathPoint > point, void* userData ), void* userData ) const
{
    std::map< unsigned int, std::vector< boost::shared_ptr< movement::PathPoint > > >::const_iterator it = points_.find( entity );
    if( it != points_.end() )
        std::for_each( it->second.begin(), it->second.end(), boost::bind( callback, _1, userData ) );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::AddPoints
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
void ModuleFacade::AddPoints( unsigned int entity, boost::shared_ptr< movement::PathPoint > point )
{
    points_[ entity ].push_back( point );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::RemovePoints
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
void ModuleFacade::RemovePoints( unsigned int entity, boost::shared_ptr< movement::PathPoint > point )
{
    std::vector< boost::shared_ptr< movement::PathPoint > >& points = points_[ entity ];
    points.erase( std::remove( points.begin(), points.end(), point ), points.end() );
}
