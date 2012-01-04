//*****************************************************************************
// Created: LDC 2011-12-28
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_UrbanCache.h"
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
void MIL_UrbanCache::GetUrbanBlocksWithinSegment( const geometry::Point2f& vSourcePoint, const geometry::Point2f& vTargetPoint, std::vector< const urban::TerrainObject_ABC* >& list )
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
    urbanModel_.GetListWithinSegment( vSourcePoint, vTargetPoint, list );
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
void MIL_UrbanCache::GetListWithinCircle( const geometry::Point2f& center, float radius, std::vector< const urban::TerrainObject_ABC* >& result ) const
{
    urbanModel_.GetListWithinCircle( center, radius, result );    
}
    