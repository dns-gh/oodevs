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

class UrbanObjectWrapper;

namespace urban
{
    class Model;
}

class MIL_UrbanCache
{
public:
    MIL_UrbanCache( urban::Model& urbanModel );
    void GetUrbanBlocksWithinSegment( const geometry::Point2f& vSourcePoint, const geometry::Point2f& vTargetPoint, std::vector< const UrbanObjectWrapper* >& list );
    void GetListWithinCircle( const geometry::Point2f& center, float radius, std::vector< UrbanObjectWrapper* >& result ) const;
    const UrbanObjectWrapper* FindBlock( const geometry::Point2f& point ) const;
    std::vector< const UrbanObjectWrapper* > GetCities() const;
    void Clear();

private:
    typedef std::map< geometry::Point2f, std::vector< const UrbanObjectWrapper* > > T_InnerCache;
    typedef std::map< geometry::Point2f, T_InnerCache > T_Cache;

private:
    urban::Model& urbanModel_;
    T_Cache cache_;
};

#endif