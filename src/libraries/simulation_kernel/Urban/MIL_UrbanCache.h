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

#include <map> // ??
#include <spatialcontainer/SpatialContainer.h>

class MIL_UrbanObject_ABC;
class MT_Vector2D;

// =============================================================================
/** @class  MIL_UrbanCache
    @brief  MIL_UrbanCache
*/
// Created: LDC 2011-12-229
// =============================================================================
class MIL_UrbanCache : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    struct QuadTreeTraits;
    typedef spatialcontainer::SpatialContainer< const MIL_UrbanObject_ABC*, QuadTreeTraits, double > T_QuadTree;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_UrbanCache();
    virtual ~MIL_UrbanCache();
    //@}

public:
    //! @name Operations
    //@{
    void CreateQuadTree( std::vector< const MIL_UrbanObject_ABC* >& cities, const geometry::Rectangle2d& rect );
    void GetUrbanBlocksWithinSegment( const MT_Vector2D& vSourcePoint, const MT_Vector2D& vTargetPoint, std::vector< const MIL_UrbanObject_ABC* >& list );
    void GetListWithinCircle( const MT_Vector2D& center, float radius, std::vector< const MIL_UrbanObject_ABC* >& result ) const;
    const MIL_UrbanObject_ABC* FindBlock( const MT_Vector2D& point ) const;
    const std::vector< const MIL_UrbanObject_ABC* >& GetCities() const;
    double GetUrbanBlockCost( float weight, const MT_Vector2D& from, const MT_Vector2D& to ) const;
    void Clear();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< MT_Vector2D, std::vector< const MIL_UrbanObject_ABC* > > T_InnerCache;
    typedef std::map< MT_Vector2D, T_InnerCache > T_Cache;
    //@}

private:
    //! @name Member data
    //@{
    T_Cache cache_;
    std::auto_ptr< T_QuadTree > quadTree_;
    double precision_;
    double maxElementSize_;
    std::vector< const MIL_UrbanObject_ABC* > cities_;
    //@}
};

#endif
