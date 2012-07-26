//*****************************************************************************
// Created: LDC 2011-12-28
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_UrbanCache.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "MT_Tools/MT_Logger.h"
#include <urban/model.h>

#define VECTOR_TO_POINT( point ) geometry::Point2f( static_cast< float >( ( point ).rX_ ), static_cast< float >( ( point ).rY_ ) )

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache constructor
// Created: LDC 2011-12-28
// -----------------------------------------------------------------------------
MIL_UrbanCache::MIL_UrbanCache( urban::Model& urbanModel )
    : urbanModel_( urbanModel )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::GetUrbanBlocksWithinSegment
// Created: LDC 2011-12-28
// -----------------------------------------------------------------------------
void MIL_UrbanCache::GetUrbanBlocksWithinSegment( const MT_Vector2D& vSourcePoint, const MT_Vector2D& vTargetPoint, std::vector< const UrbanObjectWrapper* >& list )
{
    MT_Vector2D start;
    MT_Vector2D end;
    if( vSourcePoint < vTargetPoint )
    {
        start = vSourcePoint;
        end = vTargetPoint;
    }
    else
    {
        start = vTargetPoint;
        end = vSourcePoint;
    }
    T_Cache::const_iterator it = cache_.find( start );
    if( it != cache_.end() )
    {
        T_InnerCache::const_iterator inner = it->second.find( end );
        if( inner != it->second.end() )
        {
            list = inner->second;
            return;
        }
    }
    std::vector< const urban::TerrainObject_ABC* > tmpList;
    geometry::Point2f vSource( VECTOR_TO_POINT( vSourcePoint ) );
    geometry::Point2f vTarget( VECTOR_TO_POINT( vTargetPoint ) );
    urbanModel_.GetListWithinSegment( vSource, vTarget, tmpList );
    list.reserve( tmpList.size() );
    for( std::size_t i = 0; i < tmpList.size(); ++i )
        list.push_back( &MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *tmpList[ i ] ) );
    cache_[ start ][ end ] = list;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::Clear
// Created: LDC 2011-12-28
// -----------------------------------------------------------------------------
void MIL_UrbanCache::Clear()
{
    cache_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::GetListWithinCircle
// Created: LDC 2011-12-30
// -----------------------------------------------------------------------------
void MIL_UrbanCache::GetListWithinCircle( const MT_Vector2D& center, float radius, std::vector< UrbanObjectWrapper* >& result ) const
{
    std::vector< const urban::TerrainObject_ABC* > tmpList;
    geometry::Point2f geomCenter( VECTOR_TO_POINT( center ) );
    urbanModel_.GetListWithinCircle( geomCenter, radius, tmpList );
    result.reserve( tmpList.size() );
    for( std::size_t i = 0; i < tmpList.size(); ++i )
        result.push_back( &MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *tmpList[ i ] ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::FindBlock
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
const UrbanObjectWrapper* MIL_UrbanCache::FindBlock( const MT_Vector2D& point ) const
{
    geometry::Point2f geomPoint( VECTOR_TO_POINT( point ) );
    const urban::TerrainObject_ABC* ret = urbanModel_.FindBlock( geomPoint );
    return ret ? &MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *ret ) : 0;
}

// -----------------------------------------------------------------------------
// Name: std::vector< const UrbanObjectWrapper* > MIL_UrbanCache::GetCities
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
std::vector< const UrbanObjectWrapper* > MIL_UrbanCache::GetCities() const
{
    std::vector< const UrbanObjectWrapper* > result;
    std::vector< const urban::TerrainObject_ABC* > tmpList = urbanModel_.GetCities();
    for( std::size_t i = 0; i < tmpList.size(); ++i )
        result.push_back( &MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *tmpList[ i ] ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::GetUrbanBlockCost
// Created: JSR 2012-04-23
// -----------------------------------------------------------------------------
double MIL_UrbanCache::GetUrbanBlockCost( float weight, const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    geometry::Point2f geomFrom( VECTOR_TO_POINT( from ) );
    geometry::Point2f geomTo( VECTOR_TO_POINT( to ) );
    return urbanModel_.GetUrbanBlockCost( weight, geomFrom, geomTo );
}
