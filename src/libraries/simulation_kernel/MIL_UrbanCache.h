// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanCache_h_
#define __MIL_UrbanCache_h_

#include <geometry/types.h>
#include <map>
#include <set>

namespace urban
{
    class Model;
    class TerrainObject_ABC;
}

class MIL_UrbanCache
{
public:
    MIL_UrbanCache( urban::Model& urbanModel );
    void GetUrbanBlocksWithinSegment( const geometry::Point2f& vSourcePoint, const geometry::Point2f& vTargetPoint, std::set< const urban::TerrainObject_ABC* >& list );
    void GetListWithinCircle( const geometry::Point2f& center, float radius, std::set< const urban::TerrainObject_ABC* >& result ) const;
    void Clear();

private:
    typedef std::map< geometry::Point2f, std::set< const urban::TerrainObject_ABC* > > T_InnerCache;
    typedef std::map< geometry::Point2f, T_InnerCache > T_Cache;

private:
    urban::Model& urbanModel_;
    T_Cache cache_;
};

#endif