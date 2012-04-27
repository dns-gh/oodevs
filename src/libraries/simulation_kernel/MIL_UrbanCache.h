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
#include <boost/noncopyable.hpp>

class UrbanObjectWrapper;
class MT_Vector2D;

namespace urban
{
    class Model;
}

class MIL_UrbanCache : private boost::noncopyable
{
public:
    MIL_UrbanCache( urban::Model& urbanModel );
    void GetUrbanBlocksWithinSegment( const MT_Vector2D& vSourcePoint, const MT_Vector2D& vTargetPoint, std::vector< const UrbanObjectWrapper* >& list );
    void GetListWithinCircle( const MT_Vector2D& center, float radius, std::vector< UrbanObjectWrapper* >& result ) const;
    const UrbanObjectWrapper* FindBlock( const MT_Vector2D& point ) const;
    std::vector< const UrbanObjectWrapper* > GetCities() const;
    double GetUrbanBlockCost( float weight, const MT_Vector2D& from, const MT_Vector2D& to ) const;
    void Clear();

private:
    typedef std::map< MT_Vector2D, std::vector< const UrbanObjectWrapper* > > T_InnerCache;
    typedef std::map< MT_Vector2D, T_InnerCache > T_Cache;

private:
    urban::Model& urbanModel_;
    T_Cache cache_;
};

#endif