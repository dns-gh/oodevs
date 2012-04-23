//*****************************************************************************
// Created: LDC 2011-12-28
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_UrbanCache.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "MT_Tools/MT_Logger.h"
#include <urban/model.h>

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
void MIL_UrbanCache::GetUrbanBlocksWithinSegment( const geometry::Point2f& vSourcePoint, const geometry::Point2f& vTargetPoint, std::vector< const UrbanObjectWrapper* >& list )
{
    geometry::Point2f start;
    geometry::Point2f end;
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
    urbanModel_.GetListWithinSegment( vSourcePoint, vTargetPoint, tmpList );
    list.reserve( tmpList.size() );
    for( int i = 0; i < tmpList.size(); ++i )
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
void MIL_UrbanCache::GetListWithinCircle( const geometry::Point2f& center, float radius, std::vector< UrbanObjectWrapper* >& result ) const
{
    std::vector< const urban::TerrainObject_ABC* > tmpList;
    urbanModel_.GetListWithinCircle( center, radius, tmpList );
    result.reserve( tmpList.size() );
    for( int i = 0; i < tmpList.size(); ++i )
        result.push_back( &MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *tmpList[ i ] ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::FindBlock
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
const UrbanObjectWrapper* MIL_UrbanCache::FindBlock( const geometry::Point2f& point ) const
{
    const urban::TerrainObject_ABC* ret = urbanModel_.FindBlock( point );
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
    for( int i = 0; i < tmpList.size(); ++i )
        result.push_back( &MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *tmpList[ i ] ) );
    return result;
}
