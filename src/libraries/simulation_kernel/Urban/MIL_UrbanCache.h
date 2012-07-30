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
class MT_Vector2D;
class MIL_UrbanModel;

// =============================================================================
/** @class  MIL_UrbanCache
    @brief  MIL_UrbanCache
*/
// Created: LDC 2011-12-229
// =============================================================================
class MIL_UrbanCache : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_UrbanCache( MIL_UrbanModel& urbanModel );
    virtual ~MIL_UrbanCache();
    //@}

public:
    //! @name Operations
    //@{
    void GetUrbanBlocksWithinSegment( const MT_Vector2D& vSourcePoint, const MT_Vector2D& vTargetPoint, std::vector< const UrbanObjectWrapper* >& list );
    void GetListWithinCircle( const MT_Vector2D& center, float radius, std::vector< UrbanObjectWrapper* >& result ) const;
    const UrbanObjectWrapper* FindBlock( const MT_Vector2D& point ) const;
    std::vector< const UrbanObjectWrapper* > GetCities() const;
    double GetUrbanBlockCost( float weight, const MT_Vector2D& from, const MT_Vector2D& to ) const;
    void Clear();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< MT_Vector2D, std::vector< const UrbanObjectWrapper* > > T_InnerCache;
    typedef std::map< MT_Vector2D, T_InnerCache > T_Cache;
    //@}

private:
    //! @name Member data
    //@{
    MIL_UrbanModel& urbanModel_;
    T_Cache cache_;
    //@}
};

#endif
